#ifndef main_hpp
#define main_hpp
#include <iostream>
#include <pqxx/pqxx>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <set>
#include <map>
#define dbcommand "dbname=oig2 user=oig2"
#define CHECK_INTERVAL 100000
#define SUC 2000000
#define CHECKER_CHECKED "Sprawdzono"
#define CHECKER_CHECKING "Sprawdzanie"
#define CHECKER_CRIT_COMP_ERROR "Krytyczny błąd kompilacji"
#define CHECKER_COMPILED "Skompilowane"
#define CHECKER_UNKN_ERROR "Nieznany błąd"
#define CHECKER_COMP_ERROR "Błąd kompilacji"
#define CHECKER_WRITTEN_TEST "Test pisemny"
#define CHECKER_PLAGIARISM "PLAGIAT!"

using namespace std;

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

void update_submit(pqxx::result::tuple submit);

void add_solver(solver * s);
void add_problem(task * t);
int add_contest(contest * c);
int add_stage(stage * s);
int add_round(round_ * r);
//function keeps monitoring the changes in the submits database table
int monitor();
#include "task.cpp"
#include "result.cpp"
#include "solver.cpp"
#include "ranking.cpp"
#include "contest.cpp"
#include "stage.cpp"
#include "round.cpp"
#include "submit.cpp"
#endif