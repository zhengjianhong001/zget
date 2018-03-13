#include <iostream>
using namespace std;


int main(int argc, char const *argv[])
{
    string path = "http://www.baidu.com/index.php";
    int pos = path.find_last_of('/');
    string s(path.substr(pos) );
    char* m = strdup(s.c_str());
    cout << m << endl;

    char* j = "test";
    string k(j);
    cout << k << endl;
    return 0;
}