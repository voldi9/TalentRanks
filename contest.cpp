#include "contest.hpp"


int contest::add_user(solver * solved)
{
	if(map_ids.find(solved->id) == map_ids.end()) //adding new contestant to this ranking
	{
		rows.push_back(result(solved->id, lower_num()));
		result * res = &rows[rows.size()-1];
		map_ids.insert(make_pair(solved->id, res));
	}
	//updating old contestant's points
	return 0;
}


contest::~contest()
{
	map_contests.erase(id);
}

contest::contest()
{
	rows.clear();
	lower_ids.clear();
	map_ids.clear();
}

contest::contest(int this_id)
{
	id = this_id;
	pointed = 1;
	map_ids.clear();
	rows.clear();
	lower_ids.clear();
}