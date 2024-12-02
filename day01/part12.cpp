#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;

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

int main()
{
    vector results = readContents("input.txt");

    vector<int> firstResults;
    vector<int> secondResults;

    for (string line : results)
    {
        string first = line.substr(0, line.find("   ") + 3);
        line.erase(0, line.find("   "));
        firstResults.push_back(stoi(first));
        secondResults.push_back(stoi(line));
    }

    sort(firstResults.begin(), firstResults.end());

    sort(secondResults.begin(), secondResults.end());

    int part1 = 0;
    int part2 = 0;

    for (int i = 0; i < firstResults.size(); i++)
    {
        part1 += abs(firstResults[i] - secondResults[i]);
    }

    for (int i = 0; i < firstResults.size(); i++)
    {
        int first = firstResults.at(i);
        int count = 0;

        for (int j = 0; j < firstResults.size(); j++)
        {
            if (secondResults.at(j) == first)
            {
                count += 1;
            }
        }

        part2 += first * count;
    }

    cout << "Part 1: " << part1 << "\n";
    cout << "Part 2: " << part2;
}