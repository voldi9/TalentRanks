#include "stage.hpp"

int stage::add_user(solver * solved)
{
	if(map_ids.find(solved->id) == map_ids.end()) //adding new contestant to this ranking
	{
		result * res = new result(solved->id, lower_num());
		rows.push_back(res);
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
	c = 0;
	rows.clear();
	map_ids.clear();
	lower_ids.clear();
}

stage::stage(int this_id)
{
	c = 0;
	id = this_id;
	pointed = 1;
	map_ids.clear();
	lower_ids.clear();
	rows.clear();
}