#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <TimeAlarms.h>
#include <SPI.h>
#include <SD.h>

void CheckRoomSchedule();

void StartTask4();

Task tsk6(60000, TASK_FOREVER, &CheckRoomSchedule);

Scheduler runner;

File file_ptr;

void setup(){
  Serial.begin(9600);

  runner.init();

  runner.addTask(tsk6);

  //if(!EEPROM.read(25))
    Alarm.timerOnce(60-second(), StartTask4);

  pinMode(10, OUTPUT);
  pinMode(9, OUTPUT);

  digitalWrite(10, HIGH);

  if(!SD.begin(10))
    digitalWrite(9, HIGH);
}

void loop(){
  runner.execute();
  Serial.println("nicole");
}

void CheckRoomSchedule(){
  Serial.println("nicole");
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

  int sched_index = 1;
  while(1){
    String file_path = day_abr[weekday()+1]+"/sched"+String(sched_index)+".txt";

    int time_now = (hour() * 60) + minute();

    if(SD.exists(file_path)){
      file_ptr = SD.open(file_path);
      if(file_ptr){
        int x = 0;
        unsigned char dat[5+EEPROM.read(1)];
        for(; file_ptr.available(); x++)
          dat[x] = file_ptr.read();

        int file_data[5+EEPROM.read(1)];
        for(int x=0; x<(5+EEPROM.read(1)); x++)
          file_data[x] = (int)strtol(String(dat[x]).c_str(), NULL, 0);

        int file_time = (file_data[EEPROM.read(1)+1] << 8) + file_data[EEPROM.read(1)+2];
        if(file_time==time_now){
          for(int x=0; x<(EEPROM.read(1)+1); x++){
            if(x==0){
              if(file_data[x]){
                digitalWrite(EEPROM.read(0), HIGH);
                EEPROM.update(35+x, 1);
              }
              else{
                digitalWrite(EEPROM.read(0), LOW);
                EEPROM.update(35+x, 0);
              }
            }
            else{
              if(file_data[x]){
                digitalWrite(EEPROM.read(2+x), HIGH);
                EEPROM.update(35+x, 1);
              }
              else{
                digitalWrite(EEPROM.read(2+x), LOW);
                EEPROM.update(35+x, 0);
              }
            }
          }
        }
      }

      sched_index += 1;
    }
    else
      break;
  }
}

void StartTask4(){
  tsk6.enable();
}
