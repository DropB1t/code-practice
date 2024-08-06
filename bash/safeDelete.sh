#! /bin/bash

if (( $# > 0 )); then
    mkdir "~/BIN" &> /dev/null
    for p; do
        if [ -f $p ]; then
            if gzip -f $p; then
                if mv "$p.gz" ~/BIN ; then
                    echo "$p Removed"
                fi
            fi
        else
            echo "$p Not Found"
        fi
    done
else
    echo "$0 filename <filename>..."
fi