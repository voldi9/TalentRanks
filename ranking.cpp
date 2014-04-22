#include "ranking.hpp"

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