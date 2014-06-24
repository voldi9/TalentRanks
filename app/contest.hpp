#ifndef contest_hpp
#define contest_hpp
#include "main.hpp"

class contest : public group
{
	public:
		//vector <int> stage_ids;
		int add_user(solver * solved);
		contest();
		contest(int this_id);
		~contest();
};

#endif