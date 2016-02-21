#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Time.h>
#include <TimeAlarms.h>

/***** FUNCTION Prototypes *****/
void ProcessXBeeData();
void ProcessSerialData();
void ClearDataQueue(int clear_option);

void digitalClockDisplay();
void printDigits(int digits);

void TurnOffRoomDevices(int port);
void CheckRoomDeviceSchedule();
/*******************************/

/***** GLOBAL Variables *****/
SoftwareSerial xbeeSerial(8, 7); // Rx Tx

char serial_data_queue[10];
int serial_queue_tail = 0;

unsigned char xbee_data_queue[220];
int queue_tail = 0;
int queue_size = 0;

int datetime_sync_flag = 0;
/****************************/

void setup(){
  Serial.begin(9600);
  xbeeSerial.begin(9600);

  // Make room and device ports to output
  if(EEPROM.read(0))
    pinMode(EEPROM.read(0), OUTPUT);

  for(int x=0; x<EEPROM.read(1); x++){
    if(EEPROM.read(2+x))
      pinMode(EEPROM.read(2+x), OUTPUT);
  }

  // Assign schedule start address in EEPROM
  EEPROM.write(47, 50);

  // Create alarm to check room and device schedule
  Alarm.timerRepeat(60, CheckRoomDeviceSchedule);
}

void loop(){
  // When data is present in SERIAL buffer
  while(xbeeSerial.available()>0){
    xbee_data_queue[queue_tail++] = (unsigned char)xbeeSerial.read();
    queue_size++;
    Alarm.delay(5);
  }

  while(Serial.available()>0){
    serial_data_queue[serial_queue_tail++] = (char)Serial.read();
    Alarm.delay(5);
  }

  if(queue_size)
    ProcessXBeeData();

  if(serial_queue_tail)
    ProcessSerialData();

  if(EEPROM.read(10)){
    // Check if date and time has been synced
    if(!datetime_sync_flag){
      unsigned char datetime_frame[2];
      datetime_frame[0] = 0x02;
      datetime_frame[1] = 0x00;
      xbeeSerial.write(datetime_frame, 2);
    }
    else
      digitalClockDisplay();    

    // Check if room is in manual mode
    if(EEPROM.read(25)==1)
      TurnOffRoomDevices(0);
    
    Alarm.delay(5);
  }
}

void ProcessSerialData(){
  String xbee_data = "ATID ";
  for(int x=0; x<serial_queue_tail; x++)
    xbee_data += serial_data_queue[x];

  Serial.print(xbee_data);
  
  xbeeSerial.print("+++");
  delay(1000);
  xbeeSerial.println(xbee_data);
  xbeeSerial.println("ATWR");
  xbeeSerial.println("ATCN");

  ClearDataQueue(2);
}

void ProcessXBeeData(){
  if(xbee_data_queue[0]==0x03){
    // Date & time syncing
    int hour_now = (int)xbee_data_queue[1];
    int minute_now = (int)xbee_data_queue[2];
    int second_now = (int)xbee_data_queue[3];
    int day_now = (int)xbee_data_queue[4];
    int month_now = (int)xbee_data_queue[5];
    int year_now = (((int)xbee_data_queue[6] << 8) + (int)xbee_data_queue[7]);

    setTime(hour_now, minute_now, second_now, day_now, month_now, year_now);

    unsigned char datetime_frame[7];
    datetime_frame[0] = (unsigned char)hour();
    datetime_frame[1] = (unsigned char)minute();
    datetime_frame[2] = (unsigned char)second();
    datetime_frame[3] = (unsigned char)day();
    datetime_frame[4] = (unsigned char)month();
    datetime_frame[5] = (unsigned char)(year() >> 8);
    datetime_frame[6] = (unsigned char)year();

    xbeeSerial.write(datetime_frame, 7);
  }
  else if(xbee_data_queue[0]==0x02 && xbee_data_queue[1]==0x01)
    datetime_sync_flag = 1;
  else if(xbee_data_queue[0]==0x04 && datetime_sync_flag==1){
    // Consumption reading
  }
  else if(xbee_data_queue[0]==0x07 && datetime_sync_flag==1){    
    // Port status checking
    unsigned char pin_status_frame[6];

    pin_status_frame[0] = 0x08;
    for(int x=0; x<((int)EEPROM.read(1)+1); x++){
      unsigned char stat = (unsigned char)EEPROM.read(35+x);
      
      if(stat==0xFF)
        stat = 0x00;

      pin_status_frame[1+x] = stat;
    }

    xbeeSerial.write(pin_status_frame, 6);
  }
  else if(xbee_data_queue[0]==0x09 && datetime_sync_flag==1){
    // Change room and device ports
    EEPROM.write(0, (int)xbee_data_queue[1]);
    for(int x=0; x<(int)xbee_data_queue[2]; x++)
      EEPROM.write(2+x, (int)xbee_data_queue[3+x]);
  }
  else if(xbee_data_queue[0]==0x01 && datetime_sync_flag==1){
    // Change room and device status
    EEPROM.write(25, 1);
    EEPROM.write(26, (int)xbee_data_queue[1]);
    EEPROM.write(27, (int)xbee_data_queue[2]);
    EEPROM.write(28, (int)(xbee_data_queue[3] << 8) + xbee_data_queue[4]);
    EEPROM.write(29, (int)xbee_data_queue[5]);
    EEPROM.write(30, (int)xbee_data_queue[6]);
    
    if((int)xbee_data_queue[7]==1){
      digitalWrite(EEPROM.read(0), HIGH);
      EEPROM.write(35, 1);
    }
    else{
      digitalWrite(EEPROM.read(0), LOW);
      EEPROM.write(35, 0);
    }

    for(int x=0; x<EEPROM.read(1); x++){
      if((int)xbee_data_queue[9+x]==1){
        digitalWrite(EEPROM.read(2+x), HIGH);
        EEPROM.write(36+x, 1);
      }
      else{
        digitalWrite(EEPROM.read(2+x), LOW);
        EEPROM.write(36+x, 0);
      }
    }
  }
  else if(xbee_data_queue[0]==0x06 && datetime_sync_flag==1){
    // Room schedule saving
    EEPROM.update(45, (int)xbee_data_queue[1]);

    if((int)xbee_data_queue[2]<=EEPROM.read(45)){
      for(int x=0; x<queue_size-3; x++){
        EEPROM.write(EEPROM.read(47), (int)xbee_data_queue[3+x]);
        EEPROM.write(47, EEPROM.read(47) + 1);
      }

      EEPROM.write(45, (int)xbee_data_queue[2]);
    }

    for(int x=0; x<180; x++)
      Serial.println(EEPROM.read(50+x), HEX);
  }
  else{
    for(int x=0; x<queue_size; x++)
      Serial.print((char)xbee_data_queue[x]);
    Serial.println();
  }

  ClearDataQueue(1);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println();
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void ClearDataQueue(int clear_option){
  if(clear_option==1){
    for(int x=0; x<queue_size; x++)
      xbee_data_queue[x] = 0;

    queue_size = 0;
    queue_tail = 0;
  }
  else if(clear_option==2){
    for(int x=0; x<serial_queue_tail; x++)
      serial_data_queue[x] = 0;

    serial_queue_tail = 0;
  }
}

void TurnOffRoomDevices(int port_index){
  if(!port_index){
    if(month()>=EEPROM.read(26) && day()>=EEPROM.read(27) && year()>=EEPROM.read(28) && hour()>=EEPROM.read(29) && minute()>=EEPROM.read(30)){
      EEPROM.write(25, 0);

      digitalWrite(EEPROM.read(0), LOW);
      EEPROM.write(35, 0);
      for(int x=0; x<EEPROM.read(1); x++){
        digitalWrite(EEPROM.read(2+x), LOW);
        EEPROM.write(36+x, 0);
      }

      for(int x=0; x<EEPROM.read(1)+1; x++)
        Serial.println(EEPROM.read(35+x));
    }
  }
  else if(port_index!=0){
    digitalWrite(port_index, LOW);
    Serial.println("Turn off");
  }
}

void TurnOnRoomDevices(int port_index){
  digitalWrite(port_index, HIGH);
  Serial.println("Turn on");
}

void CheckRoomDeviceSchedule(){
  digitalClockDisplay();

  if(!EEPROM.read(25)){
    int schedule_index = 0;
    int weekday_index = weekday();

    for(int x=0; x<(EEPROM.read(1)+1); x++){
      if(!x)
        schedule_index = 50;
      else
        schedule_index = 50 + (180 * x);

      if(weekday_index>1)
        schedule_index += (180 * weekday_index);

      //Serial.println(schedule_index);
      int time_now = (hour() * 60) + minute();
      int eeprom_data = EEPROM.read((int)(time_now/8)+schedule_index);
      Serial.println((time_now/8)+schedule_index);
      Serial.println(eeprom_data);

      int port_index = 0;
      if(x>0)
        port_index = 2 + x;

      if(((eeprom_data >> (time_now%8)) & 1))
        TurnOnRoomDevices(EEPROM.read(port_index));
      else
        TurnOffRoomDevices(EEPROM.read(port_index));
    }
  }
}
