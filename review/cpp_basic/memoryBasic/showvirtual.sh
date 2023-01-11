#/bin/bash

inpuitFile=$1
 clang -Xclang -fdump-vtable-layouts -stdlib=libc++ -c ${inpuitFile}