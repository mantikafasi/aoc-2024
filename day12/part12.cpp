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

bool canMove(vector<string>& positions, int i, int j) {
    if (i < 0 || j < 0 || i >= positions.size() || j >= positions[0].size()) {
        return false;
    }
    return true;
}

vector<pair<int, int>> floodFill(vector<string>& grid, char character, int i, int j) {
    vector<pair<int, int>> elements;
    vector<pair<int, int>> queue;

    queue.push_back({i, j});
    elements.push_back({i, j});
    grid[i][j] = '.';

    while (!queue.empty()) {
        auto elem = queue[queue.size() - 1];
        queue.pop_back();

        vector<pair<int, int>> positions = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

        for (auto pos : positions) {
            pair<int, int> newElem = {(elem.first + pos.first), (elem.second + pos.second)};

            if (canMove(grid, newElem.first, newElem.second) && grid[newElem.first][newElem.second] == character) {
                queue.push_back({elem.first + pos.first, elem.second + pos.second});
                elements.push_back({elem.first + pos.first, elem.second + pos.second});
                grid[newElem.first][newElem.second] = '.';
            }
        }
    }

    return elements;
}

bool includes(vector<pair<int, int>>& elements, pair<int, int> element) {
    return find_if(elements.begin(), elements.end(), [element](const pair<int, int>& p) {
               return p.first == (element.first) && p.second == (element.second);
           }) != elements.end();
}


int countSides(vector<pair<int, int>> elements, pair<int, int> element, bool reverse = false) {
    int sides = 4;
    vector<pair<int, int>> positions = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

    for (auto pos : positions) {
        if (includes(elements, {element.first + pos.first, element.second + pos.second})) {
            sides--;
        }
    }

    if (reverse) sides = 4 - sides;

    return sides;
}

int countCorners(vector<pair<int, int>>& elements, pair<int, int> element) {
    int cornerCount = 0;

    for (auto i : {1, -1}) {
        for (auto j : {1, -1}) {
            if (!includes(elements, {element.first + i, element.second}) && !includes(elements, {element.first, element.second + j})) {
                cornerCount++;
            }
            if (includes(elements, {element.first + i, element.second}) && includes(elements, {element.first, element.second + j}) && !includes(elements, {element.first + i, element.second + j})) {
                cornerCount++;
            }
        }
    }

    return cornerCount;
}

uint64_t calculatePrice(vector<pair<int, int>> elements) {
    int area = elements.size();

    int perimeter = 0;

    for (auto e : elements) {
        perimeter += countSides(elements, e);
    }

    return area * perimeter;
}

void prettyPrint(vector<string> grid) {
    for (int i = 0; i < grid.size(); ++i) {
        cout << "\033[F";
    }

    for (const auto& line : grid) {
        cout << line << endl;
    }
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> grid = readContents("input.txt");
    uint64_t part1 = 0;
    int part2 = 0;

    map<int, vector<pair<int, int>>> regions;

    int i = 0;
    int j = 0;
    for (auto line : grid) {
        j = 0;
        for (auto character : line) {
            character = grid[i][j];
            if (character == '.') {
                j++;
                continue;
            }
            // prettyPrint(grid);
            auto arr = floodFill(grid, character, i, j);
            regions[i * grid.size() + j] = arr;

            j++;
        }
        i++;
    }

    for (auto pair : regions) {
        auto cornerCount = 0;

        part1 += calculatePrice(pair.second);
        for (auto s : pair.second) {
            cornerCount += countCorners(pair.second, s);
        }

        part2 += cornerCount * pair.second.size();
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}