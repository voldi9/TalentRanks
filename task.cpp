#include "task.hpp"

task::~task()
{
	map_tasks.erase(id);
}

task::task()
{
	pointed = 1;
}

task::task(int this_id)
{
	id = this_id;
	pointed = 1;
}