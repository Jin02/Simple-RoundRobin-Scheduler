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
    
public:
    bool CreateMsgQueue()
    {
        _ipcKey = msgget(_pid, MSGGET_CREATE_FLAG);
        
#ifdef __DEBUG_SIMULATION__
        if (_ipcKey == -1)
            perror("msgget");
#endif
        
        return _ipcKey != -1;
    }
    
        
public:
    BaseProcess()
    {
        _pid = -1;
        _ipcKey = -1;
        _state = STATE::WAIT;
        _type = TYPE::CHILD;
    }
    
    BaseProcess(int pid)
    {
        _pid = pid;
        _ipcKey = -1;
        _state = STATE::WAIT;
        _type = TYPE::CHILD;
    }
    
    virtual ~BaseProcess()
    {
        kill(_pid, SIGKILL);
    }
    
protected:
    virtual void _Run() = 0;
    
public:
    void Run()
    {
        while (_ipcKey != -1)
        {
            MsgBuffer buffer;
            ssize_t rcvRes = msgrcv(_ipcKey, (void*)&buffer,
                                    sizeof(MsgBuffer),
                                    0, 0);

            if(rcvRes < 0)
            {
                perror("msgrcv");
                printf( _type == PARENT ?
                       "parent Error rcv \n" : "child Error rcv \n");
            }
            else
            {
                _Run();
            }            
        }
    }
    
};