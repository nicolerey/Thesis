#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckXBeeBuffer();
void RequestDateTimeSync();

void PerformXBeeOperation(int xbee_data_int[]);
void SyncDateTime(int xbee_data_int[]);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);
Task tsk3(500, TASK_FOREVER, &RequestDateTimeSync);

Scheduler runner;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

int datetime_sync_flag = 0;

void setup(){
  Serial1.begin(9600);

  runner.addTask(tsk1);
  runner.addTask(tsk3);

  tsk1.enable();
  tsk3.enable();
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
  else if(xbee_data_int[0]==9 && datetime_sync_flag){

  }
  else if(xbee_data_int[0]==1 && datetime_sync_flag){
    
  }
  else if(xbee_data_int[0]==7 && datetime_sync_flag){
    
  }
  else if(xbee_data_int[0]==14 && datetime_sync_flag){
    
  }
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

  unsigned char date_time[7];
  date_time[0] = hour();
  date_time[1] = minute();
  date_time[2] = second();
  date_time[3] = day();
  date_time[4] = month();
  date_time[5] = (year() >> 8) & 0xFF;
  date_time[6] = year() & 0xFF;

  Serial1.write(date_time, 7);
}
