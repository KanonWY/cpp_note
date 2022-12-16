#/bin/bash
inpuitFile=$1
 clang -Xclang -fdump-record-layouts -stdlib=libc++ -c ${inpuitFile}