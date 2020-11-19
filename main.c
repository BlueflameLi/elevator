//the coding is UTF-8
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stack.h"
#include "queue.h"
#include <Windows.h>
#define MAXN 100 + 10
#define CLS system("cls") //清空屏幕
#define E_W 50            //楼层宽度
#define E_H 6             //楼层高度
#define E_W_MAX 600       //最大载重
#define E_N_MAX 8         //最大人数
#define STOP 0
#define DOWN -1
#define UP 1
#define OPEN 2
#define CLOSE 3
#define TRUE 1
#define FALSE 0
char *str[] = {
    "█                                                 |                                                                                                  █",
    "█                                                 ██████████████████████████████████████████████████",
    "█=================================================██████████████████████████████████████████████████",
    "█████████████████████████████████████████████████████████████████"};

char *p_str[] = {"  O  ",
                 "--|--",
                 " / \\ "};
int p_num[MAXN]; //每层的人数
int e_num;       //电梯里的人数
int f_num;       //总楼层数
int e_floor = 1; //当前电梯所在层
stack S[10];
LinkQueue Q[10][2]; //上升是1，下降是0
int e_status;
int T; //运行时间
int light[10][2];
int inlight[10];

//光标移动
void gotoxy(unsigned char x, unsigned char y)
{
    //COORD是Windows API中定义的一种结构，表示一个字符在控制台屏幕上的坐标
    COORD cor;

    //句柄
    HANDLE hout;

    //设定我们要定位到的坐标
    cor.X = x;
    cor.Y = y;

    //GetStdHandle函数获取一个指向特定标准设备的句柄，包括标准输入，标准输出和标准错误。
    //STD_OUTPUT_HANDLE正是代表标准输出（也就是显示屏）的宏
    hout = GetStdHandle(STD_OUTPUT_HANDLE);

    //SetConsoleCursorPosition函数用于设置控制台光标的位置
    SetConsoleCursorPosition(hout, cor);
}

//延时
void Wait(int t)
{
    gotoxy(0, E_H * f_num + 2);

    printf("%d\n", T++);

    gotoxy(0, E_H * f_num + 4);
    Sleep(t);
}
void prstr(char *ch)
{
    gotoxy(0, E_H * f_num + 3);

    printf("%-20s\n", ch);
}
void prlight(int n, int flag, int status)
{
    light[n][flag] = status;

    int x = E_W * 2 + 1;
    int y = 1 + (f_num - n) * E_H + 1;
    if (flag == 1)
    {
        if (status)
        {
            gotoxy(x, y);
            printf("▲");
        }
        else
        {
            gotoxy(x, y);
            printf("  ");
        }
    }
    else
    {
        if (status)
        {
            gotoxy(x, y + 2);
            printf("▼");
        }
        else
        {
            gotoxy(x, y + 2);
            printf("  ");
        }
    }
}

void prinlight(int n, int status)
{
    inlight[n] = status;

    int x = E_W * 3 + 5;
    int y = 1 + (n - 1) * 4;

    if (status)
    {
        gotoxy(x, y++);
        printf("║ █████ ║");
        gotoxy(x, y++);
        printf("║ █ %d █ ║", n);
        gotoxy(x, y);
        printf("║ █████ ║");
    }
    else
    {
        gotoxy(x, y++);
        printf("║       ║");
        gotoxy(x, y++);
        printf("║   %d   ║", n);
        gotoxy(x, y);
        printf("║       ║");
    }
}
passenger Stackindex(int n)
{
    int p[10] = {0};
    int cnt = 0;
    int k;
    while (cnt < n)
    {
        int mi = 0x3f3f3f3f;
        for (int i = 1; i <= f_num; i++)
            if (p[i] < StackLength(&S[i]) && S[i].base[p[i]].entime < mi)
            {
                mi = S[i].base[p[i]].entime;
                k = i;
            }
        p[k]++;
        cnt++;
    }
    if (cnt == n)
        return S[k].base[p[k] - 1];
}

//打印初始界面
void printerface()
{
    CLS;

    puts(str[3]);
    for (int i = 0; i < f_num; i++)
    {
        for (int j = 1; j < E_H; j++)
            puts(str[0]);
        if (i < f_num - 1)
            puts(str[1]);
    }
    puts(str[2]);
    puts(str[3]);

    int x = E_W * 3 + 5;
    int y = 0;
    gotoxy(x, y++);
    printf("╔═══════╗");
    for (int i = 1; i <= f_num; i++)
    {

        gotoxy(x, y++);
        printf("║       ║");
        gotoxy(x, y++);
        printf("║   %d   ║", i);
        gotoxy(x, y++);
        printf("║       ║");
        if (i < f_num)
        {
            gotoxy(x, y++);
            printf("╠═══════╣");
        }
    }
    gotoxy(x, y++);
    printf("╚═══════╝");
    gotoxy(0, E_H * f_num + 5);
}

//随机生成小人
passenger randp()
{
    passenger p;

    p.w = rand() % 51 + 50;
    p.n_f = rand() % f_num + 1;
    p.intime = T;

    return p;
}

//打印一个人
void prap(int x, int y, passenger p)
{
    gotoxy(x, y++);
    printf(" %2dF  ", p.n_f);
    gotoxy(x, y++);
    printf("%3dkg ", p.w);
    gotoxy(x, y++);
    puts(p_str[0]);
    gotoxy(x, y++);
    puts(p_str[1]);
    gotoxy(x, y++);
    puts(p_str[2]);
}

//在第n层生成一个小人
void adp(int n)
{

    int x = E_W + 1 + (p_num[n]++) * 6;
    int y = 1 + (f_num - n) * E_H;

    passenger p = randp();
    while (p.n_f == n)
        p = randp();

    push_back(&Q[n][p.n_f > n], p);

    prlight(n, p.n_f > n, TRUE);

    if (e_status == STOP)
        e_status = n > e_floor ? UP : DOWN;
    prap(x, y, p);
    prstr("生成了一个人");
    Wait(1000);
}

void rande(int k)
{
    while (k--)
    {
        int n = rand() % f_num + 1;
        adp(n);
    }
}
//更新第n层等候的人
void udp(int n)
{
    passenger p;
    QNode *uq = Q[n][1].front->next;
    QNode *dq = Q[n][0].front->next;

    int x = 50 + 1;
    int y = 1 + (f_num - n) * E_H;

    //清空该层
    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y + i);
        puts("                                                 ");
    }

    while (uq && dq)
    {
        if (uq->data.intime < dq->data.intime)
        {
            p = uq->data;
            uq = uq->next;
        }
        else
        {
            p = dq->data;
            dq = dq->next;
        }

        prap(x, y, p);

        x += 6;
    }

    while (uq)
    {
        p = uq->data;
        uq = uq->next;

        prap(x, y, p);

        x += 6;
    }

    while (dq)
    {
        p = dq->data;
        dq = dq->next;

        prap(x, y, p);

        x += 6;
    }
}

//移除第n层一个人
void rmp(int n)
{
    int x = E_W + 1 + (--p_num[n]) * 6;
    int y = 1 + (f_num - n) * E_H;

    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y + i);
        puts("     ");
    }
    prstr("离开了一个人");
    Wait(1000);
}

//第n层一个人进电梯
int mpe(int n)
{

    passenger p;

    if (!QueueEmpty(&Q[n][1]) && e_status == UP)
    {
        p = getfront(&Q[n][1]);
        pop_front(&Q[n][1]);
    }
    else if (!QueueEmpty(&Q[n][0]) && e_status == DOWN)
    {
        p = getfront(&Q[n][0]);
        pop_front(&Q[n][0]);
    }
    else
        return FALSE;

    int x = E_W + 1;
    int y = 1 + (f_num - n) * E_H;

    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y + i);
        puts("     ");
    }

    p.entime = T;
    push(&S[p.n_f], p);

    prinlight(p.n_f, TRUE);

    x = 2 + (e_num++) * 6;
    prap(x, y, p);

    p_num[n]--;
    udp(n);
    prstr("一个人进入了电梯");
    Wait(1000);
    return TRUE;
}

//第n层一人离开电梯
int rmpe(int n)
{
    if (StackEmpty(&S[n]))
        return FALSE;
    pop(&S[n]);

    int x = 2 + (--e_num) * 6;
    int y = 1 + (f_num - n) * 6;

    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y + i);
        puts("     ");
    }
    prstr("一个人离开了电梯");
    Wait(1000);
    return TRUE;
}
void pre(int n, int k)
{
    int x = 2;
    int y = (f_num - n) * 6 - k;

    gotoxy(x, y++);
    if (y > 1)
        puts("                                                ");

    gotoxy(x, y++);
    for (int i = 1; i <= e_num; i++)
        printf(" %2dF  ", Stackindex(i).n_f);
    for (int j = x + e_num * 6; j < E_W - 1; j++)
        putchar(' ');

    gotoxy(x, y++);
    for (int i = 1; i <= e_num; i++)
        printf("%3dkg ", Stackindex(i).w);
    for (int j = x + e_num * 6; j < E_W - 1; j++)
        putchar(' ');

    for (int i = 0; i < 3; i++)
    {
        gotoxy(x, y++);
        for (int j = 0; j < e_num; j++)
            printf("%s ", p_str[i]);
        for (int j = x + e_num * 6; j < E_W; j++)
            putchar(' ');
    }
    putchar('\n');

    puts("█ ================================================");
    if (y < E_H * f_num)
        puts("█                                                 ");

    Wait(1000);
}
void updown(int flag)
{
    for (int i = 1; i <= E_H; i++)
        pre(e_floor, flag * i);

    e_floor += flag;
}
void run()
{
    if (e_status == UP)
    {
        //当前层需要停
        if (light[e_floor][1] || inlight[e_floor])
        {
            prlight(e_floor, 1, FALSE);
            prinlight(e_floor, FALSE);
            while (rmpe(e_floor))
                ;
            while (mpe(e_floor))
                ;
        }

        int flag = FALSE;
        for (int i = e_floor + 1; i <= f_num; i++)
            if (light[i][1] || light[i][0] || inlight[i])
            {
                flag = TRUE;
                break;
            }
        if (!flag)
        {
            for (int i = e_floor; i > 0; i--)
                if (light[i][1] || light[i][0] || inlight[i])
                {
                    flag = TRUE;
                    break;
                }
            if (flag)
            {
                prlight(e_floor, 0, FALSE);
                e_status = DOWN;
                while (mpe(e_floor))
                    ;
                updown(DOWN);
            }
            else
            {
                e_status = STOP;
                Wait(1000);
            }

            return;
        }
        updown(UP);
    }
    else if (e_status == DOWN)
    {
        //当层是否停
        if (light[e_floor][0] || inlight[e_floor])
        {
            prlight(e_floor, 0, FALSE);
            prinlight(e_floor, FALSE);
            while (rmpe(e_floor))
                ;
            while (mpe(e_floor))
                ;
        }

        int flag = FALSE;
        for (int i = e_floor - 1; i > 0; i--)
            if (light[i][0] || light[i][1] || inlight[i])
            {
                flag = TRUE;
                break;
            }

        if (!flag)
        {
            for (int i = e_floor; i <= f_num; i++)
                if (light[i][1] || light[i][0] || inlight[i])
                {
                    flag = TRUE;
                    break;
                }
            if (flag)
            {
                prlight(e_floor, 1, FALSE);
                e_status = UP;
                while (mpe(e_floor))
                    ;
                updown(UP);
            }
            else
            {
                e_status = STOP;
                Wait(1000);
            }

            return;
        }
        updown(DOWN);
    }
    else
    {
        prstr("等待中");
        Wait(1000);
    }
}
int main()
{

    srand((unsigned)time(NULL));

    int n;
    scanf("%d", &f_num);

    for (int i = 1; i <= f_num; i++)
    {
        InitStack(&S[i]);
        InitQueue(&Q[i][0]);
        InitQueue(&Q[i][1]);
    }

    printerface();

    Sleep(1000);

    rande(1);
    run();
    run();
    run();
    rande(2);
    run();
    run();
    run();
    rande(5);
    run();
    run();
    run();
    rande(3);
    run();
    run();
    run();

    system("pause");
    return 0;
}