//the coding is UTF-8
#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stdio.h>
#include <stdlib.h>
#include "passenger.h"
typedef passenger ElemType;
typedef struct QNode
{
    ElemType data;
    struct QNode *next;

} QNode;

typedef struct
{
    QNode *front;
    QNode *rear;

} LinkQueue;

int InitQueue(LinkQueue *Q);
int push_back(LinkQueue *Q, ElemType e);
int pop_front(LinkQueue *Q);
int pop_back(LinkQueue *Q);
ElemType getfront(LinkQueue *Q);
int QueueEmpty(LinkQueue *Q);
#endif