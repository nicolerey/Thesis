#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[]);
void EditRoomSchedule(int xbee_data_int[], char sent_schedule_data);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

Scheduler runner;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

File file_ptr;

char schedule_data[180];
int schedule_data_tail = 0;

int frame_total = 0;
int frame_count = 0;

void setup(){
  Serial1.begin(9600);

  runner.init();

  pinMode(9, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);

  runner.addTask(tsk1);

  tsk1.enable();
}

void loop(){
  runner.execute();
}

void CheckXBeeBuffer(){
  while(Serial1.available()>0){
    xbee_data_queue[xbee_tail++] = (unsigned char)Serial1.read();
    delay(5);
  }

  if(xbee_tail){
    if(xbee_data_queue[0]==0x2E){
      int data_length = (int)strtol(String(xbee_data_queue[1]).c_str(), NULL, 0);

      int xbee_data_int[data_length];
      for(int x=0; x<data_length; x++)
        xbee_data_int[x] = (int)strtol(String(xbee_data_queue[2+x]).c_str(), NULL, 0);

      if(xbee_data_int[0]==16)
        EditRoomSchedule(xbee_data_int, xbee_data_queue[data_length+1]);

      for(int x=0, y=data_length+1; x<(xbee_tail - (data_length+2)); x++, y++)
        xbee_data_queue[x] = xbee_data_queue[y];
      xbee_tail -= (data_length+2);
    }
    else
      xbee_tail = 0;
  }
}

void EditRoomSchedule(int xbee_data_int[], char sent_schedule_data){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
  String file_path = day_abr[xbee_data_int[1]]+"/"+String(xbee_data_int[2])+".txt";

  if(SD.exists(file_path)){
    file_ptr = SD.open(file_path);

    if(file_ptr){
      char file_schedule_data[180] = "";
      for(int x=0; file_ptr.available(); x++)
        file_schedule_data[x] = file_ptr.read();
      file_ptr.close();

      SD.remove(file_path);

      file_schedule_data[xbee_data_int[3]] = sent_schedule_data;

      file_ptr = SD.open(file_path, FILE_WRITE);
      if(file_ptr){
        file_ptr.write(file_schedule_data, 180);
        file_ptr.close();
      }
    }

    Serial1.write(0x11);
    delay(50);
  }
}