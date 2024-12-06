#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

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

int countVisitedPositions(vector<vector<int>> grid) {
    int count = 0;
    for (auto line : grid) {
        for (int c : line) {
            count += c != 0;
        }
    }
    return count;
}

vector<vector<int>> visitedPaths;
int calculatePath(vector<string> &grid, pair<int, int> position, pair<int, int> rotation, int part = 1) {
    auto size = grid.size();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j< size; j++) {
            visitedPaths[i][j] = 0;
        }
    }
    while (true) {
        uint8_t rot = (rotation.first == -1 ? (1 << 1) : rotation.first == 1 ? (1 << 2)
                                                                             : 0) |
                      (rotation.second == -1 ? (1 << 3) : rotation.second == 1 ? (1 << 4)
                                                                               : 0);

        if (part == 1) grid[position.first][position.second] = 'X';
        auto symbol = visitedPaths[position.first][position.second];

        if (symbol != 0 && (symbol | rot) == symbol) {
            return -1;
        }

        visitedPaths[position.first][position.second] |= rot;

        if (position.first + rotation.first < 0 || position.second + rotation.second < 0 || position.first + rotation.first >= size || position.second + rotation.second >= size) {
            break;
        }

        if (grid[position.first + rotation.first][position.second + rotation.second] == '#') {
            rotation = {rotation.second, -rotation.first};
            continue;
        }
        position.first += rotation.first;
        position.second += rotation.second;
    };
    if (part == 1) {
        return countVisitedPositions(visitedPaths);
    }
    return 0;
}

int main() {
    vector<pair<int, int>> visitedPositions;

    auto start = high_resolution_clock::now();
    vector<string> originalGrid = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    pair<int, int> position;

    for (int i = 0; i < originalGrid.size(); i++) {
        if (originalGrid[i].find("^") != -1) {
            position = {i, originalGrid[i].find("^")};
            break;
        }
    }

    pair<int, int> rotation = {-1, 0};
    int visitedPosCount = 0;

    visitedPaths = vector<vector<int>>(originalGrid.size(), vector<int>(originalGrid.size(), 0));

    part1 = calculatePath(originalGrid, position, rotation);
    
    for (int i = 0; i < originalGrid.size(); i++) {
        for (int j = 0; j < originalGrid.size(); j++) {
            if (originalGrid[i][j] != 'X') {
                continue;
            }

            originalGrid[i][j] = '#';
            if (calculatePath(originalGrid, position, rotation, 2) == -1) {
                part2 += 1;
            }
            originalGrid[i][j] = '.';
        }
    }

    // for (auto line : originalGrid) {
    //     cout << line << "\n";
    // }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}