//
//  ParentProcess.cpp
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#include "ParentProcess.h"

static ParentProcess* _this = NULL;

ParentProcess::ParentProcess(int pid)
: BaseProcess(pid)
{
    _type = TYPE::PARENT;
    _this = this;
    _tick = 0;
}

ParentProcess::~ParentProcess()
{
    
}

bool ParentProcess::Init()
{
    struct sigaction oldSighandler, newSighandler;
    itimerval itimer, oldTimer;
    
    memset (&newSighandler, 0, sizeof (newSighandler));
    newSighandler.sa_handler = &ParentProcess::EventLog;
    sigaction(SIGALRM, &newSighandler, &oldSighandler);
    
    itimer.it_interval.tv_sec = 1;
    itimer.it_interval.tv_usec = 0;
    itimer.it_value.tv_sec = 1;
    itimer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &itimer, &oldTimer);
    
    return CreateMsgQueue();
}

void ParentProcess::AddChildProcess(pid_t pid)
{
    ChildProcess* child = new ChildProcess(pid);
    child->CreateMsgQueue();
    _run.push(child);
}

void ParentProcess::NextProcess()
{
    _wait.push_back(_run.front());
    _run.pop();
    
    _tick = 0;
}

void ParentProcess::_EventLog()
{
    if (_run.empty() == false)
    {
        ChildProcess* child = _run.front();
        
        if(child->GetCPUBurstTime() <= 0)
            return;
    
        _sndBuffer.pid = _pid;
        _sndBuffer.ipcKey = _ipcKey;

        int res = msgsnd(child->GetIPCKey(), (void*)&_sndBuffer, sizeof(_sndBuffer), IPC_NOWAIT);
    
        if( res < 0)
        {
            //error
        }
        else
        {
            if( ++_tick > TIME_SLICE)
            {
                NextProcess();
            }
        }
    }
    
    for (auto iter = _wait.begin(); iter != _wait.end();)
    {
        int io = (*iter)->GetIOBurstTime();
        if( io > 0 )
        {
            (*iter)->SetIOBurstTime( io - 1 );
            ++iter;
        }
        else
        {
            _run.push(*iter);
            iter = _wait.erase(iter);
        }
    }

}

void ParentProcess::EventLog(int signo)
{
    ParentProcess* pc = _this;

    if(pc != NULL)
        pc->_EventLog();
}

ChildProcess* ParentProcess::FindChildProcess(int pid)
{
    for( auto iter = _run.front(); iter != _run.back()+1; ++iter)
    {
        if(iter->GetPid() == pid)
            return iter;
    }
    
    for( auto iter = _wait.begin(); iter != _wait.end(); ++iter )
    {
        if( (*iter)->GetPid() == pid )
            return (*iter);
    }
    
    return nullptr;
}

void ParentProcess::Run()
{
    while (_ipcKey != -1)
    {
        ChildToParentMsgBuffer rcvBuffer;
        ssize_t rcvRes = msgrcv(_ipcKey, (void*)&rcvBuffer,
                                sizeof(rcvBuffer),
                                0, IPC_NOWAIT);
        
        if(rcvRes < 0)
        {
//            perror("msgrcv");
//            printf( _type == PARENT ?
//                   "parent Error rcv \n" : "child Error rcv \n");
        }
        else
        {
            ChildProcess* child = FindChildProcess(rcvBuffer.pid);
            child->UpdateData(rcvBuffer);
            
            if(rcvBuffer.remainsCPUBurstTime <= 0)
                NextProcess();
        }
    }
}