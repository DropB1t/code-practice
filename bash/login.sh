#! /bin/bash

users=$( who | cut -f1 -d" " | sort | uniq )

for u in $users ; do
    nome=$( getent passwd $u | cut -f5 -d ":" )
    ll=$( lastlog -u "$u" | grep "$u" | tr -s ' ' | cut -f4,5,6,7,9 -d" " )
    echo "Utente $u con nome $nome e last login $ll"
done