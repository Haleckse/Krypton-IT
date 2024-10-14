#!/bin/bash

ko='\e[00;31m';
wipe='\e[00m';
ok='\e[01;32m';

BASE=../..
TEST=./tests
RET=

function test_crack_c1 {
# critère C1
    rm -rf $TEST/keys0
    mkdir $TEST/keys0

    if [ -x $BASE/break_code ]
    then
      while read k
      do
        while read i
        do
        $BASE/break_code -i $TEST/crypted_crack/${k}_${i} -l log.txt -m c1 -k ${#k} > $TEST/keys0/${k}_${i}
        diff -B $TEST/keys0/${k}_${i} $TEST/ref_keys0/${k}_${i} &>/dev/null
        RET=$?
        [ $RET -eq 0 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "$i" && return
        [ $RET -ne 0 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "$i" && return
        done < ./$TEST/file_list_crack.txt
      done  < ./$TEST/keys_crack.txt
    else
      RET=2
    fi
  }

  function test_crack_all {
  # critère C1 + C2 + C3
  # exécute le code et met le résultat dans keys1
      rm -rf $TEST/keys1
      mkdir $TEST/keys1

      if [ -x $BASE/break_code ]
      then
        while read k
        do
          while read i
          do
          $BASE/break_code -i $TEST/crypted_crack/${k}_${i} -l log.txt -d ../../Dicos/dicoFrSA.txt -m all -k ${#k} > $TEST/keys1/${k}_${i}
          diff -B $TEST/keys1/${k}_${i} $TEST/ref_keys1/${k}_${i} &>/dev/null
          RET=$?
          printf "retour $RET \n"
          [ $RET -eq 0 ] && printf "\t%-12s [${ok}OK${wipe}]\n" "$i"
          [ $RET -ne 0 ] && printf "\t%-12s [${ko}KO${wipe}]\n" "$i" && return
          done < ./$TEST/file_list_crack.txt
        done  < ./$TEST/keys_crack.txt
      else
        RET=2
      fi
    }

function test {
        test_$1
        [ $RET -eq 0 ] && printf "%-12s [${ok}OK${wipe}]\n" "$1"
        [ $RET -ne 0 ] && printf "%-12s [${ko}KO${wipe}]\n" "$1"
    }

#test crack_c1; # validité
test crack_all;
    exit 0
