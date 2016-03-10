#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

void CheckXBeeBuffer();
void RequestDateTimeSync();
void CheckScheduleInManual();
void CheckWhileInAuto();
void PrintDateTime();

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length);
void SyncDateTime(int xbee_data_int[], int xbee_data_length);
void ChangeRoomDevicePorts(int xbee_data_int[], int xbee_data_length);
void ChangeRoomDeviceStatus(int xbee_data_int[], int xbee_data_length);
void TurnOnRoomDevicePorts();
void TurnOffRoomDevicePorts();
void SaveRoomSchedule(int xbee_data_int[], int xbee_data_length);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);
Task tsk3(1000, TASK_FOREVER, &RequestDateTimeSync);
Task tsk4(60000, TASK_FOREVER, &CheckScheduleInManual);
Task tsk5(60000, TASK_FOREVER, &CheckWhileInAuto);
Task tem(1000, TASK_FOREVER, &PrintDateTime);

String xbee_data;

Scheduler runner;

int datetime_sync_flag = 0;
int set_room_device_status[5];

File file_ptr;

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
  runner.addTask(tsk3);
  runner.addTask(tem);

  tsk1.enable();
  tsk3.enable();
  tem.enable();

  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);

  if(!SD.begin(53))
    return;
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

void RequestDateTimeSync(){
  Serial1.write(0x02);
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

void CheckWhileInAuto(){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

  for(int x=0; x<(EEPROM.read(1)+1); x++){
    String file_path = day_abr[weekday()-1]+"/"+String(x+1)+".txt";

    file_ptr = SD.open(file_path, FILE_READ);
    if(file_ptr){
      int time_now = (hour() * 60) + minute();

      file_ptr.seek((time_now/8));
      byte val = file_ptr.peek();

      set_room_device_status[x] = (val >> ((time_now%8)-1)) & 1;

      file_ptr.close();
    }
    else
      set_room_device_status[x] = 0;
  }

  TurnOnRoomDevicePorts();
}

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length){
  if(xbee_data_int[0]==3)
    SyncDateTime(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==9 && datetime_sync_flag)
    ChangeRoomDevicePorts(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==1 && datetime_sync_flag)
    ChangeRoomDeviceStatus(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==6 && datetime_sync_flag)
    SaveRoomSchedule(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==14)
    digitalWrite(5, HIGH);
}

void SyncDateTime(int xbee_data_int[], int xbee_data_length){
  int hour_now = xbee_data_int[1];
  int minute_now = xbee_data_int[2];
  int second_now = xbee_data_int[3];
  int day_now = xbee_data_int[4];
  int month_now = xbee_data_int[5];
  int year_now = ((xbee_data_int[6] << 8) + xbee_data_int[7]);

  setTime(hour_now, minute_now, second_now, day_now, month_now, year_now);

  datetime_sync_flag = 1;

  tsk3.disable();
  runner.deleteTask(tsk3);

  Serial1.print(String(hour())+" "+String(minute())+" "+String(second())+" "+String(day())+" "+String(month())+" "+String(year()));
  delay(50);

  runner.addTask(tsk5);
  tsk5.enable();
}

void ChangeRoomDevicePorts(int xbee_data_int[], int xbee_data_length){
  EEPROM.update(1, xbee_data_int[2]);

  EEPROM.update(0, xbee_data_int[1]);
  for(int x=0; x<EEPROM.read(1); x++)
    EEPROM.update(2+x, xbee_data_int[3+x]);

  unsigned char return_data[EEPROM.read(1)+2];
  for(int x=0; x<EEPROM.read(1)+2; x++)
    return_data[x] = EEPROM.read(x);

  Serial1.write(return_data, EEPROM.read(1)+2);
  delay(50);
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

void SaveRoomSchedule(int xbee_data_int[], int xbee_data_length){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
  String file_path = day_abr[xbee_data_int[1]-1]+"/"+String(xbee_data_int[2])+".txt";

  byte file_data[180];  
  for(int x=0; x<180; x++)
    file_data[x] = 0;

  if(SD.exists(file_path)){
    file_ptr = SD.open(file_path, FILE_READ);
    if(file_ptr){
      int x=0;
      while(file_ptr.available())
        file_data[x++] = file_ptr.read();

      file_ptr.close();
    }

    SD.remove(file_path);
  }

  int start = ((xbee_data_int[3] << 8) + xbee_data_int[4]);
  int end = ((xbee_data_int[5] << 8) + xbee_data_int[6]);

  for(int x=start; x<end; x++)
    file_data[(int)x/8] ^= (-1 ^ file_data[(int)x/8]) & (1 << ((x%8)-1));

  file_ptr = SD.open(file_path, FILE_WRITE);
  if(file_ptr){
    file_ptr.write(file_data, 180);
    file_ptr.close();
  }
}
