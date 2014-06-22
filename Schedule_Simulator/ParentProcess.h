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
#include <list>

#define TIME_SLICE 20

class ParentProcess : public BaseProcess
{
private:
    std::queue<ChildProcess*> _run;
    std::list<ChildProcess*> _wait;
    
    ParentToChildMsgBuffer _sndBuffer;

    int _tick;
    int _globalTick;
    
public:
    bool Init();
    
    void AddChildProcess(pid_t pid);
    
    void NextProcess();
    
    void _Scheduler();
    static void Scheduler(int signo);
    
    virtual void Run();
    
    ChildProcess* FindChildProcess(int pid);
    
public:
    ParentProcess(int pid);
    ~ParentProcess();
};