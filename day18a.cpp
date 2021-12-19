#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <utility>
#include <variant>

struct SnailNumber
{
    int value;
    int level;
};

void parse(const std::string &line, std::vector<SnailNumber> &v)
{
    v.clear();
    int level{0};
    for (auto c : line)
    {
        switch (c)
        {
        case '[':
            ++level;
            break;
        case ',':
            break;
        case ']':
            --level;
            break;
        default:
            v.push_back({c - '0', level});
            break;
        }
    }
}

void add(std::vector<SnailNumber> &a, std::vector<SnailNumber> &b)
{
    a.insert(a.end(), b.begin(), b.end());
    for (auto &i : a)
        i.level++;
}

std::ostream &operator<<(std::ostream &output, SnailNumber const &values)
{
    output << "(" << values.level << ":" << values.value << ")";
    return output;
}

std::ostream &operator<<(std::ostream &output, std::vector<SnailNumber> const &values)
{
    auto current = values.begin();
    auto p = [&](int depth, auto &&rec) -> void
    {
        auto read_val = [&]()
        {
            if (current->level == depth)
                output << (current++)->value;
            else
                rec(depth + 1, rec);
        };
        output << "["; //{" << depth << '}';
        read_val();
        output << ',';
        read_val();
        output << /*'{' << depth << "}*/ "]";
    };
    p(1, p);
    return output;
}

bool explode(std::vector<SnailNumber> &a)
{
    bool ret{false};
    int maxDepth;
    do
    {
        maxDepth = 0;
        for (const auto &i : a)
            if (i.level > maxDepth)
                maxDepth = i.level;
        auto num = a.begin();
        while (num != a.end())
        {
            if ((maxDepth > 4) and ((*num).level == maxDepth))
            {
                //std::cout << "L1:" << *num << " L2:" << *(num + 1) << " L3:" << ((num + 2) != a.end() ? *(num + 2) : SnailNumber{-1,-1}) << std::endl;
                if (num != a.begin())
                    (*(num - 1)).value += (*num).value;
                if ((num + 2) != a.end())
                    (*(num + 2)).value += (*(num + 1)).value;
                num = a.erase(num);
                (*num).level--;
                (*num).value = 0;
                ret = true;
                //std::cout << "exp: " << a << std::endl;
            }
            else
                num++;
        }
    } while (maxDepth > 4);
    return ret;
}

bool split(std::vector<SnailNumber> &a)
{
    bool ret{false};
    bool again;
    do
    {
        again = false;
        auto num = a.begin();
        while (num != a.end())
        {
            if ((*num).value > 9)
            {
                int v = (*num).value;
                (*num).value = v >> 1;
                (*num).level++;
                a.insert(num + 1, {(v + 1) >> 1, (*num).level});
                again = ret = true;
                //std::cout << "spl: " << a << std::endl;
                return true;
            }
            num++;
        };
    } while (again);
    return ret;
}

int calculateMagnitude(std::string input)
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

int main(int argc, char *argv[])
{
    std::string input = "input18s.txt";
    if (argc > 1)
        input = argv[1];

    std::string line;
    std::fstream file(input);
    std::vector<SnailNumber> a, b;

    while (std::getline(file, line))
    {
        if (line.length())
        {
            if (a.size())
            {
                parse(line, b);
                //std::cout << "PAR: " << b << std::endl;
                add(a, b);
                //std::cout << "SUM: " << a << std::endl;
                bool ret;
                do
                {
                    ret = false;
                    if (explode(a))
                    {
                        //std::cout << "EXP: " << a << std::endl;
                        ret = true;
                    }
                    else
                    {
                        if (split(a))
                            ret = true;
                        //std::cout << "SPL: " << a << std::endl;
                    }
                } while (ret);
            }
            else
            {
                parse(line, a);
                //std::cout << "PAR: " << a << std::endl;
            }
        }
    }
    std::cout << "RES: " << a << std::endl;
    std::stringstream ss;
    ss << a;
    std::cout << "MAG: " << calculateMagnitude(ss.str()) << std::endl;

    return 0;
}