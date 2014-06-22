//
//  ChildProcess.cpp
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#include "ChildProcess.h"
#include <stdlib.h>

ChildProcess::ChildProcess() : BaseProcess(),
                            _cpuBurstTime(0), _ioBurstTime(0)
{
}

ChildProcess::ChildProcess(int pid) : BaseProcess(pid),
                                      _cpuBurstTime(0), _ioBurstTime(0)
{
    
}

ChildProcess::~ChildProcess()
{
    //nothing
}

void ChildProcess::UpdateData(const ChildToParentMsgBuffer& buffer)
{
    _cpuBurstTime = buffer.remainsCPUBurstTime;
    _ioBurstTime = buffer.remainsIOBurstTime;
}

void ChildProcess::GenerateRandomBurstTime()
{
    _cpuBurstTime = rand() % 100 + 60;
    _ioBurstTime = rand() % 100 + 60;
}

void ChildProcess::Run()
{
    //init
    GenerateRandomBurstTime();
    
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
            GenerateRandomBurstTime();
        
        char txt[256] = {0,};
        sprintf(txt, "child|\t time : %d / pid : %d / cpu : %d\n", rcvBuffer.tick, _pid, _cpuBurstTime);
        printf("%s",txt);
        FileWrite(txt);
        
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
