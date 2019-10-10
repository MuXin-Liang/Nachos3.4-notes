// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "elevatortest.h"

// testnum is set in main.cc
int testnum = 2;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %s, %d looped %d times\n",currentThread->getName(),which, num);
        currentThread->Yield();

    }
}

void
SimpleThread3(int which)
{
    int num;
    for (num = 0; num < 3; num++) {
        printf("*** thread %s looped %d times,priority:%d \n", currentThread->getName(), num,currentThread->getPriority());
    }
}

void
SimpleThread2(int which)
{
    int num;
    
    for (num = 0; num < 3; num++) {
        printf("*** thread %s looped %d times,priority:%d \n", currentThread->getName(), num,currentThread->getPriority());
    }

    Thread *t2 = new Thread("forked_thread2");
    t2->setPriority(1);
    t2->Fork(SimpleThread3, (void*)2);

    for (num = 3; num < 6; num++) {
        printf("*** thread %s looped %d times,priority:%d \n", currentThread->getName(), num,currentThread->getPriority());
    }

    Thread *t3 = new Thread("forked_thread3");
    t3->setPriority(3);
    t3->Fork(SimpleThread3, (void*)3);

    for (num = 6; num < 9; num++) {
        printf("*** thread %s looped %d times,priority:%d \n", currentThread->getName(), num,currentThread->getPriority());
    }
}



//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");
    t->Fork(SimpleThread, (void*)1);

    SimpleThread(0);
}

//----------------------------------------------------------------------
// ThreadTest2
// 	调用TS函数
//----------------------------------------------------------------------

void
ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");
    ThreadTest1();
    TS();
}

//----------------------------------------------------------------------
// ThreadTest3
// 测试线程生成上限
//----------------------------------------------------------------------

void
ThreadTest3()
{
    DEBUG('t', "Entering ThreadTest3");

    for(int i = 0;i<127;i++){

        Thread *t = new Thread("forked thread");
        t->Fork(SimpleThread2, (void*)1);
    }
    TS();
    Thread *t = new Thread("forked thread");
    t->Fork(SimpleThread2, (void*)1);
    
}


//----------------------------------------------------------------------
// ThreadTest4
// 测试抢占式调度算法
//----------------------------------------------------------------------

void
ThreadTest4()
{
    DEBUG('t', "Entering ThreadTest4");

    Thread *t1 = new Thread("forked_thread1");
    t1->setPriority(2);
    t1->Fork(SimpleThread2, (void*)1);
    
}


//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    case 2:
    ThreadTest2();
    break;
    case 3:
    ThreadTest3();
    break;
    case 4:
    ThreadTest4();
    default:
	printf("No test specified.\n");
	break;
    }
}

