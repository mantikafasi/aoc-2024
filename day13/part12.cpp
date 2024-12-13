#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>

using namespace std;
using namespace std::chrono;

struct Coord {
    long x = 0;
    long y = 0;
};

struct Machine {
    Coord ButtonA;
    Coord ButtonB;
    Coord result;
};

uint64_t solve(Machine machine) {
    uint64_t res = 0;

    Coord sum = {machine.ButtonA.x + machine.ButtonB.y, machine.ButtonA.y + machine.ButtonB.x};

    long matrix[2][2] = {{machine.ButtonA.x, machine.ButtonB.x}, {machine.ButtonA.y, machine.ButtonB.y}};

    auto det = (matrix[0][0] * matrix[1][1]) - (matrix[0][1] * matrix[1][0]);

    auto x = (machine.result.x * matrix[1][1]) - (machine.result.y * matrix[0][1]);

    auto y = (matrix[0][0] * machine.result.y) - (matrix[1][0] * machine.result.x);

    x /= det;
    y /= det;

    if (machine.result.x == x * machine.ButtonA.x + y * machine.ButtonB.x && machine.result.y == x * machine.ButtonA.y + y * machine.ButtonB.y) {
        res += 3 * x + y;
    }

    return res;
}

int main() {
    auto start = high_resolution_clock::now();

    int part1 = 0;
    uint64_t part2 = 0;

    ifstream file("input.txt");
    string line;

    vector<Machine> machines;

    Machine machine;

    regex const r{
        R"(Button (.): X\+(\d+), Y\+(\d+))",
    };

    regex const r2{
        R"(Prize: X=(\d+), Y=(\d+))",
    };

    while (getline(file, line)) {
        if (line == "") {
            machines.push_back(machine);
            machine = Machine{};
        }
        smatch sm;

        auto f = regex_search(line, sm, r);
        if (f) {
            if (sm[1] == 'A') {
                machine.ButtonA = {
                    .x = stoi(sm[2].str()),
                    .y = stoi(sm[3].str())};
            } else {
                machine.ButtonB = {
                    .x = stoi(sm[2].str()),
                    .y = stoi(sm[3].str())};
            }
        }

        f = regex_search(line, sm, r2);

        if (f) {
            machine.result = {
                .x = stoi(sm[1].str()),
                .y = stoi(sm[2].str())};
        };
    }
    machines.push_back(machine);

    vector<thread> threads;

    for (auto machine : machines) {
        part1 += solve(machine);
        machine.result.x += 10000000000000;
        machine.result.y += 10000000000000;
        part2 += solve(machine);
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}