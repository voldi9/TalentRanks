#ifndef main_hpp
#define main_hpp
#include <iostream>
#include <pqxx/pqxx>
#include <unistd.h>
#include <vector>
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

class submit
{
	public:
		float speed_factor, team_factor, nerve_factor;


};

class result
{
	public:
		string name;
		vector <short> points;

};

class ranking
{
	public:
		vector <result> rows;

};

#endif