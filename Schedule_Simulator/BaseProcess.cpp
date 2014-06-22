//
//  Process.cpp
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#include "BaseProcess.h"

BaseProcess::BaseProcess()
{
    _pid = -1;
    _ipcKey = -1;
    _state = STATE::WAIT;
    _type = TYPE::CHILD;
}

BaseProcess::BaseProcess(int pid)
{
    _pid = pid;
    _ipcKey = -1;
    _state = STATE::WAIT;
    _type = TYPE::CHILD;
}

BaseProcess::~BaseProcess()
{
    kill(_pid, SIGKILL);
}

void BaseProcess::FileWrite(const char* txt)
{
    FILE* fp = fopen("/Users/jin/Desktop/Schedule_dump.txt", "a");
    fprintf(fp, "%s", txt);
    fclose(fp);
}

bool BaseProcess::CreateMsgQueue()
{
    _ipcKey = msgget(_pid, MSGGET_CREATE_FLAG);
    
    if (_ipcKey == -1)
        perror("msgget");
    
    return _ipcKey != -1;
}
