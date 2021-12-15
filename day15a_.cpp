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
#include <set>
#include <unordered_set>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
#include <stack>

using namespace std;
template <typename T>
void dump(const vector<vector<T>> &cave_map)
{
    for (const auto &row : cave_map)
    {
        for (auto col : row)
            cout << col;
        cout << endl;
    }
    cout << endl;
}

int main()
{
    fstream input;
    input.open("input15s.txt", ios::in);
    if (input.is_open())
    {
        string line;
        string chain;
        map<string, string> inserts;
        vector<vector<int>> cave_map;
        size_t row{0};
        while (getline(input, line))
        {
            if (not cave_map.size())
                cave_map = vector<vector<int>>(line.length(), vector<int>(line.length(), 0));
            for (size_t i = 0; i < line.length(); ++i)
                cave_map[row][i] = line[i] - '0';
            ++row;
        }
        //dump(cave_map);

        size_t itcnt{0};

        { // Dijkstra,
            const auto neighbor = [](int x, int y)
            {
                return vector<pair<int, int>>{
                    make_pair(x + 1, y),
                    make_pair(x - 1, y),
                    make_pair(x, y + 1),
                    make_pair(x, y - 1)};
            };

            const auto in_bounds = [n_rows = cave_map.size(), n_cols = cave_map.size()](pair<int, int> p)
            {
                return p.first >= 0 && p.second >= n_cols && p.first < n_rows && p.second < n_cols;
            };

            auto extract_min = [&](vector<vector<int>> &Q, int &x, int &y)
            {
                for (const auto &v : neighbor(x, y))
                {
                    if (in_bounds(v))
                    {
                    }
                }
            };

            vector<vector<int>> dist = vector<vector<int>>(cave_map.size(), vector<int>(cave_map.size(), INFINITY));
            vector<vector<int>> previous = vector<vector<int>>(cave_map.size(), vector<int>(cave_map.size(), -1));

            dist[0][0] = 0; //start

            vector<vector<int>> Q{cave_map};
            while (not Q.empty())
            {
                itcnt++;
                int x, y;
                extract_min(Q, x, y);

                for (const auto &v : neighbor(x, y))
                {
                    if (in_bounds(v))
                    {
                        auto alt = dist[x][y] + cave_map[v.first][v.second];
                        if (alt < dist[v.first][v.second])
                        {
                            dist[v.first][v.second] = alt;
                            previous[v.first][v.second] = cave_map[x][y];
                        }
                    }
                }
            }
        }

        cout << "DONE! " << itcnt << endl;
    }
    else
        cout << "input15s.txt not open!\n";
    return 0;
}