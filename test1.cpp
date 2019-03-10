#include <fstream>
#include <string>
#include <iostream>
using namespace std;
int main()
{
    ifstream ifs("test.p"); // 改成你要打开的文件
    streambuf *old_buffer = cin.rdbuf(ifs.rdbuf());
    string read;
    while (cin >> read) // 逐词读取方法一
    {
            cout << read << endl;
    }
    cin.rdbuf(old_buffer); // 修复buffer
}