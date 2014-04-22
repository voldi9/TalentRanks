#ifndef solver_hpp
#define solver_hpp
//#include "solver.cpp"
#include "main.hpp"

class solver
{
	public:
		int id;
		int pointed; //how many pointers to this solver are being used
		float team_factor;
		//string name;
		solver();
		solver(int this_id);
		~solver();
};

#endif