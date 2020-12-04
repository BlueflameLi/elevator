//the coding is UTF-8
#ifndef _FLOOR_H_
#define _FLOOR_H_
#define F_W 50 //楼层宽度
#define F_H 6  //楼层高度
#include "queue.h"

typedef struct
{
    int num;            //总层数
    int light[10][2];   //外面楼层里的电脑按钮
    LinkQueue Q[10][2]; //上升是1，下降是0
} floor;

floor F;
#endif