#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <type_traits>

using namespace std;

struct LINE
{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
};

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    for (auto const &value : values)
        output << value << endl;
    return output;
}

ostream &operator<<(ostream &output, LINE const &values)
{
    output << "(" << values.start_x << "," << values.start_y << ") -> (" << values.end_x << "," << values.end_y << ")";
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

const LINE parseLine(const string &line)
{
    auto params = tokenize<string>(line, ' ');
    if (params.size() == 3)
    {
        auto start = tokenize<int>(params.front(), ',');
        auto end = tokenize<int>(params.back(), ',');
        return {start.front(), start.back(), end.front(), end.back()};
    }
    cout << "Invalid LINE:" << line << endl;
    return {};
}

void drawTable(const vector<vector<int>> &table)
{
    cout << "+";
    for (size_t i = 0; i < table.size(); ++i)
        cout << "-";
    cout << "+" << endl;
    for (const auto &row : table)
    {
        cout << "|";
        for (int i : row)
        {
            cout << (char)(i == 0 ? '.' : '0' + i);
        }
        cout << "|" << endl;
    }
    cout << "+";
    for (size_t i = 0; i < table.size(); ++i)
        cout << "-";
    cout << "+\n";
}

void drawLine(vector<vector<int>> &table, const LINE &line)
{
    if (line.start_x == line.end_x)
    {
        int start_y = min(line.start_y, line.end_y);
        int end_y = max(line.end_y, line.start_y);
        for (int y = start_y; y <= end_y; ++y)
            table[y][line.start_x]++;
    }
    else if (line.start_y == line.end_y)
    {
        int start_x = min(line.start_x, line.end_x);
        int end_x = max(line.end_x, line.start_x);
        for (int x = start_x; x <= end_x; ++x)
            table[line.start_y][x]++;
    }
    else
    {
        if (line.end_x - line.start_x)
        {
            double m = (line.end_y - line.start_y) / (line.end_x - line.start_x);
            if (m < 0)
            {
                if ((line.end_y - line.start_y) > 0)
                {
                    if ((line.end_x - line.start_x) < 0)
                    {
                        cout << "-m<:" << m << " :" << line << " start_x:" << line.start_x << " end_x:" << line.end_x << endl;
                        for (int x = line.start_x, y = line.start_y; x >= line.end_x; --x, ++y)
                            table[y][x]++;
                    }
                    else
                        cout << "WTF AGAIN1!!!" << endl;
                }
                else
                {
                    if ((line.end_x - line.start_x) > 0)
                    {

                        cout << "-m>:" << m << " :" << line << " start_x:" << line.start_x << " end_x:" << line.end_x << endl;
                        for (int x = line.start_x, y = line.start_y; x <= line.end_x; ++x, --y)
                            table[y][x]++;
                    }
                    else
                        cout << "WTF AGAIN2!!!" << endl;
                }
            }
            else if (m > 0)
            {
                if ((line.end_y - line.start_y) > 0) // -> /
                {
                    if ((line.end_x - line.start_x) > 0)
                    {
                        cout << "+m>:" << m << " :" << line << " start_x:" << line.start_x << " end_x:" << line.end_x << endl;
                        for (int x = line.start_x, y = line.start_y; x <= line.end_x; ++x, ++y)
                            table[y][x]++;
                    }
                    else
                        cout << "WTF NOW1!!!" << endl;
                }
                else /* -> \ */
                {
                    if ((line.end_x - line.start_x) < 0)
                    {
                        cout << "+m<:" << m << " :" << line << " start_x:" << line.start_x << " end_x:" << line.end_x << endl;
                        for (int x = line.start_x, y = line.start_y; x >= line.end_x; --x, --y)
                            table[y][x]++;
                    }
                    else
                        cout << "WTF NOW2!!!" << endl;
                }
            }
            else
                cout << "m horizontal!!!";
        }
        else
            cout << "m vertical !!!";
    }
}

vector<vector<int>> drawLines(const vector<LINE> &lines)
{
    int width{0}, height{0};
    for (const auto &line : lines)
    {
        width = max({width, line.start_x, line.end_x});
        height = max({height, line.start_y, line.end_y});
    }
    width++;
    height++;
    cout << "Width:" << width << " Height:" << height << endl;

    vector<vector<int>> table(height, vector<int>(width, 0));

    //drawTable(table);

    for (const auto &line : lines)
        drawLine(table, line);

    //drawTable(table);

    return table;
}

int main()
{
    fstream input;
    input.open("input5.txt", ios::in);
    if (input.is_open())
    {
        string line;
        vector<LINE> lines;
        while (getline(input, line))
            lines.push_back(parseLine(line));
        auto table = drawLines(lines);
        int overlap = 0;
        for (const auto &row : table)
        {
            for (int i : row)
                if (i > 1)
                    overlap++;
        }
        cout << "Overlap: " << overlap << endl;
    }
    else
        cout << "input5.txt not open!\n";
    return 0;
}