#include "includes/include.h"

void setup(void){
	manual_control_flag = 0;
	xbee_serial.begin(9600);

	pinMode(EEPROM.read(2), OUTPUT);
	for(int x=0; x<EEPROM.read(3); x++)
		pinMode(EEPROM.read(x+4), OUTPUT);

	sched.schedule_array = new (nothrow) Schedule [(EEPROM.read(2)+1)*180];
}

void loop(void){
	if(xbee_serial.available() > 0)
		ReadDataInBuffer();
}