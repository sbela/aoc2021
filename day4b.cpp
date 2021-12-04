#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <iomanip>

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

bool is_bingo(const vector<vector<int>> &board, vector<vector<int>> &winner_bingos)
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
            bool found = false;
            for (const auto &winner_bingo : winner_bingos)
            {
                if (equal(winner_bingo.cbegin(), winner_bingo.cend(), row.cbegin()))
                {
                    found = true;
                    break;
                }
            }
            if (not found)
            {
                winner_bingos.push_back(row);
                cout << "BINGO ROW: ";
                for (int row_num : row)
                    cout << row_num + 1;
                cout << endl;
                return true;
            }
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
            vector<int> bingo;
            for (auto column : board)
                bingo.push_back(column[col]);
            bool found = false;
            for (const auto &winner_bingo : winner_bingos)
            {
                if (equal(winner_bingo.cbegin(), winner_bingo.cend(), bingo.cbegin()))
                {
                    found = true;
                    break;
                }
            }
            if (not found)
            {
                winner_bingos.push_back(bingo);
                cout << "BINGO COL: ";
                for (auto column : board)
                    cout << column[col] + 1;
                cout << endl;
                return true;
            }
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
            if (row_num > 0)
                sum += row_num;
        }
    }
    return sum;
}

void mark_numbers(const vector<int> &numbers, vector<vector<vector<int>>> &boards, vector<int> &winner_scores, vector<vector<int>> &winner_bingos,
                  vector<int> &winner_boards)
{
    if (winner_boards.size() == boards.size())
        return;
    for (int number : numbers)
    {
        cout << "Searching for: " << number << endl;
        for (size_t board_index = 0; board_index < boards.size(); ++board_index)
        {
            for (vector<int> &row : boards[board_index])
            {
                for (size_t i = 0; i < row.size(); ++i)
                {
                    if (row[i] == number)
                    {
                        row[i] = -row[i] - 1;
                        if (is_bingo(boards[board_index], winner_bingos))
                        {
                            int score = calculate_score(boards[board_index]);
                            cout << "Score : #" << winner_boards.size() << " : " << number << " #" << score * number << endl;
                            if (winner_boards.size() < boards.size())
                            {
                                if (find(winner_boards.cbegin(), winner_boards.cend(), board_index) == end(winner_boards))
                                {
                                    winner_scores.push_back(score * number);
                                    winner_boards.push_back(board_index);
                                }
                            }
                            else
                                return;
                        }
                    }
                }
            }
            /*cout << "Processed board:\n"
                 << board;*/
        }
    }
    cout << endl;
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
        vector<int> winner_scores;
        vector<int> winner_boards;
        vector<vector<int>> winner_bingos;
        /*int round = 0;
        cout << "Draw round#" << ++round << ": " << draw.size() << "\n"
                << draw << endl;*/
        vector<int> draw_1(draw.cbegin(), draw.cbegin() + 5);
        cout << "Draw 1: " << draw_1.size() << "\n"
             << draw_1;
        mark_numbers(draw_1, boards, winner_scores, winner_bingos, winner_boards);
        vector<int> draw_2(draw.cbegin() + 5, draw.cbegin() + 11);
        cout << "Draw 2: " << draw_2.size() << "\n"
             << draw_2;
        mark_numbers(draw_2, boards, winner_scores, winner_bingos, winner_boards);
        draw.erase(draw.cbegin(), draw.cbegin() + 11);
        while (draw.size())
        {
            vector<int> draw_N(draw.cbegin(), draw.cbegin() + 1);
            cout << "Draw N: " << draw_N.size() << "\n"
                 << draw_N;
            mark_numbers(draw_N, boards, winner_scores, winner_bingos, winner_boards);
            draw.erase(draw.cbegin(), draw.cbegin() + 1);
        }
        if (winner_scores.size())
        {
            cout << "Last winner boards:\n"
                 << winner_boards
                 << "scores:\n"
                 << winner_scores << endl;
            cout << "Number of boards:" << boards.size() << "/" << winner_boards.size() << endl;
            cout << "Last winner board: " << winner_boards.back() << endl;
            cout << "Last winner score: " << winner_scores.back() << endl;
            cout << "Board:" << winner_boards.back() << endl;
            for (const auto &row : boards[winner_boards.back()])
            {
                for (int r : row)
                    cout << right << setw(3) << (r > 0 ? r : r + 1) << " ";
                cout << endl;
            }
            cout << endl;
        }
    }
    else
        cout << "input4s.txt not open!\n";
    return 0;
}