#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[]);
void SaveRoomSchedule(int xbee_data_int[], char sent_schedule_data[], int sent_schedule_length);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

Scheduler runner;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

File file_ptr;

void setup(){
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

  tsk1.enable();

  if(!SD.begin(10))
    return;
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

      if(xbee_data_int[0]==6){
        char sent_schedule_data[data_length-3];
        for(int x=0; x<(data_length-3); x++)
          sent_schedule_data[x] = (char)xbee_data_queue[5+x];

        SaveRoomSchedule(xbee_data_int, sent_schedule_data, data_length-3);
      }

      for(int x=0, y=data_length+1; x<(xbee_tail - (data_length+2)); x++, y++)
        xbee_data_queue[x] = xbee_data_queue[y];
      xbee_tail -= (data_length+2);
    }
    else
      xbee_tail = 0;
  }
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

  file_ptr = SD.open(file_path, FILE_READ);
  if(file_ptr){
    unsigned char nn[100];
    int x = 0;
    while(file_ptr.available())
      nn[x++] = file_ptr.read();

    Serial1.write(nn, x);
    delay(50);
  }
}
