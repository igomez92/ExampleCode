#include <iostream>
using namespace std;

int main()
{
    int total = 0;
    for(int x = 0; x < 1000;x++)
        if(x % 3 == 0 || x % 5 == 0 || x % 15 == 0)
            total += x;
    cout << total << endl;
    return 0;
}
