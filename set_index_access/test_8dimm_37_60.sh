# All the arguments are irrelevant, except for the third argument, the array contains approximately 0x80000 (1<<19) elements, the third argument should be 1<<23, so the third argument is changed to 23. 
#cd ..
#./load.sh
#cd access
for i in {0..2047}
do
	echo $i 
	./change_set_index 1 6 21 13 29 28 27 26 25 24 23 22 21 20 19 18 17 $i
	sleep 12
done
#cd ..
#./unload.sh
#cd access
