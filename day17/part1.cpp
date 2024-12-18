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
        if constexpr (is_same_v<T, int>) {
            vec.push_back(stoi(token));
        }
        if constexpr (is_same_v<T, string>) {
            vec.push_back(token);
        };
    }
    return vec;
}

int processOperand(map<char, uint64_t>& registers, int operand) {
    if (operand > -1 && operand < 4) {
        return operand;
    } else if (operand == 4) {
        return registers['A'];
    } else if (operand == 5) {
        return registers['B'];
    } else if (operand == 6) {
        return registers['C'];
    }
    return 0;
};

vector<int> solve(map<char, uint64_t> &registers, vector<int> instructions) {
    vector<int> result;

    for (int i = 0; i < instructions.size() / 2 + 0.5; i++) {
        auto opcode = instructions[i * 2];
        auto v = instructions[i * 2 + 1];

        switch (opcode) {
            case 0:
                registers['A'] /= pow(2, processOperand(registers, v));

                break;
            case 1:
                registers['B'] ^= v;
                break;
            case 2:
                registers['B'] = processOperand(registers, v) % 8;

                break;

            case 3:
                if (registers['A'] != 0) {
                    i = v - 2;
                }
                break;
            case 4:
                registers['B'] ^= registers['C'];
                break;
            case 5:
                result.push_back(processOperand(registers, v) % 8);
                break;

            case 6:
                registers['B'] = registers['A'] / pow(2, processOperand(registers, v));

                break;
            case 7:
                registers['C'] = registers['A'] / pow(2, processOperand(registers, v));

                break;
            default:
                break;
        }
    }

    return result;
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    string part1 = "";

    auto e = find(results.begin(), results.end(), "");

    vector<string> registersVector(results.begin(), e);
    auto program = results[results.size() - 1];

    map<char, uint64_t> registers;

    for (auto r : registersVector) {
        auto opcode = stoi(r.substr(r.find(':') + 1));
        registers[r.substr(r.find(':') - 1, 1)[0]] = opcode;
    }
    string c = program.substr(program.find(':') + 1);

    auto instructions = splitString<int>(c, ',');

    for (auto c : solve(registers,instructions)) {
        part1 += to_string(c);
        part1+= ",";
    }

    part1.pop_back();

    cout << "\n";

    cout << "Part 1: " << part1 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start).count();

    cout << "Time taken: " << duration << " microseconds" << endl;
}