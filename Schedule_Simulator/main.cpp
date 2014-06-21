//
//  main.cpp
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#include "ParentProcess.h"

#define NUM 2

BaseProcess* CreateProcess(int count)
{
    ParentProcess* parent = NULL;
    
    int i;
    for(i=0; i<count; ++i)
    {
        pid_t pid = fork();
        if( pid == 0 )
            return new ChildProcess(getpid());
        
        if(parent == NULL)
            parent = new ParentProcess(getpid());
        
        parent->AddChildProcess(pid);
    }
    
    if( parent->Init() == false )
    {
        printf("Parent Init fail! \n");
        delete parent;
        parent = NULL;
    }
    
    return parent;
}

int main(int argc, const char * argv[])
{
    BaseProcess* process = CreateProcess(NUM);
    
//    if(isParent)
//    {
//        msgbuf buf;
//        buf.testValue = 22;
//        int res = msgsnd(childIPCKey[0], (void*)&buf, sizeof(struct msgbuf), IPC_NOWAIT);
//        printf("Parent Send! %d // res = %d \n", childIPCKey[0], res);
//        if(res == -1)
//        {
//            perror("msgsnd");
//        }
//    }
    
    process->Run();
    return 0;
}

