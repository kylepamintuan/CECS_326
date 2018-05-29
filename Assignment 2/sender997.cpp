#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <signal.h>
#include <ctime>
#include <unistd.h>
using namespace std;

struct buf
{
    long mtype;
    int data;
};

// Sender 257 sends random multiples of 257 to the message queue

int main()
{
    cout << "Sender 997: PID = " << getpid() << endl;

    // Message queue
    int qid = msgget(ftok(".",'u'),0);

    buf msg;
    int size = sizeof(msg)-sizeof(long);
    srand(time(NULL));
    bool termReq1 = false;
    bool termReq2 = false;
    int count = 0;

    while (!termReq1 || !termReq2)
    {
        // Check queue if both receivers received our acknowledgement of termination
        msgrcv(qid, (struct msgbuf *)&msg, size, 26, 0);
        if(msg.data == 2)
        {
            cout << "Receiver 1 received my acknowledment" << endl;
            count++;
        }
        else if (msg.data == 3)
        {
            cout << "Receiver 2 received my acknowledment" << endl;
            count++;
        }
        else
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

        // If both receivers have received our acknowledgement of termination, we can now terminate
        if (count >= 2)
            termReq2 = true;

        // Continuosly generate numbers until we get a number divisble by 257
        msg.data = rand();
		while (msg.data%997 != 0)
		{
            msg.data = rand();

            // Terminate if random number is less than 100
            if (msg.data < 100)
            {
                // Send acknowledgement of before terminating
                msg.mtype = 26;
                msg.data = 1;
                msgsnd(qid, (struct msgbuf *)&msg,size, 0); // One for Receiver 1
                msgsnd(qid, (struct msgbuf *)&msg,size, 0); // Another for Receiver 2
                termReq1 = true;
            }
		}

		cout << "send to queue: " << msg.data << endl;

        // Send message to queue
         msg.mtype = 25;
        msgsnd(qid, (struct msgbuf *)&msg,size, 0);
    }

    cout << "Sender 997: terminating..." << endl;

    exit(0);
}
