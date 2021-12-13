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
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

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
            data.push_back(stoi(intermediate));
    }
    return data;
}

struct Paper
{
    vector<vector<char>> _paper;
    enum class FoldingDirection
    {
        Horizontal,
        Vertical
    };
    // [Folding Direction (0:horizontal, 1:vertical), Folding Position]
    vector<tuple<FoldingDirection, int>> _folding;

    void dump(bool showPaper = true)
    {
        for (const auto &[direction, position] : _folding)
            cout << (direction == FoldingDirection::Horizontal ? "Horizontal: " : "Vertical: ") << position << endl;

        if (showPaper)
        {
            for (size_t row = 0; row < _paper.size(); ++row)
            {
                for (size_t col = 0; col < _paper[row].size(); ++col)
                    cout << _paper[row][col];
                cout << endl;
            }
        }

        count();
    }

    void fold(FoldingDirection direction, size_t position)
    {
        if (direction == FoldingDirection::Horizontal)
        {
            vector<vector<char>> folded(position, vector<char>(_paper[0].size(), '.'));
            swap_ranges(folded.begin(), folded.end(), _paper.begin());

            for (size_t row = position + 1; row < _paper.size(); ++row)
            {
                for (size_t col = 0; col < _paper[row].size(); ++col)
                {
                    // (position - 1) - (row - (position + 1)) = 2 * p - r = (p << 1) - r
                    int cell_row = (position - 1) - (row - (position + 1));
                    if (cell_row > -1)
                    {
                        if ((folded[cell_row][col] == '#') or (_paper[row][col] == '#'))
                            folded[cell_row][col] = '#';
                    }
                }
            }
            _paper = folded;
        }
        else
        {
            vector<vector<char>> folded(_paper.size(), vector<char>(position, '.'));
            for (size_t row = 0; row < _paper.size(); ++row)
                for (size_t col = 0; col < position; ++col)
                    folded[row][col] = _paper[row][col];

            for (size_t row = 0; row < _paper.size(); ++row)
            {
                for (size_t col = position + 1; col < _paper[row].size(); ++col)
                {
                    // (position - 1) - (col - (position + 1)) = 2 * p - c = (p << 1) - r
                    int cell_col = (position - 1) - (col - (position + 1));
                    if (cell_col > -1)
                    {
                        if ((folded[row][cell_col] == '#') or (_paper[row][col] == '#'))
                            folded[row][cell_col] = '#';
                    }
                }
            }
            _paper = folded;
        }
    }

    void count()
    {
        int count{0};
        for (const auto &row : _paper)
            for (const auto &col : row)
                if (col == '#')
                    count++;
        cout << "Count: " << count << endl;
    }
};

int main()
{
    fstream input;
    input.open("input13.txt", ios::in);
    if (input.is_open())
    {
        string line;
        Paper paper;
        vector<tuple<int, int>> mark_list;
        string vertical("fold along x=");
        string horizontal("fold along y=");
        int row_max{0}, col_max{0};
        while (getline(input, line))
        {
            if (line.find(vertical) != string::npos)
                paper._folding.push_back({Paper::FoldingDirection::Vertical, stoi(line.substr(vertical.length()))});
            else if (line.find(horizontal) != string::npos)
                paper._folding.push_back({Paper::FoldingDirection::Horizontal, stoi(line.substr(horizontal.length()))});
            else
            {
                if (line.length())
                {
                    auto mark_position = tokenize<int>(line, ',');
                    if (mark_position.front() > col_max)
                        col_max = mark_position.front();
                    if (mark_position.back() > row_max)
                        row_max = mark_position.back();
                    mark_list.push_back({mark_position.front(), mark_position.back()});
                }
            }
        }

        paper._paper = vector<vector<char>>(row_max + 1, vector<char>(col_max + 1, '.'));
        for (const auto &[col, row] : mark_list)
            paper._paper[row][col] = '#';

        for (const auto &[direction, position] : paper._folding)
        {
            paper.fold(direction, position);
            break;
        }
        paper.dump(false);
    }
    else
        cout << "input13s.txt not open!\n";
    return 0;
}
