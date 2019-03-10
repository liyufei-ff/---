
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main()
{
    string str = "0|444|22|3333|aaa|bbb";
    istringstream ss(str);
    string token;
    while (getline(ss, token, '|'))
        cout << token << " " << endl;;
}