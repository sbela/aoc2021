#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>
#include <bitset>

static constexpr int literalValue{4};

struct Packet
{
    int version;
    int typeID;
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

    int bits(size_t n)
    {
        if (message.size() < n)
            std::cout << "Not enough data! " << message.size() << " < " << n << std::endl;
        int bits{0};
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
};

std::ostream &operator<<(std::ostream &os, const End &e)
{
    os << " (" << e.number_of_packets << ", " << e.total_length_in_bits << ", " << e.num_of_pkts << " )";
    return os;
}

void process(Queue &q, int &version_sum, End end)
{
    int total_length_in_bits{0};
    while (true)
    {
        //std::cout << "\n\tRemains: " << q.message.size() << end << " :" << total_length_in_bits << std::endl;

        if (end.num_of_pkts and not end.number_of_packets--)
            return;
        else if (not end.num_of_pkts and (end.total_length_in_bits <= total_length_in_bits))
            return;

        Packet p;

        p.version = q.bits(3); // version_len = 3
        version_sum += p.version;
        p.typeID = q.bits(3);  // typeID_len = 3

        total_length_in_bits += 6;

        //std::cout << "Ver: " << p.version << " SUM:" << version_sum << std::endl;
        //std::cout << "TypeID: " << p.typeID << std::endl;

        if (p.typeID == literalValue)
        {
            int64_t message{0};
            //std::cout << "Message size: " << q.message.size() << std::endl;
            while (q.message.size() > 4)
            {
                int value = q.bits(5);
                total_length_in_bits += 5;
                message <<= 4;
                message |= (value & 0b01111);
                //std::cout << "Message: " << message << std::endl;
                if (not(value & 0b10000))
                    break;
            }
            //std::cout << "All Message: " << message << std::endl;
        }
        else
        {
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
                process(s, version_sum, { numberOfPackets, 0, true });
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
                process(s, version_sum, { 0, _total_length_in_bits, false } );
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
        process(q, version_sum, { 1, 0, true });
        std::cout << "Version sum: " << version_sum << std::endl;
    }
}