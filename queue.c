//the coding is UTF-8
#include "queue.h"

//初始化队列
int InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (QNode *)malloc(sizeof(QNode));
    Q->front->next = NULL;
    return 1;
}

//入队
int push_back(LinkQueue *Q, ElemType e)
{
    QNode *p = (QNode *)malloc(sizeof(QNode));
    p->data = e;
    p->next = NULL;
    Q->rear->next = p;
    Q->rear = p;
    return 1;
}

//出队
int pop_front(LinkQueue *Q)
{
    QNode *p;
    if (Q->front == Q->rear)
        return 0;
    p = Q->front->next;
    Q->front->next = p->next;
    if (Q->rear == p)
        Q->rear = Q->front;
    free(p);
    return 1;
}

//尾出队
int pop_back(LinkQueue *Q)
{
    QNode *p;
    if (Q->front == Q->rear)
        return 0;
    p = Q->front;
    while (p->next != Q->rear)
        p = p->next;
    p->next = NULL;
    free(Q->rear);
    Q->rear = p;
    return 1;
}

//队头
ElemType getfront(LinkQueue *Q)
{
    if (Q->front != Q->rear)
        return Q->front->next->data;
}

//判队空
int QueueEmpty(LinkQueue *Q)
{
    return Q->front == Q->rear;
}
