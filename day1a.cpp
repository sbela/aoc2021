#include <iostream>
#include <fstream>
#include <string>

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
        while (getline(input, line))
        {
            if (line.length())
            {
                try 
                {
                    if ((prev < stoi(line)) and (prev > -1))
                        bigger++;
                    prev = stoi(line);
                }
                catch ( ... )
                {
                    cout << "DOES NOT MATTER:" << hex << line << dec << "\n";
                }
                cout << "line #" << ++lineCount << " :" << line << " #:" << bigger << "\n";
            }
        }
        cout << "Bigger #:" << bigger << "\n";
        input.close();
    }
    else
        cout << "day.txt not open!\n";
    return 0;
}
