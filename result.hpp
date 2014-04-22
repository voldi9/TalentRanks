#ifndef result_hpp
#define result_hpp
//#include "result.cpp"
#include "main.hpp"

class result
{
	public:
		int id;
		int pointed; //how many pointers to this result are being used
		//solver * solved;
		vector <short> points;
		result();
		result(int this_id, int num_problems);
		~result();
};


#endif