#include "ranking.hpp"

void ranking::print()
{
	for(int i=0; i<rows.size(); i++)
		rows[i]->print();
	puts("");
}

int ranking::lower_num()
{
	return lower_ids.size();
}
/*
void ranking::update(int solver_id, int lower_id, int total_points)
{
	result * res = map_ids[solver_id];
	for(vector<int>::iterator it = lower_ids.begin(); it != lower_ids.end(); it++)
	{
		if((*it) == lower_id)
		{
			int old_points = res->points[it - lower_ids.begin()];
			res->points[it - lower_ids.begin()] = total_points;
			res->sum += (total_points - old_points);
			break;
		}
	}
}
*/
ranking::~ranking(){ rows.clear(); }

ranking::ranking()
{
	pointed = 1;
	last_submit = 0;
	map_ids.clear();
	rows.clear();
}