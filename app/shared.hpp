#ifndef shared_hpp
#define shared_hpp
#include <sys/types.h>  //sem
#include <sys/ipc.h> 	//apho
#include <sys/sem.h>	//res
#define SEM_KEY ftok(".",'a')

using namespace std;

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