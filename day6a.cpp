#include <iostream>
#include <string>
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

void calculate(vector<char> &vec, size_t from, size_t to, vector<char> &add)
{
    for (size_t fish = from; fish < to; ++fish)
    {
        vec[fish]--;
        if (vec[fish] == -1)
        {
            add.push_back(NEW_TIMER_VALUE);
            vec[fish] = TIMER_RESET_VALUE;
        }
    }
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
            vector<char> fishes = tokenize<char>(line, ',');
            for (int i = 0; i <= 256; ++i)
            {
                if (i == 0)
                    cout << "  Initial state: " << line << endl;
                else
                {
                    auto start = chrono::steady_clock::now();
                    vector<char> add1;
                    vector<char> add2;
                    vector<char> add3;
                    size_t size = fishes.size();
                    size_t batch = size / 4;
                    thread t1(calculate, ref(fishes), 0, batch, ref(add1));
                    thread t2(calculate, ref(fishes), batch, 2 * batch, ref(add2));
                    thread t3(calculate, ref(fishes), 2 * batch, size, ref(add3));
                    t1.join();
                    t2.join();
                    t3.join();
                    fishes.insert(fishes.end(), add1.begin(), add1.end());
                    fishes.insert(fishes.end(), add2.begin(), add2.end());
                    fishes.insert(fishes.end(), add3.begin(), add3.end());
                    auto end = chrono::steady_clock::now();
                    chrono::duration<double> elapsed_seconds = end - start;
                    cout << "After " << setw(2) << i << " day(s): " << fishes.size() << " time:" << elapsed_seconds.count() << endl;
                }
            }
            cout << "Fish #: " << fishes.size() << endl;
        }
        else
            cout << "Invalid input" << endl;
    }
    else
        cout << "input6.txt not open!\n";
    return 0;
}