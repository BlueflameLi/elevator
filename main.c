//the coding is GB2312
#include <string.h>
#include <time.h>
#include "interface.h"
#include "work.h"

int main()
{
    //������ӳ�ʼ��
    srand((unsigned)time(NULL));

    //������ݲ���
    int n;
    puts("��������ݲ���");
    scanf("%d", &F.num);

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

    while (T < MAXTIME)
    {
        rande();
        run();
        checkwait();
    }
    prstr("����");
    system("pause");
    return 0;
}