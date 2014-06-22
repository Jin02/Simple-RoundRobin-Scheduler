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
    enum TYPE{
        RESET, NONE
    };
    //TYPE type;
    int type;
    
    ParentToChildMsgBuffer(int type, int pid, int ipcKey)
    : MsgBuffer(pid, ipcKey)
    {
        this->type = type;
    }
    
    ParentToChildMsgBuffer()
    : MsgBuffer(-1, -1)
    {
        type = NONE;
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