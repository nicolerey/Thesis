#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Time.h>

SoftwareSerial XBee(1, 0);

// interrupt pin or RX
int pin = 1;

void setup(){
	// configure interrupt pin or RX as input
	pinMode(pin, INPUT);

	// initialize serial port
	XBee.begin(9600);

	// add interrupt with pin, ISR, and mode
	attachInterrupt(digitalPinToInterrupt(pin), XBee_receive, RISING);
}

void loop(){

}

// xbee receive ISR code
void XBee_receive(){
	// check if data came in from XBee
	if(XBee.available()){
		// read start byte in data
		XBee.read();

		// read MSB and LSB data
		int MSB = XBee.read();
		int LSB = XBee.read();

		// create dynamic array
		unsigned char *frame_data = (unsigned char) malloc(LSB-5 * sizeof(unsigned char));
		// variable for the array head address
		unsigned char *data_array_head = frame_data;
		// variable for the array tail address
		unsigned char *data_array_tail;

		// variable for the room module address of the data
		unsigned char RM_address;

		// variable for the sum of data bytes
		unsigned char byte_sum = 0x00;

		// loop through data bytes after LSB
		for(int i=0; i<LSB; i++){
			if(i==5)
				RM_address = XBee.read();

			if(i>5){
				*frame_data = XBee.read();
				frame_data++;
			}

			byte_sum += XBee.read();
		}
		data_array_tail = frame_data;

		// variable for calculating checksum
		unsigned char checksum = (0xFF - (byte_sum & 0xFF));

		// if checksum is equal to 0xFF, proceed
		if(checksum==0xFF){
			frame_data = data_array_head;

			// if data address is equal to room module address, proceed
			if(*frame_data==EEPROM.read(0)){
				frame_data++;

				if(packet_format==0x00)
					Test_function(data_array_head++, data_array_tail);
				else if(packet_format==0x01)
					Change_relay_status(data_array_head++, data_array_tail);
				else if(packet_format==0x02)
					Sync_date_time(data_array_head++, data_array_tail);
			}
		}

		// send acknowledgement to the sender
		Send_ACK();
	}
}

// function for testing arduino XBee receive function
void Test_function(unsigned char *data_array_head, unsigned char *data_array_tail){
	unsigned char *frame_data = data_array_head;

	// loop through the data
	while(frame_data!=data_array_tail){
		// print data to the serial monitor
		Serial.print(*frame_data);

		frame_data++;
	}
}

// function for changing arduino relay status
void Change_relay_status(unsigned char *data_array_head, unsigned char *data_array_tail){
	unsigned char *frame_data = data_array_head;

	// loop through the data
	while(frame_data!=data_array_tail){
		// variable for the relay pin
		int relay_pin = *frame_data;

		frame_data++;
		// next data holder
		unsigned char *next_frame_data = frame_data;

		// determine if data represents HIGH or LOW
		if(*next_frame_data==0x01)
			// write to digital pin the value LOW to turn on relay
			digitalWrite(relay_pin, LOW);
		else if(*next_frame_data==0x00)
			// write to digital pin the value HIGH to turn off relay
			digitalWrite(relay_pin, HIGH);
	}
}

// function for changing arduino date & time
void Sync_date_time(unsigned char *data_array_head, unsigned char *data_array_tail){
	unsigned char *frame_data = data_array_head;

	int hour = *frame_data;
	frame_data++;

	int minute = *frame_data;
	frame_data++;

	int second = *frame_data;
	frame_data++;

	int day = *frame_data;
	frame_data++;

	int month = *frame_data;
	frame_data++;

	int year = *frame_data;

	// set arduino date & time
	setTime(hour, minute, second, day, month, year);
}

// function to send acknowledgement to sender
void Send_ACK(){
	unsigned char ack = {0x7E, 0x00, 0x06, 0x01, 0x00, x, x, 0x01, 0xFF, 0xFD};

	// send acknowledgment data to the sender
	XBee.write(ack, sizeof(ack));
}