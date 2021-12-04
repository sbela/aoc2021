#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>

using namespace std;

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    for (auto const &value : values)
        output << value << endl;
    return output;
}

const vector<int> tokenize(const string &text, char token)
{
    vector<int> data;
    stringstream check(text);
    string intermediate;
    while (getline(check, intermediate, token))
    {
        if (intermediate.length())
            data.push_back(stoi(intermediate));
    }
    return data;
}

bool is_bingo(const vector<vector<int>> &board)
{
    for (auto row : board)
    {
        size_t marked = 0;
        for (int row_num : row)
        {
            if (row_num < 0)
                marked++;
        }
        if (marked == row.size())
        {
            cout << "BINGO ROW: ";
            for (int row_num : row)
                cout << row_num << " ";
            cout << endl;
            return true;
        }
    }

    for (size_t col = 0; col < board[0].size(); ++col)
    {
        size_t marked = 0;
        for (auto column : board)
        {
            if (column[col] < 0)
                marked++;
        }
        if (marked == board[0].size())
        {
            cout << "BINGO COL: ";
            for (auto column : board)
                cout << column[col];
            cout << endl;
            return true;
        }
    }

    return false;
}

int calculate_score(const vector<vector<int>> &board)
{
    int sum = 0;
    for (const auto &row : board)
    {
        for (int row_num : row)
        {
            if (row_num >= 0)
                sum += row_num;
        }
    }
    return sum;
}

bool mark_numbers(const vector<int> &numbers, vector<vector<vector<int>>> &boards)
{
    for (int number : numbers)
    {
        for (auto &board : boards)
        {
            for (vector<int> &row : board)
            {
                for (size_t i = 0; i < row.size(); ++i)
                {
                    if (row[i] == number)
                    {
                        row[i] *= -1;
                        if (is_bingo(board))
                        {
                            int score = calculate_score(board);
                            cout << "Score:" << score * number << endl;
                            return true;
                        }
                    }
                }
            }
            cout << "Searching for:\n"
                 << numbers;
            cout << "Processed board:\n"
                 << board;
        }
    }
    return false;
}

int main()
{
    fstream input;
    input.open("input4.txt", ios::in);
    if (input.is_open())
    {
        string line;
        string draw_numbers;
        vector<vector<vector<int>>> boards;
        int current_board = -1;
        while (getline(input, line))
        {
            int line_length = line.length();
            if (line_length > 14)
                draw_numbers = line;
            else if (line_length == 14)
            {
                if (current_board == -1)
                    current_board++;
                boards[current_board].push_back(tokenize(line, ' '));
                cout << line << endl;
            }
            else
            {
                cout << endl;
                current_board++;
                boards.push_back(vector<vector<int>>());
            }
        }
        cout << endl;

        /*for (int i = 0; i < 3; ++i)
            cout << boards[i] << endl;*/

        vector<int> draw = tokenize(draw_numbers, ',');
        int round = 0;
        while (draw.size() > 0)
        {
            if (draw.size() < 12)
            {
                cout << "Unused numbers:" << draw.size() << "\n"
                     << draw << endl;
                break;
            }
            cout << "Draw round#" << ++round << ": " << draw.size() << "\n"
                 << draw << endl;
            vector<int> draw_1(draw.cbegin(), draw.cbegin() + 5);
            cout << "Draw 1: " << draw_1.size() << "\n"
                 << draw_1 << endl;
            if (mark_numbers(draw_1, boards))
                break;
            vector<int> draw_2(draw.cbegin() + 5, draw.cbegin() + 11);
            cout << "Draw 2: " << draw_2.size() << "\n"
                 << draw_2 << endl;
            if (mark_numbers(draw_2, boards))
                break;
            vector<int> draw_3(draw.cbegin() + 11, draw.cbegin() + 12);
            cout << "Draw 3: " << draw_3.size() << "\n"
                 << draw_3 << endl;
            if (mark_numbers(draw_3, boards))
                break;
            draw.erase(draw.cbegin(), draw.cbegin() + 12);
        }
    }
    else
        cout << "input4s.txt not open!\n";
    return 0;
}