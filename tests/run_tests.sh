#!/bin/bash
INIT=$(pwd)
mkdir -p build
cd build
cmake ..
make
clear

red=$'\e[1;31m'
grn=$'\e[1;32m'
yel=$'\e[1;33m'
blu=$'\e[1;34m'
mag=$'\e[1;35m'
cyn=$'\e[1;36m'
end=$'\e[0m'

PASSED=0
ALL=0
FAILED_TESTS=""
cd "$INIT"
FILES=$(find build -name 'test_*.out')
for FILE in $FILES ; do
    FILENAME=$(echo ${FILE}  | sed 's#build/##' | sed 's#test_##' | sed 's#.out##')
    printf "${yel} Running: $FILENAME ${end}\n"
    ./"$FILE"
    if (( $? == 0 ))  ; then
        PASSED=$((PASSED + 1))
    else
        FAILED_TESTS="${FAILED_TESTS} ${red}- $(echo ${FILENAME})${end}\n"
    fi
    ALL=$((ALL + 1))
done

printf "\n${mag}===========================================${end}\n"
if [[ "$PASSED" == "$ALL" ]]; then
printf "${grn}"
else
printf "${cyn}"
fi
printf "\n$PASSED/$ALL tests passed.\n${end}"
if [[ -n ${FAILED_TESTS} ]]; then
    printf "${red}\nFailed tests:\n${end}"
    printf "${FAILED_TESTS}"
else
    printf "\n${grn}All tests PASSED${end}.\n"
fi