#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[]);
void SaveRoomSchedule(int xbee_data_int[], char sent_schedule_data[], int data_length);
void EditRoomSchedule(int xbee_data_int[], char schedule_value);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

Scheduler runner;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

char schedule_data[180];
int schedule_tail = 0;

File file_ptr;

int datetime_sync_flag = 0;

int frame_total = 0;
int frame_count = 0;

void setup(){
  Serial1.begin(9600);
  Serial.begin(9600);

  runner.addTask(tsk1);

  tsk1.enable();

  if(!SD.begin(10))
    return;

  pinMode(9, OUTPUT);
  pinMode(18, OUTPUT);
}

void loop(){
  runner.execute();

    digitalWrite(18, HIGH);
    delay(1000);
    digitalWrite(18, LOW);
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
        digitalWrite(9, HIGH);

        char sent_schedule_data[data_length-5];
        for(int x=0; x<(data_length-5); x++)
          sent_schedule_data[x] = xbee_data_queue[7+x];

        Serial1.print(sent_schedule_data);

        SaveRoomSchedule(xbee_data_int, sent_schedule_data, data_length-5);

        Serial1.write(0x0F);
    delay(5);
        digitalWrite(9, LOW);
      }
      else if(xbee_data_int[0]==16)
        EditRoomSchedule(xbee_data_int, xbee_data_queue[data_length+1]);
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
  if(xbee_data_int[0]==3){

  }
  else if(xbee_data_int[0]==9 && datetime_sync_flag){

  }
  else if(xbee_data_int[0]==1 && datetime_sync_flag){
    
  }
  else if(xbee_data_int[0]==7 && datetime_sync_flag){
    
  }
  else if(xbee_data_int[0]==14 && datetime_sync_flag){
    
  }
}

void SaveRoomSchedule(int xbee_data_int[], char sent_schedule_data[], int data_length){
  /*if(!frame_total)
    frame_total = xbee_data_int[1];*/

  /*if((xbee_data_int[2]-1)==frame_count){
    digitalWrite(9, HIGH);
    delay(1000);
    frame_count = xbee_data_int[2];
    digitalWrite(9, LOW);*/
    /*for(int x=0; x<data_length; x++)
      schedule_data[schedule_tail++] = sent_schedule_data[x];

    EEPROM.update(46, xbee_data_int[2]);

    if(EEPROM.read(45)==EEPROM.read(46)){
      String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

      file_ptr = SD.open(day_abr[xbee_data_int[3]]+"/"+String(xbee_data_int[4])+".txt", FILE_WRITE);
      if(file_ptr){
        file_ptr.write(schedule_data, schedule_tail);
        file_ptr.close();

        schedule_tail = 0;
        EEPROM.update(45, 0);
        EEPROM.update(46, 0);
      }*/

      /*file_ptr = SD.open(day_abr[xbee_data_int[3]]+"/"+String(xbee_data_int[4])+".txt");
      if(file_ptr){
        unsigned char data[180]
        int c_out = 0;
        while(file_ptr.available())
          data[c_out++] = (unsigned char)file_ptr.read();

        file_ptr.close();

        Serial1.write(data, c_out);
        delay(50);
      }*/
    /*}*/

    
  /*}*/
}

void EditRoomSchedule(int xbee_data_int[], char schedule_value){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

  if(SD.exists(day_abr[xbee_data_int[1]]+"/"+String(xbee_data_int[2])+".txt")){
    file_ptr = SD.open(day_abr[xbee_data_int[1]]+"/"+String(xbee_data_int[2])+".txt");

    if(file_ptr){
      char schedule_data_in_file[180] = "";
      for(int x=0; file_ptr.available(); x++)
        schedule_data_in_file[x] = file_ptr.read();
      file_ptr.close();

      SD.remove(day_abr[xbee_data_int[1]]+"/"+String(xbee_data_int[2])+".txt");

      schedule_data_in_file[xbee_data_int[3]] = schedule_value;

      file_ptr = SD.open(day_abr[xbee_data_int[1]]+"/"+String(xbee_data_int[2])+".txt", FILE_WRITE);
      if(file_ptr){
        file_ptr.write(schedule_data_in_file, 180);
        file_ptr.close();
      }
    }

    file_ptr = SD.open(day_abr[xbee_data_int[1]]+"/"+String(xbee_data_int[2])+".txt");
    if(file_ptr){
      unsigned char data[180];
      int c_out = 0;
      while(file_ptr.available())
        data[c_out++] = (unsigned char)file_ptr.read();

      file_ptr.close();

      Serial1.write(data, c_out);
      delay(50);
    }
  }
}
