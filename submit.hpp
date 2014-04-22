#ifndef submit_hpp
#define submit_hpp
//#include "submit.cpp"
#include "main.hpp"

class submit
{
	public:
		int id;
		int pointed; //how many pointers to this submit are being used
		float speed_factor, nerve_factor;
		int total_points;
		time_t created;
		solver * solved;
		round_ * from_round;
		task * problem;
		submit(pqxx::result::tuple sub);
		~submit();//if(problem != NULL) delete problem; 
					//if(solved != NULL) delete solved; }
};

#endif