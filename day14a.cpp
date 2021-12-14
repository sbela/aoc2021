#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <list>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

using namespace std;

// trim from start (in place)
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch)
                                    { return !std::isspace(ch); }));
} // ltrim

// trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
} // rtrim

// trim from both ends (in place)
inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
} // trim

template <typename T>
static const vector<T> tokenize(const string &text, char token)
{
    vector<T> data;
    stringstream check(text);
    string intermediate;
    while (getline(check, intermediate, token))
    {
        if (intermediate.length())
            data.push_back(intermediate);
    }
    return data;
}

int main()
{
    fstream input;
    input.open("input14s.txt", ios::in);
    if (input.is_open())
    {
        string line;
        string chain;
        map<string, string> inserts;
        while (getline(input, line))
        {
            if (chain.length() == 0)
            {
                chain = line;
                cout << "Start from: " << chain << endl;
            }
            else if (line.length())
            {
                auto item = tokenize<string>(line, '-');
                auto key = item.front();
                trim(key);
                auto value = item.back().substr(2);
                trim(value);
                if (inserts.find(key) == inserts.end())
                {
                    inserts[key] = value;
                    cout << "Added:" << key << " -> " << inserts[key] << endl;
                }
                else
                    cout << "Already added:" << key << ":" << value << " <- " << key << ":" << inserts[key] << endl;
            }
        }

        for (int step = 0; step < 10; ++step)
        {
            string new_chain;
            for (size_t pos = 0; pos < (chain.size() - 1); ++pos)
            {
                auto pair = chain.substr(pos, 2);
                if (inserts.find(pair) != inserts.end())
                {
                    pair.insert(1, inserts[pair]);
                    new_chain += pair.substr(0, 2);
                }
                else
                    cout << "Missing pair: " << pair << endl;
            }
            new_chain += chain[chain.size() - 1];
            chain = new_chain;
            cout << "After step " << step + 1 << ": " << chain.length() << endl;
        }
        map<char, size_t> histogram;
        for (auto item : chain)
            histogram[item]++;

        size_t smallest{SIZE_MAX}, biggest{0};
        for (const auto &[key, value] : histogram)
        {
            cout << key << ":" << value << endl;
            if (smallest > value)
                smallest = value;
            if (biggest < value)
                biggest = value;
        }
        cout << "Diff: " << biggest - smallest << endl;
    }
    else
        cout << "input14s.txt not open!\n";
    return 0;
}