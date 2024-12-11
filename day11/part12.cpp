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
        if constexpr (is_same_v<T, uint64_t>) {
            uint64_t num;
            istringstream iss(token);
            iss >> num;

            vec.push_back(num);
        }
        if constexpr (is_same_v<T, string>) {
            vec.push_back(token);
        };
    }
    return vec;
}

int total_blink_count = 75;

uint64_t solve(map<uint64_t, uint64_t>& cache, uint64_t stone, int blinkCount) {
    auto cacheId = total_blink_count * stone + blinkCount;

    if (cache.find(cacheId) != cache.end()) {
        return cache[cacheId];
    }

    uint64_t count = 1;
    for (int j = 0; j < blinkCount; j++) {
        if (stone == 0) {
            stone = 1;
            continue;
        }

        int e = log10(stone) + 1;

        if (e % 2 == 0) {
            count--;
            int p = pow(10, e / 2);

            uint64_t second = stone % p;
            uint64_t first = (stone - second) / p;

            if (blinkCount - j - 1 <= 0) {
                count = 2;
                break;
            }

            count += solve(cache, first, blinkCount - j - 1) + solve(cache, second, blinkCount - j - 1);
            break;
        }

        stone *= 2024;
    }

    cache[cacheId] = count;
    return count;
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    vector<uint64_t> stones = splitString<uint64_t>(results[0], ' ');

    uint64_t part1 = 0;
    uint64_t part2 = 0;

    map<uint64_t, uint64_t> cache1;
    map<uint64_t, uint64_t> cache2;
    

    for (int i = 0; i < stones.size(); i++) {
        total_blink_count = 25;
        part1 += solve(cache1, stones[i], total_blink_count);
        total_blink_count = 75;
        part2 += solve(cache2, stones[i], total_blink_count);
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}