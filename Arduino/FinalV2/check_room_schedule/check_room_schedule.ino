#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <TimeAlarms.h>
#include <SPI.h>
#include <SD.h>

void CheckRoomSchedule();

Task tsk6(60000, TASK_FOREVER, &CheckRoomSchedule);

Scheduler runner;

File file_ptr;

void setup(){
  Serial.begin(9600);

  setTime(22, 00, 00, 01, 03, 2016);
  while(!Serial);

  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(10, HIGH);

  if(!SD.begin(10))
    return;

  file_ptr = SD.open("/");

  runner.init();

  runner.addTask(tsk6);

  tsk6.enable();
}

void loop(){
  runner.execute();

  Serial.println(String(hour())+":"+String(minute())+":"+String(second()));

  //CheckRoomSchedule();
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

    Serial.println(String(time_start_in_file)+" "+String(((hour()*60)+minute())));

    int time_end_in_file = (file_data_int[EEPROM.read(1)+3] << 8) + file_data_int[EEPROM.read(1)+4];
    if(time_end_in_file==((hour()*60)+minute())){
      digitalWrite(EEPROM.read(0), LOW);
      EEPROM.update(35, 0);

      for(int x=0; x<EEPROM.read(1); x++){
        digitalWrite(EEPROM.read(2+x), LOW);
        EEPROM.update(36+x, 0);
      }
    }

    Serial.println(String(time_end_in_file)+" "+String(((hour()*60)+minute())));
  }
}
