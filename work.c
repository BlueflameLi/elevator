//the coding is GB2312
#include "work.h"

//��ʱ
void Wait(int t)
{
    gotoxy(0, F_H * F.num + 2);

    printf("����������%d����\n", T++);

    gotoxy(0, F_H * F.num + 4);
    Sleep(t);
}

//�������С��
passenger randp()
{
    passenger p;

    p.w = rand() % 51 + 50;
    p.n_f = rand() % F.num + 1;
    p.intime = T;
    p.waittime = rand() % 51 + 30;

    return p;
}

//�ڵ�n������һ��С��
void adp(int n)
{
    //�ò���վ���˾Ͳ�����
    if (p_num[n] == E_N_MAX)
        return;

    int x = F_W + 1 + (p_num[n]++) * 6;
    int y = 1 + (F.num - n) * F_H;

    //�����
    passenger p = randp();
    while (p.n_f == n)
        p = randp();

    //���
    push_back(&F.Q[n][p.n_f > n], p);

    //���µ�����İ�ť
    prlight(n, p.n_f > n, TRUE);

    //������ݴ��ڵȴ�״̬����һ���������ĵ��ݵ�״̬
    if (E.status == STOP)
        E.status = n > E.floor ? UP : DOWN;

    //����С��
    prap(x, y, p);
}

//�������k����
void rande()
{
    //����������ɵ�����Խ��Խ��
    if (T >= 250)
        return;
    int k = rand() % (RANDPN - T / 50);
    if (k < 1)
        return;

    while (k--)
    {
        int n = rand() % F.num + 1;
        adp(n);
    }

    prstr("���ɳ˿�");

    Wait(SLEEPTIME);
}

//���µ�n��Ⱥ����
void udp(int n)
{
    passenger p;
    QNode *q[2] = {F.Q[n][0].front->next,
                   F.Q[n][1].front->next};
    int x = 50 + 1;
    int y = 1 + (F.num - n) * F_H;

    //��ոò�
    for (int i = 0; i < 5; i++)
    {
        gotoxy(x, y + i);
        puts("                                                 ");
    }

    //�������
    while (q[1] && q[0])
    {
        int k = q[1]->data.intime < q[0]->data.intime;
        p = q[k]->data;
        q[k] = q[k]->next;

        prap(x, y, p);

        x += 6;
    }

    for (int i = 0; i < 2; i++)
    {
        while (q[i])
        {
            p = q[i]->data;
            q[i] = q[i]->next;

            prap(x, y, p);

            x += 6;
        }
    }
}

//¥������һ����
void adpf(int n, passenger p)
{
    int x = F_W * 3 - 1 - (++p_f_num[n]) * 6;
    int y = 1 + (F.num - n) * F_H;
    prap(x, y, p);
}

//���¥�������
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

//���ĵȴ����
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
                        prstr("���˵ȴ�ʱ��������뿪");
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
                    prstr("������ȴ�ʱ��������뿪");
                }
            }
        }

    Wait(SLEEPTIME);
    for (int i = 1; i <= F.num; i++)
        clpf(i);
}

//��ⳬ��
int checkoverload(int n, passenger p)
{
    if (E.num >= E_N_MAX || E.p_w + p.w > E_W_MAX + 10)
    {
        E.overload = TRUE;
        prstr("\a����");
        Wait(SLEEPTIME);
        return FALSE;
    }
    return TRUE;
}

//������
void ocdoor(int n, int status)
{
    int x = F_W;
    int y = 1 + (F.num - n) * F_H;
    gotoxy(x, y++);
    
    for (int i = 0; i < 5; i++)
    {
        putchar(status == OPEN ? ' ' : '|');
        gotoxy(x, y++);
    }

    prstr(status == OPEN ? "����" : "����");
    Wait(SLEEPTIME);
}

//��n��һ���˽�����
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

    //��¼�������ʱ��
    p.entime = T;
    push(&E.S[p.n_f], p);

    //���°�ť
    prinlight(p.n_f, TRUE);

    E.p_w += p.w;

    x = 2 + (E.num++) * 6;
    prap(x, y, p);

    p_num[n]--;

    //���µȺ����
    udp(n);

    prstr("һ���˽����˵���");
    Wait(SLEEPTIME);
    return TRUE;
}

//һ�����뿪����
int rmpe(int n)
{
    if (StackEmpty(&E.S[n]))
        return FALSE;

    passenger p = *(gettop(&E.S[n]));
    pop(&E.S[n]);

    --E.num;
    E.p_w -= p.w;
    prstr("һ�����뿪�˵���");

    //���µ����ϵ���
    pre(n, 0);

    //�˽���¥��
    adpf(n, p);

    Wait(SLEEPTIME);

    return TRUE;
}

//�ϻ����ƶ�һ��
void updown(int flag)
{
    if (flag > 0)
        prstr("������");
    else
        prstr("�½���");
    for (int i = 1; i <= F_H; i++)
    {
        pre(E.floor, flag * i);
        Wait(SLEEPTIME);
    }

    E.floor += flag;
}

//����һ�ε��ݣ�����һ����½�һ���ԭ�صȴ�
void run()
{

    if (E.status == UP)
    {
        if (E.floor == F.num)
        {
            E.status = DOWN;
            return;
        }
        //�жϵ�ǰ���Ƿ���Ҫͣ
        if (F.light[E.floor][1] || E.light[E.floor])
        {
            //����㰴ť���
            prlight(E.floor, 1, FALSE);
            prinlight(E.floor, FALSE);

            ocdoor(E.floor, OPEN);
            //�뿪����
            while (rmpe(E.floor))
                ;

            //���¥�������
            clpf(E.floor);

            //�������
            while (mpe(E.floor))
                ;

            ocdoor(E.floor, CLOSE);
        }

        //�ж��Ƿ��и߲�����
        int flag = FALSE;
        for (int i = E.floor + 1; i <= F.num; i++)
            if (F.light[i][1] || F.light[i][0] || E.light[i])
            {
                flag = TRUE;
                break;
            }

        //���ޣ����ж��Ƿ��еͲ�����
        if (!flag)
        {
            for (int i = E.floor; i > 0; i--)
                if (F.light[i][1] || F.light[i][0] || E.light[i])
                {
                    flag = TRUE;
                    break;
                }

            //���У����˽�����ݣ������½�һ��
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
                //���ݽ���ֹͣ״̬
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
        //����һ��
        updown(UP);
    }
    else if (E.status == DOWN)
    {
        if (E.floor == 1)
        {
            E.status = UP;
            return;
        }
        //�жϵ����Ƿ�ͣ
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

        //�ж��Ƿ��еͲ�����
        int flag = FALSE;
        for (int i = E.floor - 1; i > 0; i--)
            if (F.light[i][0] || F.light[i][1] || E.light[i])
            {
                flag = TRUE;
                break;
            }

        //���ޣ����ж��Ƿ��и߲�����
        if (!flag)
        {
            for (int i = E.floor; i <= F.num; i++)
                if (F.light[i][1] || F.light[i][0] || E.light[i])
                {
                    flag = TRUE;
                    break;
                }

            //���У��������ݣ���������
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
            prstr("��ʱ�����ˣ��ص�1�����");
        }
        else
            prstr("�ȴ���");
        Wait(SLEEPTIME);
    }
}