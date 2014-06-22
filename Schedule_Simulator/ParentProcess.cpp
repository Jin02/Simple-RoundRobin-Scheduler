//
//  ParentProcess.cpp
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#include "ParentProcess.h"
#include <string>

static ParentProcess* _this = NULL;

ParentProcess::ParentProcess(int pid)
: BaseProcess(pid)
{
    _type = TYPE::PARENT;
    _this = this;
    _tick = 0;
    _globalTick = 0;
}

ParentProcess::~ParentProcess()
{
    
}

bool ParentProcess::Init()
{
    struct sigaction oldSighandler, newSighandler;
    itimerval itimer, oldTimer;
    
    memset (&newSighandler, 0, sizeof (newSighandler));
    newSighandler.sa_handler = &ParentProcess::Scheduler;
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
}

void ParentProcess::_Scheduler()
{
    std::string fileContent = "";
    char txt[256] = {0, };
    
    ++_globalTick;
    
    if (_run.empty() == false)
    {
        ChildProcess* child = _run.front();

        if( child->GetCPUBurstTime() < 0 )
        {
            child->SetCPUBurstTime(0);
            child->SetIOBurstTime(0);
            _sndBuffer.type = ParentToChildMsgBuffer::RESET;
        }
        else
        {
            _sndBuffer.type = ParentToChildMsgBuffer::NONE;
        }
        
        _sndBuffer.pid = _pid;
        _sndBuffer.ipcKey = _ipcKey;
        _sndBuffer.tick = _globalTick;

        int res = msgsnd(child->GetIPCKey(), (void*)&_sndBuffer, sizeof(ParentToChildMsgBuffer), IPC_NOWAIT);
    
        if( res < 0)
            perror("msgsnd");
        else
        {
            //runQ 덤프랑 wQ덤프는?
            if( ++_tick > TIME_SLICE)
            {
                _run.push(_run.front());
                _run.pop();

                _tick = 0;
            }
        }
    }
    
    sprintf(txt, "run  |\t time : %d / ", _globalTick);
    fileContent+=txt;
    
    std::queue<ChildProcess*> swap;
    while (_run.empty() == false)
    {
        sprintf(txt, "pid : %d / ", _run.front()->GetPid());
        fileContent+=txt;
        swap.push(_run.front());
        _run.pop();
    }
    std::swap(_run, swap);
    
    fileContent += "\n";
    sprintf(txt, "wait |\t time : %d / ", _globalTick);
    fileContent += txt;
    
    for (auto iter = _wait.begin(); iter != _wait.end();)
    {
        int io = (*iter)->GetIOBurstTime();
        if( io > 0 )
        {
            (*iter)->SetIOBurstTime( io - 1 );
            
            sprintf(txt, "pid : %d / io : %d / ", (*iter)->GetPid(), (*iter)->GetIOBurstTime());
            fileContent+=txt;
            
            ++iter;
        }
        else
        {
            _run.push(*iter);
            iter = _wait.erase(iter);
        }
    }
    fileContent+="\n";
    printf("%s", fileContent.c_str());
    FileWrite(fileContent.c_str());
}

void ParentProcess::Scheduler(int signo)
{
    ParentProcess* pc = _this;

    if(pc != NULL)
        pc->_Scheduler();
}

ChildProcess* ParentProcess::FindChildProcess(int pid)
{
    {
        std::queue<ChildProcess*> swap;
        ChildProcess* find = NULL;
        while (_run.empty() == false)
        {
            if(_run.front()->GetPid() == pid)
                find = _run.front();
            
            swap.push(_run.front());
            _run.pop();
        }
        std::swap(_run, swap);
        if(find != NULL)
            return find;
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
            {
//                printf("Done! %d\n", rcvBuffer.pid);
                _wait.push_back(_run.front());
                _run.pop();
                
                _tick = 0;
            }
        }
    }
}