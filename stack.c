//the coding is utf-8
#include "stack.h"

//创建空栈
void InitStack(stack *S)
{
    S->base = (ElemType *)malloc(INITSIZE * sizeof(ElemType)); //申请存储空间
    S->top = 0;                                                //栈顶指针初始值为0
    S->stacksize = INITSIZE;                                   //容量为初始值
}

//求栈长
int StackLength(stack *S)
{
    return (S->top);
}

//取栈顶元素
ElemType *gettop(stack *S)
{
    if (S->top == 0)
        exit(1);

    return &(S->base[S->top - 1]); //返回栈顶元素
}

//压栈
int push(stack *S, ElemType x)
{
    if (S->top == S->stacksize)
    {
        S->base = (ElemType *)realloc(S->base, (S->stacksize + 1) * sizeof(ElemType));
        if (!S->base)
            return 0;
        S->stacksize++;
    }
    S->base[S->top++] = x;
    return 1;
}

//弹栈
int pop(stack *S)
{
    if (S->top == 0)
        return 0;
    --S->top;
    return 1;
}

//判栈空
int StackEmpty(stack *S)
{
    if (S->top == 0)
        return 1;
    else
        return 0;
}

//输出栈操作
void list(stack *S)
{
    int i;
    for (i = S->top - 1; i >= 0; i--)
    {
        printf("%4d", S->base[i]);
    }
    printf("\n");
}

//清空栈
void Clear(stack *S)
{
    S->top = 0;
}

//销毁栈
void Destroy(stack *S)
{
    free(S->base);
    S->stacksize = S->top = 0;
}
