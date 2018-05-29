#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <signal.h>
#include <unistd.h>
#include "get_info.h"
using namespace std;

 struct buf
{
    long mtype;
    int data;
};

// Sender 251 sends random multiples of 251 to the message queue

int main()
{
    cout << "Sender 251: PID = " << getpid() << endl;

    // Message queue
    int qid = msgget(ftok(".",'u'),0);

    buf msg;
    int size = sizeof(msg)-sizeof(long);
    srand(time(NULL));
    bool term251 = false;

    // Call get_info method from get_info.h file
    //get_info(qid, (struct msgbuf *)&msg, size, 25);

    msg.mtype = 25;

    // Terminates by 'kill' command
    //for(;;)
    while(!term251)
    {
        // Continuosly generate numbers until we get a number divisble by 251
        msg.data = rand();
        while(msg.data%251 != 0)
        {
            msg.data = rand();

            if(msg.data < 200)
            {
                cout << msg.data << " < 200" << endl;

                // Send acknowledgement of before terminating
                msg.data = 0;
                msgsnd(qid, (struct msgbuf *)&msg,size, 0);
                term251 = true;
            }
        }

        cout << "send to queue: " << msg.data << endl;

        // Send message to queue
        msgsnd(qid, (struct msgbuf *)&msg, size, 0);
    }

    cout << "Sender 251: terminating..." << endl;

    exit(0);
}

