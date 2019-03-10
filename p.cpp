
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;
#define M 4 //迷宫大小和出口位置
#define N 4
#define MaxSize 100      //规定栈空间最大是100
int mg[M + 2][N + 2] = { // 1 障碍，0 可走点，-1已走过点
    {1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1}};
struct migong
{
    int i;
    int j;
    int di;                      //记录方向
} Stack[MaxSize], Path[MaxSize]; //栈和最短路径
int top = -1;
int count = 1;        //计步君
int minlen = MaxSize; //最短长度

void mgpath()
{
    int i, j, di, find, k; // i,j表示当前位置，di为方向，find为是否找到了可走点，找到为1，k读取用
    top++;
    Stack[top].i = 1; //初始位置(1,1)
    Stack[top].j = 1;
    Stack[top].di = -1;
    mg[1][1] = -1;
    while (top > -1)
    { //只要栈Stack不为空就继续走
        i = Stack[top].i;
        j = Stack[top].j;
        di = Stack[top].di;
        if (i == M && j == N) //找到了出口，输出路径
        {
            cout << count << ": ";
            count++;
            for (k = 0; k <= top; k++)
            {
                cout << "(" << Stack[k].i << "," << Stack[k].j << ")"
                     << " ";
            }
            cout << endl;         /*
            因为top从0开始，minlen=count从1开始，给top+1
            */
            if (top + 1 < minlen) //比较是否是最短路径
            {
                for (k = 0; k <= top; k++) //是最短，保存路径
                    Path[k] = Stack[k];
                minlen = top + 1;
            }
            mg[Stack[top].i][Stack[top].j] = 0; //让该位置变为其他路径的可走结点
            top--;                              //后退，找其他路径
            i = Stack[top].i;
            j = Stack[top].j;
            di = Stack[top].di;
        }
        find = 0; /*
        0     0123上右下左四个方向走，找可走点
      3   1
        2
        */
        while (di < 4 && find == 0)
        { //在4个方向内，寻找可走点
            di++;
            switch (di)
            {
            case 0:
                i = Stack[top].i - 1;
                j = Stack[top].j;
                break; //上面
            case 1:
                i = Stack[top].i;
                j = Stack[top].j + 1;
                break; //右边
            case 2:
                i = Stack[top].i + 1;
                j = Stack[top].j;
                break; //下面
            case 3:
                i = Stack[top].i;
                j = Stack[top].j - 1;
                break; //左边
            }
            if (mg[i][j] == 0) //找到可走点
                find = 1;
        }
        if (find == 1) //找到了下一个可走结点
        {
            Stack[top].di = di; //修改原栈顶元素的di值
            top++;              //下一个可走结点进栈
            Stack[top].i = i;
            Stack[top].j = j;
            Stack[top].di = -1;
            mg[i][j] = -1; //避免重复走到该结点
        }
        else //没找到
        {
            mg[Stack[top].i][Stack[top].j] = 0; //让该位置变为其他路径的可走结点
            top--;
        }
    }
    cout << "最短路径如下" << endl;
    cout << "长度:  " << minlen << endl;
    cout << "路径:  " << endl;
    for (k = 0; k < minlen; k++)
    {
        cout << "(" << Path[k].i << "," << Path[k].j << ")"
             << " ";
    }
}

int main()
{
    cout << "迷宫路径如下:" << endl;
    mgpath();
    return 0;
}
