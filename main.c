//the coding is GB2312
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
#define E_W_MAX 520       //最大载重
#define E_N_MAX 8         //最大人数
#define STOP 0
#define DOWN -1
#define UP 1
#define OPEN 1
#define CLOSE 0
#define TRUE 1
#define FALSE 0
#define SLEEPTIME 500
#define WAITTIME 30
#define RANDPN 5
//电梯界面
char *str[] = {
    "█                                                 |                                                                                                  █",
    "█                                                 ██████████████████████████████████████████████████",
    "█=================================================██████████████████████████████████████████████████",
    "█████████████████████████████████████████████████████████████████"};

char *p_str[] = {"  O  ",
                 "--|--",
                 " / \\ "};
int p_num[MAXN];   //每层的人数
int p_f_num[MAXN]; //每层楼道里的人
int e_num;         //电梯里的人数
int f_num;         //总楼层数
int e_floor = 1;   //当前电梯所在层
int e_p_w;
stack S[10];
LinkQueue Q[10][2]; //上升是1，下降是0
int e_status;       //电梯运行状态
int T;              //运行时间
int light[10][2];   //外面楼层离=里的电脑按钮
int inlight[10];    //电梯里面的按钮
int MaxTime = 300;
int overload;
int randp_num;
int waitedtime;
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

    printf("电梯已运行%d分钟\n", T++);

    gotoxy(0, E_H * f_num + 4);
    Sleep(t);
}

//打印提示信息
void prstr(char *ch)
{
    gotoxy(0, E_H * f_num + 3);

    printf("%-30s", ch);
}

//打印楼层电梯按钮，n为当前层，flag为上行还是下行按钮，status为开关状态
void prlight(int n, int flag, int status)
{
    //更改状态
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

//打印电梯内的按钮，n为按钮对应的层，status为开关状态
void prinlight(int n, int status)
{
    //切换状态
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

//乘客栈索引，按进入电梯的顺序排序，n为第n个进入电梯的
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
    //清空屏幕
    CLS;

    //打印天花板
    puts(str[3]);

    //打印中间层
    for (int i = 0; i < f_num; i++)
    {
        for (int j = 1; j < E_H; j++)
            puts(str[0]);
        if (i < f_num - 1)
            puts(str[1]);
    }

    //打印地板
    puts(str[2]);
    puts(str[3]);

    //绘制电梯内按钮
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

//打印一个人，x、y为小人的位置（左上角那点），p为小人信息
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
    if (p_num[n] == E_N_MAX)
        return;
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
}

//随机生成k个人
void rande()
{
    if (T >= 200)
        return;
    int k = rand() % (RANDPN - T / 40);
    if (k < 1)
        return;
    while (k--)
    {
        int n = rand() % f_num + 1;
        adp(n);
    }
    prstr("生成乘客");
    Wait(SLEEPTIME);
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

//楼道增加一个人
void adpf(int n, passenger p)
{
    int x = E_W * 3 - 1 - (++p_f_num[n]) * 6;
    int y = 1 + (f_num - n) * E_H;
    prap(x, y, p);
}

//清空楼道里的人
void clpf(int n)
{
    int x = E_W * 2 + 2;
    int y = 1 + (f_num - n) * E_H;
    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y++);
        puts("                                               ");
    }
    p_f_num[n] = 0;
}

//检测超重
int checkoverload(int n, passenger p)
{
    if (e_num >= E_N_MAX || e_p_w + p.w > E_W_MAX + 10)
    {
        overload = TRUE;
        prstr("\a超载");
        Wait(SLEEPTIME);
        return FALSE;
    }
    return TRUE;
}
void ocdoor(int n, int status)
{
    int x = E_W;
    int y = 1 + (f_num - n) * E_H;
    gotoxy(x, y++);
    if (status == OPEN)
        for (int i = 0; i < 5; i++)
        {
            putchar(' ');
            gotoxy(x, y++);
        }
    else
        for (int i = 0; i < 5; i++)
        {
            putchar('|');
            gotoxy(x, y++);
        }
    prstr(status == OPEN ? "开门" : "关门");
    Wait(SLEEPTIME);
}
//第n层一个人进电梯
int mpe(int n)
{

    passenger p;

    if (!QueueEmpty(&Q[n][1]) && e_status == UP)
    {
        p = getfront(&Q[n][1]);
        if (checkoverload(n, p))
            pop_front(&Q[n][1]);
        else
            return FALSE;
    }
    else if (!QueueEmpty(&Q[n][0]) && e_status == DOWN)
    {
        p = getfront(&Q[n][0]);
        if (checkoverload(n, p))
            pop_front(&Q[n][0]);
        else
            return FALSE;
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

    //记录进入电梯时间
    p.entime = T;
    push(&S[p.n_f], p);

    //按下按钮
    prinlight(p.n_f, TRUE);

    e_p_w += p.w;

    x = 2 + (e_num++) * 6;
    prap(x, y, p);

    p_num[n]--;

    //更新等候的人
    udp(n);

    prstr("一个人进入了电梯");
    Wait(SLEEPTIME);
    return TRUE;
}

//绘制电梯及上面的人，n为层数，k表示上移或下移k格
void pre(int n, int k)
{
    int x = 2;
    int y = (f_num - n) * 6 - k;

    //覆盖上面的字符
    gotoxy(x, y++);
    if (y > 1)
        puts("                                                ");

    //画人
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

    //画电梯
    puts("█ ================================================");

    //覆盖下面的字符
    if (y < E_H * f_num)
        puts("█                                                 ");
}

//一个人离开电梯
int rmpe(int n)
{
    if (StackEmpty(&S[n]))
        return FALSE;

    passenger p = *(gettop(&S[n]));
    pop(&S[n]);

    --e_num;
    e_p_w -= p.w;
    prstr("一个人离开了电梯");

    //更新电梯上的人
    pre(n, 0);

    //人进入楼道
    adpf(n, p);

    Wait(SLEEPTIME);

    return TRUE;
}

//上或下移动一层
void updown(int flag)
{
    if (flag > 0)
        prstr("上升中");
    else
        prstr("下降中");
    for (int i = 1; i <= E_H; i++)
    {
        pre(e_floor, flag * i);
        Wait(SLEEPTIME);
    }

    e_floor += flag;
}

//运行一次电梯，上行一层或下降一层或原地等待
void run()
{

    if (e_status == UP)
    {
        if (e_floor == f_num)
        {
            e_status = DOWN;
            return;
        }
        //判断当前层是否需要停
        if (light[e_floor][1] || inlight[e_floor])
        {
            //到达层按钮灭掉
            prlight(e_floor, 1, FALSE);
            prinlight(e_floor, FALSE);

            ocdoor(e_floor, OPEN);
            //离开电梯
            while (rmpe(e_floor))
                ;

            //清空楼道里的人
            clpf(e_floor);

            //进入电梯
            while (mpe(e_floor))
                ;

            ocdoor(e_floor, CLOSE);
        }

        //判断是否有高层请求
        int flag = FALSE;
        for (int i = e_floor + 1; i <= f_num; i++)
            if (light[i][1] || light[i][0] || inlight[i])
            {
                flag = TRUE;
                break;
            }

        //若无，则判断是否有低层请求
        if (!flag)
        {
            for (int i = e_floor; i > 0; i--)
                if (light[i][1] || light[i][0] || inlight[i])
                {
                    flag = TRUE;
                    break;
                }

            //若有，则人进入电梯，电梯下降一层
            if (flag)
            {
                prlight(e_floor, 0, FALSE);
                e_status = DOWN;
                if (overload)
                {
                    prlight(e_floor, 1, TRUE);
                    overload = FALSE;
                }
                ocdoor(e_floor, OPEN);
                while (mpe(e_floor))
                    ;
                ocdoor(e_floor, CLOSE);
                if (overload)
                {
                    prlight(e_floor, 0, TRUE);
                    overload = FALSE;
                }
                updown(DOWN);
            }
            else
            {
                //电梯进入停止状态
                e_status = STOP;
                Wait(SLEEPTIME);
            }

            return;
        }
        if (overload)
        {
            prlight(e_floor, 1, TRUE);
            overload = FALSE;
        }
        //上行一层
        updown(UP);
    }
    else if (e_status == DOWN)
    {
        if (e_floor == 1)
        {
            e_status = UP;
            return;
        }
        //判断当层是否停
        if (light[e_floor][0] || inlight[e_floor])
        {
            prlight(e_floor, 0, FALSE);
            prinlight(e_floor, FALSE);

            ocdoor(e_floor, OPEN);
            while (rmpe(e_floor))
                ;

            clpf(e_floor);

            while (mpe(e_floor))
                ;
            ocdoor(e_floor, CLOSE);
        }

        //判断是否有低层请求
        int flag = FALSE;
        for (int i = e_floor - 1; i > 0; i--)
            if (light[i][0] || light[i][1] || inlight[i])
            {
                flag = TRUE;
                break;
            }

        //若无，则判断是否有高层请求
        if (!flag)
        {
            for (int i = e_floor; i <= f_num; i++)
                if (light[i][1] || light[i][0] || inlight[i])
                {
                    flag = TRUE;
                    break;
                }

            //若有，则进入电梯，电梯上行
            if (flag)
            {
                prlight(e_floor, 1, FALSE);
                e_status = UP;
                if (overload)
                {
                    prlight(e_floor, 0, TRUE);
                    overload = FALSE;
                }
                ocdoor(e_floor, OPEN);
                while (mpe(e_floor))
                    ;
                ocdoor(e_floor, CLOSE);
                if (overload)
                {
                    prlight(e_floor, 1, TRUE);
                    overload = FALSE;
                }
                updown(UP);
            }
            else
            {
                e_status = STOP;
                Wait(SLEEPTIME);
            }

            return;
        }
        if (overload)
        {
            prlight(e_floor, 0, TRUE);
            overload = FALSE;
        }
        updown(DOWN);
    }
    else
    {
        if (++waitedtime >= 10 && e_floor > 1)
        {
            updown(DOWN);
            prstr("长时间无人，回到1层待命");
        }
        else
            prstr("等待中");
        Wait(SLEEPTIME);
    }
}
int main()
{
    //随机种子初始化
    srand((unsigned)time(NULL));

    //读入电梯层数
    int n;
    puts("请输入电梯层数");
    scanf("%d", &f_num);

    //初始化栈和队列
    for (int i = 1; i <= f_num; i++)
    {
        InitStack(&S[i]);
        InitQueue(&Q[i][0]);
        InitQueue(&Q[i][1]);
    }

    //打印初始界面
    printerface();

    Sleep(SLEEPTIME);

    while (T < MaxTime)
    {
        rande();
        run();
    }
    prstr("结束");
    system("pause");
    return 0;
}