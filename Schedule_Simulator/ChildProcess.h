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
    int                     _cpuBurstTime;
    int                     _ioBurstTime;
    ChildToParentMsgBuffer  _sndBuffer;
    
public:
    GET(CPUBurstTime, int, _cpuBurstTime);
    SET(CPUBurstTime, int, _cpuBurstTime);
    
    GET(IOBurstTime, int, _ioBurstTime);
    SET(IOBurstTime, int, _ioBurstTime);
    
private:
    void GenerateRandomBurstTime();
    
public:
    virtual void Run();
    void UpdateData(const ChildToParentMsgBuffer& buffer);
    
public:
    ChildProcess();
    ChildProcess(int pid);
    ~ChildProcess();
};