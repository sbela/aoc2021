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
    input.open("input8.txt", ios::in);
    if (input.is_open())
    {
        string line;
        int output_value_counter[10] = { 0 };
        while (getline(input, line))
        {
            const auto data = tokenize<string>(line, '|');
            //cout << "Line: " << line << endl;
            if (data.size() > 1)
            {
                const auto unique_signal_patterns = tokenize<string>(data[0], ' ');
                const auto four_digit_output_value = tokenize<string>(data[1], ' ');
                for (const auto &output : four_digit_output_value)
                {
                    //cout << output << ":" << output.size() << endl;
                    output_value_counter[output.size()]++;
                }
            }
        }
        auto sum = output_value_counter[2];
        sum += output_value_counter[4];
        sum += output_value_counter[3];
        sum += output_value_counter[7];
        cout << "1,4,7,8 appears #" << sum << endl;
    }
    else
        cout << "input8.txt not open!\n";
    return 0;
}
