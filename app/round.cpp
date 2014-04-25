#include "round.hpp"
/*
void round_::update(submit * sub)
{
	result * row = map_ids[sub->solved->id];
	for(vector<int>::iterator it = lower_ids.begin(); it != lower_ids.end(); it++)
	{
		if((*it) == sub->problem->id)
		{
			row->points[it - lower_ids.begin()] = sub->total_points;
			break;
		}
	}
}
*/
int round_::add_user(solver * solved)
{
	if(map_ids.find(solved->id) == map_ids.end()) //adding new contestant to this ranking
	{
		printf("lower_num(): %d\n", lower_num());
		result * res = new result(solved->id, lower_num());
		rows.push_back(res);
		map_ids.insert(make_pair(solved->id, res));
		if(s)
			s->add_user(solved);
	}
	//updating old contestant's points
	return 0;
}

round_::~round_()
{
	map_rounds.erase(id);
	//if(s)
	//	delete s;
}

round_::round_()
{
	s = 0;
	rows.clear();
	map_ids.clear();
	lower_ids.clear();
}

round_::round_(int this_id)
{
	s = 0;
	id = this_id;
	pointed = 1;
	map_ids.clear();
	lower_ids.clear();
	rows.clear();
}