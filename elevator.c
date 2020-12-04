//the coding is UTF-8
#include "elevator.h"

elevator E = {0, 0, 0, 1};

//乘客栈索引，按进入电梯的顺序排序，n为第n个进入电梯的
passenger Stackindex(int n)
{
    int p[10] = {0};
    int cnt = 0;
    int k;

    while (cnt < n)
    {
        int mi = 0x3f3f3f3f;
        for (int i = 1; i <= F.num; i++)
            if (p[i] < StackLength(&E.S[i]) && E.S[i].base[p[i]].entime < mi)
            {
                mi = E.S[i].base[p[i]].entime;
                k = i;
            }
        p[k]++;
        cnt++;
    }

    if (cnt == n)
        return E.S[k].base[p[k] - 1];
}