//the coding is GB2312
#include <string.h>
#include <time.h>
#include "interface.h"
#include "work.h"

//输入检查
#define INPUT_CHECK(a, b)           \
    {                               \
        int t;                      \
        while ((t = a) != EOF)      \
            if (b)                  \
            {                       \
                puts("请重新输入"); \
                CLEAR_BUF           \
            }                       \
            else                    \
                break;              \
    }
//清除缓存区
#define CLEAR_BUF                                     \
    {                                                 \
        int ch;                                       \
        while ((ch = getchar()) != EOF && ch != '\n') \
        {                                             \
            ;                                         \
        }                                             \
    }
void init()
{
    //随机种子初始化
    srand((unsigned)time(NULL));

    //读入电梯层数
    int n;
    puts("请输入电梯层数");
    INPUT_CHECK(scanf("%d", &F.num), t != 1 || F.num < 5 || F.num > 8)
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
}
int main()
{
    init();

    while (T < MAXTIME)
    {
        rande();
        run();
        checkwait();
    }

    prstr("结束\n");
    system("pause");
    return 0;
}