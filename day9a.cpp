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

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    for (auto const &value : values)
        output << value;
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

bool is_lowest(int row, int col, const vector<vector<int>> &bottom_map)
{
    int at = bottom_map[row][col];
    int smaller_count { 0 };
    int allowed_position { 0 };
    if ((row - 1) > -1) // fent
    {
        allowed_position++;
        if (bottom_map[row - 1][col] > at)
            smaller_count++;
    }
    if ((col + 1) < static_cast<int>(bottom_map[row].size())) // jobbra
    {
        allowed_position++;
        if (bottom_map[row][col + 1] > at)
            smaller_count++;
    }
    if ((row + 1) < static_cast<int>(bottom_map.size())) // lent
    {
        allowed_position++;
        if (bottom_map[row + 1][col] > at)
            smaller_count++;
    }
    if ((col - 1) > -1) // balra
    {
        allowed_position++;
        if (bottom_map[row][col - 1] > at)
            smaller_count++;
    }
    return smaller_count == allowed_position;
}

int main()
{
    fstream input;
    input.open("input9.txt", ios::in);
    if (input.is_open())
    {
        string line;
        vector<vector<int>> bottom_map;
        vector<int> lowest_points;
        while (getline(input, line))
        {
            vector<int> row_map; 
            for (char b : line)
                row_map.push_back(b - '0');
            bottom_map.push_back(row_map);
        }
        for (const auto &row: bottom_map)
            cout << row << endl;
        for (size_t row = 0; row < bottom_map.size(); ++row)
        {
            for (size_t col = 0; col < bottom_map[row].size(); ++col)
            {
                if (is_lowest(row, col, bottom_map))
                    lowest_points.push_back(1 + bottom_map[row][col]);
            }
        }
        int sum { 0 };
        for (auto point: lowest_points)
            sum += point;
        cout << "Sum: " << sum << endl;
    }
    else
        cout << "input9.txt not open!\n";
    return 0;
}
