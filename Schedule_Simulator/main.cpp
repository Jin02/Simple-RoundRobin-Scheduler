//
//  main.cpp
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#include "ParentProcess.h"

#define NUM 10

BaseProcess* CreateProcess(int count)
{
    ParentProcess* parent = NULL;
    
    int i;
    for(i=0; i<count; ++i)
    {
        pid_t pid = fork();
        if( pid == 0 )
        {
            ChildProcess* child = new ChildProcess(getpid());
            child->CreateMsgQueue();
            return child;
        }
        
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
    process->Run();
    
//    FILE* fp = fopen("/Users/jin/Desktop/Test.txt", "a");
//    fprintf(fp, "test");
//    fclose(fp);
    
    return 0;
}

