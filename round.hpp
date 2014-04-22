#ifndef round_hpp
#define round_hpp
#include "main.hpp"

class round_ : public ranking
{
	public:
		stage * s;
		vector <int> problem_ids;
		round_();
		round_(int this_id);
		~round_();
};

#endif