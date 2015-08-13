#include <iostream>
#include <list>
using namespace std;

bool checkDivisibles(int triNum)
{
    list<int> divs;
    for(int x = 1; x < triNum; x++)
        if(triNum % x == 0)
            divs.push_front(x);
    return divs.size() >= 500;
}

int main()
{
    int currentTriNumber = 1;
    int count = 2;
    while(!checkDivisibles(currentTriNumber))
    {
        currentTriNumber += count;
        count++;
    }

    cout << currentTriNumber << endl;

}
