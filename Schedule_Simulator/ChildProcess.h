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
    ChildToParentMsgBuffer _sndBuffer;
    
public:
    GET(CPUBurstTime, int, _cpuBurstTime);
    SET(CPUBurstTime, int, _cpuBurstTime);
    
    GET(IOBurstTime, int, _ioBurstTime);
    SET(IOBurstTime, int, _ioBurstTime);
    
public:
    virtual void Run()
    {
        //init
        _cpuBurstTime = 5;
        _ioBurstTime = 10;
        
        printf("Child IPC %d %d\n", _ipcKey, _pid);
        while (_ipcKey != -1)
        {
            ParentToChildMsgBuffer rcvBuffer;
            //printf("Child On!\n");
            ssize_t rcvRes = msgrcv(_ipcKey, (void*)&rcvBuffer,
                                    sizeof(rcvBuffer),
                                    0, 0);

            if(rcvRes < 0)
            {
                perror("msgrcv");
                continue;
            }
            
            if(rcvBuffer.type == ParentToChildMsgBuffer::RESET)
            {
                _cpuBurstTime = 5;
                _ioBurstTime = 10;
            }

            printf("Child Rcv! %d %d\n", _pid, _cpuBurstTime);
            
            if( --_cpuBurstTime < 0 )
            {
                _sndBuffer.ipcKey = _ipcKey;
                _sndBuffer.pid = _pid;
                _sndBuffer.remainsIOBurstTime = _ioBurstTime;
                _sndBuffer.remainsCPUBurstTime = _cpuBurstTime;
            
                rcvRes = msgsnd(rcvBuffer.ipcKey, (void*)&_sndBuffer, sizeof(ChildToParentMsgBuffer), IPC_NOWAIT);
            
                if(rcvRes < 0)
                    perror("msgsnd");
            }
        }
    }
    
    void UpdateData(const ChildToParentMsgBuffer& buffer)
    {
        _cpuBurstTime = buffer.remainsCPUBurstTime;
        _ioBurstTime = buffer.remainsIOBurstTime;
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