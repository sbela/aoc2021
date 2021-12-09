#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <vector>
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

bool is_lowest(int row, int col, const vector<vector<int>> &bottom_map)
{
    int at = bottom_map[row][col];
    int smaller_count{0};
    int allowed_position{0};
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

size_t basin_size(int row, int col, const vector<vector<int>> &bottom_map)
{
    size_t row_count = bottom_map.size();
    size_t col_count = bottom_map[row].size();
    vector<tuple<size_t, size_t>> positions;

    auto add_position = [&](size_t _row, size_t _col)
    {
        if (bottom_map[_row][_col] < 9)
        {
            if (find(positions.begin(), positions.end(), make_tuple(_row, _col)) == positions.end())
                positions.push_back(make_tuple(_row, _col));
            return false;
        }
        return true;
    };
    // menjunk balra
    for (int x = col; x > -1; --x)
    {
        if (add_position(row, x))
            break;
    }

    // menjunk jobbra
    for (size_t x = col; x < col_count; ++x)
    {
        if (add_position(row, x))
            break;
    }

    vector<tuple<size_t, size_t>> horizontal_positions(positions);
    for (const auto &position : horizontal_positions)
    {
        auto column = get<1>(position);
        // menjunk felfele
        for (int y = get<0>(position); y > -1; --y)
        {
            if (add_position(y, column))
                break;
        }

        // menjunk lefele
        for (size_t y = get<0>(position); y < row_count; ++y)
        {
            if (add_position(y, column))
                break;
        }
    }
    vector<tuple<size_t, size_t>> vertical_positions(positions);
    for (const auto &position : vertical_positions)
    {
        auto row = get<0>(position);
        // menjunk balra
        for (int x = get<1>(position); x > -1; --x)
        {
            if (add_position(row, x))
                break;
        }

        // menjunk jobbra
        for (size_t x = get<1>(position); x < col_count; ++x)
        {
            if (add_position(row, x))
                break;
        }
    }

    cout << "Positions[" << row << "," << col << "]: " << positions << endl;
    return positions.size();
}

int main()
{
    fstream input;
    input.open("input9.txt", ios::in);
    if (input.is_open())
    {
        string line;
        vector<vector<int>> bottom_map;
        vector<int> lowest_points_value;
        vector<tuple<size_t, size_t>> lowest_points;
        while (getline(input, line))
        {
            vector<int> row_map;
            for (char b : line)
                row_map.push_back(b - '0');
            bottom_map.push_back(row_map);
        }
        for (const auto &row : bottom_map)
            cout << row << endl;
        for (size_t row = 0; row < bottom_map.size(); ++row)
        {
            for (size_t col = 0; col < bottom_map[row].size(); ++col)
            {
                if (is_lowest(row, col, bottom_map))
                {
                    lowest_points_value.push_back(1 + bottom_map[row][col]);
                    lowest_points.emplace_back(make_tuple(row, col));
                }
            }
        }
        int sum{0};
        for (auto point : lowest_points_value)
            sum += point;
        cout << "Lowest points sum: " << sum << endl;

        vector<size_t> basin_sizes;
        for (const auto &point : lowest_points)
            basin_sizes.emplace_back(basin_size(get<0>(point), get<1>(point), bottom_map));

        sort(basin_sizes.begin(), basin_sizes.end(), greater<size_t>());

        size_t mult{1};
        for (int i = 0; i < 3; ++i)
        {
            mult *= basin_sizes[i];
            cout << i << " : " << basin_sizes[i] << " mult: " << mult << endl;
        }
        cout << "Basin size product: " << mult << endl;
    }
    else
        cout << "input9.txt not open!\n";
    return 0;
}
