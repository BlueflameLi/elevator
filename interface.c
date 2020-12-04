//the coding is GB2312
#include "interface.h"

//电梯界面
char *str[4] = {
    "█                                                 |                                                                                                  █",
    "█                                                 ██████████████████████████████████████████████████",
    "█=================================================██████████████████████████████████████████████████",
    "█████████████████████████████████████████████████████████████████"};

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

//打印提示信息
void prstr(char *ch)
{
    gotoxy(0, F_H * F.num + 3);

    printf("%-30s", ch);
}

//打印初始界面
void printerface()
{
    //清空屏幕
    CLS;

    //打印天花板
    puts(str[3]);

    //打印中间层
    for (int i = 0; i < F.num; i++)
    {
        for (int j = 1; j < F_H; j++)
            puts(str[0]);
        if (i < F.num - 1)
            puts(str[1]);
    }

    //打印地板
    puts(str[2]);
    puts(str[3]);

    //绘制电梯内按钮
    int x = F_W * 3 + 5;
    int y = 0;
    gotoxy(x, y++);
    printf("╔═══════╗");
    for (int i = 1; i <= F.num; i++)
    {

        gotoxy(x, y++);
        printf("║       ║");
        gotoxy(x, y++);
        printf("║   %d   ║", i);
        gotoxy(x, y++);
        printf("║       ║");
        if (i < F.num)
        {
            gotoxy(x, y++);
            printf("╠═══════╣");
        }
    }
    gotoxy(x, y++);
    printf("╚═══════╝");
    gotoxy(0, F_H * F.num + 5);
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

//打印楼层电梯按钮，n为当前层，flag为上行还是下行按钮，status为开关状态
void prlight(int n, int flag, int status)
{
    //更改状态
    F.light[n][flag] = status;

    int x = F_W * 2 + 1;
    int y = 1 + (F.num - n) * F_H + 1;
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
    E.light[n] = status;

    int x = F_W * 3 + 5;
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

//绘制电梯及上面的人，n为层数，k表示上移或下移k格
void pre(int n, int k)
{
    int x = 2;
    int y = (F.num - n) * 6 - k;

    //覆盖上面的字符
    gotoxy(x, y++);
    if (y > 1)
        puts("                                                ");

    //画人
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

    //画电梯
    puts("█ ================================================");

    //覆盖下面的字符
    if (y < F_H * F.num)
        puts("█                                                 ");
}