#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
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

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    regex const r{
        R"(mul\(([0-9]{1,3}),([0-9]{1,3})\))",
    };

    regex const g{
        R"(don't\(\)|do\(\))"};

    bool good = true;

    smatch sm;

    for (auto line : results) {
        while (regex_search(line, sm, r)) {
            smatch sg;
            auto str = sm.prefix().str();

            if (regex_search(str, sg, g)) {
                if (sg[0] == "don't()") {
                    good = false;
                } else {
                    good = true;
                }
            }

            part1 += stoi(sm[1]) * stoi(sm[2]);

            if (good) {
                part2 += stoi(sm[1]) * stoi(sm[2]);
            }
            line = sm.suffix().str();
        }
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}