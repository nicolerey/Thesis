#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckSerialBuffer();
void RequestDateTimeSync();

void ProcessSerialData();

Task tsk2(50, TASK_FOREVER, &CheckSerialBuffer);
Task tsk3(100, TASK_FOREVER, &RequestDateTimeSync);

Scheduler runner;

String serial_data_queue;

int at_mode_flag = 0;
int datetime_sync_flag = 0;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);

  runner.addTask(tsk2);
  runner.addTask(tsk3);

  tsk2.enable();
}

void loop(){
  runner.execute();
}

void CheckSerialBuffer(){
  while(Serial.available()>0){
    serial_data_queue += (char)Serial.read();
    delay(5);
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
    delay(1000);
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

    for(int x=0; x<8; x++)
      EEPROM.update(10+x, 0);

    for(int x=0; x<7; x++)
      EEPROM.update(25+x, 0);

    for(int x=0; x<EEPROM.read(1)+1; x++)
      EEPROM.update(35+x, 0);

    EEPROM.update(1, 0);

    Serial.println("RESET finished");

    at_mode_flag = 0;
  }

  if(EEPROM.read(13) && EEPROM.read(12))
    EEPROM.update(10, 1);

  if(EEPROM.read(10) && EEPROM.read(11)){
    if(!datetime_sync_flag)
      tsk3.enable();
  }

  serial_data_queue = "";
}

void RequestDateTimeSync(){
  Serial.println("nicole");
}
