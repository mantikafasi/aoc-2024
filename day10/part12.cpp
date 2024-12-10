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

struct Node {
    int length;
    int height;
};

bool canMove(vector<vector<Node>>& positions, Node currentNode, int i, int j) {
    if (i < 0 || j < 0 || i >= positions.size() || j >= positions[0].size() || currentNode.height + 1 != positions[i][j].height) {
        return false;
    }
    return true;
}

vector<pair<int, int>> possibilties = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

int solve(vector<vector<Node>>& positions, int i, int j, Node previousNode) {
    if (positions[i][j].height == 9) return 1;

    int distinctTraits = 0;

    for (auto possibility : possibilties) {
        auto di = possibility.first;
        auto dj = possibility.second;

        if (canMove(positions, previousNode, i + di, j + dj)) {
            positions[i + di][j + dj].length += 1;
            distinctTraits += solve(positions, i + di, j + dj, positions[i + di][j + dj]);
        }
    }
    return distinctTraits;
}

int main() {

    auto start = high_resolution_clock::now();
    vector<string> results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    vector<vector<Node>> positions;

    for (auto line : results) {
        vector<Node> array;

        for (auto character : line) {
            array.push_back(Node{.length = 0,
                                 .height = character - '0'});
        }
        positions.push_back(array);
    }

    for (int i = 0; i < positions.size(); i++) {
        for (int j = 0; j < positions[0].size(); j++) {
            if (positions[i][j].height == 0) {

                // clear array so we can work on it again
                for (int i = 0; i < positions.size(); i++) {
                    for (int j = 0; j < positions[0].size(); j++) {
                        positions[i][j].length = 0;
                    }
                };

                part2 += solve(positions, i, j, positions[i][j]);
                for (auto array : positions) {
                    for (auto node : array) {
                        if (node.length != 0 && node.height == 9) {
                            part1 += 1;
                        }
                    }
                }
            }
        }
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}