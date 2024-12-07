#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

using namespace std;
using namespace std::chrono;

// inspired by https://github.com/Kyuuhachi/advent-of-code/blob/3e83833/24/07c.py
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
        if (token == "") continue;
        if constexpr (is_same_v<T, int>) {
            vec.push_back(stoi(token));
        }
        if constexpr (is_same_v<T, string>) {
            vec.push_back(token);
        };

        if constexpr (is_same_v<T, uint64_t>) {
            istringstream iss(token);
            uint64_t result = 0;
            iss >> result;
            vec.push_back(result);
        }
    }
    return vec;
}

bool isPossiblyValid(uint64_t result, vector<uint64_t>& numbers, int index = 0, bool part2 = false) {
    auto temp = numbers[index - 1];
    if (index > 0) {
        if (numbers[index] != 0 && numbers[index - 1] != 0 && numbers[index] % numbers[index - 1] == 0) {
            numbers[index - 1] = numbers[index] / numbers[index - 1];
            if (isPossiblyValid(result, numbers, index - 1, part2)) {
                return true;
            }
            numbers[index - 1] = temp;
        }

        uint64_t pow10 = pow(10,(int)log10(numbers[index - 1]) + 1);
        if (part2 && ((numbers[index] - numbers[index-1]) % pow10) == 0) {
            numbers[index - 1] = (numbers[index] - numbers[index-1]) / pow10;

            if (isPossiblyValid(result, numbers, index - 1, part2)) {
                return true;
            };
            numbers[index - 1] = temp;
        }

        if (numbers[index - 1] <= numbers[index]) {
            numbers[index - 1] = numbers[index] - numbers[index - 1];
            if (isPossiblyValid(result, numbers, index - 1, part2)) {
                return true;
            };
        }
        numbers[index - 1] = temp;
    }

    if (numbers[0] == 0) {
        return true;
    }
    return false;
};

int main() {

    auto start = high_resolution_clock::now();
    vector<string> results = readContents("input.txt");
    uint64_t part1 = 0;
    uint64_t part2 = 0;

    for (auto line : results) {
        auto result = line.substr(0, line.find(":"));
        line.erase(0, line.find(":") + 1);
        istringstream iss(result);
        uint64_t resNumber = 0;

        iss >> resNumber;

        auto numbers = splitString<uint64_t>(line, ' ');
        numbers.push_back(resNumber);

        if (isPossiblyValid(resNumber, numbers, numbers.size() - 1)) {
            part1 += resNumber;
        } else {
            if (isPossiblyValid(resNumber, numbers, numbers.size() - 1, true)) {
                part2 += resNumber;
            }
        }
    };

    part2 += part1;

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}