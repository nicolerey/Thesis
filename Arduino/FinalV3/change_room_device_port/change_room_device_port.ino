#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>

void CheckXBeeBuffer();

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length);
void ChangeRoomDevicePorts(int xbee_data_int[], int xbee_data_length);

Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);

String xbee_data;

Scheduler runner;

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
    ChangeRoomDevicePorts(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==1)
    digitalWrite(7, HIGH);
  else if(xbee_data_int[0]==7)
    digitalWrite(6, HIGH);
  else if(xbee_data_int[0]==14)
    digitalWrite(5, HIGH);
}

void ChangeRoomDevicePorts(int xbee_data_int[], int xbee_data_length){
  EEPROM.update(1, xbee_data_int[2]);

  EEPROM.update(0, xbee_data_int[1]);
  for(int x=0; x<EEPROM.read(1); x++)
    EEPROM.update(2+x, xbee_data_int[3+x]);

  unsigned char return_data[EEPROM.read(1)+2];
  for(int x=0; x<EEPROM.read(1)+2; x++)
    return_data[x] = EEPROM.read(x);

  Serial1.write(return_data, EEPROM.read(1)+2);
  delay(50);
}
