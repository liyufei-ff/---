#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <memory>
#include <string.h>
#include <stdlib.h>
using namespace std;
//哈夫曼树使用静态三叉链表来进行
#define N 1000
#define M 2 * N - 1
int sumadd = 0;
int numadd = 0;

typedef struct
{
    int weight;
    int parent, lchild, rchild;
    int size;
} HTNode, HuffmanTree[M + 1];
typedef char *HuffmanTreeCode[N + 1];
void select(HuffmanTree ht, int n, int *s1, int *s2)
{
    int min1 = 0, min2 = 0;
    for (int i = 1; i <= n; i++)
    {
        if (ht[i].parent == 0)
        {
            if (ht[min1].weight > ht[i].weight)
                min1 = i;
            if (ht[min1].weight < ht[min2].weight)
            {
                swap(min1, min2);
            }
        }
        *s2 = min1;
        *s1 = min2;
    }
}
void crtHuffmanTree(HuffmanTree ht, vector<pair<string, int>> &v1, int n)
{
    int m = 2 * n - 1;
    int i;
    int s1 = 0, s2 = 0;
    for (i = 1; i <= n; i++)
    {
        ht[i] = {v1[i - 1].second, 0, 0, 0};
    }
    for (i = n + 1; i <= m; i++)
        ht[i] = {0, 0, 0, 0};
    ht[0].weight = 99999;
    for (i = n + 1; i <= m; i++)
    {
        select(ht, i - 1, &s1, &s2);
        ht[i].weight = ht[s1].weight + ht[s2].weight;
        ht[i].lchild = s1;
        ht[i].rchild = s2;
        ht[s1].parent = i;
        ht[s2].parent = i;
        s1 = 0;
        s2 = 0;
    }
}
void crtHuffmanTreeCode(HuffmanTree ht, HuffmanTreeCode hc, int n)
{
    char *cd;
    int start, c, i, p;
    cd = (char *)malloc(sizeof(char)); //临时编码数组
    cd[n - 1] = '\0';                  //从后往前求解
    for (i = 1; i <= n; i++)
    {
        start = n - 1;
        c = i;
        p = ht[i].parent;
        while (p != 0)
        {
            --start;
            if (ht[p].lchild == c)
                cd[start] = '0';
            else
                cd[start] = '1';
            c = p;
            p = ht[p].parent;
        }
        hc[i] = (char *)malloc((n - start) * sizeof(char));
        strcpy(hc[i], &cd[start]);
    }
    free(cd);
}

void IncreatHuffmancode(HuffmanTreeCode hc, vector<pair<string, int>> &v1)
{
    int i = 0, g = 0;
    for (i = 1; i <= sumadd; i++)
        cout << v1[i - 1].first << " : " << hc[i] << endl;
    cout << "编码长度" << endl;
    ifstream input("test1.souce", ifstream::in);
    unsigned char buffer;
    string number_test;
    string test, test1,g_test;
    input >> noskipws;
    string s;
    while (!input.eof())
    {
        input >> buffer;
        if (buffer >= 128)
        {
            g_test = buffer;
            s += g_test;
            g++;
            continue;
        }
        if (g != 0 && buffer < 128)
        {
            for (int p = 1; p <= sumadd; p++)
            {
                if (s == v1[p - 1].first)
                {
                    test = hc[p];
                    number_test += test;
                }
            }
            g = 0;
            s = "";
            test = "";
        }
        if (buffer < 128 && g == 0)
        {
            s = buffer;
            for (int p = 1; p <= sumadd; p++)
            {
                if (s == v1[p - 1].first)
                {
                    test = hc[p];
                    number_test += test;
                }
            }
            g = 0;
            s = "";
            test = "";
        }
    }
    input.close();
    ofstream outfile("test.code");
    outfile << number_test;
    outfile.close();
}
void decode(HuffmanTree ht, vector<pair<string, int>> &v1)
{
    ifstream input("test.code", ifstream::in);
    unsigned char number;
    string test;
    HTNode temp;
    temp = ht[sumadd * 2 - 1];
    for (int j = 1; j <= sumadd; j++)
    {
        ht[j].size = j;
    }
    input >> noskipws;
    while (!input.eof())
    {
        input >> number;
        if (number == '0')
        {
            temp = ht[temp.lchild];
        }
        if (number == '1')
        {
            temp = ht[temp.rchild];
        }
        if (temp.rchild == 0 && temp.lchild == 0)
        {
            test += v1[temp.size - 1].first;
            temp = ht[sumadd * 2 - 1];
        }
    }
    int sum_a = test.length();
    test[sum_a - 1] = '\0';
    input.close();
    int k = 0;
    int p = test.rfind('\n', test.length());
    for (k = test.length(); k >p; k--)
    {
        test[k] = '\0';
    }
    char *k_test;
    k_test = (char *)test.c_str();
    ofstream outfile("test.decode");
    outfile << noskipws;
    outfile << k_test;
    outfile.close();
}
int main()
{
    ifstream input("test1.souce", ifstream::in);
    unordered_map<string, int> num;
    HuffmanTree ht;
    HuffmanTreeCode hc;
    string s;
    string test,g_test;
    unsigned char buffer;
    int i = 0;
    input >> noskipws; //不忽视空白符号，也不忽视换行
    while (!input.eof())
    {
        input >> buffer;
        if (buffer >= 128)
        {
            g_test = buffer;
            s += g_test;
            i++;
            continue;
        }
        if (i != 0 && buffer < 128)
        {
            if (num.find(s) == num.end()) //没有在map之中
            {
                num[s] = 1;
                sumadd++;
            }
            else
                num[s]++;
            i = 0;
            s = "";
        }
        if (buffer < 128 && i == 0)
        {
            s = buffer;
            if (num.find(s) == num.end()) //没有在map之中
            {
                num[s] = 1;
                sumadd++;
            }
            else
                num[s]++;
            i = 0;
            s = "";
        }
    }
    // //排序
    vector<pair<string, int>> v1;
    for (unordered_map<string, int>::iterator it = num.begin(); it != num.end(); it++)
    {
        v1.push_back(make_pair(it->first, it->second));
        // cout << it->first << endl;
    }
    for (vector<pair<string, int>>::iterator it = v1.begin(); it != v1.end(); it++)
    {
        cout << it->first << "         " << it->second << "      " << endl;
    }
    num.clear();
    crtHuffmanTree(ht, v1, sumadd);
    crtHuffmanTreeCode(ht, hc, sumadd);
    IncreatHuffmancode(hc, v1);
    decode(ht, v1);
    // v1.clear();
    input.close();
    return 0;
}
