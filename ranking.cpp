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

ranking::~ranking(){ rows.clear(); }

ranking::ranking()
{
	pointed = 1;
	map_ids.clear();
	rows.clear();
}