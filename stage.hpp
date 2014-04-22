#ifndef stage_hpp
#define stage_hpp
#include "main.hpp"

class stage : public ranking
{
	public:
		contest * c;
		vector <int> round_ids;
		stage();
		stage(int this_id);
		~stage();
};

#endif