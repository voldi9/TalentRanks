#include "stage.hpp"

int stage::add_user(solver * solved)
{
	if(map_ids.find(solved->id) == map_ids.end()) //adding new contestant to this ranking
	{
		rows.push_back(result(solved->id, lower_num()));
		result * res = &rows[rows.size()-1];
		map_ids.insert(make_pair(solved->id, res));
		if(c)
			c->add_user(solved);
	}
	//updating old contestant's points
	return 0;
}

stage::~stage()
{
	map_stages.erase(id);
}

stage::stage()
{
	rows.clear();
	map_ids.clear();
	lower_ids.clear();
}

stage::stage(int this_id)
{
	id = this_id;
	pointed = 1;
	map_ids.clear();
	lower_ids.clear();
	rows.clear();
}