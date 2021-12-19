#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>
#include <variant>

int calculateMagnitude(std::string &input)
{
    std::cout << "CALC:" << input << std::endl;
    size_t pos = input.find(',');
    while (pos != std::string::npos)
    {
        if (input[pos - 1] != ']' and input[pos + 1] != '[')
        {
            size_t lpos = input.rfind('[', pos);
            size_t rpos = input.find(']', pos);
            if ((rpos == std::string::npos) || (lpos == std::string::npos))
            {
                std::cout << "WTF:" << input << " " << rpos << " " << lpos << std::endl;
            }
            else
            {
                auto l = input.substr(pos - (pos - lpos - 1), (pos - lpos - 1));
                auto r = input.substr(pos + 1, rpos - pos - 1);
                int mag = 3 * std::stoi(l) + 2 * std::stoi(r);
                input.erase(lpos, rpos - lpos + 1);
                input.insert(lpos, std::to_string(mag));
                pos = input.find(',');
            }
        }
        else
            pos = input.find(',', pos + 1);
    }
    return std::stoi(input);
}

void sum(std::string &a, std::string &b)
{
    //std::cout << a << " + " << b << " = ";
    a = "[" + a + "," + b + "]";
    std::cout << "SUM :" << a << std::endl;
}

bool explode(std::string &input)
{
    bool ret{false};
    bool reScan;

    do
    {
        reScan = false;
        //std::cout << ">EXP :" << input << std::endl;
        int open{0};
        int close{0};
        for (char b : input)
        {
            if (b == '[')
                open++;
            else if (b == ']')
                close++;
        }

        if (open != close)
            std::cout << "WTH EXPLODED:" << input << " open [:" << open << " close [:" << close << std::endl;
        else if (open > 4)
        {
            size_t pos = input.find(',');
            if (pos != std::string::npos)
            {
                for (size_t i = 0, open = 0; i < input.size(); ++i)
                {
                    if (input[i] == '[')
                    {
                        open++;
                        if (open == 5)
                        {
                            bool found;
                            do
                            {
                                found = true;
                                while (input[++i] == '[');
                                --i;
                                size_t candidate { i };
                                while (input[++i] != ']')
                                {
                                    //std::cout << i << ":" << input[i] << std::endl;
                                    if (input[i] == '[')
                                    {
                                        found = false;
                                        //--i;
                                        break;
                                    }
                                }
                                if (found)
                                    i = candidate;
                            }
                            while (not found);

                            //std::cout << "found: " << i << std::endl;

                            ret = true;
                            auto rpos = input.find(']', i);
                            auto expl = input.substr(i, rpos - i + 1);
                            std::cout << "EXPL:" << expl << std::endl;
                            input.erase(i, rpos - i + 1);
                            input.insert(i, "0");
                            //std::cout << "EX1: " << i << " : " << input << std::endl;
                            {
                                size_t p = expl.find(',');
                                size_t l = expl.rfind('[', p);
                                size_t r = expl.find(']', p);
                                auto nl = expl.substr(p - (p - l - 1), (p - l - 1));
                                auto nr = expl.substr(p + 1, r - p - 1);
                                //std::cout << nl << ":" << nr << std::endl;
                                std::string n;
                                for (size_t balra = i - 1; balra > 0; --balra)
                                {
                                    if (input[balra] != ']' and input[balra] != '[' and input[balra] != ',')
                                    {
                                        n += input[balra];
                                    }
                                    else if (n.length())
                                    {
                                        auto v = std::stoi(n);
                                        //std::cout << "B:" << n << " -> " << v + std::stoi(nl) << std::endl;
                                        input.erase(balra + 1, n.length());
                                        input.insert(balra + 1, std::to_string(v + std::stoi(nl)));
                                        i -= n.length();
                                        i += std::to_string(v + std::stoi(nl)).length();
                                        n.clear();
                                        break;
                                    }
                                }
                                //std::cout << "EX2: " << i << " : " << input << std::endl;
                                for (size_t jobbra = i + 1; jobbra < input.length(); ++jobbra)
                                {
                                    if (input[jobbra] != ']' and input[jobbra] != '[' and input[jobbra] != ',')
                                    {
                                        n += input[jobbra];
                                    }
                                    else if (n.length())
                                    {
                                        auto v = std::stoi(n);
                                        //std::cout << "J:" << n << " -> " << v + std::stoi(nr) << std::endl;
                                        input.erase(jobbra - n.length(), n.length());
                                        input.insert(jobbra - n.length(), std::to_string(v + std::stoi(nr)));
                                        n.clear();
                                        break;
                                    }
                                }
                                //std::cout << "EX3 :" << input << std::endl;
                            }
                            //std::cout << "EXPLODE2:" << input << std::endl;
                            reScan = true;
                            break;
                        }
                    }
                    else if (input[i] == ']')
                        open--;
                }
                //std::cout << "<EXP :" << input << std::endl;
            }
            else
                std::cout << "WTH EXPLODED:" << input << std::endl;
        }
    } while (reScan);
    return ret;
}

bool split(std::string &input)
{
    bool ret{false};
    bool reScan;
    std::string n;
    do
    {
        //std::cout << "SPLIT :" << input << std::endl;
        reScan = false;
        for (size_t i = 0; i < input.length(); ++i)
        {
            if ((input[i] != '[') and (input[i] != ',') and (input[i] != ']'))
                n += input[i];
            else if (n.length())
            {
                if (std::stoi(n) > 9)
                {
                    input.erase(i - n.length(), n.length());
                    auto v = std::stoi(n);
                    input.insert(i - n.length(), "[" + std::to_string(v >> 1) + "," + std::to_string((v + 1) >> 1) + "]");
                    reScan = true;
                    n.clear();
                    ret = true;
                    break;
                }
                n.clear();
            }
        }
        //std::cout << "SPL :" << input << std::endl;
    } while (reScan);
    return ret;
}

int main(int argc, char *argv[])
{
    std::string input = "input18s.txt";
    if (argc > 1)
        input = argv[1];

    std::string line, a;
    std::fstream file(input);

    while (std::getline(file, line))
    {
        //explode(line);
        //break;
        if (line.length())
        {
            if (a.length())
            {
                sum(a, line);
                bool doAgain;
                do
                {
                    std::cout << "--> " << a << std::endl;
                    doAgain = false;
                    while (explode(a))
                        doAgain = true;
                    while (split(a))
                        doAgain = true;
                } while (doAgain);
            }
            else
                a = line;
        }
        else
        {
            //std::cout << "rMAG: " << calculateMagnitude(a) << std::endl;
            std::cout << "---------------" << std::endl;
        }
    }
    //std::cout << "MAG: " << calculateMagnitude(a) << std::endl;
    return 0;
}