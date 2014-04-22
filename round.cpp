#include "round.hpp"

round_::~round_()
{
	map_rounds.erase(id);
	if(s)
		delete s;
}

round_::round_()
{
	rows.clear();
	problem_ids.clear();
}

round_::round_(int this_id)
{
	id = this_id;
	pointed = 1;
}