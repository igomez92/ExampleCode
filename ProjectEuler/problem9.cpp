#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int a, b, c;
    a = 3;
    b = 4;
    c = 5;
    while (a + b + c < 1000)
    {
        while(b < c)
        {
            a = 1;
            while(a < b)
            {
                if(a + b + c == 1000 && (a*a + b*b == c*c))
                    break;
                a++;
                c = sqrt((a*a + b*b));
            }
            if(a + b + c == 1000 && (a*a + b*b == c*c))
                    break;
            b++;
            c = sqrt((a*a + b*b));
        }

    }
    cout << a << " " << b << " " << c << endl;
    cout << a + b + c << endl;
    cout << a * b * c << endl;
}
