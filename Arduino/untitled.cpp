#include <SoftwareSerial.h>

void ProcessXBeeData();
void PerformXBeeOperation(unsigned char xbee_data[220]);

SoftwareSerial xbeeSerial(8, 7);

unsigned char xbee_data_queue[220];
int queue_tail = 0;
int stat = 0;

void setup() {
    // put your setup code here, to run once:
    xbeeSerial.begin(9600);
	Serial1.begin(9600);

	pinMode(9, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
	while(Serial1.available()>0){
		xbee_data_queue[queue_tail++] = (unsigned char)Serial1.read();
		delay(5);
	}

	if(queue_tail)
		ProcessXBeeData();
}

void ProcessXBeeData(){
	//Serial.println("ProcessXBeeData");
  	if(xbee_data_queue[0]==0x2E){
		unsigned char xbee_data[220];
		int data_count = 0;
		for(; data_count<(int)xbee_data_queue[1]; data_count++){
		  xbee_data[data_count] = xbee_data_queue[2+data_count];
		}

		PerformXBeeOperation(xbee_data);

		data_count += 2;
		for(int y=0, z=data_count+1; y<data_count; y++, z++)
		  xbee_data_queue[y] = xbee_data_queue[z]
		queue_tail -= data_count;
	}	

	Serial1.write(queue_tail);
}

void PerformXBeeOperation(unsigned char xbee_data[220]){
	if(xbee_data[0]==0x01)
		digitalWrite(9, HIGH);
	else if(xbee_data[0]==0x02)
		digitalWrite(9, LOW);
}