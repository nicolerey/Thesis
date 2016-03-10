#include <Time.h>
#include <TaskScheduler.h>

void CheckBLEBuffer();
void BLEFunction();

Task ble_tsk1(50, TASK_FOREVER, &CheckBLEBuffer);
Task ble_tsk2(500, TASK_FOREVER, &BLEFunction);

String RXdata;

Scheduler runner;

void setup(){
  Serial2.begin(9600);

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);

  runner.init();

  runner.addTask(ble_tsk1);
  runner.addTask(ble_tsk2);

  ble_tsk1.enable();
  ble_tsk2.enable();
}

void loop(){
  runner.execute();
}

void BLEFunction(){
  if(checkifconnected()){
    if(RXdata.equals("12345")){
      Serial2.print(1);
      RXdata = "";

      digitalWrite(8, HIGH);

      if(RXdata.equals("0"))
        digitalWrite(9, HIGH);
      else if(RXdata.equals("1"))
        digitalWrite(9, LOW);
      else if(RXdata.equals("2"))
        Serial2.print(0);

      RXdata = "";
    }
    else{
      Serial2.print(0);
      RXdata = "";
    }
  }
}

int checkifconnected(){
  char RXdata_chara[RXdata.length()];
  RXdata.toCharArray(RXdata_chara, RXdata.length());
  if(!strstr(RXdata_chara, "OK+CONN")){
    RXdata = "";
    digitalWrite(8, HIGH);

    return 1;
  }
  else
    return 0;
}

int checkifdisconnected(){
  char RXdata_chara[RXdata.length()];
  RXdata.toCharArray(RXdata_chara, RXdata.length());
  if(!strstr(RXdata_chara, "OK+LOST")){
    digitalWrite(8, LOW);

    return 1;
  }
  else
    return 0;
}

void CheckBLEBuffer(){
  while(Serial2.available()){
    RXdata += (char)Serial2.read();
    delay(50);
  }

  char RXdata_chara[RXdata.length()];
  RXdata.toCharArray(RXdata_chara, RXdata.length());
  if(!strstr(RXdata_chara, "DISCE")){
    if(!strstr(RXdata_chara, "20C38FF67F85"))
      digitalWrite(8, HIGH);
    else
      digitalWrite(8, LOW);
  }
}
