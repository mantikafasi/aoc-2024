#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <cstring>

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
};

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

template <typename T>
vector<T> splitString(string &input, char delimiter) {
    istringstream stream(input);
    vector<T> vec;

    string token;
    while (getline(stream, token, delimiter)) {
        if constexpr (is_same_v<T, const char*>) {
            ltrim(token);
            char* copy = new char[token.size() + 1];
            strcpy(copy, token.c_str()); 

            vec.push_back(copy);
        };
    }
    return vec;
}

unordered_map<const char*, uint64_t> cache;

inline int size(const char* e) {
    int size = 0;
    for (int i = 0; e[i] != '\0'; i++) {
        size++;
    }
    return size;
}

inline bool startsWith(const char* key, const char* e) {
    for (int i = 0; e[i] != '\0'; i++) {
        if (key[i] != e[i]) {
            return false;
        }
    }
    return true;
};


void solve(const char* key, vector<const char*> &arr, const char* endIndex) {
    for (auto e : arr) {
        if (startsWith(key,e)) {
            const char* newKey = key + size(e);

            if (cache.find(newKey) != cache.end()) {
                cache[key] += cache[newKey];
            } else if (newKey == endIndex) {
                cache[key] += 1;
            } else {
                solve(newKey, arr, endIndex);
                cache[key] += cache[newKey];
            }
        }
    }
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    int part1 = 0;
    uint64_t part2 = 0;

    auto elements = splitString<const char*>(results[0], ',');

    for (int i = 2; i < results.size(); i++) {
        auto elem = results[i].c_str();

        solve(elem, elements, elem + size(elem));

        if (cache[elem] != 0) {
            part1+=1;
        } 
        part2 += cache[elem];
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();

    cout << "Time taken: " << duration << " milliseconds" << endl;
}