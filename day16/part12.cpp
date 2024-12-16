#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>
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

bool canMove(vector<string>& positions, int i, int j) {
    if (i < 0 || j < 0 || i >= positions.size() || j >= positions[0].size() || positions[i][j] == '#') {
        return false;
    }
    return true;
}

pair<int, int> findInGrid(vector<string>& grid, char c) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == c) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

void prettyprint(vector<string> grid) {
    cout << "\033[H";

    for (int i = 0; i < grid.size(); i++) {
        // Move cursor to start of each row
        cout << "\033[" << i + 1 << ";1H";
        cout << grid[i];
    }
    cout << "\n";

    cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

struct Node {
    uint8_t y;
    uint8_t x;
    bool direction;
};

int main() {
    auto start = high_resolution_clock::now();

    vector<string> grid = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    map<int, int> visitedPlaces;

    auto startPosition = findInGrid(grid, 'S');
    auto endPosition = findInGrid(grid, 'E');

    vector<Node> placesToVisit;

    map<int, vector<vector<pair<int, int>>>> visitedPathOperations;

    auto size = grid.size();

    auto resultGrid = grid;

    int lowest = INT32_MAX;

    placesToVisit.push_back({(uint8_t)startPosition.first, (uint8_t)startPosition.second, false});
    visitedPlaces[startPosition.first * size + startPosition.second] = 0;

    visitedPathOperations[startPosition.first * size + startPosition.second] = vector<vector<pair<int, int>>>();
    visitedPathOperations[startPosition.first * size + startPosition.second].push_back(vector<pair<int, int>>());

    while (placesToVisit.size() != 0) {
        auto node = placesToVisit[placesToVisit.size() - 1];
        placesToVisit.pop_back();

        auto key = node.y * size + node.x | (node.direction << 30);

        vector<pair<int, int>> positions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

        for (auto pos : positions) {
            uint8_t f = node.y + pos.first;
            uint8_t s = node.x + pos.second;
            auto newDir = pos.first != 0;

            auto newKey = f * size + s | (newDir << 30);

            pair<int, int> nextPos = {f + pos.first, s + pos.second};

            if (canMove(grid, f, s)) {
                auto points = 1;

                if (newDir != node.direction) {
                    points += 1000;
                }

                if (visitedPlaces.find(newKey) == visitedPlaces.end() || visitedPlaces[newKey] >= visitedPlaces[key] + points) {
                    if (visitedPlaces[newKey] == visitedPlaces[key] + points) {
                        auto temp(visitedPathOperations[key][visitedPathOperations[key].size() - 1]);
                        temp.push_back(pos);

                        visitedPathOperations[newKey].push_back(temp);
                    } else {
                        visitedPathOperations[newKey].clear();
                        visitedPathOperations[newKey] = visitedPathOperations[key];

                        for (auto& l : visitedPathOperations[newKey]) {
                            l.push_back(pos);
                        }
                        visitedPlaces[newKey] = visitedPlaces[key] + points;
                    };

                    if (f == endPosition.first && s == endPosition.second) {
                        if (lowest > visitedPlaces[newKey]) {
                            lowest = visitedPlaces[newKey];

                            for (auto &l : resultGrid) {
                                for (auto &c: l) {
                                    if (c == '@')
                                        c = '.';
                                }
                            }

                        } 
                        if (lowest >= visitedPlaces[newKey]) {
                            pair<int, int> resPos = startPosition;

                            for (auto v : visitedPathOperations[newKey][visitedPathOperations[newKey].size() - 1]) {
                                resPos.first += v.first;
                                resPos.second += v.second;
                                resultGrid[resPos.first][resPos.second] = '@';
                            }
                        }
                        //prettyprint(resultGrid);
                        break;
                    };

                    placesToVisit.push_back({f, s, newDir});
                }
            }
        }
    }


    for (auto &line : resultGrid) {
        cout << line << "\n";

        for (auto &c : line) {
            if (c == '@')
                part2++;
        }
    }
    // add start position
    part2++;
    part1=lowest;

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start).count();

    cout << "Time taken: " << duration << " seconds" << endl;
}