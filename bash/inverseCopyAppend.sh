#!/bin/bash

exec 3<$1

i=0
while read -u3 line; do
    lines[$i]=$line
    ((i++))
done

exec 3<&-

for ((i=i-1;i>=0;i--)); do
    echo ${lines[$i]} >>$2
done

exec 4>&-