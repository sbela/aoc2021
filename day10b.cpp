#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <vector>
#include <deque>
#include <ostream>
#include <sstream>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <type_traits>
#include <limits.h>
#include <map>
#include <array>
#include <tuple>

using namespace std;
static vector<char> valid_opens { '(', '[', '{', '<' };
static vector<char> valid_closes { ')', ']', '}', '>' };
static map<char, char> valid_tokens { { '(', ')' }, { '[', ']' }, { '{', '}' }, { '<', '>' } };
static map<char, int> token_values { { ')', 1 }, { ']', 2 }, { '}', 3 }, { '>', 4 } };

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    if constexpr (is_integral<T>::value)
    {
        for (auto const &value : values)
            output << value;
    }
    else
    {
        for (auto const &value : values)
            output << get<0>(value) << "," << get<1>(value) << " ";
    }
    return output;
}

template <typename T>
ostream &operator<<(ostream &output, deque<T> const &values)
{
    for (auto const &value : values)
        output << value;
    return output;
}

template <typename K, typename V>
ostream &operator<<(ostream &output, map<K, V> const &values)
{
    for (auto const &value : values)
        output << value.first << ":" << value.second << " ";
    return output;
}

char last_corrupted_token(const string &line, vector<char> &last_valid_open)
{    
    for (char c : line)
    {
        if (last_valid_open.size() == 0)
        {
            if (find(valid_opens.begin(), valid_opens.end(), c) == valid_opens.end())
            {
                if (find(valid_closes.begin(), valid_closes.end(), c) == valid_closes.end())
                {
                    cout << "Invalid token: " << c << endl;
                    return '@';
                }
                return c;
            }
            last_valid_open.push_back(c);
        }
        else
        {
            if (find(valid_closes.begin(), valid_closes.end(), c) != valid_closes.end())
            {
                if (valid_tokens[last_valid_open.back()] == c)
                    last_valid_open.pop_back();
                else
                    return c;
            }
            else
            {
                if (find(valid_opens.begin(), valid_opens.end(), c) == valid_opens.end())
                {
                    cout << "Invalid token: " << c << endl;
                    return '@';
                }
                last_valid_open.push_back(c);
            }
        }
    }
    return '@';
}

deque<char> completion_tokens(vector<char> &last_valid_open)
{
    deque<char> closing_tokens;
    for (char c : last_valid_open)
        closing_tokens.push_front(valid_tokens[c]);
    return closing_tokens;
}

size_t calculate_score(const deque<char> &tokens)
{
    size_t sum { 0 };
    for (auto c : tokens)
        sum = (sum * 5) + token_values[c];
    cout << "Calculating score for: " << tokens << " = " << sum << endl;
    return sum;
}

int main()
{
    fstream input;
    input.open("input10.txt", ios::in);
    if (input.is_open())
    {
        string line;
        map<char, int> corrupt_tokens;
        vector<size_t> completion_scores;
        while (getline(input, line))
        {
            vector<char> last_valid_open;
            auto c = last_corrupted_token(line, last_valid_open);
            corrupt_tokens[c]++;
            if (c == '@')
                completion_scores.push_back(calculate_score(completion_tokens(last_valid_open)));
        }
        
        cout << "Corrupt tokens: " << corrupt_tokens << endl;

        int sum { 0 };
        for (auto &token : corrupt_tokens)
            sum += token.second * token_values[token.first];

        cout << "Corrupt tokens sum: " << sum << endl;

        sort(completion_scores.begin(), completion_scores.end());
        auto middle = completion_scores.size() % 2 ? completion_scores[completion_scores.size() / 2] : 0;

        cout << "Middle completion score: " << middle << endl ;
    }
    else
        cout << "input10s.txt not open!\n";
    return 0;
}
