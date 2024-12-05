#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<string> readContents(string filename)
{
    ifstream file(filename);

    vector<string> contents;

    string line;
    while (getline(file, line))
    {
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

int main()
{
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;


    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();
    
    cout << "Time taken: " << duration << " microseconds" << endl;

}