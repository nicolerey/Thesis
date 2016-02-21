#ifndef Schedule_H
#define Schedule_H

class Schedule{
private:
	int total_frames;
	int last_frame_number;
	unsigned char *schedule_array;
	int array_index;

public:
	Schedule();
	void InsertToArray(int sched);
};

#endif