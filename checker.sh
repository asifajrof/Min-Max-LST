#!/bin/bash
# stresstester GENERATOR SOL1 SOL2 ITERATIONS


for i in $(seq 1 "$4") ; do
    echo -en "\rAttempt $i/$4"

    # take random integer n between 3 and 22
    n=$(( ( RANDOM % 20 )  + 3 ))
    # take random integer m between n-1 and (n-1)*n/2
    m=$(( ( RANDOM % (n*(n-1)/2-n+1) )  + n-1 ))
    # write n and m to genparameters.txt
    echo $n > genparameters.txt
    echo $m >> genparameters.txt

    $1 < genparameters.txt > in.txt

    start_time=$(date +%s%3N)
    $2 < in.txt > out1.txt
    end_time=$(date +%s%3N)
    time_diff=$((end_time - start_time))

    start_time=$(date +%s%3N)
    $3 < in.txt > out2.txt
    end_time=$(date +%s%3N)
    time_diff2=$((end_time - start_time))

    diff -y out1.txt out2.txt > diff.txt
    if [ $? -ne 0 ] ; then
        echo -e "\nTestcase Found:"; cat in.txt
        echo -e "\nOutputs:"; cat diff.txt
        exit
    fi

    # add time_diff to brute
    fernau=$((fernau + time_diff))
    # add time_diff2 to fernau
    brute=$((brute + time_diff2))

done

echo -e "\nBrute: $brute ms"
echo -e "Fernau: $fernau ms"
