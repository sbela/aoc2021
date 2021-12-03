#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

char getCommonValue(const vector<string> &lines, int pos, bool most)
{
    int diagnostic_report[2]{0};
    for (const string &line : lines)
    {
        //cout << line << endl;
        line[pos] == '0' ? diagnostic_report[0]++ : diagnostic_report[1]++;
    }
    //cout << "Diagnostic: " << diagnostic_report[0] << " : " << diagnostic_report[1] << endl;
    if (diagnostic_report[1] >= diagnostic_report[0])
        return most ? '1' : '0';
    return most ? '0' : '1';
}

int get_rating(vector<string> ogr, int length, bool most)
{
    for (int i = 0; i < length; ++i)
    {
        if (ogr.size() > 1)
        {
            char mcv = getCommonValue(ogr, i, most);
            //cout << "i:" << i << " v:" << mcv << " ogr:" << ogr.size() << endl;
            for (int j = ogr.size() - 1; j >= 0; --j)
            {
                if (ogr[j][i] != mcv)
                    ogr.erase(ogr.cbegin() + j);
            }
            /*for (const string &o : ogr)
                cout << "ogr:" << o << endl;*/
        }
        else
        {
            cout << "Finished at : " << i << " with:" << ogr[0] << endl;
            break;
        }
    }
    if (ogr.size() > 0)
    {
        /*for (const string &o : ogr)
            cout << "ratings:" << o << endl;*/
        if (ogr.size() == 1)
        {
            int rating { 0 };
            for (int i = 0; i < length; ++i)
            {
                if (ogr[0][length - 1 - i] == '1')
                    rating |= (1 << i);
            }
            //cout << "rating: " << rating << endl;
            return rating;
        }
    }
    return -1;
}

int main()
{
    fstream input;
    input.open("input3.txt", ios::in);
    if (input.is_open())
    {
        int oxygen_generator_rating{0};
        int C02_scrubber_rating{0};
        string line;
        vector<string> lines;
        int length;
        while (getline(input, line))
        {
            length = line.size();
            lines.push_back(line);
        }
        oxygen_generator_rating = get_rating(lines, length, true);
        cout << "oxygen_generator_rating: " << oxygen_generator_rating << endl;
        C02_scrubber_rating = get_rating(lines, length, false);
        cout << "C02_scrubber_rating: " << C02_scrubber_rating << endl;
        cout << "Life support rating: " << oxygen_generator_rating * C02_scrubber_rating << endl;
    }
    else
        cout << "input3.txt not open!\n";
    return 0;
}