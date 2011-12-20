/**
 * @file	Thread.h
 * @brief	线程类的声明
 * @author	hrh <huangrh@landuntec.com>
 * @version	1.0.0
 * @date	2011-12-07
 * 
 * @verbatim
 * ============================================================================
 * Copyright (c) Shenzhen Landun technology Co.,Ltd. 2011
 * All rights reserved. 
 * 
 * Use of this software is controlled by the terms and conditions found in the
 * license agreenment under which this software has been supplied or provided.
 * ============================================================================
 * 
 * @endverbatim
 * 
 */


#ifndef _THREAD_H_
#define _THREAD_H_


#include <pthread.h>

class Thread {

private:
        static void* RunThread(void* pArg);
        pthread_t  m_Pid;
        bool m_bRunning;

public:
        Thread();
        virtual ~Thread();

        virtual void Terminate();
        bool IsTerminated();
        bool Start();
        void Detach();
        void Join();

protected :

        virtual void Run();
	void* ThreadID();
};

#endif

