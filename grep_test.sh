#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""
TEMPLATE1="dab"
TEMPLATE2="q"

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
        echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

for var1 in i v c l n h s o
do
    for i in "VAR test.txt"
    do  
        var="-$var1 $TEMPLATE1"
        testing $i
    done
done

for var1 in i v c l n h s o
do
    for var2 in i v c l n h s o
    do
        for i in "VAR test.txt"
        do  
            if [ $var1 != $var2 ]
            then
                var="-$var1 -$var2 $TEMPLATE1"
                testing $i
            fi
        done
    done
done

# for var1 in i v c l n h s o
# do
#     for var2 in i v c l n h s o
#     do
#         for var3 in i v c l n h s o
#         do
#             for i in "VAR test.txt"
#             do  
#                 if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var2 != $var3 ]
#                 then
#                     var="-$var1 -$var2 -$var3 $TEMPLATE1"
#                     testing $i
#                 fi
#             done
#         done
#     done
# done

# for var1 in i v c l n h s o
# do
#     for var2 in i v c l n h s o
#     do
#         for var3 in i v c l n h s o
#         do
#             for var4 in i v c l n h s o
#             do
#                 for i in "VAR test.txt"
#                 do  
#                     if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var2 != $var3 ]
#                     then
#                         var="-$var1 -$var2 -$var3 -$var4 $TEMPLATE1"
#                         testing $i
#                     fi
#                 done
#             done
#         done
#     done
# done

for var1 in i v c l n h s o
do
    for var2 in i v c l n h s o
    do
        for i in "VAR test.txt"
        do  
            if [ $var1 != $var2 ]
            then
                var="-$var1$var2 $TEMPLATE1"
                testing $i
            fi
        done
    done
done

for var1 in i v c l n h s o "f file.txt"
do
    for i in "VAR test.txt"
    do  
        var="-e $TEMPLATE1 -$var1"
        testing $i
    done
done

for var1 in i v c l n h s
do
    for i in "VAR test.txt"
    do  
        var="-f file.txt -$var1"
        testing $i
    done
done