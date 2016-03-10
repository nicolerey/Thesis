#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length);
void SaveRoomSchedule(int xbee_data_int[], int xbee_data_length);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

String xbee_data;

Scheduler runner;

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

  tsk1.enable();

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

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length){
  if(xbee_data_int[0]==3)
    digitalWrite(9, HIGH);
  else if(xbee_data_int[0]==9)
    digitalWrite(8, HIGH);
  else if(xbee_data_int[0]==1)
    digitalWrite(7, HIGH);
  else if(xbee_data_int[0]==6)
    SaveRoomSchedule(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==14)
    digitalWrite(5, HIGH);
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
