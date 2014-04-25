#ifndef task_hpp
#define task_hpp
#include "main.hpp"

class task
{
	public:
		int id;
		int pointed; //how many pointers to this task are being used
		float dynamic_factor, speed_factor;
		task();
		task(int this_id);
		~task();
};

#endif