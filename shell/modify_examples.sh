clear
mkdir examples
cd examples
touch one TWO 
mkdir test0
cd test0
touch white BLACK BRown blUE
cd ../
mkdir test1
cd test1
touch one TWO three FOUR FIve
cd ../
mkdir test2
cd test2
touch one TWO three FOUR FIve
mkdir test3
cd test3
touch one TWO three FOUR FIve
cd ../../
mkdir test4
cd test4
touch There This That
cd ../
mkdir test5
cd test5
touch warsaw PARIS bERliN
mkdir test6
cd test6
touch ROME madrid AthENs 
cd ../../

ls -R

echo
echo "===================TEST 0: Basic usage==================="
echo "Running..."
echo "../modify.sh -u one"
bash ../modify.sh -u one
echo "../modify.sh -l TWO"
bash ../modify.sh -l TWO
echo "../modify.sh -l test0"
bash ../modify.sh -l test0
echo "../modify.sh -h"
bash ../modify.sh -h
echo "../modify.sh -u test5"
bash ../modify.sh -u test5
echo
echo "RESULT:"
echo "./examples:"
ls 
echo "./test0: "
ls test0
echo "./test5: "
ls test5

echo
echo "================TEST 1: Recursive usage=================="
#cd test1
echo "Running..."
echo "../modify.sh -r -l test1 test2"
bash ../modify.sh -r -l test1 test2
echo "./test1: "
ls test1
#cd ../
echo "./test2: "
#cd test2 
ls test2
echo "./test2/test3: "
ls test2/test3
echo "../modify.sh -r -u test2"
bash ../modify.sh -r -u test2 
echo "RESULT:"
echo "./test2: "
ls test2
echo "./test2/test3: "
ls test2/test3
#cd ../
echo

echo "==========TEST 2: Recursive usage with pattern==========="
echo "Running..."
echo "../modify.sh -r 's/F/_F_WAS_HERE_/g' test2"
bash ../modify.sh -r 's/F/_F_WAS_HERE_/g' test2
echo "../modify.sh -r 's/Th/abc/g' test4"
bash ../modify.sh -r 's/Th/abc/g' test4
echo "../modify.sh -r 's/R/_new_/g' test5"
bash ../modify.sh -r 's/R/_new_/g' test5
echo "RESULT:" 
echo "./test2: "
ls test2
echo "./test4: "
ls test4
echo "./test5: "
ls test5
echo "./test5/test6: "
ls test5/test6

echo
echo "=================TEST 3: Incorrect usage================="
echo "Running..."
echo
echo "../modify.sh -r "
bash ../modify.sh -r
echo
echo "../modify.sh -r 's/F/f/gI'"
bash ../modify.sh -r 's/F/f/gI'
echo
echo "../modify.sh -l -u one"
bash ../modify.sh -l -u one
echo
echo "../modify.sh -p"
bash ../modify.sh -p
echo 
echo "======================FINAL STATE======================="
ls -R

cd ../
rm -r examples
