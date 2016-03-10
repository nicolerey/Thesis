#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[]);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

Scheduler runner;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

int datetime_sync_flag = 0;

void setup(){
  Serial.begin(9600);

  runner.addTask(tsk1);

  tsk1.enable();

  pinMode(9, OUTPUT);
}

void loop(){
  runner.execute();
}

void CheckXBeeBuffer(){
  while(Serial.available()>0){
    xbee_data_queue[xbee_tail++] = (unsigned char)Serial.read();
    delay(5);
  }

  if(xbee_tail){
    if(xbee_data_queue[0]==0x2E){
      int data_length = (int)strtol(String(xbee_data_queue[1]).c_str(), NULL, 0);

      int xbee_data_int[data_length];
      for(int x=0; x<data_length; x++)
        xbee_data_int[x] = (int)strtol(String(xbee_data_queue[2+x]).c_str(), NULL, 0);

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
    digitalWrite(9, HIGH);
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
