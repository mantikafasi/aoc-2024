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

bool isInBounds(vector<string> &grid, int i, int j) {
    if (i < 1 || j < 1 || i >= grid.size() - 1 || j >= grid[0].size() - 1 || grid[i][j] == '#') {
        return false;
    }
    return true;
}

struct Box {
    int y;
    int x;
    char side;
};

pair<int, int> move(vector<string> &grid, char moveChar, pair<int, int> rotation, int i, int j) {
    pair<int, int> nextPos = {i + rotation.first, j + rotation.second};

    if (grid[nextPos.first][nextPos.second] == '.') return nextPos;

    bool canMove = true;

    vector<Box> boxesToLook;
    vector<Box> boxesToUpdate;
    boxesToLook.push_back({i, j, grid[i][j]});
    grid[i][j] = '.';

    while (boxesToLook.size() != 0) {
        auto box = boxesToLook[boxesToLook.size() - 1];
        boxesToUpdate.push_back(box);
        boxesToLook.pop_back();
        grid[box.y][box.x] = '.';

        pair<int, int> nextPos = {box.y + rotation.first, box.x + rotation.second};

        auto nextChar = grid[nextPos.first][nextPos.second];

        if (nextChar == '[') {
            boxesToLook.push_back({nextPos.first, nextPos.second + 1, ']'});
        } else if (nextChar == ']') {
            boxesToLook.push_back({nextPos.first, nextPos.second - 1, '['});
        } else if (nextChar == '#') {
            canMove = false;
            break;
        } else if (nextChar != 'O') {
            continue;
        };

        boxesToLook.push_back({nextPos.first, nextPos.second, nextChar});
        grid[nextPos.first][nextPos.second] = '.';
    };

    for (auto b : boxesToUpdate) {
        if (canMove) {
            grid[b.y + rotation.first][b.x + rotation.second] = b.side;
        } else {
            grid[b.y][b.x] = b.side;
        }
    }

    if (!canMove) {
        return {i, j};
    }

    grid[nextPos.first][nextPos.second] = grid[i][j];
    grid[i][j] = '.';

    return nextPos;
}

pair<int, int> findInGrid(vector<string> &grid, char c) {
    for (int i = 0; i < grid.size(); i++) {
        for (int j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == c) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

void print(vector<string> &grid) {
    // Move cursor to home position
    cout << "\033[H";

    for (int i = 0; i < grid.size(); i++) {
        // Move cursor to start of each row
        cout << "\033[" << i + 1 << ";1H";
        cout << grid[i];
    }
    cout << "\n";

    cout.flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    auto e = find(results.begin(), results.end(), "");

    vector<string> grid(results.begin(), e);
    vector<string> moves(e, results.end());

    vector<string> part2input;

    for (auto line : grid) {
        string newLine = "";

        for (auto c : line) {
            if (c == 'O')
                newLine += "[]";
            else {
                if (c != '@')
                    newLine = newLine + c + c;
                else
                    newLine += "@.";
            }
        }
        part2input.push_back(newLine);
    }

    pair<int, int> part1pos = findInGrid(grid, '@');
    pair<int, int> part2pos = findInGrid(part2input, '@');
    
    //print(part2input);

    for (auto line : moves) {
        for (auto c : line) {
            pair<int, int> rotation = {0, 0};

            switch (c) {
                case '>':
                    rotation = {0, 1};
                    break;
                case '<':
                    rotation = {0, -1};
                    break;
                case '^':
                    rotation = {-1, 0};
                    break;
                case 'v':
                    rotation = {1, 0};
                    break;
            }
            part1pos = move(grid, c, rotation, part1pos.first, part1pos.second);

            part2pos = move(part2input, c, rotation, part2pos.first, part2pos.second);
            // print(part2input);
        }
    }

    int i = 0;
    for (auto line : part2input) {
        int j = 0;

        for (auto c : line) {
            if (c == '[') {
                part2 += 100 * i + j;
            }
            j++;
        }
        i++;
    }

    i = 0;
    for (auto line : grid) {
        int j = 0;

        for (auto c : line) {
            if (c == 'O') {
                part1 += 100 * i + j;
            }
            j++;
        }
        i++;
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}