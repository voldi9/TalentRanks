#ifndef round_hpp
#define round_hpp
#include "main.hpp"

class round_ : public ranking
{
	public:
		int id;
		stage * s;
		//vector <int> problem_ids;
		int add_user(solver * solved);
		//void update(submit * sub);
		round_();
		round_(int this_id);
		~round_();
};

#endif