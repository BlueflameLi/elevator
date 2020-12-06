//the coding is UTF-8
#ifndef _WORK_H_
#define _WORK_H_
#include "interface.h"
#define TRUE 1
#define FALSE 0
#define SLEEPTIME 800
#define RANDPN 6
#define MAXTIME 280

int T;          //运行时间
int waitedtime; //已等待时间

//延时
void Wait(int t);

//随机生成小人
passenger randp();

//随机生成k个人
void rande();

//在第n层生成一个小人
void adp(int n);

//更新第n层等候的人
void udp(int n);

//楼道增加一个人
void adpf(int n, passenger p);

//清空楼道里的人
void clpf(int n);

//耐心等待检测
void checkwait();

//检测超重
int checkoverload(int n, passenger p);

//开关门
void ocdoor(int n, int status);

//第n层一个人进电梯
int mpe(int n);

//一个人离开电梯
int rmpe(int n);

//上或下移动一层
void updown(int flag);

//运行一次电梯，上行一层或下降一层或原地等待
void run();

#endif