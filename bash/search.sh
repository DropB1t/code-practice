#! /bin/bash

# Scrivere uno script che prende due argomenti:
# un directory D e una parola W, e stampa
# i nomi di tutti i file nel directory D che contengono la parola W.
# Lo script fa il controllo che i parametri passati sono giusti.

if [ $# = 2 ] ; then
    if [ -d $1 ] ; then
        cd $1
        for file in * ; do
            if grep $2 $file > /dev/null ; then
                echo $file
            fi
        done
    else
        echo "The first parameter must be a directory"
    fi
else
    echo "Usage: $0 directory word"
fi