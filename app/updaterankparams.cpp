#include "updaterankparams.hpp"

int main()
{
	sem_id = semget(SEM_KEY, 1, 0);
//	semctl(sem_id, 0, SETVAL, 1);
	printf("%d\n", sem_id);
	//while(true)
	{
		printf("want to lower: %d\n", semctl(sem_id, 0, GETVAL));

		semop(sem_id, &P, 1);

		printf("lowered: %d\n", semctl(sem_id, 0, GETVAL));

		scanf("%*c");
		puts("t");

		semop(sem_id, &V, 1);
	}
	return 0;
}