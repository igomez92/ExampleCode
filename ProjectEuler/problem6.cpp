#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    unsigned int sum = 0, sqrsum = 0;
    for(int x = 1; x < 101; x++)
    {
        sum+=x;
        sqrsum += x * x;
    }
    cout << sum * sum << " " << sqrsum << endl;
    sum = sum * sum;
    cout << sum - sqrsum << endl;
    return 0;
}
