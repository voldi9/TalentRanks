#include "result.hpp"

void result::print()
{
	printf("%d   ", id);
	for(int i=0; i<points.size(); i++)
	{
		if(points[i]<100)
			printf(" ");
		if(points[i]<10)
			printf(" ");
		printf("%d ", points[i]);
	}
	printf("  |  %d\n", sum);
	return;
}

result::~result(){ points.clear(); }

result::result()
{
	pointed = 1;
	sum = 0;
	points.clear();
}

result::result(int this_id, int num_problems)
{
	id = this_id;
	points.clear();
	points.resize(num_problems, 0);
	pointed = 1;
	sum = 0;
}