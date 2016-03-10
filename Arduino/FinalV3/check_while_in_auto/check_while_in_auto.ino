#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

void CheckWhileInAuto();
void PrintDateTime();

void TurnOnRoomDevicePorts();

Task tsk8(60000, TASK_FOREVER, &CheckWhileInAuto);
Task timeLoop(1000, TASK_FOREVER, &PrintDateTime);

Scheduler runner;

File file_ptr;

int set_room_device_status[5];

void setup(){
  Serial.begin(9600);

  runner.init();

  runner.addTask(timeLoop);
  timeLoop.enable();

  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);

  if(!SD.begin(53))
    return;

  if(!EEPROM.read(25)){
    runner.addTask(tsk8);
    tsk8.enable();
  }

  //setTime(10, 36, 0, 9, 3, 2016);
}

void loop(){
  runner.execute();
}

void CheckWhileInAuto(){
  if(tsk8.isFirstIteration())
    setTime(10, 36, 0, 9, 3, 2016);
  
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

  for(int x=0; x<(EEPROM.read(1)+1); x++){
    String file_path = day_abr[weekday()-1]+"/"+String(x+1)+".txt";

    file_ptr = SD.open(file_path, FILE_READ);
    if(file_ptr){    
      Serial.println("nicole");
      int time_now = (hour() * 60) + minute();

      file_ptr.seek((time_now/8));
      byte val = file_ptr.peek();

      set_room_device_status[x] = (val >> ((time_now%8)-1)) & 1;

      Serial.println(set_room_device_status[x]);

      file_ptr.close();
    }
  }

  TurnOnRoomDevicePorts();
}

void TurnOnRoomDevicePorts(){
  digitalWrite(EEPROM.read(0), set_room_device_status[0]);
  EEPROM.update(35, set_room_device_status[0]);

  for(int x=0; x<EEPROM.read(1); x++){
    digitalWrite(EEPROM.read(2+x), set_room_device_status[1+x]);
    EEPROM.update(36+x, set_room_device_status[1+x]);
  }
}

void PrintDateTime(){
  Serial.println(String(hour())+":"+String(minute())+":"+String(second())+" "+String(day())+"-"+String(month())+"-"+String(year()));
}
