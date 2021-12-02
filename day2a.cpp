#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
    fstream input;
    input.open("input2a.txt", ios::in);
    if (input.is_open())
    {
        string line;
        int depth = 0;
        int position = 0;
        int lineCount = 0;
        while (getline(input, line))
        {
            cout << "Line #:" << lineCount++ << " " << line;
            size_t pos = line.find("forward");
            if (pos != string::npos)
            {
                cout << " fw:" << pos;
                position += stoi(line.substr(line.find(" ")));
            }

            pos = line.find("down");            
            if (pos != string::npos)
            {
                cout << " dw:" << pos;
                depth += stoi(line.substr(line.find(" ")));
            }

            pos = line.find("up");            
            if (pos != string::npos)
            {
                cout << " up:" << pos;
                depth -= stoi(line.substr(line.find(" ")));
            }
            cout << "\n";
        }
        cout << "Position:" << depth * position << "\n";
    }
    else
        cout << "input2.txt not open!\n";
    return 0;
}