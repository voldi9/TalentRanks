#include "stage.hpp"

stage::~stage()
{
	map_stages.erase(id);
}

stage::stage()
{
	rows.clear();
	round_ids.clear();
}

stage::stage(int this_id)
{
	id = this_id;
	pointed = 1;
}