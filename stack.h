//the coding is UTF-8
#ifndef _STACK_H_
#define _STACK_H_
#include <stdio.h>
#include <stdlib.h>
#include "passenger.h"
#define INITSIZE 100 //储存空间的初始分配量
typedef passenger ElemType;
typedef struct
{
    ElemType *base; //存放元素的动态数组空间
    int top;        //栈顶指针
    int stacksize;  //当前栈空间的大小
} stack;

void InitStack(stack *S);
int StackLength(stack *S);
ElemType *gettop(stack *S);
int push(stack *S, ElemType x);
int pop(stack *S);
int StackEmpty(stack *S);
void list(stack *S);
void Clear(stack *S);
void Destroy(stack *S);
#endif