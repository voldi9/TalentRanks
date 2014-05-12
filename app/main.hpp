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
#define dbparams "dbname=oig2 user=oig2"
#define rankdbparams "dbname=rankings user=oig2"
#define CHECK_INTERVAL 100000 //in microseconds
#define SUCCESS 1000000	//this too
#define CHECKER_CHECKED "Sprawdzono"
#define CHECKER_CHECKING "Sprawdzanie"
#define CHECKER_CRIT_COMP_ERROR "Krytyczny błąd kompilacji"
#define CHECKER_COMPILED "Skompilowane"
#define CHECKER_UNKN_ERROR "Nieznany błąd"
#define CHECKER_COMP_ERROR "Błąd kompilacji"
#define CHECKER_WRITTEN_TEST "Test pisemny"
#define CHECKER_PLAGIARISM "PLAGIAT!"
#define row(i) row[i].as<int>()

class ranking;
class contest;
class stage;
class round_;
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
int check_if_rank_in_base(ranking * r);
void handle(submit * sub, string status);
int pickup_ranking(round_ * r);
int pickup_round(int id);
//function keeps monitoring the changes in the submits database table
int monitor();
int main();

#include "task.cpp"
#include "result.cpp"
#include "solver.cpp"
#include "ranking.cpp"
#include "contest.cpp"
#include "stage.cpp"
#include "round.cpp"
#include "submit.cpp"
#include "utils.cpp"
#endif