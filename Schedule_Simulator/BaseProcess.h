
//
//  Process.h
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <unistd.h>

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <signal.h>

#include "Common.h"
#include "MsgBuffer.h"

class BaseProcess
{
public:
    enum STATE
    {
        WAIT, RUN
    };
    enum TYPE
    {
        PARENT, CHILD
    };
    
    const int MSGGET_CREATE_FLAG = IPC_CREAT | 0660;
    
protected:
    pid_t   _pid;
    int     _ipcKey;
    STATE   _state;
    TYPE    _type;
    
public:
    GET(Type, TYPE, _type);
    GET(State, STATE, _state);
    GET(IPCKey, int, _ipcKey);
    GET(Pid, pid_t, _pid);
    
protected:
    static void FileWrite(const char* txt);
    
public:
    bool CreateMsgQueue();
        
public:
    BaseProcess();
    BaseProcess(int pid);
    virtual ~BaseProcess();
    
public:
    virtual void Run() = 0;
};