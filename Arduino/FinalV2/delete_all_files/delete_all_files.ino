#include <SPI.h>
#include <SD.h>

File file_ptr;

void setup(){
	pinMode(10, OUTPUT);
	digitalWrite(10, HIGH);

	if(!SD.begin(10))
		return;

	String day_abr[7] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat"};
	int day_count = 0;
	int device_count = 0;
	for(int x=0; x<7; x++){
		for(int y=0; y<5; y++){
			if(SD.exists(day_abr[x]+"/"+String(y)+".txt"))
				SD.remove(day_abr[x]+"/"+String(y)+".txt");
		}
	}
}

void loop(){
  
}

