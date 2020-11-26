//the coding is GB2312
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "stack.h"
#include "queue.h"
#include <Windows.h>
#define MAXN 100 + 10
#define CLS system("cls") //�����Ļ
#define E_W 50            //¥����
#define E_H 6             //¥��߶�
#define E_W_MAX 520       //�������
#define E_N_MAX 8         //�������
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
//���ݽ���
char *str[] = {
    "��                                                 |                                                                                                  ��",
    "��                                                 ����������������������������������������������������������������������������������������������������",
    "��=================================================����������������������������������������������������������������������������������������������������",
    "����������������������������������������������������������������������������������������������������������������������������������"};

char *p_str[] = {"  O  ",
                 "--|--",
                 " / \\ "};
int p_num[MAXN];   //ÿ�������
int p_f_num[MAXN]; //ÿ��¥�������
int e_num;         //�����������
int f_num;         //��¥����
int e_floor = 1;   //��ǰ�������ڲ�
int e_p_w;
stack S[10];
LinkQueue Q[10][2]; //������1���½���0
int e_status;       //��������״̬
int T;              //����ʱ��
int light[10][2];   //����¥����=��ĵ��԰�ť
int inlight[10];    //��������İ�ť
int MaxTime = 300;
int overload;
int randp_num;
int waitedtime;
//����ƶ�
void gotoxy(unsigned char x, unsigned char y)
{
    //COORD��Windows API�ж����һ�ֽṹ����ʾһ���ַ��ڿ���̨��Ļ�ϵ�����
    COORD cor;

    //���
    HANDLE hout;

    //�趨����Ҫ��λ��������
    cor.X = x;
    cor.Y = y;

    //GetStdHandle������ȡһ��ָ���ض���׼�豸�ľ����������׼���룬��׼����ͱ�׼����
    //STD_OUTPUT_HANDLE���Ǵ����׼�����Ҳ������ʾ�����ĺ�
    hout = GetStdHandle(STD_OUTPUT_HANDLE);

    //SetConsoleCursorPosition�����������ÿ���̨����λ��
    SetConsoleCursorPosition(hout, cor);
}

//��ʱ
void Wait(int t)
{
    gotoxy(0, E_H * f_num + 2);

    printf("����������%d����\n", T++);

    gotoxy(0, E_H * f_num + 4);
    Sleep(t);
}

//��ӡ��ʾ��Ϣ
void prstr(char *ch)
{
    gotoxy(0, E_H * f_num + 3);

    printf("%-30s", ch);
}

//��ӡ¥����ݰ�ť��nΪ��ǰ�㣬flagΪ���л������а�ť��statusΪ����״̬
void prlight(int n, int flag, int status)
{
    //����״̬
    light[n][flag] = status;

    int x = E_W * 2 + 1;
    int y = 1 + (f_num - n) * E_H + 1;
    if (flag == 1)
    {
        if (status)
        {
            gotoxy(x, y);
            printf("��");
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
            printf("��");
        }
        else
        {
            gotoxy(x, y + 2);
            printf("  ");
        }
    }
}

//��ӡ�����ڵİ�ť��nΪ��ť��Ӧ�Ĳ㣬statusΪ����״̬
void prinlight(int n, int status)
{
    //�л�״̬
    inlight[n] = status;

    int x = E_W * 3 + 5;
    int y = 1 + (n - 1) * 4;

    if (status)
    {
        gotoxy(x, y++);
        printf("�U ���������� �U");
        gotoxy(x, y++);
        printf("�U �� %d �� �U", n);
        gotoxy(x, y);
        printf("�U ���������� �U");
    }
    else
    {
        gotoxy(x, y++);
        printf("�U       �U");
        gotoxy(x, y++);
        printf("�U   %d   �U", n);
        gotoxy(x, y);
        printf("�U       �U");
    }
}

//�˿�ջ��������������ݵ�˳������nΪ��n��������ݵ�
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

//��ӡ��ʼ����
void printerface()
{
    //�����Ļ
    CLS;

    //��ӡ�컨��
    puts(str[3]);

    //��ӡ�м��
    for (int i = 0; i < f_num; i++)
    {
        for (int j = 1; j < E_H; j++)
            puts(str[0]);
        if (i < f_num - 1)
            puts(str[1]);
    }

    //��ӡ�ذ�
    puts(str[2]);
    puts(str[3]);

    //���Ƶ����ڰ�ť
    int x = E_W * 3 + 5;
    int y = 0;
    gotoxy(x, y++);
    printf("�X�T�T�T�T�T�T�T�[");
    for (int i = 1; i <= f_num; i++)
    {

        gotoxy(x, y++);
        printf("�U       �U");
        gotoxy(x, y++);
        printf("�U   %d   �U", i);
        gotoxy(x, y++);
        printf("�U       �U");
        if (i < f_num)
        {
            gotoxy(x, y++);
            printf("�d�T�T�T�T�T�T�T�g");
        }
    }
    gotoxy(x, y++);
    printf("�^�T�T�T�T�T�T�T�a");
    gotoxy(0, E_H * f_num + 5);
}

//�������С��
passenger randp()
{
    passenger p;

    p.w = rand() % 51 + 50;
    p.n_f = rand() % f_num + 1;
    p.intime = T;

    return p;
}

//��ӡһ���ˣ�x��yΪС�˵�λ�ã����Ͻ��ǵ㣩��pΪС����Ϣ
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

//�ڵ�n������һ��С��
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

//�������k����
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
    prstr("���ɳ˿�");
    Wait(SLEEPTIME);
}

//���µ�n��Ⱥ����
void udp(int n)
{
    passenger p;
    QNode *uq = Q[n][1].front->next;
    QNode *dq = Q[n][0].front->next;

    int x = 50 + 1;
    int y = 1 + (f_num - n) * E_H;

    //��ոò�
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

//¥������һ����
void adpf(int n, passenger p)
{
    int x = E_W * 3 - 1 - (++p_f_num[n]) * 6;
    int y = 1 + (f_num - n) * E_H;
    prap(x, y, p);
}

//���¥�������
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

//��ⳬ��
int checkoverload(int n, passenger p)
{
    if (e_num >= E_N_MAX || e_p_w + p.w > E_W_MAX + 10)
    {
        overload = TRUE;
        prstr("\a����");
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
    prstr(status == OPEN ? "����" : "����");
    Wait(SLEEPTIME);
}
//��n��һ���˽�����
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

    //��¼�������ʱ��
    p.entime = T;
    push(&S[p.n_f], p);

    //���°�ť
    prinlight(p.n_f, TRUE);

    e_p_w += p.w;

    x = 2 + (e_num++) * 6;
    prap(x, y, p);

    p_num[n]--;

    //���µȺ����
    udp(n);

    prstr("һ���˽����˵���");
    Wait(SLEEPTIME);
    return TRUE;
}

//���Ƶ��ݼ�������ˣ�nΪ������k��ʾ���ƻ�����k��
void pre(int n, int k)
{
    int x = 2;
    int y = (f_num - n) * 6 - k;

    //����������ַ�
    gotoxy(x, y++);
    if (y > 1)
        puts("                                                ");

    //����
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

    //������
    puts("�� ================================================");

    //����������ַ�
    if (y < E_H * f_num)
        puts("��                                                 ");
}

//һ�����뿪����
int rmpe(int n)
{
    if (StackEmpty(&S[n]))
        return FALSE;

    passenger p = *(gettop(&S[n]));
    pop(&S[n]);

    --e_num;
    e_p_w -= p.w;
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
    for (int i = 1; i <= E_H; i++)
    {
        pre(e_floor, flag * i);
        Wait(SLEEPTIME);
    }

    e_floor += flag;
}

//����һ�ε��ݣ�����һ����½�һ���ԭ�صȴ�
void run()
{

    if (e_status == UP)
    {
        if (e_floor == f_num)
        {
            e_status = DOWN;
            return;
        }
        //�жϵ�ǰ���Ƿ���Ҫͣ
        if (light[e_floor][1] || inlight[e_floor])
        {
            //����㰴ť���
            prlight(e_floor, 1, FALSE);
            prinlight(e_floor, FALSE);

            ocdoor(e_floor, OPEN);
            //�뿪����
            while (rmpe(e_floor))
                ;

            //���¥�������
            clpf(e_floor);

            //�������
            while (mpe(e_floor))
                ;

            ocdoor(e_floor, CLOSE);
        }

        //�ж��Ƿ��и߲�����
        int flag = FALSE;
        for (int i = e_floor + 1; i <= f_num; i++)
            if (light[i][1] || light[i][0] || inlight[i])
            {
                flag = TRUE;
                break;
            }

        //���ޣ����ж��Ƿ��еͲ�����
        if (!flag)
        {
            for (int i = e_floor; i > 0; i--)
                if (light[i][1] || light[i][0] || inlight[i])
                {
                    flag = TRUE;
                    break;
                }

            //���У����˽�����ݣ������½�һ��
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
                //���ݽ���ֹͣ״̬
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
        //����һ��
        updown(UP);
    }
    else if (e_status == DOWN)
    {
        if (e_floor == 1)
        {
            e_status = UP;
            return;
        }
        //�жϵ����Ƿ�ͣ
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

        //�ж��Ƿ��еͲ�����
        int flag = FALSE;
        for (int i = e_floor - 1; i > 0; i--)
            if (light[i][0] || light[i][1] || inlight[i])
            {
                flag = TRUE;
                break;
            }

        //���ޣ����ж��Ƿ��и߲�����
        if (!flag)
        {
            for (int i = e_floor; i <= f_num; i++)
                if (light[i][1] || light[i][0] || inlight[i])
                {
                    flag = TRUE;
                    break;
                }

            //���У��������ݣ���������
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
            prstr("��ʱ�����ˣ��ص�1�����");
        }
        else
            prstr("�ȴ���");
        Wait(SLEEPTIME);
    }
}
int main()
{
    //������ӳ�ʼ��
    srand((unsigned)time(NULL));

    //������ݲ���
    int n;
    puts("��������ݲ���");
    scanf("%d", &f_num);

    //��ʼ��ջ�Ͷ���
    for (int i = 1; i <= f_num; i++)
    {
        InitStack(&S[i]);
        InitQueue(&Q[i][0]);
        InitQueue(&Q[i][1]);
    }

    //��ӡ��ʼ����
    printerface();

    Sleep(SLEEPTIME);

    while (T < MaxTime)
    {
        rande();
        run();
    }
    prstr("����");
    system("pause");
    return 0;
}