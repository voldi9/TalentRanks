#ifndef result_hpp
#define result_hpp
//#include "result.cpp"
#include "main.hpp"

class result
{
	public:
		int pointed; //how many pointers to this result are being used
		solver * solved;
		vector <short> points;
		result();
		~result();
};


#endif