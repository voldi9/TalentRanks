#include "solver.hpp"

solver::~solver()
{
	map_solvers.erase(id);
}

solver::solver()
{
	pointed = 1;
}

solver::solver(int this_id)
{
	id = this_id;
	pointed = 1;
}