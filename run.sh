CUDA_PATH=/usr/local/cuda
export PATH=$CUDA_PATH/bin:$PATH
export LD_LIBRARY_PATH=$CUDA_PATH/lib64:$LD_LIBRARY_PATH
LLVM_PATH=$(ls -d /usr/lib/llvm-19 | sort -V | tail -n 1)
export LD_LIBRARY_PATH=$LLVM_PATH/lib:$LD_LIBRARY_PATH
export NVCC_PREPEND_FLAGS='-ccbin /usr/bin/g++-13'

DAY=$1 PART=$2 GPU=$3 && (cd day$DAY && \

if [ "$GPU" = "y" ]; then
    /usr/local/cuda/bin/nvcc -std=c++20 \
        -I$CUDA_PATH/include \
        -L$CUDA_PATH/lib64 \
        -L$LLVM_PATH/lib \
        -I$LLVM_PATH/include \
        -O3 part$PART.cu -o out
else
    clang++ --std=c++20 \
        -O3 \
        -I$LLVM_PATH/include \
        -L$LLVM_PATH/lib \
        part$PART.cpp -o out
fi && ./out)