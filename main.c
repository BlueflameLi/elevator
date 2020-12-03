//the coding is GB2312
#include <string.h>
#include <time.h>
#include "interface.h"

#define TRUE 1
#define FALSE 0
#define SLEEPTIME 1000
#define RANDPN 5
#define MAXTIME 300

int T; //运行时间
int randp_num;
int waitedtime;

//延时
void Wait(int t)
{
    gotoxy(0, F_H * F.num + 2);

    printf("电梯已运行%d分钟\n", T++);

    gotoxy(0, F_H * F.num + 4);
    Sleep(t);
}

//随机生成小人
passenger randp()
{
    passenger p;

    p.w = rand() % 51 + 50;
    p.n_f = rand() % F.num + 1;
    p.intime = T;
    p.waittime = rand() % 21 + 30;
    return p;
}

//在第n层生成一个小人
void adp(int n)
{
    if (p_num[n] == E_N_MAX)
        return;
    int x = F_W + 1 + (p_num[n]++) * 6;
    int y = 1 + (F.num - n) * F_H;

    passenger p = randp();
    while (p.n_f == n)
        p = randp();

    push_back(&F.Q[n][p.n_f > n], p);

    prlight(n, p.n_f > n, TRUE);

    if (E.status == STOP)
        E.status = n > E.floor ? UP : DOWN;
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
        int n = rand() % F.num + 1;
        adp(n);
    }
    prstr("生成乘客");
    Wait(SLEEPTIME);
}

//更新第n层等候的人
void udp(int n)
{
    passenger p;
    QNode *uq = F.Q[n][1].front->next;
    QNode *dq = F.Q[n][0].front->next;

    int x = 50 + 1;
    int y = 1 + (F.num - n) * F_H;

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
    int x = F_W * 3 - 1 - (++p_f_num[n]) * 6;
    int y = 1 + (F.num - n) * F_H;
    prap(x, y, p);
}

//清空楼道里的人
void clpf(int n)
{
    int x = F_W * 2 + 2;
    int y = 1 + (F.num - n) * F_H;
    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y++);
        puts("                                               ");
    }
    p_f_num[n] = 0;
}
void checkwait()
{
    for (int i = 1; i <= F.num; i++)
        for (int j = 0; j < 2; j++)
        {
            if (!QueueEmpty(&F.Q[i][j]))
            {
                QNode *p = F.Q[i][j].front;
                while (p->next != F.Q[i][j].rear)
                {
                    if (T >= p->next->data.intime + p->next->data.waittime)
                    {
                        p_num[i]--;
                        adpf(i, p->next->data);
                        QNode *q = p->next;
                        p->next = p->next->next;
                        free(q);
                        udp(i);
                        prstr("一个人因等待时间过长而离开");
                    }
                    else
                        p = p->next;
                }
                if (T >= F.Q[i][j].rear->data.intime + F.Q[i][j].rear->data.waittime)
                {
                    p_num[i]--;
                    adpf(i, F.Q[i][j].rear->data);
                    free(F.Q[i][j].rear);
                    F.Q[i][j].rear = p;
                    p->next = NULL;
                    udp(i);
                    prstr("一个人因等待时间过长而离开");
                }
            }
        }
    Wait(SLEEPTIME);
    for (int i = 1; i <= F.num; i++)
        clpf(i);
}
//检测超重
int checkoverload(int n, passenger p)
{
    if (E.num >= E_N_MAX || E.p_w + p.w > E_W_MAX + 10)
    {
        E.overload = TRUE;
        prstr("\a超载");
        Wait(SLEEPTIME);
        return FALSE;
    }
    return TRUE;
}
void ocdoor(int n, int status)
{
    int x = F_W;
    int y = 1 + (F.num - n) * F_H;
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

    if (!QueueEmpty(&F.Q[n][1]) && E.status == UP)
    {
        p = getfront(&F.Q[n][1]);
        if (checkoverload(n, p))
            pop_front(&F.Q[n][1]);
        else
            return FALSE;
    }
    else if (!QueueEmpty(&F.Q[n][0]) && E.status == DOWN)
    {
        p = getfront(&F.Q[n][0]);
        if (checkoverload(n, p))
            pop_front(&F.Q[n][0]);
        else
            return FALSE;
    }
    else
        return FALSE;

    int x = F_W + 1;
    int y = 1 + (F.num - n) * F_H;

    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y + i);
        puts("     ");
    }

    //记录进入电梯时间
    p.entime = T;
    push(&E.S[p.n_f], p);

    //按下按钮
    prinlight(p.n_f, TRUE);

    E.p_w += p.w;

    x = 2 + (E.num++) * 6;
    prap(x, y, p);

    p_num[n]--;

    //更新等候的人
    udp(n);

    prstr("一个人进入了电梯");
    Wait(SLEEPTIME);
    return TRUE;
}

//一个人离开电梯
int rmpe(int n)
{
    if (StackEmpty(&E.S[n]))
        return FALSE;

    passenger p = *(gettop(&E.S[n]));
    pop(&E.S[n]);

    --E.num;
    E.p_w -= p.w;
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
    for (int i = 1; i <= F_H; i++)
    {
        pre(E.floor, flag * i);
        Wait(SLEEPTIME);
    }

    E.floor += flag;
}

//运行一次电梯，上行一层或下降一层或原地等待
void run()
{

    if (E.status == UP)
    {
        if (E.floor == F.num)
        {
            E.status = DOWN;
            return;
        }
        //判断当前层是否需要停
        if (F.light[E.floor][1] || E.light[E.floor])
        {
            //到达层按钮灭掉
            prlight(E.floor, 1, FALSE);
            prinlight(E.floor, FALSE);

            ocdoor(E.floor, OPEN);
            //离开电梯
            while (rmpe(E.floor))
                ;

            //清空楼道里的人
            clpf(E.floor);

            //进入电梯
            while (mpe(E.floor))
                ;

            ocdoor(E.floor, CLOSE);
        }

        //判断是否有高层请求
        int flag = FALSE;
        for (int i = E.floor + 1; i <= F.num; i++)
            if (F.light[i][1] || F.light[i][0] || E.light[i])
            {
                flag = TRUE;
                break;
            }

        //若无，则判断是否有低层请求
        if (!flag)
        {
            for (int i = E.floor; i > 0; i--)
                if (F.light[i][1] || F.light[i][0] || E.light[i])
                {
                    flag = TRUE;
                    break;
                }

            //若有，则人进入电梯，电梯下降一层
            if (flag)
            {
                prlight(E.floor, 0, FALSE);
                E.status = DOWN;
                if (E.overload)
                {
                    prlight(E.floor, 1, TRUE);
                    E.overload = FALSE;
                }
                ocdoor(E.floor, OPEN);
                while (mpe(E.floor))
                    ;
                ocdoor(E.floor, CLOSE);
                if (E.overload)
                {
                    prlight(E.floor, 0, TRUE);
                    E.overload = FALSE;
                }
                updown(DOWN);
            }
            else
            {
                //电梯进入停止状态
                E.status = STOP;
                Wait(SLEEPTIME);
            }

            return;
        }
        if (E.overload)
        {
            prlight(E.floor, 1, TRUE);
            E.overload = FALSE;
        }
        //上行一层
        updown(UP);
    }
    else if (E.status == DOWN)
    {
        if (E.floor == 1)
        {
            E.status = UP;
            return;
        }
        //判断当层是否停
        if (F.light[E.floor][0] || E.light[E.floor])
        {
            prlight(E.floor, 0, FALSE);
            prinlight(E.floor, FALSE);

            ocdoor(E.floor, OPEN);
            while (rmpe(E.floor))
                ;

            clpf(E.floor);

            while (mpe(E.floor))
                ;
            ocdoor(E.floor, CLOSE);
        }

        //判断是否有低层请求
        int flag = FALSE;
        for (int i = E.floor - 1; i > 0; i--)
            if (F.light[i][0] || F.light[i][1] || E.light[i])
            {
                flag = TRUE;
                break;
            }

        //若无，则判断是否有高层请求
        if (!flag)
        {
            for (int i = E.floor; i <= F.num; i++)
                if (F.light[i][1] || F.light[i][0] || E.light[i])
                {
                    flag = TRUE;
                    break;
                }

            //若有，则进入电梯，电梯上行
            if (flag)
            {
                prlight(E.floor, 1, FALSE);
                E.status = UP;
                if (E.overload)
                {
                    prlight(E.floor, 0, TRUE);
                    E.overload = FALSE;
                }
                ocdoor(E.floor, OPEN);
                while (mpe(E.floor))
                    ;
                ocdoor(E.floor, CLOSE);
                if (E.overload)
                {
                    prlight(E.floor, 1, TRUE);
                    E.overload = FALSE;
                }
                updown(UP);
            }
            else
            {
                E.status = STOP;
                Wait(SLEEPTIME);
            }

            return;
        }
        if (E.overload)
        {
            prlight(E.floor, 0, TRUE);
            E.overload = FALSE;
        }
        updown(DOWN);
    }
    else
    {
        if (++waitedtime >= 10 && E.floor > 1)
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