#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <type_traits>
#include <limits.h>

using namespace std;

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    for (auto const &value : values)
        output << value << " ";
    return output;
}

template <typename T>
static const vector<T> tokenize(const string &text, char token)
{
    vector<T> data;
    stringstream check(text);
    string intermediate;
    while (getline(check, intermediate, token))
    {
        if (intermediate.length())
        {
            if constexpr (is_integral<T>::value)
                data.push_back(stoi(intermediate));
            else
                data.push_back(intermediate);
        }
    }
    return data;
}

int main()
{
    fstream input;
    input.open("input7.txt", ios::in);
    if (input.is_open())
    {
        string line;
        if (getline(input, line))
        {
            vector<int> positions = tokenize<int>(line, ',');
            if (positions.size())
            {
                cout << "POSITIONS:" << positions << endl;
                int min_pos = *min_element(positions.begin(), positions.end());
                int max_pos = *max_element(positions.begin(), positions.end());
                cout << "MIN POS:" << min_pos << "\nMAX POS:" << max_pos << endl;
                size_t min_delta = UINT_MAX;
                int min_position = -1;
                for (int pos = min_pos; pos <= max_pos; ++pos)
                {
                    size_t sum_delta = 0;
                    for (int p : positions)
                    {
                        //cout << "Move from " << p << " to " << pos << ": " << abs(p - pos) << "fuel" << endl;
                        sum_delta += (abs(p - pos) * (abs(p - pos) + 1)) / 2;
                    }
                    //cout << "Sum of " << pos << ": " << sum_delta << endl;
                    if (sum_delta < min_delta)
                    {
                        min_delta = sum_delta;
                        min_position = pos;
                    }
                }
                cout << "MIN DELTA @" << min_position <<  " :" << min_delta << endl;
            }
        }
    }
    else
        cout << "input7.txt not open!\n";
    return 0;
}
