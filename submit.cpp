#include "submit.hpp"
#define SUBMIT_ID 0
#define SUBMIT_CREATED 1
#define SUBMIT_ROUND_ID 2
#define SUBMIT_USER_ID 3
#define SUBMIT_PROB_ID 4
#define SUBMIT_STATUS 5
#define SUBMIT_POINTS 6


submit::~submit()
{
	map_submits.erase(id);
	if(solved)
		delete solved;
	if(from_round)
		delete from_round;
	if(problem)
		delete problem;
}

submit::submit(pqxx::result::tuple sub)
{
	pointed = 1;
	id = sub[SUBMIT_ID].as<int>();
	struct tm when = {0};
	strptime(sub[SUBMIT_CREATED].as<string>().c_str(), "%Y-%m-%d %H:%M:%S", &when);
	created = mktime(&when);

	solved = 0;
	from_round = 0;
	problem = 0;

	if(!sub[SUBMIT_POINTS].is_null())
		total_points = sub[SUBMIT_POINTS].as<int>();

	if(!sub[SUBMIT_USER_ID].is_null())
	{
		if(map_solvers.find(sub[SUBMIT_USER_ID].as<int>()) != map_solvers.end()){
			solved = map_solvers[sub[SUBMIT_USER_ID].as<int>()]; printf("stary\n"); }
		else
		{
			printf("nowy\n");
			solved = new solver(sub[SUBMIT_USER_ID].as<int>());
			add_solver(sub[SUBMIT_USER_ID].as<int>(), solved);
		}
		solved->pointed++;
	}

	if(!sub[SUBMIT_ROUND_ID].is_null())
	{
		if(map_rounds.find(sub[SUBMIT_ROUND_ID].as<int>()) != map_rounds.end())
			from_round = map_rounds[sub[SUBMIT_ROUND_ID].as<int>()];
		else
		{
			from_round = new round_(sub[SUBMIT_ROUND_ID].as<int>());
			add_round(sub[SUBMIT_ROUND_ID].as<int>(), from_round);
		}
		from_round->pointed++;
	}

	if(!sub[SUBMIT_PROB_ID].is_null())
	{
		if(map_tasks.find(sub[SUBMIT_PROB_ID].as<int>()) != map_tasks.end())
			problem = map_tasks[sub[SUBMIT_PROB_ID].as<int>()];
		else
		{
			problem = new task(sub[SUBMIT_PROB_ID].as<int>());
			add_problem(sub[SUBMIT_PROB_ID].as<int>(), problem);
		}
		problem->pointed++;
	}
}