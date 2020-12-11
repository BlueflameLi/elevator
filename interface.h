//the coding is UTF-8
#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <Windows.h>
#include "floor.h"
#include "elevator.h"

#define CLS system("cls") //清空屏幕
#define R_N 11
//电梯界面
char *str[4];

//标记已运行的规则
int r_n[20];

//电梯的运行规则
char *rule[20];

//打印当前运行规则
void prrule(int k);

//光标移动
void gotoxy(unsigned char x, unsigned char y);

//打印提示信息
void prstr(char *ch);

//打印初始界面
void printerface();

//打印一个人，x、y为小人的位置（左上角那点），p为小人信息
void prap(int x, int y, passenger p);

//打印楼层电梯按钮，n为当前层，flag为上行还是下行按钮，status为开关状态
void prlight(int n, int flag, int status);

//打印电梯内的按钮，n为按钮对应的层，status为开关状态
void prinlight(int n, int status);

//绘制电梯及上面的人，n为层数，k表示上移或下移k格
void pre(int n, int k);

#endif