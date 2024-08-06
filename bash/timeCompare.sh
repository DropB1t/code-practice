#!/bin/bash

echo "Ciao stiamo lavorando in bash" > file.txt

for ((i=0;i<20;i++)); do
    start=$(date +"%s.%N")
    ./inverseCopy.sh file.txt fileCopy1.txt
    end=$(date +"%s.%N")
    t1=$(echo "scale=5;$end-$start" | bc)

    start=$(date +"%s.%N")
    ./inverseCopyAppend.sh file.txt fileCopy2.txt
    end=$(date +"%s.%N")
    t2=$(echo "scale=5;$end-$start" | bc)

    echo $(ls -sh file.txt | cut -f1 -d " ") $t1 $t2
    cat fileCopy1.txt >> file.txt
    rm fileCopy1.txt fileCopy2.txt
done
rm file.txt