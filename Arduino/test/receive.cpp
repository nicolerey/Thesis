#include <SoftwareSerial.h>

SoftwareSerial xbeeSerial(8, 7);

void setup(){
	xbeeSerial.begin(9600);
	Serial.begin(9600);

	pinMode(9, OUTPUT);
}

void loop(){
	if(xbeeSerial.available())
		Serial.print(xbeeSerial.read());
}