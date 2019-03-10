#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <functional>
#include <map>
#include <unordered_map>
#include <memory>
#include <string.h>
#include <stdlib.h>
#include "BitStream.cpp"
#include "BitStream.h"
using namespace std;
//哈夫曼树使用静态三叉链表来进行
#define N 100
#define M 2 * N - 1
long long int sumadd = 0;//防止超出int类型的范围
long long int numadd = 0;
string filename;
long long int p_test = 0;
long long int s_flag = 0;
int p_flag = 0;
typedef struct
{
    int weight;
    int parent, lchild, rchild;
    int size;
} HTNode, HuffmanTree[M + 1];
typedef char *HuffmanTreeCode[N + 1];

void select(HuffmanTree ht, int n, int *s1, int *s2)
{
    int min1=0,min2=0;
    for (int i = 1; i <=n; i++)
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
void crtHuffmanTree(HuffmanTree ht, vector<pair<string,int>>&v1, int n)
{
    int m = 2 * n - 1;
    int i;
    int s1 = 0, s2 = 0;
    for (i = 1; i <= n; i++)
    {
        ht[i] = {v1[i-1].second, 0, 0, 0};
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
void crtHuffmanTreeCode(HuffmanTree ht, HuffmanTreeCode hc, long long int n)
{
    char *cd;
    long long int c=0 ,p=0;
    long long int i = 0;
    long long int start = 0;
    cd = (char *)malloc(sizeof(char));    //临时编码数组
    cd[n - 1] = '\0'; //从后往前求解
    for(i=1;i<=n;i++)
    {
        start = n -1;
        c = i;
        p = ht[i].parent;
        while(p!=0)
        {
            --start;
            if(ht[p].lchild == c)
                cd[start] ='0';
            else
                cd[start] ='1';
            c=p;
            p=ht[p].parent;
        }
        hc[i] = (char *)malloc((257) * sizeof(char));
        strcpy(hc[i],&cd[start]);

    }
    free(cd);
}
void writeCode(string &number_test);
void IncreatHuffmancode(HuffmanTreeCode hc,vector<pair<string,int>>&v1)
{
    int i=0,g=0,w=0;
    ifstream input(filename);
    unsigned char buffer;
    string number_test;
    string s,test1,test,g_test;
    input >>noskipws;
    while(!input.eof())
    {
        input >> buffer;
        if (buffer >= 128)
        {
            g_test = buffer;
            s += g_test;
            w++;
            continue;
        }
        if (w != 0 && buffer < 128)
        {
            for (int p = 1; p <= sumadd; p++)
            {
                if (s == v1[p - 1].first)
                {
                    test = hc[p];
                    number_test += test;
                }
            }
            w = 0;
            s = "";
            test = "";
        }
        if (buffer < 128 && w == 0)
        {    s = buffer;
            for (int p = 1; p <= sumadd; p++)
            {
                if (s == v1[p - 1].first)
                {
                    test = hc[p];
                    number_test += test;
                }
            }
            w = 0;
            s = "";
            test = "";
        }
    }
    input.close(); 
    writeCode(number_test);
}
void writeCode(string & number_test)
{
    string name = filename +".process";
    char *filename_test = (char *)name.c_str();
    FILE *outfile = fopen(filename_test, "awb+");
    fseek(outfile,s_flag,SEEK_SET);
    fpos_t startPos; //记录初始写的位置
    fgetpos(outfile, &startPos);
    long bits = 0;  //记录写入压缩文件中的比特数
    int buffer = 0; //缓存字节
    vector<int> codes;
    // cout << "222222222" << endl;
    for (int l = 0; l < number_test.size(); l++)
    {
        buffer <<= 1;
        if (number_test[l] == '1')
            buffer += 1;
        bits++;
        if (bits % 8 == 0) //满8位存入
        {
            codes.push_back(buffer);
            buffer = 0;
        }
    }
    // cout << "3333333333" << endl;
    //刚好没有剩余的bit
    if (bits % 8 == 0)
    {
        //存入8表示最后一个字节8位都是有用的编码
        fputc(8, outfile);
        p_flag++ ;
        int lastCodeBitsCount = bits % 8;
        fputc(lastCodeBitsCount,outfile);
        //写入编码后的数据
        for (int i = 0; i < codes.size(); i++)
        {
            fputc(codes.at(i),outfile);
        }
        return;
    }
    int lastCodeBitsCount = bits % 8;
    fputc(lastCodeBitsCount, outfile);
    //写入编码后的数据
    // cout << "444444444" << endl;
    for (int i = 0; i < codes.size(); i++)
    {
        fputc(codes.at(i), outfile);
    }
    fputc(buffer, outfile);
    fclose(outfile);
}
void decode(HuffmanTree ht, vector<pair<string, int>> &v1, int add);
void unprocess()
{
    ifstream if_s(filename+".process",ios::binary | ios::in);
    char ch;
    string s,test,test_s,str;
    int value=0 , add=0,keep=0;
    int i = 0,flag = 0 ;
    vector<pair<string, int>> v1;
    if_s >> noskipws;
    while(!if_s.eof())
    {
        getline(if_s,test_s);
        flag = test_s.find('+',0);
        if(flag > 0 )
        {
                test += test_s.substr(0, flag);
                break;
        }
        else
            test += test_s;
    }
    if_s.close();
    stringstream out(test);
    while(getline(out,s,'$'))
    {
        keep++;
        if(keep % 2 != 0)
        {
            str = s;
        }
        else
        {
            value = atoi(s.c_str());
        }
        if( str != "" && value != 0)
        {
            v1.push_back(make_pair(str,value));
            str ="";
            value = 0;
            add++;
        }
    }
    for(vector<pair<string,int>>::iterator it = v1.begin();it!=v1.end();it++)
    {
            if(it->first == "/")
                it->first ="\n";
    }
    // for (vector<pair<string, int>>::iterator it = v1.begin(); it != v1.end(); it++)
    // {
    //     cout << it->first << "         " << it->second << "      " << endl;
    // }
    HuffmanTree ht;
    HuffmanTreeCode hc;
    crtHuffmanTree(ht,v1,add);
    crtHuffmanTreeCode(ht,hc,add);
    decode(ht,v1,add);
}
void decode(HuffmanTree ht, vector<pair<string, int>>&v1,int add)
{
    HTNode temp;
    string test,name;
    temp = ht[add*2-1];
    for (int j = 1; j <= add; j++)
    {
        ht[j].size = j;
    }
    char * s_test;
    name = filename +".process";
    s_test = (char *)name.c_str();
    FILE *fin = fopen(s_test,"rb");
    fseek(fin,s_flag + 1 + p_flag,SEEK_SET);
    // cout << "1111111111   " << endl;
    int lastCodeBitsCount = fgetc(fin);
    BitStream stream(fin,lastCodeBitsCount);
    bool bit;
    // cout << "1111111111   "  << endl;
    while(stream.getBit(bit))
    {
         if (bit == 0)
        {
            temp = ht[temp.lchild];
        }
        else
        {
            temp = ht[temp.rchild];
        }
        if (temp.rchild == 0 && temp.lchild == 0)
        {
            test+=v1[temp.size-1].first;
            temp = ht[add*2-1];
        }
    }
    fclose(fin);
    int sum_a = test.length();
    test[sum_a -1] = '\0';
    int k= 0;
    int p = test.rfind('\n',test.length());
    for( k = test.length(); k >p ; k--)
    {
        test[k] = '\0';
    }
    char * k_test;
    k_test = (char *) test.c_str();
    ofstream outfile(filename+".unprocess");
    outfile << noskipws;
    outfile << k_test;
    outfile.close();
}
int main()
{
    cin >> filename;
    ifstream input(filename, ifstream::in);
    unordered_map<string, int> num;
    HuffmanTree ht;
    HuffmanTreeCode hc;
    string s,g_test;
    unsigned char buffer;
    int i = 0,w = 0;
    input >> noskipws;//不忽视空白符号，也不忽视换行
    while (!input.eof())
    {
        input >> buffer;
        if(buffer >= 128)
        {
                g_test = buffer;
                s += g_test;
                w++;
                continue;
        }
        if(w != 0 && buffer < 128 )
        {
            if (num.find(s) == num.end()) //没有在map之中
            {
                num[s] = 1;
                sumadd++;
            }
            else
                num[s]++;
            w = 0;
            s = "";
        }
        if(buffer < 128 && w == 0)
        {   
             s = buffer;
            if (num.find(s) == num.end()) //没有在map之中
            {
                num[s] = 1;
                sumadd++;
            }
            else
                num[s]++;
            w = 0;
            s = "";
        }

    }
    input.close();
    //排序

    vector<pair<string, int>> v1;
    for (unordered_map<string, int>::iterator it = num.begin(); it != num.end(); it++)
    {
        v1.push_back(make_pair(it->first, it->second));
    }

    for (vector<pair<string, int>>::iterator it = v1.begin(); it != v1.end(); it++)
    {
        cout << it->first << "         " << it->second << "      " << endl;
    }
    // cout << "sssssssss" << endl;
    //出现的个数写在配置文件之中
    //出现的权值数组写入到文件之中
    num.clear();
    ofstream output(filename+".process",ios::app|ios::binary);
    for (vector<pair<string,int>>::iterator it1 = v1.begin();it1!=v1.end();it1++)
    {
        if(it1->first != "\n")
            output << it1->first;
        else
            output << "/";
        output << "$"; 
        output << it1->second ;
        output <<"$";
    }
    output << "+";
    output.close();
    ifstream if_s(filename + ".process", ios::binary | ios::in);
    string test_s,name;
    while (!if_s.eof())
    {
        getline(if_s, test_s);
        name += test_s;
        s_flag = name.find('+', 0);
        if (s_flag > 0)
        {
            break;
        }
    }
    if_s.close();
    crtHuffmanTree(ht,v1, sumadd);
    crtHuffmanTreeCode(ht, hc, sumadd);
    IncreatHuffmancode(hc,v1);
    for(i = 0;i<sumadd;i++)
        free(hc[i]);
    v1.clear();
    unprocess();
    return 0;
}