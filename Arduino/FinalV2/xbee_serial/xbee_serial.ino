#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[]);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

Scheduler runner;

String xbee_data_queue;

int datetime_sync_flag = 0;

void setup(){
  Serial1.begin(9600);

  runner.addTask(tsk1);

  tsk1.enable();
}

void loop(){
  runner.execute();
}

void CheckXBeeBuffer(){
  while(Serial1.available()>0){
    xbee_data_queue += (char)Serial1.read();
    delay(5);
  }

  if(xbee_data_queue.length()!=0){
    if(xbee_data_queue.charAt(0)=='.'){
      int data_length = (int)strtol(String((unsigned char)xbee_data_queue.charAt(1)).c_str(), NULL, 0);

      char xbee_data_char[data_length];
      (xbee_data_queue.substring(2, data_length+2)).toCharArray(xbee_data_char, data_length+1);

      int xbee_data_int[data_length];
      for(int x=0; x<data_length; x++)
        xbee_data_int[x] = (int)strtol(String((unsigned char)xbee_data_char[x]).c_str(), NULL, 0);

      PerformXBeeOperation(xbee_data_int);

      xbee_data_queue = xbee_data_queue.substring(data_length+1);
    }
    else
      xbee_data_queue = "";
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
