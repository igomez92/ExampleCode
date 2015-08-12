#include<cstdlib>
#include<string>
#include<iostream>
#include<sstream>

using namespace std;

int main() {

	unsigned int max = 0;
    unsigned int first =0,second = 0;
	for(unsigned int i = 999; i != 99; --i)
    {
        first = i;
		for(unsigned int j = 999; j != 99; --j) {
            second = j;
			int prod = i*j;
			stringstream ss;
			ss << prod;
			string string = ss.str();
			int len = string.length();
			bool check = true;
			for(unsigned int k = 0; k < len/2.0; ++k)
				if(string[k] != string[(len-k)-1]) {

					check = false;
					break;

					}

				if(max <= prod && check)
					max = prod;

			}
    }
    cout << first << " " << second << endl;
	cout << max << endl;

	return 0;

	}
