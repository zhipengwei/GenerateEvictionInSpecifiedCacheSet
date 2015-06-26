# All the arguments are irrelevant, except for the third argument, the array contains approximately 0x80000 (1<<19) elements, the third argument should be 1<<23, so the third argument is changed to 23. 
#cd ..

# The last parameter is the set index that will be accessed
echo "Set index $1"
taskset -c 0 ./change_set_index 1 6 21 13 29 28 27 26 25 24 23 22 21 20 19 18 17 $1
