//
//  Common.h
//  Schedule_Simulator
//
//  Created by Jin on 2014. 6. 20..
//  Copyright (c) 2014년 __Jin__. All rights reserved.
//

#pragma once

#define __DEBUG_SIMULATION__

#define GET(n, x, y) inline x Get##n()const { return y; }
#define SET(n, x, y) inline void Set##n(x t)	{ y = t; }