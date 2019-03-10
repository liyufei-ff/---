
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
#include <stack>
using namespace std;
int maze[10][10];
int vis[10][10], dist[10][10];
int dr[] = {-1, 1, 0, 0}; //上,下,左,右
int dc[] = {0, 0, -1, 1};
struct Node
{
    int r, c; //也可以在Node中加一个int pre属性,然后做一个全局的nodes,就不用pre[][]数组了.
    Node(int r, int c) : r(r), c(c) {}
    Node() {}
} pre[10][10];
queue<Node> Q;
void BFS()
{
    while (!Q.empty())
        Q.pop();
    memset(vis, 0, sizeof(vis));
    dist[0][0] = 0;
    vis[0][0] = 1;
    Q.push(Node(0, 0));
    while (!Q.empty())
    {
        Node node = Q.front();
        Q.pop();
        int r = node.r, c = node.c;
        for (int d = 0; d < 4; d++)
        {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < 5 && nc >= 0 && nc < 5 && vis[nr][nc] == 0 && maze[nr][nc] == 0)
            {
                vis[nr][nc] = 1;
                Q.push(Node(nr, nc));
                dist[nr][nc] = 1 + dist[r][c];
                pre[nr][nc] = Node(r, c);
                if (nr == 4 && nc == 4)
                    return;
            }
        }
    }
}
int main()
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            scanf("%d", &maze[i][j]);
    BFS();
    stack<Node> S;
    int cur_r = 4, cur_c = 4;
    while (true)
    {
        S.push(Node(cur_r, cur_c));
        if (cur_r == 0 && cur_c == 0)
            break;
        int r = cur_r, c = cur_c;
        cur_r = pre[r][c].r;
        cur_c = pre[r][c].c;
    }
    while (!S.empty())
    {
        Node node = S.top();
        S.pop();
        printf("(%d, %d)\n", node.r, node.c);
    }
    return 0;
}