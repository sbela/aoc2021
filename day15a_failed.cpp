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
template<typename T>
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

// a function which check whether our step is safe or not
bool issafe(int r, int c, vector<vector<int>> &visited, int n, int m)
{
    return (r < n and c < m and visited[r][c] != 0); // return true if all values satisfied else false
}

static int shortest { INT_MAX };
void FindPaths(vector<vector<int>> &grid, int r, int c, int n, int m, int &path_len, size_t &itcnt)
{
    itcnt++;
    if (shortest <= (path_len + grid[r][c]))
        return;
    // when we hit the last cell we reach to destination then directly push the path
    if ((r == (n - 1)) and (c == (m - 1)))
    {
        shortest = path_len + grid[r][c];
        cout << "Shortest: " << (shortest - 1) << endl;
        return;
    }

    // we will store the current value in ch and mark the visited place as -1
    int ch = grid[r][c];

    path_len += ch; // push the path in ans array
    grid[r][c] = 0;   // mark the visited place with -1

    // if is it safe to take next downward step then take it
    if (issafe(r + 1, c, grid, n, m))
    {
        FindPaths(grid, r + 1, c, n, m, path_len, itcnt);
    }

    // if is it safe to take next rightward step then take it
    if (issafe(r, c + 1, grid, n, m))
    {
        FindPaths(grid, r, c + 1, n, m, path_len, itcnt);
    }

    // backtracking step we need to make values original so to we can visit it by some another path
    grid[r][c] = ch;

    // remove the current path element we explore
    path_len -= ch;
    return;
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

        int path_len{0};                                                       // it will store the path which we have covered
        size_t itcnt { 0 };
        FindPaths(cave_map, 0, 0, cave_map.size(), cave_map.size(), path_len, itcnt); // here 0,0 are initial position to start with
        cout << "DONE! " << itcnt << endl;
    }
    else
        cout << "input15s.txt not open!\n";
    return 0;
}