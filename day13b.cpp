#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <type_traits>
#include <limits.h>
#include <map>
#include <array>
#include <list>

using namespace std;

int main()
{
    fstream input;
    input.open("input12s.txt", ios::in);
    if (input.is_open())
    {
        string line;
        while (getline(input, line))
    }
    else
        cout << "input12s.txt not open!\n";
    return 0;
}
