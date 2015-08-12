#include <iostream>
using namespace std;

int main()
{
    int even_total = 0 ,first = 1, second = 2, temp = 0;
    while(second < 4000000)
    {
        if(second % 2 == 0)
            even_total += second;
        temp = second;
        second += first;
        first = temp;
    }
    cout << even_total << endl;
    return 0;
}
