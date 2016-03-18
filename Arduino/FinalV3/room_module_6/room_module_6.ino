#include <Time.h>
#include <TaskScheduler.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>

/********* TASK Functions **********/
void CheckXBeeBuffer();
void RequestDateTimeSync();
void CheckScheduleInManual();
void CheckWhileInAuto();
void PrintDateTime();
void TurnOnRoomDevicePorts();
void CalculateCurrent();

/* BLE */
void CheckBLEBuffer();
void BLEFunction();
void TurnOffDelay();
/************************/

/******************* Functions ********************/
void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length);
void SyncDateTime(int xbee_data_int[], int xbee_data_length);
void ChangeRoomDevicePorts(int xbee_data_int[], int xbee_data_length);
void ChangeRoomDeviceStatus(int xbee_data_int[], int xbee_data_length);
void TurnOffRoomDevicePorts();
void SaveRoomSchedule(int xbee_data_int[], int xbee_data_length);
float getVPP();
void SendConsumption();
void SendRoomDeviceStatus();

/* BLE */
int checkifdisconnected();
int checkifconnected();
void getRXdata();
void clearBuffer();
/**************************************************/

/***************** TASKS ******************/
Task tsk1(50, TASK_FOREVER, &CheckXBeeBuffer);
Task tsk3(3000, TASK_FOREVER, &RequestDateTimeSync);
Task tsk4(60000, TASK_FOREVER, &CheckScheduleInManual);
Task tsk5(60000, TASK_FOREVER, &CheckWhileInAuto);
Task tem(1000, TASK_FOREVER, &PrintDateTime);
Task tsk6(500, TASK_FOREVER, &TurnOnRoomDevicePorts);
Task tsk7(1000, TASK_FOREVER, &CalculateCurrent);

/* BLE */
Task ble_tsk1(50, TASK_FOREVER, &CheckBLEBuffer);
Task ble_tsk2(500, TASK_FOREVER, &BLEFunction);
//Task ble_tsk3(3000, 1, &TurnOffDelay);
/******************************************/

/******* GLOBALS ******/
String xbee_data;

Scheduler runner;

int datetime_sync_flag = 0;
int set_room_device_status[5];

File file_ptr;

const int ADCread = A7;
int ACSSensitivity = 100; // use 100 for 20A Module and 66 for 30A Module
double VoltageP2P = 0;
double Vrms = 0;
double Irms = 0;

double current_consump = 0;

/* BLE */
String RXdata;
char buff[500];
int data, x, conn, con_stat, requestcon, DISCE, ATROLE1, readyRole1, delay3s,flagonPowerstatus,flagoffPowerstatus,notdiscovered;
long previousTime = 0;

int ble_status = 0;

int status_value = 0;
int prev_status_value = 0;
/**********************/

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

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);

  runner.init();

  runner.addTask(tsk1);
  runner.addTask(tsk3);
  runner.addTask(tsk4);
  runner.addTask(tsk5);
  runner.addTask(tsk6);
  runner.addTask(tsk7);
  runner.addTask(tem);
  runner.addTask(ble_tsk1);
  runner.addTask(ble_tsk2);

  tsk1.enable();
  tsk3.enable();
  tem.enable();
  ble_tsk1.enable();
  ble_tsk2.enable();

  pinMode(53, OUTPUT);
  digitalWrite(53, HIGH);

  if(!SD.begin(53))
    return;

  clearBuffer();

  memcpy(set_room_device_status, 0, 5);
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

void RequestDateTimeSync(){
  Serial1.write(0x02);
}

void CheckScheduleInManual(){
  int time_now_sum = month()+day()+year()+(hour() * 60)+minute();
  int sent_time_sum = EEPROM.read(26)+EEPROM.read(27)+((EEPROM.read(28) << 8) + EEPROM.read(29))+(EEPROM.read(30)*60)+EEPROM.read(31);

  if(time_now_sum==sent_time_sum){
    EEPROM.update(25, 0);

    TurnOffRoomDevicePorts();

    tsk4.disable();

    tsk5.enable();
  }
}

void CheckWhileInAuto(){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};

  for(int x=0; x<(EEPROM.read(1)+1); x++){
    String file_path = day_abr[weekday()-1]+"/"+String(x+1)+".txt";

    file_ptr = SD.open(file_path, FILE_READ);
    if(file_ptr){
      int time_now = (hour() * 60) + minute();

      file_ptr.seek((time_now/8));
      byte val = file_ptr.peek();

      set_room_device_status[x] = (val >> ((time_now%8)-1)) & 1;

      Serial.println(set_room_device_status[x]);

      file_ptr.close();
    }
    else
      set_room_device_status[x] = 0;
  }
}

void TurnOnRoomDevicePorts(){
  for(int x=0; x<EEPROM.read(1)+1; x++)
    status_value ^= (-EEPROM.read(35+x) ^ status_value) & (1 << x);

  if(status_value!=prev_status_value){
    SendRoomDeviceStatus();
    SendConsumption();
  }

  prev_status_value = status_value;

  if(ble_status){
    int status_flag = 0;
    for(int x=0; x<EEPROM.read(1)+1; x++){
      if(set_room_device_status[x]){
        status_flag = 1;
        break;
      }
    }

    if(status_flag){
      tsk7.enableIfNot();

      digitalWrite(EEPROM.read(0), set_room_device_status[0]);
      EEPROM.update(35, set_room_device_status[0]);
    
      for(int x=0; x<EEPROM.read(1); x++){
        digitalWrite(EEPROM.read(2+x), set_room_device_status[1+x]);
        EEPROM.update(36+x, set_room_device_status[1+x]);
      }
    }
    else
      tsk7.disable();
  }
  else
    TurnOffRoomDevicePorts();
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
          if(ble_status)
            ble_status = 0;
        }
        else if (RXdata.equals("1")){
          if(!ble_status)
            ble_status = 1;
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

void CheckBLEBuffer() {
  int temp;  
  while (Serial2.available()) {
    delay(50);
    temp = Serial2.read();
    RXdata += (char)temp;
  }
  
  RXdata.toCharArray(buff, 500);  
  if(strstr(buff, "DISCE") != NULL){
    if (strstr(buff, "20C38FF67F85") != NULL){      
      if(!flagonPowerstatus){
        flagonPowerstatus=1;
        flagoffPowerstatus=0;    
        if(!ble_status){
          ble_status = 1;

          TurnOnRoomDevicePorts();
        }
      }
    }   
    else{         
      if(!flagoffPowerstatus){
        flagoffPowerstatus=1;
        flagonPowerstatus=0;   
        if(ble_status){
          ble_status = 0;

          TurnOffRoomDevicePorts();
        } 
      }             
    } 
  }         
}

void CalculateCurrent(){
  VoltageP2P = getVPP();
  Vrms = (VoltageP2P/2.0) *0.7071; 
  Irms = ((Vrms * 1000)/ACSSensitivity)-.11;
  if(Irms<0)Irms=0.0;

  current_consump += Irms;
}

void PerformXBeeOperation(int xbee_data_int[], int xbee_data_length){
  if(xbee_data_int[0]==3)
    SyncDateTime(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==9 && datetime_sync_flag)
    ChangeRoomDevicePorts(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==1 && datetime_sync_flag)
    ChangeRoomDeviceStatus(xbee_data_int, xbee_data_length);
  else if((xbee_data_int[0]==6 || xbee_data_int[0]==10) && datetime_sync_flag)
    SaveRoomSchedule(xbee_data_int, xbee_data_length);
  else if(xbee_data_int[0]==14)
    digitalWrite(5, HIGH);
}

void SyncDateTime(int xbee_data_int[], int xbee_data_length){
  int hour_now = xbee_data_int[1];
  int minute_now = xbee_data_int[2];
  int second_now = xbee_data_int[3];
  int day_now = xbee_data_int[4];
  int month_now = xbee_data_int[5];
  int year_now = ((xbee_data_int[6] << 8) + xbee_data_int[7]);

  setTime(hour_now, minute_now, second_now, day_now, month_now, year_now);

  datetime_sync_flag = 1;

  tsk6.enable();

  tsk3.disable();
  runner.deleteTask(tsk3);

  Serial1.print(String(hour())+" "+String(minute())+" "+String(second())+" "+String(day())+" "+String(month())+" "+String(year()));
  delay(50);

  if(!EEPROM.read(25))
    tsk5.enable();
  else
    tsk4.enable();
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

void ChangeRoomDeviceStatus(int xbee_data_int[], int xbee_data_length){
  int time_now_sum = month()+day()+year()+(hour() * 60)+minute();
  int sent_time_sum = xbee_data_int[1]+xbee_data_int[2]+((xbee_data_int[3] << 8) + xbee_data_int[4])+((xbee_data_int[5]*60)+xbee_data_int[6]);

  if(time_now_sum<sent_time_sum){
    EEPROM.update(25, 1);
    EEPROM.update(26, xbee_data_int[1]);
    EEPROM.update(27, xbee_data_int[2]);
    EEPROM.update(28, xbee_data_int[3]);
    EEPROM.update(29, xbee_data_int[4]);
    EEPROM.update(30, xbee_data_int[5]);
    EEPROM.update(31, xbee_data_int[6]);

    set_room_device_status[0] = xbee_data_int[7];
    for(int x=0; x<xbee_data_int[8]; x++)
      set_room_device_status[1+x] = xbee_data_int[9+x];

    CheckScheduleInManual();

    tsk5.disable();

    tsk4.enable();
  }
}

void TurnOffRoomDevicePorts(){
  digitalWrite(EEPROM.read(0), 0);
  EEPROM.update(35, 0);

  for(int x=0; x<EEPROM.read(1); x++){
    digitalWrite(EEPROM.read(2+x), 0);
    EEPROM.update(36+x, 0);
  }
}

void PrintDateTime(){
  Serial.println(String(hour())+":"+String(minute())+":"+String(second())+" "+String(day())+" "+String(month())+" "+String(year()));
}

void SaveRoomSchedule(int xbee_data_int[], int xbee_data_length){
  String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
  String file_path = day_abr[xbee_data_int[1]-1]+"/"+String(xbee_data_int[2])+".txt";

  byte file_data[180];  
  for(int x=0; x<180; x++)
    file_data[x] = 0;

  if(SD.exists(file_path)){
    if(xbee_data_int[0]==6){
      file_ptr = SD.open(file_path, FILE_READ);
      if(file_ptr){
        int x=0;
        while(file_ptr.available())
          file_data[x++] = file_ptr.read();

        file_ptr.close();
      }
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

      TurnOffRoomDevicePorts();
    }
  }
  else requestcon = 1;
  delay(100);
  return requestcon;
}

void clearBuffer() {
  while (Serial2.available()) {
    Serial2.read();
  }
}

float getVPP()
{
  float result;  
  int AnalogRead;             
  int Vplus = 512;        
  int Vminus = 512;       
  
  unsigned long start_time = millis();
  while((millis()-start_time) < 1000) //sample for 1 Sec
  {
    AnalogRead = analogRead(ADCread);

    if (AnalogRead > Vplus) 
    {
      Vplus = AnalogRead;
    }
    if (AnalogRead < Vminus) 
    {
      Vminus = AnalogRead;
    }
  }
  result = ((Vplus - Vminus) * 5)/1024.0;
  return result;
}

void SendConsumption(){
  byte consumption_data[sizeof(current_consump)+3];
  consumption_data[0] = 0x05;
  consumption_data[1] = (byte)EEPROM.read(12);
  consumption_data[2] = sizeof(current_consump);

  byte data[sizeof(current_consump)];
  (String(current_consump)).getBytes(data, sizeof(current_consump));

  for(int x=0; x<sizeof(data); x++)
    consumption_data[3+x] = data[x];

  Serial1.write(consumption_data, sizeof(consumption_data));
  delay(50);

  current_consump = 0;
}

void SendRoomDeviceStatus(){
  unsigned char device_status[EEPROM.read(1)+3];
  device_status[0] = 0x07;
  device_status[1] = EEPROM.read(12);
  device_status[2] = EEPROM.read(35);
  for(int x=0; x<EEPROM.read(1); x++)
    device_status[x+3] = EEPROM.read(36+x);

  Serial1.write(device_status, EEPROM.read(1)+3);
  delay(50);
}
