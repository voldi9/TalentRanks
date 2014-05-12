#ifndef shared_hpp
#define shared_hpp
#include <sys/types.h>  //sem
#include <sys/ipc.h> 	//apho
#include <sys/sem.h>	//res
#include <string>
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
#define RANK_ID 0
#define RANK_SUB_ID 1
#define RANK_ROUND_ID 2
#define RANK_USER_ID 3
#define RANK_TEAM_ID 4
#define RANK_PROB_ID 5
#define RANK_STATUS 6
#define RANK_POINTS 7
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
#define USER 0
#define TEAM 1
#define MULTIPL 100000
#define row(i) row[i].as<int>()
#define SEM_KEY ftok(".",'a')
#define MULTIPL 100000

using namespace std;

const string round_query = "SELECT ord, problem_id FROM contest_round_problems\n"
							"WHERE contest_round_id = ";
const string stage_query = "SELECT ord, id FROM contest_rounds\n"
							"WHERE contest_stage_id = ";
const string contest_query = "SELECT ord, id FROM contest_stages\n"
							"WHERE contest_id = ";

static sembuf P =
{
	0,
	-1,
	0
};

static sembuf V =
{
	0,
	1,
	0
};

int sem_id;



#endif