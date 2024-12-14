#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <vector>
#include <thread>

using namespace std;
using namespace std::chrono;

vector<string> readContents(string filename) {
    ifstream file(filename);

    vector<string> contents;

    string line;
    while (getline(file, line)) {
        contents.push_back(line);
    }
    return contents;
}

struct Coord {
    int x;
    int y;
};

struct Robot {
    Coord position;
    Coord velocity;
};

void move(vector<vector<int>> &space, Robot &robot) {
    robot.position.x += robot.velocity.x;
    robot.position.y += robot.velocity.y;
    if (robot.position.y > 0 && robot.position.y >= space.size()) {
        robot.position.y -= space.size();
    };

    if (robot.position.y < 0) {
        robot.position.y += space.size();
    }

    if (robot.position.x > 0 && robot.position.x >= space[0].size()) {
        robot.position.x -= space[0].size();
    }

    if (robot.position.x < 0) {
        robot.position.x += space[0].size();
    }
}

int robotCountInPosition(vector<vector<int>> &space, vector<Robot> robots, int x, int y) {
    int count = 0;

    for (auto robot : robots) {
        if (robot.position.x == x && robot.position.y == y) {
            count += 1;
        }
    }

    return count;
}

void prettyPrint(vector<vector<int>> &space, vector<Robot> &robots) {
    // Move cursor to home position
    cout << "\033[H";
    
    for (int i = 0; i < space.size(); i++) {
        // Move cursor to start of each row
        cout << "\033[" << i + 1 << ";1H";
        for (int j = 0; j < space[0].size(); j++) {
            auto number = robotCountInPosition(space, robots, j, i);
            if (number == 0) {
                cout << '.';
            } else {
                cout << number;
            }
        }
    }

    cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

}

// Add this at program start to clear screen once:
// cout << "\033[2J";

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    int part1 = 1;
    int part2 = 0;

    regex const r{
        R"(p=(-?\d+),(-?\d+) v=(-?\d+),(-?\d+))",
    };

    vector<Robot> robots;

    for (auto line : results) {
        smatch sm;
        regex_search(line, sm, r);

        robots.push_back(Robot{
            .position = {
                .x = stoi(sm[1]),
                .y = stoi(sm[2])},
            .velocity = {.x = stoi(sm[3]), .y = stoi(sm[4])},
        });
    }

    vector<vector<int>> space(103, vector<int>(101));

    // change to 100 for part1
    for (int i = 0; i < 100000; i++) {
        for (auto &robot : robots) {
            move(space, robot);
        }

        if (i > 5000) {

            cout << i + 1 << "\n";
            prettyPrint(space, robots);
        }

    }

    int midX = space[0].size() / 2 + 0.5;
    int midY = space.size() / 2 + 0.5;

    vector<vector<int>> quadrant = {{0, 0}, {0, 0}};

    for (int i = 0; i < space.size(); i++) {
        for (int j = 0; j < space[0].size(); j++) {
            auto number = robotCountInPosition(space, robots, j, i);
            int qi = 0;
            int qy = 0;

            if (i > midY) qi = 1;
            if (j > midX) qy = 1;

            if (number != 0 && i != midY && j != midX) {
                quadrant[qi][qy] += number;
            }
        }
    }

    part1 = quadrant[0][0] * quadrant[0][1] * quadrant[1][0] * quadrant[1][1];

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}