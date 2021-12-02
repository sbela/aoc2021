#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    fstream input;
    input.open("input2b.txt", ios::in);
    if (input.is_open())
    {
        string line;
        int depth = 0;
        int position = 0;
        int aim = 0;
        int lineCount = 0;
        while (getline(input, line))
        {
            cout << "Line #:" << lineCount++ << " " << line;
            size_t pos = line.find("forward");
            if (pos != string::npos)
            {
                position += stoi(line.substr(line.find(" ")));
                depth += aim * stoi(line.substr(line.find(" ")));
                cout << " fw:" << pos << " p:" << position << " d:" << depth << " a:" << aim;
            }

            pos = line.find("down");            
            if (pos != string::npos)
            {
                //depth += stoi(line.substr(line.find(" ")));
                aim += stoi(line.substr(line.find(" ")));
                cout << " dw:" << pos << " d:" << depth << " a:" << aim;
            }

            pos = line.find("up");            
            if (pos != string::npos)
            {
                //depth -= stoi(line.substr(line.find(" ")));
                aim -= stoi(line.substr(line.find(" ")));
                cout << " up:" << pos << " d:" << depth << " a:" << aim;
            }
            cout << "\n";
        }
        cout << "Position: " << depth * position << " depth:" << depth << " position:" << position << " aim:" << aim << "\n";
    }
    else
        cout << "input2.txt not open!\n";
    return 0;
}