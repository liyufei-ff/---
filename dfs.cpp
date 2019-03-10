#include <iostream>
#include <queue>
#include  <stdlib.h>
#include <vector>
#include <time.h>
#include <list>
#include "g.cpp"
#include <fstream>
using namespace std;
int a[20][20] ;
int b[20][20] ;
int dis[20][20];
int minlen = 225;
int min_test = 9999999;
int step = 0;
int **Maze_test = (int **)malloc(L * sizeof(int *));
struct node
{
    int x,y;
    int step;
};
void bfs(node &st)
{
    int next[4][2] = {{0,1},{-1,0},{0,-1},{1,0}};
    int tx=0,ty = 0;
    queue <node > s;
    node test;
    s.push(st);
    node pre[50][50];
    int flag = 0;
    while(!s.empty())
    {
            test = s.front();
            s.pop();
             for (int k = 0; k < 4; k++)
            {
                tx = test.x + next[k][0];
                ty = test.y + next[k][1];
                if (tx < 0 || tx > 14 || ty > 14 || ty < 0)
                    continue;
                if (a[tx][ty] == 0 && b[tx][ty] == 0)
                {
                    st.x = tx;
                    st.y = ty;
                    st.step++;
                    dis[tx][ty] = 1 + dis[tx][ty];
                    pre[tx][ty] =test;
                    b[tx][ty] = 1;
                    s.push(st);
                    if(tx == 4 && ty == 4)
                        break;
                }
            }
            if (tx == 14 && ty == 14)
                break;
    }
    int cur_r = 14,cur_c = 14;
    node l_test;
    l_test.x = 14;
    l_test.y = 14;
    list <node> st1;
    while(1)
    {
        st1.push_front(l_test);
        if(cur_r == 0 && cur_c == 0)
            break;
        int r =cur_r,c=cur_c;
        l_test.x = cur_r =pre[r][c].x;
        l_test.y = cur_c = pre[r][c].y;
    }
    ofstream outfile1("迷宫.out", ios::in | ios::app);
    if(a[14][14] == 0)
    {
        printf("\n");
        cout << "迷宫的最短路径的解为" << endl;
        outfile1 << "迷宫的最短路径的解为"<<endl;
        for (list<node>::iterator it = st1.begin(); it != st1.end(); it++)
        {
            outfile1 << "(" <<it->x << "," << it->y << ")" << endl;
            cout << "(" << it->x << "," << it->y << ")" << endl;
        }
        cout << "结束..............." << endl;
        outfile1<< "结束............" << endl;
    }
    else
    {
        cout << "此迷宫无法走通" << endl;
        outfile1 << "此迷宫无法走通" << endl;
    }
    outfile1.close();
}

int main()
{
   main_test();
   ofstream outfile("迷宫.in",ios::in|ios::app);
   for (int p = 2; p < L-2; p++)
   {
       for (int k = 2; k < L-2; k++)
       {
           if (Maze[p][k] == ROUTE)
           {
               a[p-2][k-2] = 0;
               outfile << " ";
           }
           else
           {
               a[p-2][k-2] = 1;
               outfile << '#';
           }
       }
       outfile << '\n';
   }
   outfile << "迷宫......................" << endl;
   outfile.close();
   int i,j;
   for (i = 0; i < 15; i++)
   {
           for (j = 0; j < 15; j++)
            {
                   cout << a[i][j]  << " ";
            }
            cout << endl;
   }
   b [0][0] = 1;

   node st;
   st.x = st.y = 0;
   st.step = 1;
   bfs(st);
   return 0;
}