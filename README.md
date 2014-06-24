A single field in column named with problem/round/stage id in each table from ranking database indicates as follows:

field_value = MULTIPL * partial_points + total_points

right now MULTIP = 100000 //thus it's only safe to have no more than 999 problems in a contest

this way, one can easily access both total_points value as well as partial_points value by fetching just one integer.

Assumption: every round, stage and contest is also a ranking.

"lower" is a conventional name for a ranking lower in hierarchy (or problem for a round), that is:

contest's lower is a stage that belongs to this contest,
stages's lower is a round that belongs to this stage,
round's lower is a problem that belongs to this round.

