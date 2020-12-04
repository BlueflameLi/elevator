//the coding is UTF-8
#ifndef _PASSENGER_H_
#define _PASSENGER_H_

#define MAXN 100 + 10
typedef struct
{
    int w;        //乘客的体重
    int n_f;      //乘客想去的楼层
    int intime;   //乘客生成的时间
    int entime;   //乘客进入电梯的时间
    int waittime; //乘客耐心时间
} passenger;

char *p_str[4];    //绘制小人的字符
int p_num[MAXN];   //每层的人数
int p_f_num[MAXN]; //每层楼道里的人


#endif