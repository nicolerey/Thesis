#include <Time.h>
#include <TaskScheduler.h>

void CheckBLEBuffer();
void BLEFunction();
void TurnOffDelay();

int checkifdisconnected();
int checkifconnected();
void getRXdata();
void clearBuffer();

Task ble_tsk1(50, TASK_FOREVER, &CheckBLEBuffer);
Task ble_tsk2(500, TASK_FOREVER, &BLEFunction);
Task ble_tsk3(3000, 1, &TurnOffDelay);

String RXdata;
char buff[500];
int data, x, conn, con_stat, requestcon, DISCE, ATROLE1, readyRole1, delay3s,flagonPowerstatus,flagoffPowerstatus,notdiscovered;
long previousTime = 0;

int ble_status = 0;

Scheduler runner;

void setup(){
  flagoffPowerstatus=0;
  flagonPowerstatus=0;
  notdiscovered=1;
  DISCE = 0;
  delay3s = 0;
  ATROLE1 = 0;
  requestcon = 0;
  con_stat = 0;
  readyRole1=1;

  Serial2.begin(9600);
  Serial.begin(9600);

  runner.init();
  runner.addTask(ble_tsk1);
  runner.addTask(ble_tsk2);

  ble_tsk1.enable();
  ble_tsk2.enable();

  clearBuffer();

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop(){
  runner.execute();
}

void BLEFunction(){
  int temp;
  String serial_char;
  unsigned long currentTime = millis();

  if (requestcon) {
    if (!con_stat)
    {
      delay(100);
      conn = checkifdisconnected(); 
      if (conn) {
        if (RXdata.equals("12345")) {
          Serial2.print(1);
          delay(5);
          con_stat = 1;
          RXdata = "";
        }
        else {
          Serial2.print(0);
          RXdata = "";          
        }
      }
    }
    if (con_stat) {
      delay(100);
      conn = checkifdisconnected();
      if (conn) {
        if (RXdata.equals("0")){
          if(ble_status){
            ble_status = 0;
            
          digitalWrite(9, LOW);
          digitalWrite(8, LOW);
          digitalWrite(7, LOW);
          digitalWrite(6, LOW);
          digitalWrite(5, LOW);
          }
        }
        else if (RXdata.equals("1")){
          if(!ble_status){            
            ble_status = 1;            
          digitalWrite(9, HIGH);
          digitalWrite(8, HIGH);
          digitalWrite(7, HIGH);
          digitalWrite(6, HIGH);
          digitalWrite(5, HIGH);
          }
        }
        else if (RXdata.equals("2"))
        {
          Serial2.print(0);
          con_stat = 0;
          requestcon = 0;
        }
        else;
        RXdata = "";
      }
    }
  }
  else
  {
    conn = checkifconnected();
    if (!conn) {                                                            
      if (readyRole1) {
        Serial2.print("AT");
        delay(50);
        Serial2.print("AT+ROLE1");
        readyRole1 = 0;                                                     
        ATROLE1=1;
      }
      if (currentTime - previousTime > 3000 && ATROLE1) {
        previousTime = currentTime;
        Serial2.print("AT+DISI?");
        delay3s = 1;
        ATROLE1 = 0;
      }
      if (currentTime - previousTime > 5000 && delay3s) {
        previousTime = currentTime;
        delay3s=0;     
        Serial2.print("AT+ROLE0");        
        DISCE=1;        
      }      
      if (currentTime - previousTime > 10000 && DISCE) {
        previousTime = currentTime;
        DISCE = 0;
        readyRole1 = 1;
        RXdata = "";
      }      
    }
  }
}

void TurnOffDelay(){
  digitalWrite(9, LOW);
}

int checkifconnected() {
  RXdata.toCharArray(buff, 500);
  if (strstr(buff, "OK+CONN") != NULL) {
    requestcon = 1;
    RXdata = "";
  }
  else {
    requestcon = 0;
  }
  delay(100);
  return requestcon;
}

int checkifdisconnected() {
  RXdata.toCharArray(buff, 500);
  if (strstr(buff, "OK+LOST") != NULL) {
    requestcon = 0;
    DISCE = 0;
    delay3s = 0;
    ATROLE1 = 0;
    con_stat = 0;
    readyRole1=1;

    if(ble_status){
      ble_status = 0;
      digitalWrite(9, LOW);
    }
  }
  else requestcon = 1;
  delay(100);
  return requestcon;

}

void CheckBLEBuffer() {
  int temp;  
  while (Serial2.available()) {
    //delay(50);
    temp = Serial2.read();
    RXdata += (char)temp;
  }
  
  RXdata.toCharArray(buff, 500); 
  Serial.println(RXdata); 
  if(strstr(buff, "DISCE") != NULL){
    if (strstr(buff, "74DAEAB326BC") != NULL){      
      if(!flagonPowerstatus){
        flagonPowerstatus=1;
        flagoffPowerstatus=0;    
        if(!ble_status){
          ble_status = 1;
          digitalWrite(9, HIGH);
          digitalWrite(8, HIGH);
          digitalWrite(7, HIGH);
          digitalWrite(6, HIGH);
          digitalWrite(5, HIGH);
        }
      }
    }   
    else{         
      if(!flagoffPowerstatus){
        flagoffPowerstatus=1;
        flagonPowerstatus=0;   
        if(ble_status){
          ble_status = 0;
          digitalWrite(9, LOW);
          digitalWrite(8, LOW);
          digitalWrite(7, LOW);
          digitalWrite(6, LOW);
          digitalWrite(5, LOW);
        } 
      }             
    } 
  }         
}

void clearBuffer() {
  while (Serial2.available()) {
    Serial2.read();
  }
}
