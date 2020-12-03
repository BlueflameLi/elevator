#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_
#include "stack.h"
typedef struct
{
    int num;     //电梯内的人数
    int p_w;     //电梯内的人总重
    int status;  //电梯运行状态
    int floor;   //电梯当前所在层
    stack S[10]; //电梯内乘客栈,每个栈对应乘客要去的一层
} elevator;

elevator E;
#endif