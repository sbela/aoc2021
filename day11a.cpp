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

void print(const vector<vector<int>> &light_map)
{
    for (const auto &line : light_map)
        cout << line << endl;
    cout << endl;
};

void check_step(vector<vector<int>> &light_map, vector<vector<int>> &already_flashed_light_map, size_t &flashing_lights)
{
    //print(light_map);
    //print(already_flashed_light_map);

    bool calcAgain{false};
    for (size_t row = 0; row < light_map.size(); ++row)
    {
        for (size_t col = 0; col < light_map.at(row).size(); ++col)
        {
            if (light_map[row][col] > 9)
            {
                light_map[row][col] = 0;
                flashing_lights++;
                already_flashed_light_map[row][col]++;
                calcAgain = true;
                if (already_flashed_light_map[row][col] == 1)
                {                    
                    auto check_light = [&](size_t row, size_t col)
                    {
                        if (light_map[row][col])
                            light_map[row][col]++;
                    };

                    if (row > 0)
                    {
                        if (col > 0)
                            check_light(row - 1, col - 1);
                        check_light(row - 1, col);
                        if (col < (light_map.at(row).size() - 1))
                            check_light(row - 1, col + 1);
                    }

                    if (col < (light_map.at(row).size() - 1))
                        check_light(row, col + 1);

                    if (row < (light_map.size() - 1))
                    {
                        if (col < (light_map.at(row).size() - 1))
                            check_light(row + 1, col + 1);
                        check_light(row + 1, col);
                        if (col > 0)
                            check_light(row + 1, col - 1);
                    }

                    if (col > 0)
                        check_light(row, col - 1);
                }
            }
        }
    }
    if (calcAgain)
        check_step(light_map, already_flashed_light_map, flashing_lights);
}

bool is_all_flashing_simultaneously(const vector<vector<int>> &light_map)
{
    for (const auto &line : light_map)
    {
        for (int i : line)
        {
            if (i)
                return false;
        }
    }
    return true;
}

int main()
{
    fstream input;
    input.open("input11.txt", ios::in);
    if (input.is_open())
    {
        string line;
        vector<vector<int>> light_map;
        while (getline(input, line))
        {
            light_map.push_back({});
            for (auto i : line)
                light_map.back().push_back(i - '0');
        }

        cout << "Light map:" << endl;
        print(light_map);

        size_t flashing_lights { 0 };
        for (int i = 0, j = 0; ; ++i)
        {
            vector<vector<int>> already_flashed_light_map(light_map.size(), vector<int>(light_map.at(0).size(), 0));
            for (size_t row = 0; row < light_map.size(); ++row)
                for (size_t col = 0; col < light_map.at(row).size(); ++col)
                    light_map[row][col]++;
            check_step(light_map, already_flashed_light_map, flashing_lights);

            if (is_all_flashing_simultaneously(light_map)) 
            {
                cout << "Light map: " << (i + 1) << endl;
                print(light_map);
                if (++j > 9)
                    break;
            }
        }        
    }
    else
        cout << "input11s.txt not open!\n";
    return 0;
}