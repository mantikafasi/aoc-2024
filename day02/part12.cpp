#include <bits/stdc++.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

vector<string> readContents(string filename)
{
    ifstream file(filename);

    vector<string> contents;

    string line;
    while (getline(file, line)) {
        contents.push_back(line);
    }
    return contents;
}

bool isSafe(vector<int> numbers)
{
    int number = numbers[0];
    vector<int> original(numbers);

    sort(numbers.begin(), numbers.end());

    if (original[0] - original[1] > 0) {
        reverse(numbers.begin(), numbers.end());
    }
    if (original != numbers) {
        return false;
    }

    for (int i = 0; i < numbers.size() - 1; i++) {
        if (abs(numbers[i + 1] - numbers[i]) > 3
            || (numbers[i + 1] - numbers[i]) == 0) {
            return false;
        }
    }
    return true;
}

int main()
{
    vector<string> results = readContents("input.txt");
    int part1 = 0;
    int part2 = 0;

    vector<vector<int>> numbers;

    for (string line : results) {
        int find = line.find(" ") + 1;

        vector<int> row;

        stringstream ss(line);
        string token;
        while (getline(ss, token, ' ')) {
            row.push_back(stoi(token));
        }

        numbers.push_back(row);
    }

    for (vector<int> group : numbers) {
        int safe = isSafe(group);
        part1 += safe;
        part2 += safe;
        if (safe) {
            continue;
        }

        // yes very optimized i dont care
        for (int i = 0; i < group.size(); i++) {
            vector<int> newGroup(group);

            remove(newGroup.begin(), newGroup.end(), i);
            newGroup.erase(newGroup.begin() + i);
            if (isSafe(newGroup)) {
                part2 += 1;
                break;
            }
        };
    };

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2;
}