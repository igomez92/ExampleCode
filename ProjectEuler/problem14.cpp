#include <iostream>
#include <list>
using namespace std;

list<unsigned int> calculateChain(unsigned int currNum)
{
    list<unsigned int> tempChain;
    while(currNum != 1)
    {
        tempChain.push_back(currNum);
        if(currNum % 2 == 0)
            currNum = currNum /2;
        else
            currNum = (3*currNum) + 1;
    }
    tempChain.push_back(1);
    return tempChain;
}


int main()
{
    list<unsigned int> maxChain, currChain;
    unsigned int currNum;
    maxChain = calculateChain(13);
    for(unsigned int x = 14; x < 1000000; x++)
    {
        currChain = calculateChain(x);
        if(currChain.size() > maxChain.size())
        {
            maxChain = currChain;
            currNum = x;
        }
    }
    cout << currNum << endl;
}
