/**
 * @file	Thread.cpp
 * @brief	线程类的实现
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

#include "thread.h"
#include <stdio.h>

Thread::Thread()
{
        m_bRunning = false;
        m_Pid = 0;
}


Thread::~Thread()
{
        if(m_bRunning) {

                Terminate();
        }
}


void* Thread::RunThread(void* pArg)
{        
	((Thread*)pArg)->ThreadID();
        return NULL;
}


void* Thread::ThreadID()
{
	m_Pid = pthread_self();
	Run();
	m_Pid = 0;
	printf("exit thread\n");
	pthread_exit(NULL);
}

void Thread::Run()
{

}


void Thread::Detach()
{
        pthread_detach(m_Pid);
}


void Thread::Join()
{
	if (m_Pid > 0) {

		printf("join thread\n");
        	pthread_join(m_Pid, NULL);
	}
}


bool Thread::Start()
{
        m_bRunning = true;
        return pthread_create(&m_Pid, NULL, RunThread, this) == 0;
}


void Thread::Terminate()
{
        m_bRunning = false;
}


bool Thread::IsTerminated()
{
        return !m_bRunning;
}

