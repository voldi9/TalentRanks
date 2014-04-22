#ifndef contest_hpp
#define contest_hpp
#include "main.hpp"

class contest : public ranking
{
	public:
		vector <int> stage_ids;
		contest();
		contest(int this_id);
		~contest();
};

#endif