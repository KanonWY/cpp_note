#! /bin/bash

cd ~/LLVM
cmake -S llvm -B build \ 
    -DCMAKE_BUILD_TYPE=Release 
    -DLLVM_ENABLE_PROJECTS="clang;lldb;clang-tools-extra"  
    -DLLVM_TEMPORARILY_ALLOW_OLD_TOOLCHAIN=true 
    -G "Ninja"
cmake --build build -j16
cmake --build build --target install