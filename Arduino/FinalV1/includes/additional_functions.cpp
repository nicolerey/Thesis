#include "include.h"

void ChangeRoomStatus(unsigned char *msgBuff, int start_index){
	digitalWrite(EEPROM.read(2), msgBuff[start_index++]);
	for(int x=0; x<EEPROM.read(3); x++){
		digitalWrite(EEPROM.read(x+4), msgBuff[start_index+x]);
	}
}

void SyncRoomDateTime(unsigned char *msgBuff, int start_index){
	int hour, minute, second, day, month, year;
	hour = msgBuff[start_index];
	minute = msgBuff[start_index+1];
	second = msgBuff[start_index+2];
	day = msgBuff[start_index+3];
	month = msgBuff[start_index+4];
	year = (msgBuff[start_index+5] << 8) | (msgBuff[start_index+6] & 0xFF);

	setTime(hour, minute, second, day, month, year);
}

void SendAcknowledgement(unsigned char *msgBuff){
	int len, frameLen;
	unsigned char *msg, outFrame;

	len = 5 + msgBuff[9];
	msg = new (nothrow) unsigned char [len];
	msg[0] = 0x06;
	for(int x=1; x<len; x++)
		msg[x] = msgBuff[8+x];
	msg[2] -= 1;

	frameLen = xbee.Send(msg, len, outFrame, msg[len-4], msg[len-3]);
	xbee_serial.write(outFrame, frameLen);
}

void SendFrameToOtherModules(unsigned char *msgBuff){
	int msgLen = msgBuff[2] - 5, frameLen;
	unsigned char *msg, outFrame;

	msg = new (nothrow) unsigned char [msgLen];
	for(int x=0; x<msgLen; x++)
		msg[x] = msgBuff[8+x];
	msg[2] += 1;

	frameLen = xbee.Send(msg, msgLen, outFrame, msg[msg[2]+2], msg[msg[2]+3]);
	xbee_serial.write(outFrame, frameLen);
}

void ChangePort(unsigned char *msgBuff, int data_start_index){
	EEPROM.write(1, msgBuff[data_start_index++]);

	for(int x=0; x<msgBuff[data_start_index++]; x++)
		EEPROM.write(x+3, msgBuff[data_start_index++]);
}

void ARPRequest(unsigned char *msgBuff){
	int exist_flag = 0, msgLen = msgBuff[2] - 5, frameLen;
	unsigned char *msg, outFrame;

	msg = new (nothrow) unsigned char [msgLen];

	if(msgBuff[10]<msgBuff[9]){
		for(int x=0; x<msgBuff[10]; x++){
			if(msgBuff[11+x]==EEPROM.read(0)){
				exist_flag = 1;
				break;
			}
		}

		if(exist_flag==0){
			msgBuff[11+msgBuff[10]] = EEPROM.read(0);
			msgBuff[10] += 1;
		}
	}

	if(msgBuff[9]==msgBuff[10]){
		msgBuff[8] = 8;
		msgBuff[10] -= 1;
	}

	for(int x=0; x<msgLen; x++)
		msg[x] = msgBuff[8+x];

	if(msgBuff[8]==8)
		frameLen = xbee.Send(msg, msgLen, outFrame, msg[(msg[2]*3)-(msg[2]-1)], msg[((msg[2]*3)-(msg[2]-1))+1]);
	else
		frameLen = xbee.Send(msg, msgLen, outFrame, 255, 255);

	xbee_serial.write(outFrame, frameLen);
}

void CheckRoomStatus(unsigned char *msgBuff, int data_start_index){
	int port_count = EEPROM.read(2)+1, frameLen, msgLen = msgBuff[2] - 5, val;
	unsigned char *msg, outFrame;

	data_start_index++;
	for(int x=0; x<port_count; x++){
		if(x==0){
			if(EEPROM.read(1)==15)
				val = bitRead(PORTB, 15);
			else
				val = bitRead(PORTF, EEPROM.read(1));
			msgBuff[data_start_index+x] = val;
		}
		else{
			if(EEPROM.read(x+2)==15)
				val = bitRead(PORTB, 15);
			else
				val = bitRead(PORTF, EEPROM.read(x+2));
			msgBuff[data_start_index+x] = val;
		}
	}

	msgBuff[data_start_index] = 11;

	for(int x=0; x<msgLen; x++)
		msg[x] = msgBuff[8+x];

	msg[2] -= 1;
	frameLen = xbee.Send(msg, msgLen, outFrame, msg[(msg[2]*3)-(msg[2]-1)], msg[((msg[2]*3)-(msg[2]-1))+1]);
	xbee_serial.write(outFrame, frameLen);
}

void ScheduleFunction(unsigned char *msgBuff){
	int msgLen = msgBuff[2] - (11 + (msgBuff[9]*2));
	unsigned char *msg;

	if(sched.total_frames==0)
		sched.total_frames = msgBuff[(msgBuff[9]*2)+12];

	sched.last_frame_number = msgBuff[(msgBuff[9]*2)+13];

	for(int x=0; x<msgLen; x++)
		msg[x] = msgBuff[14+(msgBuff[9]*2)+x];

	sched.InsertToArray(msg, msgLen);

	if(sched.total_frames==sched.last_frame_number){
		for(int x=0; x<((EEPROM.read(2)+1)*180); x++)
			EEPROM.write(3+EEPROM.read(2)+x, sched.schedule_array[x]);
	}
}