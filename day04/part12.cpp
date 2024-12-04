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

bool followWord(vector<string> results, int i, int j, int iDirection, int jDirection, string search) {
    int pos = 0;
    int k = i;
    int l = j;

    while (results[k][l] == search[pos]) {
        pos += 1;
        k += iDirection;
        l += jDirection;

        if (pos == 4) {
            return true;
        }

        if (k < 0 || l < 0 || k >= results.size() || l >= results[0].size()) {
            return false;
        }
    }
    return false;
}

bool isMas(vector<string> results, int i, int j) {
    if (i < 1 || j < 1 || i >= results.size() - 1 || j >= results[0].size() - 1) {
        return false;
    };

    if (results[i][j] == 'A' &&
        (((results[i - 1][j - 1] == 'M' && results[i - 1][j + 1] == 'S') &&
          (results[i + 1][j - 1] == 'M' && results[i + 1][j + 1] == 'S')) ||

         ((results[i - 1][j - 1] == 'M' && results[i - 1][j + 1] == 'M') &&
          (results[i + 1][j - 1] == 'S' && results[i + 1][j + 1] == 'S')) ||

         ((results[i - 1][j - 1] == 'S' && results[i - 1][j + 1] == 'S') &&
          (results[i + 1][j - 1] == 'M' && results[i + 1][j + 1] == 'M')))) return true;
    return false;
}

int main() {
    auto start = high_resolution_clock::now();

    auto results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    string search = "XMAS";
    for (int i = 0; i < results.size(); i++) {
        for (int j = 0; j < results[0].size(); j++) {
            vector<pair<int, int>> directions = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}, {1, -1}, {-1, 1}, {1,1}, {-1,-1}};
            for (const auto& [di, dj] : directions) {
                part1 += followWord(results, i, j, di, dj, "XMAS");
            }
            if (isMas(results, i, j)) {
                part2 += 1;
            }
        }
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}