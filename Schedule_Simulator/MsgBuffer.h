//
//  MsgBuffer.h
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#pragma once

#include "Common.h"

struct MsgBuffer
{
    int pid;
    int ipcKey;
    
    MsgBuffer(int pid, int ipcKey)
    {
        this->pid = pid;
        this->ipcKey = ipcKey;
    }
    
    ~MsgBuffer()
    {
        
    }
};

struct ParentToChildMsgBuffer : public MsgBuffer
{
    int timeSlice;
    
    ParentToChildMsgBuffer(int timeSlice, int pid, int ipcKey)
    : MsgBuffer(pid, ipcKey)
    {
        this->timeSlice = timeSlice;
    }
    
    ParentToChildMsgBuffer()
    : MsgBuffer(-1, -1)
    {
        timeSlice = 0;
    }
    
    ~ParentToChildMsgBuffer()
    {
        
    }
};

struct ChildToParentMsgBuffer : public MsgBuffer
{
    int remainsCPUBurstTime;
    int remainsIOBurstTime;
    
    ChildToParentMsgBuffer(int remainsCPUBurstTime,
                           int remainsIOBurstTime, int pid, int ipcKey)
    : MsgBuffer(pid, ipcKey)
    {
        this->remainsIOBurstTime = remainsIOBurstTime;
        this->remainsCPUBurstTime = remainsCPUBurstTime;
    }
    
    ChildToParentMsgBuffer()
    :MsgBuffer(-1, -1)
    {
        this->remainsIOBurstTime = 0;
        this->remainsCPUBurstTime = 0;
    }
    
    ~ChildToParentMsgBuffer()
    {
        
    }
};