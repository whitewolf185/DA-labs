echo $1 | python3 testGener.py

echo "python: MY WORK IS DONE"

g++ benchmark.cpp

# shellcheck disable=SC2129
printf "\n" >> logfile.txt
echo "test $1" >> logfile.txt
cat i.txt | ./a.out >> logfile.txt