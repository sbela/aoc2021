#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main() 
{
    fstream input;
    input.open("input3.txt", ios::in);
    if (input.is_open())
    {
        int gamma_rate { 0 };
        int epsilon_rate { 0 };
        int diagnostic_report[12][2] { 0 };
        string line;
        while (getline(input, line))
        {
            for (int i = 0; i < 12; ++i)
                line[i] == '0' ? diagnostic_report[11 - i][0]++ : diagnostic_report[11 - i][1]++;
            //for (int i = 0; i < 5; ++i)
                //cout << "Line: " << line << " " << i << " 0:" << diagnostic_report[i][0] << " 1:" << diagnostic_report[i][1] << endl;
        }

        for (int i = 0; i < 12; ++i)
        {
            if (diagnostic_report[i][1] == diagnostic_report[i][0])
                cout << "WTF:" << i << " 0:" << diagnostic_report[i][0] << " 1:" << diagnostic_report[i][1] << endl;
            if (diagnostic_report[i][1] > diagnostic_report[i][0])
                gamma_rate |= (1 << i);
            else
                epsilon_rate |= (1 << i);
        }
        cout << "Power: " << gamma_rate * epsilon_rate << " gamma_rate:" << gamma_rate << " epsilon_rate:" << epsilon_rate << endl;
    }
    else
        cout << "input3.txt not open!\n";        
    return 0;
}