#include "contest.hpp"

contest::~contest()
{
	map_contests.erase(id);
}

contest::contest()
{
	rows.clear();
	stage_ids.clear();
}

contest::contest(int this_id)
{
	id = this_id;
	pointed = 1;
}