#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

using namespace std;

int main()
{
    fstream input;
    input.open("input.txt", ios::in);
    if (input.is_open())
    {
        string line;
        int bigger = 0;
        int prev = -1;
        int lineCount = 0;
        vector<int> tripleSums;
        deque<int> triplet;
        while (getline(input, line))
        {
            if (line.length())
            {
                triplet.push_back(stoi(line));
                if (triplet.size() == 3)
                {
                    tripleSums.push_back(triplet.at(0) + triplet.at(1) + triplet.at(2));
                    triplet.pop_front();
                }
                cout << "line #" << ++lineCount << " :" << line << " #:" << triplet.size() << " sum:" <<  tripleSums.size() << "\n";
            }
        }

        for (int v : tripleSums)
        {
            if (prev != -1)
            {
                if (prev < v)
                    bigger++;
            }
            prev = v;
        }
        cout << "Bigger #:" << bigger << "\n";
        input.close();
    }
    else
        cout << "input.txt not open!\n";
    return 0;
}
