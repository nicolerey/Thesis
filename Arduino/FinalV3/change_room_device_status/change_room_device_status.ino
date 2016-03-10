#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckXBeeBuffer();
void CheckScheduleInManual();
void PrintDateTime();

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length);
void ChangeRoomDeviceStatus(int xbee_data_int[], int xbee_data_length);
void TurnOnRoomDevicePorts();
void TurnOffRoomDevicePorts();

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);
Task tsk4(60000, TASK_FOREVER, &CheckScheduleInManual);
Task tem(1000, TASK_FOREVER, &PrintDateTime);

String xbee_data;

Scheduler runner;

int set_room_device_status[5];

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  runner.init();

  runner.addTask(tsk1);
  runner.addTask(tem);

  tsk1.enable();
  tem.enable();

  setTime(10, 0, 0, 8, 3, 2016);
}

void loop(){
  runner.execute();
}

void CheckXBeeBuffer(){
  while(Serial1.available()){
    xbee_data += (char)Serial1.read();
    delay(50);
  }

  if(!xbee_data.equals("")){
    if((xbee_data.charAt(0))=='.'){
      int xbee_data_length = (int)xbee_data[1];
      int xbee_data_int[xbee_data_length];

      for(int x=0; x<xbee_data_length; x++)
        xbee_data_int[x] = (int)((byte)xbee_data[2+x]);

      PerformXBeeOperation(xbee_data_int, xbee_data_length);

      xbee_data = xbee_data.substring(xbee_data_length+2);
    }
    else{
      xbee_data = "";
    }
  }
}

void CheckScheduleInManual(){
  int time_now_sum = month()+day()+year()+(hour() * 60)+minute();
  int sent_time_sum = EEPROM.read(26)+EEPROM.read(27)+((EEPROM.read(28) << 8) + EEPROM.read(29))+(EEPROM.read(30)*60)+EEPROM.read(31);

  if(time_now_sum==sent_time_sum){
    EEPROM.update(25, 0);

    TurnOffRoomDevicePorts();

    tsk4.disable();
    runner.deleteTask(tsk4);
  }
}

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length){
  if(xbee_data_int[0]==3)
    digitalWrite(9, HIGH);
  else if(xbee_data_int[0]==9)
    digitalWrite(8, HIGH);
  else if(xbee_data_int[0]==1)
    ChangeRoomDeviceStatus(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==7)
    digitalWrite(6, HIGH);
  else if(xbee_data_int[0]==14)
    digitalWrite(5, HIGH);
}

void ChangeRoomDeviceStatus(int xbee_data_int[], int xbee_data_length){
  int time_now_sum = month()+day()+year()+(hour() * 60)+minute();
  int sent_time_sum = xbee_data_int[1]+xbee_data_int[2]+((xbee_data_int[3] << 8) + xbee_data_int[4])+((xbee_data_int[5]*60)+xbee_data_int[6]);

  if(time_now_sum<sent_time_sum){
    EEPROM.update(25, 1);
    EEPROM.update(26, xbee_data_int[1]);
    EEPROM.update(27, xbee_data_int[2]);
    EEPROM.update(28, xbee_data_int[3]);
    EEPROM.update(29, xbee_data_int[4]);
    EEPROM.update(30, xbee_data_int[5]);
    EEPROM.update(31, xbee_data_int[6]);

    set_room_device_status[0] = xbee_data_int[7];
    for(int x=0; x<xbee_data_int[8]; x++)
      set_room_device_status[1+x] = xbee_data_int[9+x];

    TurnOnRoomDevicePorts();

    CheckScheduleInManual();

    runner.addTask(tsk4);
    tsk4.enable();
  }
}

void TurnOnRoomDevicePorts(){
  digitalWrite(EEPROM.read(0), set_room_device_status[0]);
  EEPROM.update(35, set_room_device_status[0]);

  for(int x=0; x<EEPROM.read(1); x++){
    digitalWrite(EEPROM.read(2+x), set_room_device_status[1+x]);
    EEPROM.update(36+x, set_room_device_status[1+x]);
  }
}

void TurnOffRoomDevicePorts(){
  digitalWrite(EEPROM.read(0), 0);
  EEPROM.update(35, 0);

  for(int x=0; x<EEPROM.read(1); x++){
    digitalWrite(EEPROM.read(2+x), 0);
    EEPROM.update(36+x, 0);
  }
}

void PrintDateTime(){
  Serial.println(String(hour())+":"+String(minute())+":"+String(second())+" "+String(day())+" "+String(month())+" "+String(year()));
}
