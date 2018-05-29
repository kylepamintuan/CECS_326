#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "semaphore.h"
#include <iostream>
using namespace std;

// Size of shared memory buffer
const int BUFFSIZE = 1;

// Name of semaphore variable
enum {semVar};

// Process functions
void parent_cleanup(SEMAPHORE &, int);
void child_proc(SEMAPHORE &, int *);

int main()
{
	int shmid;
	int *shmBUF;
	srand(time(NULL));

    // Create 1 set of semaphores
	SEMAPHORE sem(1);

	// Initial value: semVar = 2
	sem.V(semVar);
	sem.V(semVar);

    // Create shared memory buffer
	shmid = shmget(IPC_PRIVATE, BUFFSIZE*sizeof(int), PERMS);
	shmBUF = (int *)shmat(shmid, 0, SHM_RND);

    // shmBUF[0] will serve as TOGGLE varaible
    *(shmBUF+(0%BUFFSIZE)) = 1;

    // Spawn child processes
    pid_t pid;
    for (int i = 0; i < 4; i++)
    {
        pid = fork();

        if(pid == 0)
            break;
    }

    for(;;)
    {
        if(pid) // parent process
        {
            string answer;
            //cout << "Quit?" << endl;
            //cin >> answer;
            if (answer == "!wq")
            {
                parent_cleanup(sem, shmid);
                break;
            }

        }
        else if(pid == 0) // child process
        {
            child_proc(sem, shmBUF);
        }
    }

	exit(0);
}

// Child Process Function
void child_proc(SEMAPHORE &sem, int *shmBUF)
{
    sem.P(semVar);

    long int U = 827395609;
	long int V = 962094883;

    // Decide which variable (U or V) to use for the dividend
    long int dividend;
    if (*(shmBUF+(0%BUFFSIZE)) == 1)
    {
        cout << getpid() << ": Operating on U" << endl;

        dividend = U;
        *(shmBUF+(0%BUFFSIZE)) = 0; //toggle
    }
    else if (*(shmBUF+(0%BUFFSIZE)) == 0)
    {
        cout << getpid() << ": Operating on V" << endl;

        dividend = V;
        *(shmBUF+(0%BUFFSIZE)) = 1; //toggle
    }

    // Perform calculations
    int randomNum = (rand()%10000000) + 1;
    int result = dividend%randomNum;
    while (randomNum > 100 || result != 0)
    {
        randomNum = (rand()%10000000) + 1;
        result = dividend%randomNum;
    }

    cout << getpid() << ": finished" << endl;

    sem.V(semVar);
}

// Parent Cleanup Function
void parent_cleanup (SEMAPHORE &sem, int shmid)
{
	kill(getppid(), SIGKILL);
	shmctl(shmid, IPC_RMID, NULL);
	sem.remove();
}
