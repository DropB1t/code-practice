#!/bin/bash

arr=($@)
n=${#arr[@]}

exec 4>${arr[$((n-1))]}
for ((i=n-2;i>=0;i--)); do
    cat ${arr[$i]} >& 4
done
exec 4>&-

echo "Append done"