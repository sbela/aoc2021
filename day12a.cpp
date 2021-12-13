#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <fstream>
#include <vector>
#include <ostream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <type_traits>
#include <limits.h>
#include <map>
#include <array>
#include <list>

using namespace std;

template <typename T>
ostream &operator<<(ostream &output, vector<T> const &values)
{
    for (auto const &value : values)
        output << value << ",";
    output << endl;
    return output;
}

template <typename T>
static const vector<T> tokenize(const string &text, char token)
{
    vector<T> data;
    stringstream check(text);
    string intermediate;
    while (getline(check, intermediate, token))
    {
        if (intermediate.length())
            data.push_back(intermediate);
    }
    return data;
}

struct Node
{
    explicit Node(const string &node, const string &to) : _node{node}, _adjacent{to} {}

    string _node;
    size_t _visited{0};
    list<string> _adjacent;

    bool operator==(const string &other) const
    {
        return other == _node;
    }

    void add(const string &to)
    {
        if (find(_adjacent.cbegin(), _adjacent.cend(), to) == _adjacent.cend())
            _adjacent.push_back(to);
    }

    bool contains(const string &to) const
    {
        return find(_adjacent.cbegin(), _adjacent.cend(), to) != _adjacent.cend();
    }

    void operator++(int)
    {
        _visited++;
    }

    void operator--(int)
    {
        _visited--;
    }

    const string &operator()() const
    {
        return _node;
    }

    const list<string> &operator*() const
    {
        return _adjacent;
    }

    explicit operator bool() const
    {
        if (((_node[0] >= 'a') and (_node[0] <= 'z')) and _visited)
            return false;
        return true;
    }
};

ostream &operator<<(ostream &output, Node const &values)
{
    output << "( " << values() << ": ";
    for (auto const &value : *values)
        output << value << " ";
    output << ")" << endl;
    return output;
}

struct Tree
{
    vector<Node> _nodes;
    vector<vector<string>> _paths;

    void traversAllAdjacentItem(Node &node, vector<string> &path, const string &end)
    {
        node++;
        path.push_back(node());

        if (node() == end)
        {
            _paths.push_back(path);
            cout << "PATH:" << path;
            path.pop_back();
            node--;
            return;
        }

        for (auto &adjacent_item : *node)
        {
            for (auto &adjacent_node : _nodes)
            {
                if (adjacent_node() == adjacent_item)
                {
                    if (adjacent_node)
                        traversAllAdjacentItem(adjacent_node, path, end);
                }
            }
        }
        path.pop_back();
        node--;
    }

    void findAllPathFrom(const string &start, const string &end)
    {
        vector<string> path;

        for (auto &node : _nodes)
        {
            if (node() == start)
                traversAllAdjacentItem(node, path, end);
        }

        cout << "SIZE: " << _paths.size() << endl;
    }

    vector<Node> &operator*()
    {
        return _nodes;
    }

    const vector<Node> &operator*() const
    {
        return _nodes;
    }

    void addNode(const vector<string> &path)
    {
        addNode(path.front(), path.back());
    }

    void addNode(const string &from, const string &to)
    {
        bool found{false};
        for (auto &node : _nodes)
        {
            if (node == from)
            {
                if (not node.contains(to))
                {
                    node.add(to);
                    addNode(to, from);
                }
                found = true;
                break;
            }
        }
        if (not found)
        {
            _nodes.push_back(Node(from, to));
            addNode(to, from);
        }
    }
};

ostream &operator<<(ostream &output, Tree const &values)
{
    for (const auto &node : *values)
        output << node;
    return output;
}

int main()
{
    fstream input;
    input.open("input12s.txt", ios::in);
    if (input.is_open())
    {
        string line;
        Tree tree;
        while (getline(input, line))
            tree.addNode(tokenize<string>(line, '-'));
        cout << "Tree:\n"
             << tree << endl;

        tree.findAllPathFrom("start", "end");
    }
    else
        cout << "input12s.txt not open!\n";
    return 0;
}
