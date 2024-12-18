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

template <typename T>
vector<T> splitString(string& input, char delimiter) {
    istringstream stream(input);
    string token;
    vector<T> vec;

    while (getline(stream, token, delimiter)) {
        if constexpr (is_same_v<T, int>) {
            vec.push_back(stoi(token));
        }
        if constexpr (is_same_v<T, string>) {
            vec.push_back(token);
        };
    }
    return vec;
}

bool canMove(vector<string> &grid, int i, int j) {
    if (j < 0 || i < 0 || i >= grid.size() || j >= grid[0].size() || grid[i][j] == '#') {
        return false;
    }
    return true;
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    int part1 = 0;
    string part2;

    string s = "";

    auto gridSize = 70 + 1;

    for (int i = 0; i < gridSize; i++) {
        s.push_back('.');
    }

    vector<string> grid(gridSize, s);

    int j = 0;
    for (j = 0; j < 1023; j++) {
        auto line = results[j];
        auto r = splitString<int>(line, ',');
        grid[r[1]][r[0]] = '#';
    }

    vector<pair<int, int>> positions = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    auto iteration = 0;

    for (;j<results.size();j++) {
        auto r = splitString<int>(results[j], ',');
        grid[r[1]][r[0]] = '#';

        vector<pair<int, int>> placesToVisit;
        map<int, int> visitedPlaces;
        placesToVisit.push_back({0, 0});

        while (placesToVisit.size() != 0) {
            auto e = placesToVisit[placesToVisit.size() - 1];
            placesToVisit.pop_back();
            auto key = e.first * gridSize + e.second;

            auto cost = __INT32_MAX__ - 1;

            if (visitedPlaces.find(key) != visitedPlaces.end()) {
                cost = visitedPlaces[key];
            }

            for (auto pos : positions) {
                auto f = pos.first + e.first;
                auto s = pos.second + e.second;
                auto newKey = f * gridSize + s;

                if (canMove(grid, f, s)) {
                    auto newCost = cost + 1;
                    if (visitedPlaces.find(newKey) == visitedPlaces.end() || visitedPlaces[newKey] > newCost) {
                        if (newCost == __INT32_MAX__) newCost = 1;

                        visitedPlaces[newKey] = newCost;
                        placesToVisit.push_back({f, s});

                        if (f == gridSize - 1 && s == gridSize - 1) {
                            if (j == 1024) {
                                part1=newCost;
                            } else {
                                placesToVisit.clear();
                            }
                                printf("cost: %d\n", newCost);
                        }
                    }
                }
            }
        }
        auto fKey = (gridSize - 1 ) * gridSize + (gridSize - 1);
        if (visitedPlaces.find(fKey) == visitedPlaces.end()) {
            part2 =  to_string(r[0]) + "," +  to_string(r[1]);
            break;
        };
        visitedPlaces.clear();

    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start).count();

    cout << "Time taken: " << duration << " seconds" << endl;
}