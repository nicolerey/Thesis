#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckXBeeBuffer();
void CheckWhileInManualMode();

void PerformXBeeOperation(int xbee_data_int[]);
void ChangeRoomDeviceStatus(int xbee_data_int[]);
void CheckRoomDeviceStatus();

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);
Task tsk4(60000, TASK_FOREVER, &CheckWhileInManualMode);

Scheduler runner;

unsigned char xbee_data_queue[220];
int xbee_tail = 0;

int datetime_sync_flag = 1;

void setup(){
  Serial1.begin(9600);

  runner.addTask(tsk1);
  runner.addTask(tsk4);

  tsk1.enable();

  pinMode(9, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);

  setTime(9, 20, 0, 27, 2, 2016);
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
  if(xbee_data_int[0]==3){
    //digitalWrite(9, HIGH);
  }
  else if(xbee_data_int[0]==9 && datetime_sync_flag){

  }
  else if(xbee_data_int[0]==1 && datetime_sync_flag)
    ChangeRoomDeviceStatus(xbee_data_int);
  else if(xbee_data_int[0]==7 && datetime_sync_flag){
    
  }
  else if(xbee_data_int[0]==14 && datetime_sync_flag){
    
  }
}

void ChangeRoomDeviceStatus(int xbee_data_int[]){
      //digitalWrite(9, HIGH);
  int time_now_sum = month()+day()+year()+(hour()*60)+minute();
  int sent_time_sum = xbee_data_int[1]+xbee_data_int[2]+((xbee_data_int[3] << 8) + xbee_data_int[4])+(xbee_data_int[5]*60)+xbee_data_int[6];
  
  if(time_now_sum<=sent_time_sum){
    EEPROM.update(25, 1);
    EEPROM.update(26, xbee_data_int[1]);
    EEPROM.update(27, xbee_data_int[2]);
    EEPROM.update(28, xbee_data_int[3]);
    EEPROM.update(29, xbee_data_int[4]);
    EEPROM.update(30, xbee_data_int[5]);
    EEPROM.update(31, xbee_data_int[6]);

    if(xbee_data_int[7]){
      digitalWrite(EEPROM.read(0), HIGH);
      EEPROM.update(35, 1);
    }
    else{
      digitalWrite(EEPROM.read(0), LOW);
      EEPROM.update(35, 0);
    }

    for(int x=0; x<EEPROM.read(1); x++){
      if(xbee_data_int[9+x]){
        digitalWrite(EEPROM.read(2+x), HIGH);
        EEPROM.update(36+x, 1);
      }
      else{
        digitalWrite(EEPROM.read(2+x), LOW);
        EEPROM.update(36+x, 0);
      }
    }

    tsk4.enable();
    CheckRoomDeviceStatus();

    unsigned char return_data[EEPROM.read(1)+2];
    return_data[0] = 0x0C;
    for(int x=0; x<EEPROM.read(1)+1; x++)
      return_data[1+x] = EEPROM.read(35+x);

    Serial1.write(return_data, EEPROM.read(1)+2);
    delay(5);
  }
}

void CheckWhileInManualMode(){
  int time_now_sum = month()+day()+year()+(hour()*60)+minute();
  int sent_time_sum = EEPROM.read(26)+EEPROM.read(27)+((EEPROM.read(28) << 8) + EEPROM.read(29))+(EEPROM.read(30)*60)+EEPROM.read(31);

  Serial1.print(String(time_now_sum)+" "+sent_time_sum);
  delay(5);

  if(time_now_sum>sent_time_sum){
    EEPROM.update(25, 0);

    digitalWrite(EEPROM.read(0), LOW);
    EEPROM.update(35, 0);
    for(int x=0; x<EEPROM.read(1); x++){
      digitalWrite(EEPROM.read(2+x), LOW);
      EEPROM.update(36+x, 0);
    }

    tsk4.disable();

    CheckRoomDeviceStatus();
  }
}

void CheckRoomDeviceStatus(){
  unsigned char room_device_status[EEPROM.read(1)+2];
  room_device_status[0] = 0x08;
  for(int x=0; x<EEPROM.read(1)+1; x++)
    room_device_status[1+x] = EEPROM.read(35+x);

  Serial1.write(room_device_status, EEPROM.read(1)+2);
  delay(5);
}
