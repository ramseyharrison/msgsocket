#include <iostream>
#include <regex>
#include <string> 

using namespace std;

int main()
{

    regex reg("chat [\\w]+ [\\w " "]+");
    if (regex_search("chat potato hi", reg))
        cout << "matched" << endl;
    else
        cout << "not matched" << endl;

    return 0;
}
