#ifndef main_hpp
#define main_hpp
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
#define SUC 500000	//this too
#define CHECKER_CHECKED "Sprawdzono"
#define CHECKER_CHECKING "Sprawdzanie"
#define CHECKER_CRIT_COMP_ERROR "Krytyczny błąd kompilacji"
#define CHECKER_COMPILED "Skompilowane"
#define CHECKER_UNKN_ERROR "Nieznany błąd"
#define CHECKER_COMP_ERROR "Błąd kompilacji"
#define CHECKER_WRITTEN_TEST "Test pisemny"
#define CHECKER_PLAGIARISM "PLAGIAT!"
#define USER_ID 1
#define TEAM_ID 2
#define PROB_ID 3
#define ROUND_ID 4
#define STAGE_ID 5
#define CONTEST_ID 6
#define TOTAL_POINTS 7
#define PARTIAL_POINTS 8
#define ROUND 0
#define STAGE 1
#define CONTEST 2
#define row(i) row[i].as<int>()

using namespace std;

const string round_query = "SELECT ord, problem_id FROM contest_round_problems\n"
							"WHERE contest_round_id = ";
const string stage_query = "SELECT ord, id FROM contest_rounds\n"
							"WHERE contest_stage_id = ";
const string contest_query = "SELECT ord, id FROM contest_stages\n"
							"WHERE contest_id = ";
pqxx::connection * database;
pqxx::connection * rankbase;

int check_if_rank_in_base(int id, int flag);
string int_to_string(int x);
int copy_ranks();
int main();

#include "utils.cpp"
#endif