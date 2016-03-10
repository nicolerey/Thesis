#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <TimeAlarms.h>
#include <SPI.h>
#include <SD.h>

void CheckXBeeBuffer();
void CheckSerialBuffer();
void RequestDateTimeSync();
void CheckWhileInManualMode();
void CheckRoomDeviceStatus();
void CheckRoomSchedule();

void ProcessSerialData();
void PerformXBeeOperation(int xbee_data_int[]);
void SyncDateTime(int xbee_data_int[]);
void ChangeRoomDevicePorts(int xbee_data_int[]);;
void ChangeRoomDeviceStatus(int xbee_data_int[]);
void StartTask4();
void SaveRoomSchedule(int xbee_data_int[], char sent_schedule_data[], int sent_schedule_length);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);
Task tsk2(50, TASK_FOREVER, &CheckSerialBuffer);
Task tsk3(500, TASK_FOREVER, &RequestDateTimeSync);
Task tsk4(60000, TASK_FOREVER, &CheckWhileInManualMode);
Task tsk5(500, TASK_FOREVER, &CheckRoomDeviceStatus);
Task tsk6(60000, TASK_FOREVER, &CheckRoomSchedule);

Scheduler runner;

String serial_data_queue;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

int datetime_sync_flag = 0;
int at_mode_flag = 0;

File file_ptr;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);

  runner.init();

  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);

  digitalWrite(10, HIGH);

  runner.addTask(tsk1);
  runner.addTask(tsk2);
  runner.addTask(tsk3);
  runner.addTask(tsk4);
  runner.addTask(tsk5);

  tsk1.enable();
  tsk2.enable();

  if(!SD.begin(10))
    return;

  file_ptr = SD.open("/");

  EEPROM.update(11, 1);

  if(EEPROM.read(10)){
    if(!datetime_sync_flag)
      tsk3.enable();
    else
      tsk6.enable();
  }
}

void loop(){
  runner.execute();
}

void CheckSerialBuffer(){
  while(Serial.available()>0){
    serial_data_queue += (char)Serial.read();
    Alarm.delay(5);
  }

  if(serial_data_queue.length()!=0)
    ProcessSerialData();
}

void ProcessSerialData(){
  if(serial_data_queue.equals("***")){
    at_mode_flag = 1;
    Serial.println("Entered AT mode...");
  }
  else if(!serial_data_queue.indexOf("ATID") && at_mode_flag){
    Serial1.print("+++");
    Alarm.delay(1000);
    Serial1.println(serial_data_queue);
    Serial1.println("ATWR");
    Serial1.println("ATCN");

    int pan_id = (serial_data_queue.substring(serial_data_queue.indexOf(" ")+1)).toInt();
    EEPROM.update(13, pan_id);

    Serial.println("XBee configuration saved.");

    at_mode_flag = 0;
  }
  else if(!serial_data_queue.indexOf("ID") && at_mode_flag){
    int room_id = (serial_data_queue.substring(serial_data_queue.indexOf(" ")+1)).toInt();
    EEPROM.update(12, room_id);

    Serial.println("Room ID changed to "+serial_data_queue.substring(serial_data_queue.indexOf(" ")+1));

    at_mode_flag = 0;
  }
  else if(!serial_data_queue.indexOf("RESET") && at_mode_flag){
    for(int x=0; x<EEPROM.read(1)+2; x++)
      EEPROM.update(x, 0);

    for(int x=0; x<4; x++)
      EEPROM.update(10+x, 0);

    for(int x=0; x<7; x++)
      EEPROM.update(25+x, 0);

    for(int x=0; x<EEPROM.read(1)+1; x++)
      EEPROM.update(35+x, 0);

    EEPROM.update(45, 0);
    EEPROM.update(46, 0);

    EEPROM.update(1, 0);

    Serial.println("RESET finished");

    at_mode_flag = 0;
  }

  if(EEPROM.read(13) && EEPROM.read(12))
    EEPROM.update(10, 1);

  if(EEPROM.read(10)){
    if(!datetime_sync_flag)
      tsk3.enable();
    else
      tsk6.enable();
  }

  serial_data_queue = "";
}

void CheckXBeeBuffer(){
  while(Serial1.available()>0){
    xbee_data_queue[xbee_tail++] = (unsigned char)Serial1.read();
    Alarm.delay(5);
  }

  if(xbee_tail){
    if(xbee_data_queue[0]==0x2E){
      int data_length = (int)strtol(String(xbee_data_queue[1]).c_str(), NULL, 0);

      int xbee_data_int[data_length];
      for(int x=0; x<data_length; x++)
        xbee_data_int[x] = (int)strtol(String(xbee_data_queue[2+x]).c_str(), NULL, 0);

      if(xbee_data_int[0]==6){
        char sent_schedule_data[data_length-3];
        for(int x=0; x<(data_length-3); x++)
          sent_schedule_data[x] = (char)xbee_data_queue[5+x];

        SaveRoomSchedule(xbee_data_int, sent_schedule_data, data_length-3);
      }
      else
        PerformXBeeOperation(xbee_data_int);

      for(int x=0, y=data_length+1; x<(xbee_tail - (data_length+2)); x++, y++)
        xbee_data_queue[x] = xbee_data_queue[y];
      xbee_tail -= (data_length+2);
    }
    else
      xbee_tail = 0;
  }
}

void PerformXBeeOperation(int xbee_data_int[]){
  if(xbee_data_int[0]==3)
    SyncDateTime(xbee_data_int);
  else if(xbee_data_int[0]==9 && datetime_sync_flag)
    ChangeRoomDevicePorts(xbee_data_int);
  else if(xbee_data_int[0]==1 && datetime_sync_flag)
    ChangeRoomDeviceStatus(xbee_data_int);
  else if(xbee_data_int[0]==7 && datetime_sync_flag)
    CheckRoomDeviceStatus();
  else if(xbee_data_int[0]==14 && datetime_sync_flag)
    tsk5.disable();
}

void RequestDateTimeSync(){
  Serial1.write(0x02);
}

void SyncDateTime(int xbee_data_int[]){
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
}

void ChangeRoomDevicePorts(int xbee_data_int[]){
  EEPROM.update(1, xbee_data_int[2]);

  EEPROM.update(0, xbee_data_int[1]);
  for(int x=0; x<EEPROM.read(1); x++)
    EEPROM.update(2+x, xbee_data_int[3+x]);

  unsigned char return_data[EEPROM.read(1)+3];
  return_data[0] = 0x0B;
  for(int x=0; x<EEPROM.read(1)+2; x++)
    return_data[1+x] = EEPROM.read(x);

  Serial1.write(return_data, EEPROM.read(1)+3);
  Alarm.delay(50);
}

void ChangeRoomDeviceStatus(int xbee_data_int[]){
  int time_now_sum = month()+day()+year()+(hour()*60)+minute();
  int sent_time_sum = xbee_data_int[1]+xbee_data_int[2]+((xbee_data_int[3] << 8) + xbee_data_int[4])+(xbee_data_int[5]*60)+xbee_data_int[6];
  
  if(time_now_sum<sent_time_sum){
    EEPROM.update(25, 1);
    EEPROM.update(26, xbee_data_int[1]);
    EEPROM.update(27, xbee_data_int[2]);
    EEPROM.update(28, xbee_data_int[3]);
    EEPROM.update(29, xbee_data_int[4]);
    EEPROM.update(30, xbee_data_int[5]);
    EEPROM.update(31, xbee_data_int[6]);

    if(xbee_data_int[7]){
      digitalWrite(EEPROM.read(0), HIGH);
      EEPROM.update(35, 1);
    }
    else{
      digitalWrite(EEPROM.read(0), LOW);
      EEPROM.update(35, 0);
    }

    for(int x=0; x<EEPROM.read(1); x++){
      if(xbee_data_int[9+x]){
        digitalWrite(EEPROM.read(2+x), HIGH);
        EEPROM.update(36+x, 1);
      }
      else{
        digitalWrite(EEPROM.read(2+x), LOW);
        EEPROM.update(36+x, 0);
      }
    }

    Alarm.timerOnce(60-second(), StartTask4);
    tsk5.enable();
    tsk6.disable();

    unsigned char return_data[EEPROM.read(1)+2];
    return_data[0] = 0x0C;
    for(int x=0; x<EEPROM.read(1)+1; x++)
      return_data[1+x] = EEPROM.read(35+x);

    Serial1.write(return_data, EEPROM.read(1)+2);
    Alarm.delay(50);
  }
}

void CheckWhileInManualMode(){
  int time_now_sum = month()+day()+year()+(hour()*60)+minute();
  int sent_time_sum = EEPROM.read(26)+EEPROM.read(27)+((EEPROM.read(28) << 8) + EEPROM.read(29))+(EEPROM.read(30)*60)+EEPROM.read(31);

  Serial1.print(String(time_now_sum)+" "+sent_time_sum);
  Alarm.delay(50);

  if(time_now_sum==sent_time_sum){
    EEPROM.update(25, 0);

    digitalWrite(EEPROM.read(0), LOW);
    EEPROM.update(35, 0);
    for(int x=0; x<EEPROM.read(1); x++){
      digitalWrite(EEPROM.read(2+x), LOW);
      EEPROM.update(36+x, 0);
    }

    tsk4.disable();
    tsk5.enable();
    tsk6.enable();
  }
}

void CheckRoomDeviceStatus(){
  unsigned char room_device_status[EEPROM.read(1)+2];
  room_device_status[0] = 0x08;
  for(int x=0; x<EEPROM.read(1)+1; x++)
    room_device_status[1+x] = EEPROM.read(35+x);

  Serial1.write(room_device_status, EEPROM.read(1)+2);
  Alarm.delay(50);
}

void StartTask4(){  
  tsk4.enable();
}

void SaveRoomSchedule(int xbee_data_int[], char sent_schedule_data[], int sent_schedule_length){

  Serial1.write(0x0F);
  delay(50);

  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
  String file_path = day_abr[xbee_data_int[1]]+"/sched"+String(xbee_data_int[4])+".txt";

  if(SD.exists(file_path))
    SD.remove(file_path);
  
  file_ptr = SD.open(file_path, FILE_WRITE);
  if(file_ptr){
    file_ptr.write(sent_schedule_data, sent_schedule_length);
    file_ptr.close();
  }
}

void CheckRoomSchedule(){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

  file_ptr = SD.open(day_abr[weekday()-1]+"/");    
  while(true){
    File entry = file_ptr.openNextFile();
    if(!entry)
      break;

    int file_data_int[EEPROM.read(1)+5];
    for(int x=0; entry.available(); x++)
      file_data_int[x] = (int)strtol(String((unsigned char)entry.read()).c_str(), NULL, 0);

    int time_start_in_file = (file_data_int[EEPROM.read(1)+1] << 8) + file_data_int[EEPROM.read(1)+2];
    if(time_start_in_file==((hour()*60)+minute())){
      digitalWrite(EEPROM.read(0), HIGH);
      EEPROM.update(35, 1);

      for(int x=0; x<EEPROM.read(1); x++){
        digitalWrite(EEPROM.read(2+x), HIGH);
        EEPROM.update(36+x, 1);
      }
    }

    int time_end_in_file = (file_data_int[EEPROM.read(1)+3] << 8) + file_data_int[EEPROM.read(1)+4];
    if(time_end_in_file==((hour()*60)+minute())){
      digitalWrite(EEPROM.read(0), LOW);
      EEPROM.update(35, 0);

      for(int x=0; x<EEPROM.read(1); x++){
        digitalWrite(EEPROM.read(2+x), LOW);
        EEPROM.update(36+x, 0);
      }
    }
  }
}
