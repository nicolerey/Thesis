#include "include.h"
#include <string.h>

Schedule::Schedule(){
	total_frames(0),
	last_frame_number(0),
	array_index(0);
}

void Schedule::InsertToArray(unsigned char *val, int msgLen){
	for(int x=0; x<msgLen; array_index++)
		schedule_array[array_index] = val[x];
}