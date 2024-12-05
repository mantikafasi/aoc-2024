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

bool isCorrectlyOrdered(vector<int> &updates, vector<pair<int, int>> rules) {
    for (int i = 0; i < updates.size() - 1; i++) {
        int first = updates[i];
        int second = updates[i + 1];

        for (int j = 0; j < rules.size(); j++) {
            auto rule = rules[j];
            if (rule.second == first && rule.first == second) {
                swap(updates[i+1], updates[i]);
                return false;
            }
        }
    }
    return true;
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    vector<pair<int, int>> pageRules;
    vector<vector<int>> updates;

    bool isUpdates = false;
    for (auto line : results) {
        if (line == "") {
            isUpdates = true;
            continue;
        }

        if (!isUpdates) {
            auto split = splitString<int>(line, '|');
            pageRules.push_back({split[0], split[1]});
        } else {
            auto split = splitString<int>(line, ',');
            updates.push_back(split);
        }
    }

    int part1 = 0;
    int part2 = 0;

    for (int i = 0; i < updates.size(); i++) {
        auto update = updates[i];

        if (isCorrectlyOrdered(update, pageRules)) {
            part1 += update[update.size() / 2 + 0.5];
        } else { 
            while (!isCorrectlyOrdered(update, pageRules));
            part2 += update[update.size() / 2 + 0.5];
        };
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}