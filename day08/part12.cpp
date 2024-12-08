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

struct Antenna {
    int x;
    int y;
};
int xsize = 0;
int ysize = 0;
bool inBounds(pair<int, int> p) {
    if (p.first < 0 || p.second < 0 || p.first >= xsize || p.second >= ysize)
        return false;
    return true;
}

vector<vector<uint64_t>> solve(vector<string> &results, map<char, vector<Antenna>> &antennamap,vector<vector<uint64_t>> &antinodeMap,  bool part2 = false) {
    auto size = results[0].size();

    int i = 1;
    for (auto antennaPair : antennamap) {
        auto vec = antennaPair.second;

        do {
            auto dx = vec[0].x - vec[1].x;
            auto dy = vec[0].y - vec[1].y;

            pair<int, int> c = {vec[0].x, vec[0].y};

            while (inBounds(c = {c.first + dx, c.second + dy})) {
                antinodeMap[c.second][c.first] = '#';
                if (!part2) break;
            }

            c = {vec[1].x, vec[1].y};

            while (inBounds(c = {c.first - dx, c.second - dy})) {
                antinodeMap[c.second][c.first] = '#';
                if (!part2) break;
            }

        } while (next_permutation(vec.begin(), vec.end(), [size](const auto &lhs, const auto &rhs) { return lhs.y * size + lhs.x < size * rhs.y + rhs.x; }));
    }
    return antinodeMap;
}

int main() {
    auto start = high_resolution_clock::now();
    vector<string> results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    map<char, vector<Antenna>> antennaMap;

    for (int i = 0; i < results.size(); i++) {
        for (int j = 0; j < results.size(); j++) {
            if (results[i][j] != '.') {
                if (antennaMap.find(results[i][j]) == antennaMap.end()) {
                    antennaMap.insert({results[i][j], vector<Antenna>()});
                }

                antennaMap[results[i][j]].push_back(
                    Antenna{
                        .x = j,
                        .y = i});
            }
        }
    }
    vector<vector<uint64_t>> antinodeMap(results.size(), vector<uint64_t>(results[0].size(), 0));

    xsize = antinodeMap[0].size();
    ysize = antinodeMap.size();
    
    auto p1map = solve(results, antennaMap, antinodeMap);
    auto p2map = solve(results, antennaMap,antinodeMap,  true);

    for (int y = 0; y < p1map.size(); y++) {
        auto line = p1map[y];

        for (int x = 0; x < line.size(); x++) {
            auto c = line[x];
            auto d = p2map[y][x];
            if (c != 0 ) {
                part1++;
            }

            if (d != 0) {
                part2 += 1;
            } else {
                if (results[y][x] != '.')
                    part2++;
            }
        }
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}