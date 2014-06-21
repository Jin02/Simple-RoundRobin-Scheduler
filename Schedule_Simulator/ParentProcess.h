//
//  ParentProcess.h
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#pragma once

#include "ChildProcess.h"
#include <queue>

class ParentProcess : public BaseProcess
{
private:
    std::queue<ChildProcess*> _run;
    std::queue<ChildProcess*> _wait;
    static ParentProcess* _this;
    
public:
    bool Init()
    {
        struct sigaction oldSighandler, newSighandler;
        itimerval itimer, oldTimer;
        
        memset (&newSighandler, 0, sizeof (newSighandler));
        newSighandler.sa_handler = &ParentProcess::Schdule;
        sigaction(SIGALRM, &newSighandler, &oldSighandler);
        
        itimer.it_interval.tv_sec = 1;
        itimer.it_interval.tv_usec = 0;
        itimer.it_value.tv_sec = 1;
        itimer.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &itimer, &oldTimer);
        
        return CreateMsgQueue();
    }
    
    void AddChildProcess(pid_t pid)
    {
        ChildProcess* child = new ChildProcess(pid);
        child->CreateMsgQueue();
        _wait.push(child);
    }
    
    static void Schdule(int signo)
    {
        ParentProcess* pc = ParentProcess::_this;
        if(pc == NULL)      return;
        
        
        
    }
    
    virtual void _Run()
    {
        
    }
    
    
public:
    ParentProcess() : BaseProcess()
    {
        _type = TYPE::PARENT;
        _this = this;
    }
    
    ParentProcess(int pid) : BaseProcess(pid)
    {
        _type = TYPE::PARENT;
    }
    
    ~ParentProcess()
    {
        
    }
};