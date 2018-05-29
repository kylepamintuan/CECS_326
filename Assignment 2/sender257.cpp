#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
using namespace std;

// Sender 257 sends random multiples of 257 to the message queue

struct buf
{
    long mtype;
    int data;
};

int main()
{
    cout << "Sender 257: PID = " << getpid() << endl;

    // Message queue
    int qid = msgget(ftok(".",'u'),0);

    buf msg;
    int size = sizeof(msg)-sizeof(long);
    srand(time(NULL));
    bool term257 = false;

    while(!term257)
    {
        // Check queue for Receiver's acknowledgement of termination
        msgrcv(qid, (struct msgbuf *)&msg, size, 26, 0);
        if(msg.data == 3)
        {
            cout << "Receiver 2 has terminated" << endl;
            term257 = true;
        }
        else
            msgsnd(qid, (struct msgbuf *)&msg, size, 0);

        // Continuosly generate numbers until we get a number divisble by 257
        msg.data = rand();
        while(msg.data%257 != 0)
        {
            msg.data = rand();
        }

        cout << "send to queue: " << msg.data << endl;

        // Send message to queue
        msg.mtype = 25;
        msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    }

    cout << "Sender 257: terminating..." << endl;

    exit(0);
}

