#!/bin/bash

isInteger() {
    re='^[0-9]+$'
    if ! [[ $1 =~ $re ]] ; then
    echo "Error: $1 is not a positive Integer number."; exit 1
    fi
}

isFloat() {
    re='^[0-9]+([.][0-9]+)?$'
    if ! [[ $1 =~ $re ]] ; then
    echo "Error: $1 is not a positive Float number."; exit 1
    fi
}

dirSize() {
    sizes=( $(find $1 -maxdepth 1 -type f -printf '%s\n') )
    res=0
    for i in "${sizes[@]}"
    do
        ((res+=$i))
    done
    echo $res
}

if [ "$#" -ne 8 ]; then
    echo "Error: Illegal number of parameters."
    exit 1
fi

while getopts ":d: s: f: n:" flag &> /dev/null
do
    case "${flag}" in
        d) dir=${OPTARG};;
        s) size=${OPTARG};;
        f) factor=${OPTARG};;
        n) maxnum=${OPTARG};;
        ?) echo "Error: Invalid option was specified -$OPTARG"
            exit 1;;
    esac
done

if [ ! -d "$dir" ] ; then
    echo "Error: $dir directory does not exist."
    exit 1
fi

isFloat $size
isFloat $factor
isInteger $maxnum

if (( $(echo "$factor < 0" | bc -l) || $(echo "$factor > 1" | bc -l) )); then
    echo "Error: Warning-factor need to be in range [0,1]."
    exit 1
fi

if (( $maxnum == 0)); then
    maxnum=$( find $dir -maxdepth 1 -type f | wc -l )
fi

currDirSize=$( dirSize $dir )
size=$(($size*1024*1024))
remainSize=$(($size - $currDirSize))
warningSizeFloat=$(echo "$size*$factor" | bc -l)
warningSizeInt=${warningSizeFloat/.*}

if [ "$remainSize" -gt "$warningSizeInt" ]; then
    echo "Current size of directory $dir: $currDirSize bytes. Done..."
    exit 0
fi

i=0
j=0
array=($(find $dir -maxdepth 1 -type f -printf '%A@ %s %p\n' | sort))
while (( $(echo "$remainSize < $warningSizeInt" | bc -l) && $(echo "$i < $maxnum" | bc -l) )); do
    rm "${array[$(($j+2))]}"
    echo "Removed file: $(basename "${array[$(($j+2))]}")  with access time: ${array[$(($j))]} and size of ${array[$(($j+1))]} bytes"
    remainSize=$(($remainSize + ${array[$(($j+1))]}))
    j=$(($j+3))
    ((i++))
done
currDirSize=$( dirSize $dir )
echo "Current size of directory $dir: $currDirSize bytes. Done..."