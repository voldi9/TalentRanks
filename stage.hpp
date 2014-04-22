#ifndef stage_hpp
#define stage_hpp
#include "main.hpp"

class stage : public ranking
{
	public:
		int id;
		contest * c;
		//vector <int> round_ids;
		int add_user(solver * solved);
		stage();
		stage(int this_id);
		~stage();
};

#endif