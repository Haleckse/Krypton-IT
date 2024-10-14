#!/bin/bash

ko='\e[00;31m';
wipe='\e[00m';
ok='\e[01;32m';

BASE=../..
TEST=./tests
RET=0

function test_cipher_xor {
    RET=0

    if [ -x $BASE/crypt_test ]
    then

    rm -rf $TEST/crypted/
    mkdir $TEST/crypted/

    while read k;
    do
	while read i
	do
 	    $BASE/crypt_test -i $TEST/ref/$i  -o $TEST/crypted/xor_${k}_${i} -k $k -m xor> /dev/null
	    diff $TEST/ref_crypted/xor_${k}_$i $TEST/crypted/xor_${k}_${i}  &>/dev/null
	    RET=$?
	    [ $RET -eq 0 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "$i"
	    [ $RET -ne 0 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "$i" && return;

	done <./$TEST/file_list.txt
    done < ./$TEST/keys.txt
    else
	RET=2
    fi

}

function test_decipher_xor {
    RET=0

    rm -rf $TEST/decrypted/
    mkdir $TEST/decrypted/

    if [ -x $BASE/crypt_test ]
    then

	while read k
	do
	    while read i
	    do
 		$BASE/crypt_test -o $TEST/decrypted/xor_${k}_$i  -i $TEST/crypted/xor_${k}_${i} -k $k -m xor&> /dev/null
		diff $TEST/ref/$i $TEST/decrypted/xor_${k}_${i}  &>/dev/null
		RET=$?
		[ $RET -eq 0 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "$i"
		[ $RET -ne 0 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "$i" && return
	    done < ./$TEST/file_list.txt
	done  < ./$TEST/keys.txt
    else
	RET=2
    fi
}

function test_cipher_cbc {
    RET=0

    if [ -x $BASE/crypt_test ]
    then

    rm -rf $TEST/crypted/
    mkdir $TEST/crypted/

    while read k;
    do
	while read i
	do
 	    $BASE/crypt_test -i $TEST/ref/$i  -o $TEST/crypted/cbc_${k}_${i} -k $k -m cbc-crypt -v azertyuiopqsdfgh> /dev/null
	    diff $TEST/ref_crypted/cbc_${k}_$i $TEST/crypted/cbc_${k}_${i}  &>/dev/null
	    RET=$?
	    [ $RET -eq 0 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "$i"
	    [ $RET -ne 0 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "$i" && return;

	done <./$TEST/file_list.txt
    done < ./$TEST/keys.txt
    else
	RET=2
    fi

}

function test_decipher_cbc {
    RET=0

    rm -rf $TEST/decrypted/
    mkdir $TEST/decrypted/

    if [ -x $BASE/crypt_test ]
    then

	while read k
	do
	    while read i
	    do
 		$BASE/crypt_test -o $TEST/decrypted/cbc_${k}_$i  -i $TEST/crypted/cbc_${k}_${i} -k $k -m cbc-uncrypt -v azertyuiopqsdfgh&> /dev/null
		diff -b $TEST/ref/$i $TEST/decrypted/cbc_${k}_${i}  &>/dev/null
		RET=$?
		[ $RET -eq 0 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "$i"
		[ $RET -ne 0 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "$i" && return
	    done < ./$TEST/file_list.txt
	done  < ./$TEST/keys.txt
    else
	RET=2
    fi
}

function test {
    test_$1

    [ $RET -eq 0 ] && printf "===> %-12s [${ok}OK${wipe}]\n" "$1"
    [ $RET -ne 0 ] && printf "xxx> %-12s [${ko}KO${wipe}]\n" "$1"
}

if [ "$#" -eq 0 ]
  then
    echo "Lancement complet des tests"
    test cipher_xor;
    test decipher_xor;
    test cipher_cbc;
    test decipher_cbc;
  else
    echo "Test de $1"
    test "$1"
fi
exit 0
