//the coding is GB2312
#include <string.h>
#include <time.h>
#include "interface.h"
#include "work.h"

//������
#define INPUT_CHECK(a, b)           \
    {                               \
        int t;                      \
        while ((t = a) != EOF)      \
            if (b)                  \
            {                       \
                puts("����������"); \
                CLEAR_BUF           \
            }                       \
            else                    \
                break;              \
    }
//���������
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
    //������ӳ�ʼ��
    srand((unsigned)time(NULL));

    //������ݲ���
    int n;
    puts("��������ݲ���");
    INPUT_CHECK(scanf("%d", &F.num), t != 1 || F.num < 5 || F.num > 8)
    //��ʼ��ջ�Ͷ���
    for (int i = 1; i <= F.num; i++)
    {
        InitStack(&E.S[i]);
        InitQueue(&F.Q[i][0]);
        InitQueue(&F.Q[i][1]);
    }

    //��ӡ��ʼ����
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

    prstr("����\n");
    system("pause");
    return 0;
}