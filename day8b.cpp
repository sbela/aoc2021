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

using namespace std;

constexpr int a = 0;
constexpr int b = 1;
constexpr int c = 2;
constexpr int d = 3;
constexpr int e = 4;
constexpr int f = 5;
constexpr int g = 6;

const map<int, vector<int>> seven_segment_valid_outputs = {
    {0, {a, b, c, e, f, g}},
    {1, {c, f}},
    {2, {a, c, d, e, g}},
    {3, {a, c, d, f, g}},
    {4, {b, c, d, f}},
    {5, {a, b, d, f, g}},
    {6, {a, b, d, e, f, g}},
    {7, {a, c, f}},
    {8, {a, b, c, d, e, f, g}},
    {9, {a, b, c, d, f, g}}};

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

int decode(const vector<string> &output, const map<vector<char>, int> &digits)
{
    int sum { 0 }, pos { 3 };
    for (const auto &digit : output)
    {
        vector<char> v_digit = vector<char>(digit.begin(), digit.end());
        sort(v_digit.begin(), v_digit.end());
        if (digits.find(v_digit) != digits.end())
        {
            sum += digits.at(v_digit) * pow(10, pos);
        }
        else
        {
            cout << "Missing: " << v_digit << endl;
        }
        pos--;
    }
    return sum;
}

map<vector<char>, int> map_digits(const vector<string> &unique_signal_patterns, bool silent)
{
    map<vector<char>, int> digits;
    // serach for one
    vector<char> one;
    for (const auto &_one : unique_signal_patterns)
    {
        if (_one.length() == 2)
        {
            if (not silent) cout << "one: " << _one;
            one = vector<char>(_one.begin(), _one.end());
            break;
        }
    }
    sort(one.begin(), one.end());
    if (not silent) cout << " -> " << one << endl;
    digits[one] = 1;

    // search for seven
    vector<char> seven;
    for (const auto &_seven : unique_signal_patterns)
    {
        if (_seven.length() == 3)
        {
            if (not silent) cout << "seven: " << _seven;
            seven = vector<char>(_seven.begin(), _seven.end());
            break;
        }
    }
    sort(seven.begin(), seven.end());
    if (not silent) cout << " -> " << seven << endl;
    digits[seven] = 7;

    // search for four
    vector<char> four;
    for (const auto &_four : unique_signal_patterns)
    {
        if (_four.length() == 4)
        {
            if (not silent) cout << "four: " << _four;
            four = vector<char>(_four.begin(), _four.end());
            break;
        }
    }
    sort(four.begin(), four.end());
    if (not silent) cout << " -> " << four << endl;
    digits[four] = 4;

    // search for eight
    vector<char> eight;
    for (const auto &_eight : unique_signal_patterns)
    {
        if (_eight.length() == 7)
        {
            if (not silent) cout << "eight: " << _eight;
            eight = vector<char>(_eight.begin(), _eight.end());
            break;
        }
    }
    sort(eight.begin(), eight.end());
    if (not silent) cout << " -> " << eight << endl;
    digits[eight] = 8;

    auto search_digit = [](const vector<char> &haystack, const vector<char> &needle){
        size_t foundCount { 0 };
        for (auto item : needle) {
            for (auto set : haystack) {
                if (set == item) {
                    foundCount++;
                    break;
                }
            }
        }
        return foundCount == needle.size();
    };

    // search for three
    vector<char> three;
    for (const auto &_three : unique_signal_patterns)
    {
        if (_three.length() == 5)
        {
            vector<char> v_three = vector<char>(_three.begin(), _three.end());
            sort(v_three.begin(), v_three.end());
            if (search_digit(v_three, seven))
            {
                if (not silent) cout << "three: " << _three;
                three = v_three;
                break;
            }
        }
    }
    if (not silent) cout << " -> " << three << endl;
    digits[three] = 3;

    // search for nine
    vector<char> nine;
    for (const auto &_nine : unique_signal_patterns)
    {
        if (_nine.length() == 6)
        {
            vector<char> v_nine = vector<char>(_nine.begin(), _nine.end());
            sort(v_nine.begin(), v_nine.end());
            if (search_digit(v_nine, three))
            {
                if (not silent) cout << "nine: " << _nine;
                nine = v_nine;
                break;
            }
        }
    }
    if (not silent) cout << " -> " << nine << endl;
    digits[nine] = 9;

    // search for zero
    vector<char> zero;
    for (const auto &_zero : unique_signal_patterns)
    {
        if (_zero.length() == 6)
        {
            vector<char> v_zero = vector<char>(_zero.begin(), _zero.end());
            sort(v_zero.begin(), v_zero.end());
            if (search_digit(v_zero, seven))
            {
                if (v_zero != nine)
                {
                    if (not silent) cout << "zero: " << _zero;
                    zero = v_zero;
                    break;
                }
            }
        }
    }
    if (not silent) cout << " -> " << zero << endl;
    digits[zero] = 0;

    // search for six
    vector<char> six;
    for (const auto &_six : unique_signal_patterns)
    {
        if (_six.length() == 6)
        {
            vector<char> v_six = vector<char>(_six.begin(), _six.end());
            sort(v_six.begin(), v_six.end());
            if ((v_six != zero) and (v_six != nine))
            {
                if (not silent) cout << "six: " << _six;
                six = v_six;
                break;
            }
        }
    }
    if (not silent) cout << " -> " << six << endl;
    digits[six] = 6;

    // search for five
    vector<char> five;
    for (const auto &_five : unique_signal_patterns)
    {
        if (_five.length() == 5)
        {
            vector<char> v_five = vector<char>(_five.begin(), _five.end());
            sort(v_five.begin(), v_five.end());
            if (search_digit(six, v_five))
            {
                if (not silent) cout << "five: " << _five;
                five = v_five;
                break;
            }
        }
    }
    if (not silent) cout << " -> " << five << endl;
    digits[five] = 5;

    // search for two
    vector<char> two;
    for (const auto &_two : unique_signal_patterns)
    {
        if (_two.length() == 5)
        {
            vector<char> v_two = vector<char>(_two.begin(), _two.end());
            sort(v_two.begin(), v_two.end());
            if ((v_two != three) and (v_two != five))
            {
                if (not silent) cout << "two: " << _two;
                two = v_two;
                break;
            }
        }
    }
    if (not silent) cout << " -> " << two << endl;
    digits[two] = 2;
    return digits;
}

int main()
{
    fstream input;
    input.open("input8.txt", ios::in);
    if (input.is_open())
    {
        string line;
        int output_value_sum{0};
        while (getline(input, line))
        {
            const auto data = tokenize<string>(line, '|');
            if (data.size() > 1)
            {
                cout << "Line: " << data[0] << endl;
                const auto unique_signal_patterns = tokenize<string>(data[0], ' ');

                auto digits = map_digits(unique_signal_patterns, true);
                const auto four_digit_output_value = tokenize<string>(data[1], ' ');
                cout << four_digit_output_value << "= " << decode(four_digit_output_value, digits) << endl;
                output_value_sum += decode(four_digit_output_value, digits);
            }
            else
                cout << "Invalid Line: " << line << endl;
        }
        cout << "SUM# " << output_value_sum << endl;
    }
    else
        cout << "input8.txt not open!\n";
    return 0;
}
