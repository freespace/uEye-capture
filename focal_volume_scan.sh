#!/usr/bin/bash

# These must be specified in um
START=9000
STEP=10
END=11000

# Where pyAPT lives and the serial number of the stage controller
GOTOPY="../pyAPT/goto.py"
SERIAL=83815070

echo "Scanning from $START um to $END um at $STEP um intervals"

for z in $(seq $START $STEP $END); do 
    # converts specifications in um and turns it into mm by inserting a . 3
    # position from the end
	z=$(echo $z|sed 's/\(...\)$/\.\1/');
	
	echo $z;
	
	python "$GOTOPY" $SERIAL $z;
	./capture;
	
	# the capture program currently always outputs snapshot.bmp, so rename it
	mv snapshot.bmp $z.bmp; 
done

