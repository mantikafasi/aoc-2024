#include <cuda.h>
#include <cuda_runtime.h>
#include <cuda_runtime_api.h>
#include <device_atomic_functions.h>
#include <device_launch_parameters.h>
#include <omp.h>
#include <stdio.h>

#include <algorithm>
#include <atomic>
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
};

__device__ __forceinline__ bool processInstruction(
    const int opcode, const int64_t v, const uint64_t processed,
    uint64_t& reg_a, uint64_t& reg_b, uint64_t& reg_c, int& i) {
    switch (opcode) {
        case 0:
            reg_a >>= processed;
            break;
        case 1:
            reg_b ^= v;
            break;
        case 2:
            reg_b = processed & 7;
            break;
        case 3:
            if (reg_a) i = (v / 2) - 1;
            break;
        case 4:
            reg_b ^= reg_c;
            break;
        case 6:
            reg_b = reg_a >> processed;
            break;
        case 7:
            reg_c = reg_a >> processed;
            break;
    }
    return true;
};

__device__ __inline__ uint64_t process(uint64_t v, uint64_t reg_0, uint64_t reg_1, uint64_t reg_2) {
    if (v < 4)
        return v;
    else if (v == 4) {
        return reg_0;
    } else if (v == 5) {
        return reg_1;
    } else if (v == 6) {
        return reg_2;
    };
    return 0;
}

__global__ void searchKernel(const int* inst_array, const int inst_size,
                             const uint64_t _start_k, const uint64_t chunk_size,
                             uint64_t* result, bool* found, uint64_t* found_k) {
    uint64_t tid = blockIdx.x * blockDim.x + threadIdx.x;
    uint64_t start_k = _start_k + tid * chunk_size;

    if (*found && start_k > *found_k) return;

    for (uint64_t k = start_k; k < start_k + chunk_size && !(*found && k > *found_k); k++) {
        uint64_t reg_0 = k;
        uint64_t reg_1 = 0;
        uint64_t reg_2 = 0;

        int j = 0;

        while (true) {
            reg_1 = reg_0 & 7;

            reg_1 ^= 7;

            reg_2 = reg_0 >> reg_1;

            reg_0 >>= 3;

            reg_1 ^= reg_2;

            reg_1 ^= 7;

            if (inst_array[j] != (reg_1 & 7)) {
                break;
            }

            if (++j == inst_size) {
                printf("Found result: %lu\n", k);
                atomicMin((unsigned long long*)result, k);
                *found = true;
                *found_k = k;
                break;
            };

            if (reg_0 == 0) break;
        }
    };
}

int main() {
    auto start = high_resolution_clock::now();

    vector<string> results = readContents("input.txt");
    uint64_t part2 = 0;

    auto e = find(results.begin(), results.end(), "");

    vector<string> registersVector(results.begin(), e);
    auto program = results[results.size() - 1];

    vector<uint64_t> original_registers;

    for (auto r : registersVector) {
        auto opcode = stoi(r.substr(r.find(':') + 1));
        original_registers.push_back(opcode);
    }
    string c = program.substr(program.find(':') + 1);

    auto instructions = splitString<int>(c, ',');
    const int inst_size = instructions.size();
    int inst_array[instructions.size()];

    copy(instructions.begin(), instructions.end(), inst_array);

    uint64_t* h_result = (uint64_t*)malloc(sizeof(uint64_t));
    bool* h_found = (bool*)malloc(sizeof(bool));
    uint64_t* h_found_k = (uint64_t*)malloc(sizeof(uint64_t));

    *h_result = UINT64_MAX;
    *h_found = false;
    *h_found_k = UINT64_MAX;

    const uint64_t max_k = 300000000000000;
    const uint64_t start_k = 0;
    const uint64_t chunk_size = 10000000ULL;

    int* d_inst_array;
    uint64_t* d_result;
    bool* d_found;
    uint64_t* d_found_k;

    cudaMalloc(&d_inst_array, inst_size * sizeof(int));
    cudaMalloc(&d_result, sizeof(uint64_t));
    cudaMalloc(&d_found, sizeof(bool));
    cudaMalloc(&d_found_k, sizeof(uint64_t));

    cudaMemcpy(d_inst_array, inst_array, inst_size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_result, h_result, sizeof(uint64_t), cudaMemcpyHostToDevice);
    cudaMemcpy(d_found, h_found, sizeof(bool), cudaMemcpyHostToDevice);
    cudaMemcpy(d_found_k, h_found_k, sizeof(uint64_t), cudaMemcpyHostToDevice);

    const int BLOCK_SIZE = 256;
    const int GRID_SIZE = 64;

    for (uint64_t current_start_k = start_k; current_start_k < max_k; current_start_k += chunk_size * GRID_SIZE * BLOCK_SIZE) {
        if (*h_found && current_start_k > *h_found_k) break;

        auto parallel_start_time = high_resolution_clock::now();

        printf("Current start k: %lu\n", current_start_k);

        searchKernel<<<GRID_SIZE, BLOCK_SIZE>>>(d_inst_array, inst_size,
                                                current_start_k, chunk_size,
                                                d_result, d_found, d_found_k);
    
        cudaDeviceSynchronize();

        cudaError_t error = cudaGetLastError();
        if (error != cudaSuccess) {
            printf("CUDA error: %lu %s\n", current_start_k, cudaGetErrorString(error));
        }

        auto parallel_end_time = high_resolution_clock::now();
        auto parallel_duration = duration_cast<milliseconds>(parallel_end_time - parallel_start_time).count();

        printf("Parallel duration: %d milliseconds for: %lu elements\n", parallel_duration, chunk_size * GRID_SIZE * BLOCK_SIZE);

        cudaMemcpy(h_result, d_result, sizeof(uint64_t), cudaMemcpyDeviceToHost);
        cudaMemcpy(h_found, d_found, sizeof(bool), cudaMemcpyDeviceToHost);
    }

    cudaFree(d_inst_array);
    cudaFree(d_result);
    cudaFree(d_found);

    cout << "\n";
    part2 = *h_result;

    cout << "Part 2: " << part2 << "\n";

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(end - start).count();

    cout << "Time taken: " << duration << " seconds" << endl;
}