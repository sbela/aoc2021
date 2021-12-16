#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>
#include <bitset>

enum class PacketTypes
{
    sum,
    product,
    minimum,
    maximum,
    literal_value,
    greater_than,
    less_than,
    equal_to
};

struct Packet
{
    int version;
    PacketTypes typeID;
    std::vector<int> data;
};

struct Queue
{
    Queue() {}
    Queue(const std::string &q)
    {
        parse(q);
    }

    Queue(std::deque<std::string> &q) : message(q) {}

    std::deque<std::string> message;
    void parse(const std::string &line)
    {
        auto intval = [](char hexchar)
        {
            return (hexchar >= 'A') ? (hexchar - 'A' + 10) : (hexchar - '0');
        };
        for (char c : line)
        {
            std::bitset<4> b(intval(c));
            for (auto bit : b.to_string())
                message.emplace_back(std::string{bit});
            //std::cout << b.to_string();
        }
    }

    uint64_t bits(size_t n)
    {
        if (message.size() < n)
            std::cout << "Not enough data! " << message.size() << " < " << n << std::endl;
        uint64_t bits{0};
        for (int i = (n - 1); (i >= 0) and message.size(); --i)
        {
            bits |= (message.front()[0] - '0') << i;
            message.pop_front();
        }
        return bits;
    }

    void copy(Queue &packet)
    {
        if (message.size())
            std::cout << "Not empty queue: " << message.size() << std::endl;
        while (packet.message.size())
        {
            message.push_back(packet.message.front());
            packet.message.pop_front();
        }
    }

    void sub_packet(Queue &packet, int n)
    {
        while (n--)
        {
            packet.message.push_back(message.front());
            message.pop_front();
        }
    }
};

struct End
{
    int number_of_packets;
    int total_length_in_bits;
    bool num_of_pkts;
    PacketTypes pType;
};

std::ostream &operator<<(std::ostream &os, const PacketTypes &p)
{
    std::string op{"invalid"};
    switch (p)
    {
    case PacketTypes::sum:
        op = "sum";
        break;
    case PacketTypes::product:
        op = "product";
        break;
    case PacketTypes::minimum:
        op = "minimum";
        break;
    case PacketTypes::maximum:
        op = "maximum";
        break;
    case PacketTypes::literal_value:
        op = "literal_value";
        break;
    case PacketTypes::less_than:
        op = "less_than";
        break;
    case PacketTypes::greater_than:
        op = "greater_than";
        break;
    case PacketTypes::equal_to:
        op = "equal_to";
        break;
    }
    os << op;
    return os;
}

std::ostream &operator<<(std::ostream &os, const End &e)
{
    os << " (" << e.number_of_packets << ", " << e.total_length_in_bits << ", " << e.num_of_pkts << " )";
    return os;
}

struct Node
{
    std::string value;
    PacketTypes type;
    bool is_value; // value or operand
};

std::deque<Node> &operator<<(std::deque<Node> &os, PacketTypes p)
{
    std::string op{"invalid"};
    switch (p)
    {
    case PacketTypes::sum:
        op = "+";
        break;
    case PacketTypes::product:
        op = "*";
        break;
    case PacketTypes::minimum:
        op = "min";
        break;
    case PacketTypes::maximum:
        op = "max";
        break;
    case PacketTypes::literal_value:
        op = "val";
        break;
    case PacketTypes::less_than:
        op = "<";
        break;
    case PacketTypes::greater_than:
        op = ">";
        break;
    case PacketTypes::equal_to:
        op = "==";
        break;
    }
    os.push_back({op, p, p == PacketTypes::literal_value});
    return os;
}

std::deque<Node> &operator<<(std::deque<Node> &os, uint64_t v)
{
    os.push_back({std::to_string(v), PacketTypes::literal_value, true});
    return os;
}

uint64_t calcRPN(std::deque<Node> &rpn)
{
    if (rpn.size() == 0)
    {
        std::cout << "Invalid deque!" << std::endl;
        return 0;
    }

    uint64_t ret{0};

    auto node = rpn.front();
    rpn.pop_front();

    if (node.is_value)
    {
        //std::cout << "[val]:" << std::stoull(node.value) << std::endl;
        return std::stoull(node.value);
    }
    else
    {
        if (node.type == PacketTypes::equal_to)
        {
            auto op1 = calcRPN(rpn);
            //std::cout << "op1=" << op1 << std::endl;
            auto op2 = calcRPN(rpn);
            //std::cout << "op2=" << op2 << std::endl;
            ret = op1 == op2;
            //std::cout << op1 << " [==] " << op2 << " -> " << ret << std::endl;
        }
        else if (node.type == PacketTypes::less_than)
        {
            auto op1 = calcRPN(rpn);
            //std::cout << "op1=" << op1 << std::endl;
            auto op2 = calcRPN(rpn);
            //std::cout << "op2=" << op2 << std::endl;
            ret = op1 < op2;
            //std::cout << op1 << " [<] " << op2 << " -> " << ret << std::endl;
        }
        else if (node.type == PacketTypes::greater_than)
        {
            auto op1 = calcRPN(rpn);
            //std::cout << "op1=" << op1 << std::endl;
            auto op2 = calcRPN(rpn);
            //std::cout << "op2=" << op2 << std::endl;
            ret = op1 > op2;
            //std::cout << op1 << " [>] " << op2 << " -> " << ret << std::endl;
        }
        else
        {
            std::deque<uint64_t> ops;
            while (rpn.size())
            {
                auto res = calcRPN(rpn);
                //std::cout << "partial: " << node.value << " res: " << res << std::endl;
                ops.push_back(res);
            }

            if (node.type == PacketTypes::sum)
            {
                while (ops.size())
                {
                    ret += ops.front();
                    ops.pop_front();
                }
                //std::cout << "[+]:" << ret << std::endl;
            }
            else if (node.type == PacketTypes::product)
            {
                ret = 1;
                while (ops.size())
                {
                    ret *= ops.front();
                    ops.pop_front();
                }
                //std::cout << "[*]:" << ret << std::endl;
            }
            else if (node.type == PacketTypes::minimum)
            {
                ret = UINT64_MAX;
                while (ops.size())
                {
                    if (ops.front() < ret)
                        ret = ops.front();
                    ops.pop_front();
                }
                //std::cout << "[m]:" << ret << std::endl;
            }
            else if (node.type == PacketTypes::maximum)
            {
                ret = 0;
                while (ops.size())
                {
                    if (ops.front() > ret)
                        ret = ops.front();
                    ops.pop_front();
                }
                //std::cout << "[M]:" << ret << std::endl;
            }
            else
                std::cout << "WTF!" << std::endl;
        }
    }
    //std::cout << ret << std::endl;
    return ret;
}

uint64_t process(Queue &q, int &version_sum, End end, std::deque<Node> &rpn)
{
    int total_length_in_bits{0};
    uint64_t ret{0};
    while (true)
    {
        //std::cout << "\n\tRemains: " << q.message.size() << end << " :" << total_length_in_bits << std::endl;
        if (end.num_of_pkts and not end.number_of_packets--)
        {
            ret = calcRPN(rpn);
            //std::cout << "RET1: " << ret << " #" << rpn.size() << std::endl;
            return ret;
        }
        else if (not end.num_of_pkts and (end.total_length_in_bits <= total_length_in_bits))
        {
            ret = calcRPN(rpn);
            //std::cout << "RET2: " << ret << std::endl;
            return ret;
        }

        Packet p;

        p.version = q.bits(3); // version_len = 3
        version_sum += p.version;
        p.typeID = static_cast<PacketTypes>(q.bits(3)); // typeID_len = 3

        total_length_in_bits += 6;

        //std::cout << "Ver: " << p.version << " SUM:" << version_sum << std::endl;
        //std::cout << "TypeID: " << p.typeID << std::endl;

        if (p.typeID == PacketTypes::literal_value)
        {
            uint64_t message{0};
            //std::cout << "Message size: " << q.message.size() << std::endl;
            while (q.message.size() > 4)
            {
                uint64_t value = q.bits(5);
                total_length_in_bits += 5;
                message <<= 4;
                message |= (value & 0b01111);
                //std::cout << "Message: " << message << std::endl;
                if (not(value & 0b10000))
                    break;
            }
            rpn << message;
            //std::cout << "All Message: " << message << std::endl;
        }
        else
        {
            //rpn << p.typeID;
            //std::cout << "Operation: " << p.typeID << std::endl;
            int I = q.bits(1);
            total_length_in_bits++;
            if (I)
            {
                int numberOfPackets = q.bits(11);
                total_length_in_bits += 11;
                //std::cout << "Number of Packets: " << numberOfPackets << std::endl;
                //std::cout << "Processing sub: " << numberOfPackets << " #:" << q.message.size() << std::endl;
                Queue s;
                total_length_in_bits += q.message.size();
                q.sub_packet(s, q.message.size());
                std::deque<Node> _rpn;
                _rpn << p.typeID;
                rpn << process(s, version_sum, {numberOfPackets, 0, true, p.typeID}, _rpn);
                q.copy(s);
                total_length_in_bits -= q.message.size();
                //std::cout << "Processing sub: " << numberOfPackets << " done!" << std::endl;
            }
            else
            {
                int _total_length_in_bits = q.bits(15);
                total_length_in_bits += 15;
                //std::cout << "total_length_in_bits: " << _total_length_in_bits << " #" << q.message.size() << std::endl;
                Queue s;
                total_length_in_bits += _total_length_in_bits;
                q.sub_packet(s, _total_length_in_bits);
                std::deque<Node> _rpn;
                _rpn << p.typeID;
                rpn << process(s, version_sum, {0, _total_length_in_bits, false, p.typeID}, _rpn);
                if (s.message.size())
                {
                    std::cout << "s not processed fully! " << s.message.size() << std::endl;
                    total_length_in_bits -= s.message.size();
                    q.copy(s);
                }
                //std::cout << "total_length_in_bits: " << _total_length_in_bits << " done!" << std::endl;
            }
        }
    }
    return ret;
}

int main(int argc, char *argv[])
{
    std::string input = "input16.txt";
    if (argc > 1)
        input = argv[1];

    std::string line;
    std::fstream file(input);

    while (std::getline(file, line))
    {
        std::cout << "\nProcess: " << line << std::endl;
        Queue q{std::move(line)};
        int version_sum{0};
        std::deque<Node> rpn;
        uint64_t ret = process(q, version_sum, {1, 0, true, PacketTypes::literal_value}, rpn);
        std::cout << "\nVersion sum: " << version_sum << " ret: " << ret << std::endl;
    }
}
