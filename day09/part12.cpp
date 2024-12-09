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

struct File {
    int id;
    unsigned long position;
    int size;
};

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    uint64_t part1 = 0;
    uint64_t part2 = 0;

    wstring input;

    vector<File> files;

    vector<pair<int, int>> freeSpaces;

    int id = 0;
    for (auto character : results[0]) {
        if (id % 2 == 0) {
            files.push_back(File{
                .id = id / 2,
                .position = input.size(),
                .size = character - '0'});

            for (int j = 0; j < character - '0'; j++) {
                input.push_back(id / 2 + '0');
            };

        } else {
            freeSpaces.push_back({input.size(), character - '0'});

            for (int j = 0; j < character - '0'; j++) {
                input.push_back('.');
            }
        }
        id++;
    }

    wstring part1out = input;
    auto dotPos = part1out.find('.');
    for (int i = part1out.size() - 1; i >= 0; i--) {
        auto letter = part1out[i];

        if (letter != '.') {
            part1out[dotPos] = letter;
        }

        if (dotPos != string::npos) {
            part1out.pop_back();
        }
        dotPos = part1out.find('.', dotPos);
    };

    reverse(files.begin(), files.end());

    for (auto file : files) {
        for (int l = 0 ; l < freeSpaces.size(); l++) {
            auto freeSpace = freeSpaces[l];

            if (file.size <= freeSpace.second && file.position > freeSpace.first) {
                input.replace(freeSpace.first, file.size, wstring(file.size, file.id + '0'));
                input.replace(file.position, file.size, wstring(file.size, '.'));
                freeSpaces[l].second -= file.size;
                freeSpaces[l].first += file.size;
                if (freeSpaces[l].second == 0) {
                    freeSpaces.erase(freeSpaces.begin() + l);
                }
                break;
            }
        }
    }

    for (int i = 0; i < part1out.size(); i++) {
        part1 += (part1out[i] - '0') * i;
    }

    for (int i = 0; i < input.size(); i++) {
        if (input[i] == '.') continue;
        part2 += (input[i] - '0') * i;
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}