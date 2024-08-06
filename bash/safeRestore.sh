#! /bin/bash

if (( $# > 0 )); then
    if [ -d ~/BIN ] ; then
        for p; do
            if [ -f  ~/BIN/$p.gz ] ; then
                if gunzip ~/BIN/$p.gz ; then
                    if mv "$p" . ; then
                        echo "File $p is restored"
                    else
                        echo "Cannot move the file $p"
                    fi
                else
                    echo "Cannot unzip file $p"
                fi
            else
                echo "$p is not found"
            fi
        done
    else
        echo "BIN does not exist"
    fi
else
    echo "$0 filename <filename>..."
fi