//
//  ChildProcess.h
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#pragma once

#include "BaseProcess.h"

class ChildProcess : public BaseProcess
{
private:
    int     _cpuBurstTime;
    int     _ioBurstTime;
    
public:
    GET(CPUBurstTime, int, _cpuBurstTime);
    GET(IOBurstTime, int, _ioBurstTime);
    
public:
    virtual void _Run()
    {
        
    }

    
public:
    ChildProcess() : BaseProcess(),
    _cpuBurstTime(0),
    _ioBurstTime(0)
    {
    }
    
    ChildProcess(int pid) : BaseProcess(pid),
    _cpuBurstTime(0),
    _ioBurstTime(0)
    {
        
    }
    
    ~ChildProcess()
    {
        //nothing
    }
};