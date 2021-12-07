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

using namespace std;

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

constexpr int N = 9;

size_t CountFish(size_t *table)
{
    size_t result = 0;
    for (int i = 0; i < N; ++i)
        result += table[i];
    return result;
}

void NextDay(size_t *table)
{
    size_t back_table[N] = { 0 };
    back_table[8] = table[0];
    back_table[6] = table[0];

    for (int i = 1; i < N; ++i)
        back_table[i - 1] += table[i];
    memcpy(table, back_table, sizeof(back_table));
}

int main()
{
    fstream input;
    input.open("input6.txt", ios::in);
    if (input.is_open())
    {
        string line;
        if (getline(input, line))
        {
            vector<int> fishes = tokenize<int>(line, ',');
            size_t table[N] = { 0 };
            for (int x : fishes)
                table[x]++;
            
            for (int i = 0; i < 80; ++i) 
                NextDay(table);
            
            cout << "Fish #: " << CountFish(table) << endl;
        }
        else
            cout << "Invalid input" << endl;
    }
    else
        cout << "input6.txt not open!\n";
    return 0;
}