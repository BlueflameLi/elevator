//the coding is GB2312
#include "interface.h"

//���ݽ���
char *str[4] = {
    "��                                                 |                                                                                                  ��",
    "��                                                 ����������������������������������������������������������������������������������������������������",
    "��=================================================����������������������������������������������������������������������������������������������������",
    "����������������������������������������������������������������������������������������������������������������������������������"};

char *lightstr[3][3] = {
    {"�U       �U",
     "�U   %d   �U",
     "�U       �U"},
    {"�U ���������� �U",
     "�U �� %d �� �U",
     "�U ���������� �U"},
    {"�X�T�T�T�T�T�T�T�[",
     "�d�T�T�T�T�T�T�T�g",
     "�^�T�T�T�T�T�T�T�a"}};
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
    printf(lightstr[2][0]);
    for (int i = 1; i <= F.num; i++)
    {
        gotoxy(x, y++);
        printf(lightstr[0][0]);
        gotoxy(x, y++);
        printf(lightstr[0][1], i);
        gotoxy(x, y++);
        printf(lightstr[0][2]);
        if (i < F.num)
        {
            gotoxy(x, y++);
            printf(lightstr[2][1]);
        }
    }
    gotoxy(x, y++);
    printf(lightstr[2][2]);
    gotoxy(0, F_H * F.num + 5);
}

//��ӡһ���ˣ�x��yΪС�˵�λ�ã����Ͻ��ǵ㣩��pΪС����Ϣ
void prap(int x, int y, passenger p)
{
    gotoxy(x, y++);
    printf(" %2dF  ", p.n_f);
    gotoxy(x, y++);
    printf("%3dkg ", p.w);
    for (int i = 0; i < 3; i++)
    {
        gotoxy(x, y++);
        puts(p_str[i]);
    }
}

//��ӡ¥����ݰ�ť��nΪ��ǰ�㣬flagΪ���л������а�ť��statusΪ����״̬
void prlight(int n, int flag, int status)
{
    //����״̬
    F.light[n][flag] = status;

    int x = F_W * 2 + 1;
    int y = 1 + (F.num - n) * F_H + 1;

    gotoxy(x, flag == 1 ? y : y + 2);
    printf(status ? (flag == 1 ? "��" : "��") : "  ");
}

//��ӡ�����ڵİ�ť��nΪ��ť��Ӧ�Ĳ㣬statusΪ����״̬
void prinlight(int n, int status)
{
    //�л�״̬
    E.light[n] = status;

    int x = F_W * 3 + 5;
    int y = 1 + (n - 1) * 4;

    gotoxy(x, y++);
    printf(lightstr[status][0]);
    gotoxy(x, y++);
    printf(lightstr[status][1], n);
    gotoxy(x, y);
    printf(lightstr[status][2]);
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
    for (int k = 0; k < 2; k++)
    {
        gotoxy(x, y++);
        for (int i = 1; i <= E.num; i++)
            printf(k ? "%3dkg " : " %2dF  ", k ? Stackindex(i).w : Stackindex(i).n_f);
        for (int i = x + E.num * 6; i < F_W; i++)
            putchar(' ');
    }

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