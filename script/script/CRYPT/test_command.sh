#!/bin/bash

ko='\e[00;31m';
wipe='\e[00m';
ok='\e[01;32m';

BASE=../..
TEST=./tests
RET=

function test_commandcrypt {
    RET=0

    if [ -x $BASE/dh_crypt ]
    then
	for i in "-i" "-z" "-i -o -k" "-i toto -o tata -m xor" "-i toto -m xor -k hello" "-i toto -m cbc-crypt -k hello" "-i toto -o tata -k qadiejf9ejfkldos" "-i toto -o tata"
	do
	    $BASE/dh_crypt $i &> /dev/null
	    RET=$?
	    [ $RET -eq 255 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "dh_crypt $i"
	    [ $RET -ne 255 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "dh_crypt $i" && RET=2 && return;
	done
	RET=0
    else
	RET=2
    fi
}

function test_commandgenkey {
    RET=0

    if [ -x $BASE/dh_genkey ]
    then
	for i in "-z" "-o" "-i"
	do
	    $BASE/dh_genkey $i &> /dev/null
	    RET=$?
	    [ $RET -eq 255 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "dh_genkey $i"
	    [ $RET -ne 255 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "dh_genkey $i" && RET=2 && return;
	done
	RET=0
    else
	RET=2
    fi
}

function test_commandcrack {
    RET=0

    if [ -x $BASE/dh_crack ]
    then
	for i in "-i" "-z" "-i -o -k" "-i toto -m c1" "-i toto -m xx -k hello" "-m xx -k -d -i toto" "-i toto -d tata -k qadiejf9ejfkldos" "-i toto -d tata"
	do
	    $BASE/dh_crack $i &> /dev/null
	    RET=$?
	    [ $RET -eq 255 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "dh_crack $i"
	    [ $RET -ne 255 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "dh_crack $i" && RET=2 && return;
	done
	RET=0
    else
	RET=2
    fi
}

function test {
    test_$1

    [ $RET -eq 0 ] && printf "===> %-12s [${ok}OK${wipe}]\n" "$1"
    [ $RET -ne 0 ] && printf "xxx> %-12s [${ko}KO${wipe}]\n" "$1"
}

test commandcrypt;
test commandgenkey;
test commandcrack;

exit 0
