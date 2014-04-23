#include "result.hpp"

void result::print()
{
	printf("%d   ", id);
	for(int i=0; i<points.size(); i++)
		printf("%d ", points[i]);
	puts("");
	return;
}

result::~result(){ points.clear(); }

result::result()
{
	pointed = 1;
	points.clear();
}

result::result(int this_id, int num_problems)
{
	id = this_id;
	points.clear();
	points.resize(num_problems, 0);
	pointed = 1;
}