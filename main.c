//the coding is GB2312
#include <string.h>
#include <time.h>
#include "interface.h"
#include "work.h"

int main()
{
    //随机种子初始化
    srand((unsigned)time(NULL));

    //读入电梯层数
    int n;
    puts("请输入电梯层数");
    scanf("%d", &F.num);

    //初始化栈和队列
    for (int i = 1; i <= F.num; i++)
    {
        InitStack(&E.S[i]);
        InitQueue(&F.Q[i][0]);
        InitQueue(&F.Q[i][1]);
    }

    //打印初始界面
    printerface();

    Sleep(SLEEPTIME);

    while (T < MAXTIME)
    {
        rande();
        run();
        checkwait();
    }
    prstr("结束");
    system("pause");
    return 0;
}