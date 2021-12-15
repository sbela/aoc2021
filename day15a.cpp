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

void dump(const vector<vector<int>> &cave_map)
{
    for (const auto &row : cave_map)
    {
        for (auto col : row)
            cout << col;
        cout << endl;
    }
    cout << endl;
}

// function to display the path
vector<tuple<int, vector<int>>> path;
void display(vector<int> &ans)
{
    int sum { 0 };
    for (auto i : ans)
        sum += i;
    path.push_back({ sum, ans });
}

// a function which check whether our step is safe or not
bool issafe(int r, int c, vector<vector<int>> &visited, int n, int m)
{
    return (r < n and c < m and visited[r][c] != -1); // return true if all values satisfied else false
}

void FindPaths(vector<vector<int>> &grid, int r, int c, int n, int m, vector<int> &ans)
{
    // when we hit the last cell we reach to destination then directly push the path
    if (r == n - 1 and c == m - 1)
    {
        ans.push_back(grid[r][c]);
        display(ans);   // function to display the path stored in ans vector
        ans.pop_back(); // pop back because we need to backtrack to explore more path
        return;
    }

    // we will store the current value in ch and mark the visited place as -1
    int ch = grid[r][c];

    ans.push_back(ch); // push the path in ans array
    grid[r][c] = -1;      // mark the visited place with -1

    // if is it safe to take next downward step then take it
    if (issafe(r + 1, c, grid, n, m))
    {
        FindPaths(grid, r + 1, c, n, m, ans);
    }

    // if is it safe to take next rightward step then take it
    if (issafe(r, c + 1, grid, n, m))
    {
        FindPaths(grid, r, c + 1, n, m, ans);
    }

    // backtracking step we need to make values original so to we can visit it by some another path
    grid[r][c] = ch;

    // remove the current path element we explore
    ans.pop_back();
    return;
}

int main()
{
    fstream input;
    input.open("input15.txt", ios::in);
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

        vector<int> ans; // it will store the path which we have covered

        FindPaths(cave_map, 0, 0, cave_map.size(), cave_map.size(), ans); // here 0,0 are initial position to start with
        int sum = 10000000/*, index { 0 }, sum_index { 0 }*/;
        for (const auto &[_sum, _path] : path)
        {
            if (_sum < sum) 
            {
                sum = _sum;
                //sum_index = index;
            }
            //++index;
        }
        /*for (auto _path : get<1>(path[sum_index]))
            cout << _path << " ";*/
        cout << endl;
        cout << "Sum: " << (sum - 1) << endl;
    }
    else
        cout << "input15s.txt not open!\n";
    return 0;
}