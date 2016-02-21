#include "include.h"

void ReadDataInBuffer(){
	int delPos = 0;
	int queueLen = 0;

	while(xbee_serial.available() > 0){
		unsigned char in = (unsigned char)xbee_serial.read();
		if(!RxQ.Enqueue(in))
			break;
	}

	queueLen = RxQ.Size()
	for(int x=0; x<queueLen; x++){
		if(RxQ.Peek(x)==0x7E){
			unsigned char checkBuff[Q_SIZE];
			unsigned char msgBuff[Q_SIZE];
			int checkLen = 0;
			int msgLen = 0;
			int data_start_index = 0;

			checkLen = RxQ.Copy(checkBuff, x);
			msgLen = xbee.Receive(checkBuff, checkLen, msgBuff);

			if(msgLen > 0){
				data_start_index = ((int)msgBuff[9]) + 13;

				if(msgBuff[9]==msgBuff[10]){
					if(msgBuff[8]==0x01 || msgBuff==0x02){
						if(msgBuff[8]==0x01)
							manual_control_flag = 1;
						else
							manual_control_flag = 0;

						ChangeRoomStatus(msgBuff, data_start_index);
					}
					else if(msgBuff[8]==0x03){
						SyncRoomDateTime(msgBuff, data_start_index);
					}
					else if(msgBuff[8]==0x04){
						// not yet coded
						RequestConsumption(msgBuff);
					}
					else if(msgBuff[8]==0x07)
						ARPRequest(msgBuff);
					else if(msgBuff[8]==0x09)
						ScheduleFunction(msgBuff);
					else if(msgBuff[8]==0x0A)
						CheckRoomStatus(msgBuff, data_start_index);
					else if(msgBuff[8]==0x0C)
						ChangePort(msgBuff, data_start_index);

					switch(msgBuff[8]){
						case 0x01:
						case 0x02:
						case 0x03:
						case 0x04:
						case 0x09:
						case 0x0C:
							SendAcknowledgement(msgBuff);
							break;
						default:
							break;
					}
				}
				else
					SendFrameToOtherModules(msgBuff);

				x += msgLen;
				delPos = x;
			}
			else{
				if(x>0)
					delPos = x-1;
			}
		}
	}

	RxQ.Clear(delPos);
}