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

struct buf
{
    long mtype;
    int data;
};

// Receiver 2 - Receives random multiples of 257 or 997 from the message queue

int main()
{
    cout << "Receiver 2: PID = " << getpid() << endl;

    // Message queue
    int qid = msgget(ftok(".",'u'),0);

    buf msg;
    int size = sizeof(msg)-sizeof(long);
    int count = 0;

    // Terminate after receiving 5000 messages from the queue
    while(count < 5000)
    {
        // Receive messages from queue with mtype = 25
        msgrcv(qid, (struct msgbuf *)&msg, size, 25, 0);

        // Display messages
        if(msg.data%257 == 0)
        {
            cout << "Received from Sender 257: " << msg.data << endl;
            count++;
        }
        else if(msg.data%997 == 0)
        {
            cout << "Received from Sender 997: " << msg.data << endl;
            count++;
        }
    }

    // Send acknowledgement of termination to Sender 997 and Sender 257
    msg.mtype = 26;
    msg.data = 3;
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    msgsnd(qid, (struct msgbuf *)&msg, size, 0);

    cout  << "Sent Messages Counter = " << count << endl;
    cout << "Receiver 2: terminating..." << endl;

    exit(0);
}

