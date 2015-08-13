#include <iostream>
#include <list>
using namespace std;

bool checkPrime(list<int> primeList, int check)
{
    int currPrime;
    while(!primeList.empty())
    {
        currPrime = primeList.front();
        if(check % currPrime == 0)
            return false;
        primeList.pop_front();
    }

    return true;
}

int main()
{
    /*
        I need to find a much faster way of calculating
        prime numbers.
    */
    list<int> primeNumbers;
    primeNumbers.push_back(2);
    int total = 2;
    for(int x = 3; x < 2000000; x+=2)
    {
        if(checkPrime(primeNumbers, x))
        {
            primeNumbers.push_back(x);
            total += x;
        }

    }
    cout << total << endl;

}
