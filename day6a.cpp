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
#include <chrono>
#include <thread>
#include <functional>

using namespace std;

constexpr int TIMER_RESET_VALUE = 6;
constexpr int NEW_TIMER_VALUE = 8;

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    for (auto const &value : values)
        output << value << endl;
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

size_t how_many_fish(int start, int days)
{
    if (days >= start)
    {
        size_t fish = 1 + ((days - start) / 7);
        size_t fishes = 0;        
        for (size_t i = 0; i < fish; ++i)
        {
            fishes += how_many_fish(9, (days - start - (i * 7)));
        }
        cout << "FISH: s:" << start << " d:" << days << " f:" << fish << " sum:" << fish + fishes << endl;
        return fish + fishes;
    }
    cout << "FISH: s:" << start << " d:" << days << " f:0" << " sum:0" << endl;
    return 0;
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
    size_t back_table[N];
    memset(back_table, 0, sizeof(back_table));
    back_table[8] += table[0];
    back_table[6] += table[0];

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
            size_t table[N];
            for (int x : fishes)
                table[x]++;
            
            for (int i = 0; i < 256; ++i) 
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