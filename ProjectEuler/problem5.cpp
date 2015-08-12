#include <iostream>
using namespace std;

bool checkCurrentNum(int num, int numlist[])
{
    for(int x = 0; x < 10; x++)
        if(num % numlist[x] != 0)
            return false;
    return true;
}

int main()
{
    int checklist[] = {20, 19, 18, 17, 16, 15, 14, 13, 12, 11};
    int check = 20;
    while(!checkCurrentNum(check, checklist))
    {
        check+=2;
    }
    cout << check << endl;
}
