//the coding is GB2312
#include "interface.h"

//���ݽ���
char *str[4] = {
    "��                                                 |                                                                                                  ��",
    "��                                                 ����������������������������������������������������������������������������������������������������",
    "��=================================================����������������������������������������������������������������������������������������������������",
    "����������������������������������������������������������������������������������������������������������������������������������"};

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

//��ӡ��ʾ��Ϣ
void prstr(char *ch)
{
    gotoxy(0, F_H * F.num + 3);

    printf("%-30s", ch);
}

//��ӡ��ʼ����
void printerface()
{
    //�����Ļ
    CLS;

    //��ӡ�컨��
    puts(str[3]);

    //��ӡ�м��
    for (int i = 0; i < F.num; i++)
    {
        for (int j = 1; j < F_H; j++)
            puts(str[0]);
        if (i < F.num - 1)
            puts(str[1]);
    }

    //��ӡ�ذ�
    puts(str[2]);
    puts(str[3]);

    //���Ƶ����ڰ�ť
    int x = F_W * 3 + 5;
    int y = 0;
    gotoxy(x, y++);
    printf("�X�T�T�T�T�T�T�T�[");
    for (int i = 1; i <= F.num; i++)
    {

        gotoxy(x, y++);
        printf("�U       �U");
        gotoxy(x, y++);
        printf("�U   %d   �U", i);
        gotoxy(x, y++);
        printf("�U       �U");
        if (i < F.num)
        {
            gotoxy(x, y++);
            printf("�d�T�T�T�T�T�T�T�g");
        }
    }
    gotoxy(x, y++);
    printf("�^�T�T�T�T�T�T�T�a");
    gotoxy(0, F_H * F.num + 5);
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

//��ӡ¥����ݰ�ť��nΪ��ǰ�㣬flagΪ���л������а�ť��statusΪ����״̬
void prlight(int n, int flag, int status)
{
    //����״̬
    F.light[n][flag] = status;

    int x = F_W * 2 + 1;
    int y = 1 + (F.num - n) * F_H + 1;
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
    E.light[n] = status;

    int x = F_W * 3 + 5;
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

//���Ƶ��ݼ�������ˣ�nΪ������k��ʾ���ƻ�����k��
void pre(int n, int k)
{
    int x = 2;
    int y = (F.num - n) * 6 - k;

    //����������ַ�
    gotoxy(x, y++);
    if (y > 1)
        puts("                                                ");

    //����
    gotoxy(x, y++);
    for (int i = 1; i <= E.num; i++)
        printf(" %2dF  ", Stackindex(i).n_f);
    for (int j = x + E.num * 6; j < F_W - 1; j++)
        putchar(' ');
    gotoxy(x, y++);
    for (int i = 1; i <= E.num; i++)
        printf("%3dkg ", Stackindex(i).w);
    for (int j = x + E.num * 6; j < F_W - 1; j++)
        putchar(' ');
    for (int i = 0; i < 3; i++)
    {
        gotoxy(x, y++);
        for (int j = 0; j < E.num; j++)
            printf("%s ", p_str[i]);
        for (int j = x + E.num * 6; j < F_W; j++)
            putchar(' ');
    }
    putchar('\n');

    //������
    puts("�� ================================================");

    //����������ַ�
    if (y < F_H * F.num)
        puts("��                                                 ");
}