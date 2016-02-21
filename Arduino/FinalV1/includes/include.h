#ifndef INCLUDE_H
#define INCLUDE_H

#include "XBee.h"
#include "queue.h"
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <Time.h>
#include <DateTimeString.h>
#include "queue.h"
#include "XBee.h"
#include "schedule.h"

XBee xbee;
Queue RxQ;
Schedule *sched;
SoftwareSerial xbee_serial(0, 1);
int manual_control_flag;

void ReadDataInBuffer();
void ChangeRoomStatus(unsigned char *msgBuff, int start_index);
void SyncRoomDateTime(unsigned char *msgBuff, int start_index);
void SendAcknowledgement(unsigned char *msgBuff);
void ChangePort(unsigned char *msgBUff, int data_start_index);
void ARPRequest(unsigned char *msgBuff);

#endif