#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
#include <iomanip>
#include <bitset>

// Sample: target area: x=20..30, y=-10..-5
// Part1 : target area: x=185..221, y=-122..-74

struct Point
{
    int x, y;
    friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

std::ostream &operator<<(std::ostream &os, const Point &p)
{
    os << " (" << p.x << ", " << p.y << " )";
    return os;
}

struct TargetArea
{
    Point TopLeft, BottomRight;
};

struct Probe
{
    Probe(Point pos, Point vel) : position(pos), velocity(vel) {}
    Point position;
    Point velocity;
};

void takeOneStep(Probe &probe)
{
    probe.position.x += probe.velocity.x;
    probe.position.y += probe.velocity.y;
    if (probe.velocity.x > 0)
        probe.velocity.x--;
    else if (probe.velocity.x < 0)
        probe.velocity.x++;
    probe.velocity.y--;
}

bool isInTarget(const Probe &probe, const TargetArea &target)
{
    if ((probe.position.x >= target.TopLeft.x) and
        (probe.position.y <= target.TopLeft.y) and
        (probe.position.x <= target.BottomRight.x) and
        (probe.position.y >= target.BottomRight.y))
        return true;
    return false;
}

bool isOverTarget(const Probe &probe, const TargetArea &target)
{
    if ((probe.position.x > target.BottomRight.x) ||
        (probe.position.y < target.BottomRight.y))
        return true;
    return false;
}

int main()
{
    //TargetArea sample_target{{20, -5}, {30, -10}};
    TargetArea sample_target{{185, -74}, {221, -122}};

    int allTimeHighestY { 0 };
    for (int vx = 1; vx <= sample_target.BottomRight.x; ++vx)
    {
        for (int vy = 1; vy <= sample_target.BottomRight.x; ++vy)
        {
            int highestY { 0 };
            Probe probe = Probe({0, 0}, {vx, vy});
            //std::cout << "Starting Pos:" << probe.position << " Vel:" << probe.velocity << std::endl;
            auto max_step = std::max(vy - sample_target.BottomRight.y, sample_target.BottomRight.x);
            for (int step = 0; step <= max_step; ++step)
            {
                takeOneStep(probe);
                //std::cout << "Pos:" << probe.position << " Vel:" << probe.velocity << std::endl;
                if (probe.position.y > highestY)
                    highestY = probe.position.y;
                if (isInTarget(probe, sample_target))
                {
                    std::cout << "Initial velocity @" << step <<" :" << Point{vx, vy} << " y:" << highestY << std::endl;
                    if (highestY > allTimeHighestY)
                        allTimeHighestY = highestY;
                    else if (highestY < allTimeHighestY)
                        goto done;
                    break;
                }
                if (isOverTarget(probe, sample_target))
                    break;
            }
        }
    }
done:    
    std::cout << "All Time high: " << allTimeHighestY << std::endl;
    return 0;
}