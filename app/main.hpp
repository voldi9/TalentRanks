#ifndef main_hpp
#define main_hpp
#include "shared.hpp"
#include <iostream>
#include <pqxx/pqxx>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <map>
#include <set>

class group;
class contest;
class stage;
class round_;
class ranking;
class solver;
class task;
class submit;
class result;

map <int, round_*> map_rounds; //might wanna change the key type to long long in the future
map <int, stage*> map_stages;
map <int, contest*> map_contests;
map <int, solver*> map_solvers;
map <int, task*> map_tasks;
map <int, submit*> map_submits;

pqxx::connection * database;
pqxx::connection * rankbase;

void update_submit(pqxx::result::tuple submit);

void add_solver(solver * s);
void add_problem(task * t);
int add_contest(contest * c);
int add_stage(stage * s);
int add_round(round_ * r);
int check_if_rank_in_base(group * r);
void handle(submit * sub, string status);
int pickup_ranking(ranking * r);
int pickup_round(int id);
//function keeps monitoring the changes in the submits database table
int monitor();
int main();

#include "task.cpp"
#include "result.cpp"
#include "solver.cpp"
#include "group.cpp"
#include "contest.cpp"
#include "stage.cpp"
#include "round.cpp"
#include "submit.cpp"
#include "ranking.cpp"
#include "utils.cpp"
#endif