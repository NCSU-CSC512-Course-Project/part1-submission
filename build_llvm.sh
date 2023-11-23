# Script for building LLVM with LibClang (Clang tools Extra).

# Ensure CMake and Ninja are Installed
sudo apt update
sudo apt -y install cmake ninja-build

# Install LLVM
git clone https://github.com/llvm/llvm-project.git
cd llvm-project
mkdir build
cmake -B build -S llvm -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang"
sudo ninja -C build install -j$(nproc)

