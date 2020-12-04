//the coding is UTF-8
#ifndef _ELEVATOR_H_
#define _ELEVATOR_H_

#define INF 0x3f3f3f3f
#define E_W_MAX 520 //最大载重
#define E_N_MAX 8   //最大人数
#define STOP 0
#define DOWN -1
#define UP 1
#define OPEN 1
#define CLOSE 0

#include "stack.h"
#include "floor.h"

typedef struct
{
    int num;      //电梯内的人数
    int p_w;      //电梯内的人总重
    int status;   //电梯运行状态
    int floor;    //电梯当前所在层
    int overload; //超载标记
    stack S[10];  //电梯内乘客栈,每个栈对应乘客要去的一层
    int light[10]; //电梯里面的按钮
} elevator;

elevator E;

//乘客栈索引，按进入电梯的顺序排序，n为第n个进入电梯的
passenger Stackindex(int n);
#endif