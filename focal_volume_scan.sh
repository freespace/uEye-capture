#!/usr/bin/bash

# Where pyAPT lives and the serial number of the stage controller
GOTOPY="../pyAPT/goto.py"
SERIAL=83815070

if [ $# -ne 4 ]; then
  echo "Usage: $0 <centre um> <width um> <step um> <data dir>"
  exit 1
fi

CENTRE="$1"
WIDTH="$2"
STEP="$3"
DATADIR="$4"

halfwidth=$(($WIDTH/2))
START=$(($CENTRE-$halfwidth))
END=$(($CENTRE+$halfwidth))


echo
echo "Using data dir [ $DATADIR ]"
echo
echo "Scanning from [ $START ] um to [ $END ] um at [ $STEP ] um intervals"
echo
echo "Press [ENTER] to begin"
read

PORT=5555

pkill capture;
./capture 5555 &

# this forces the script to wait until the capture is ready
./client localhost $PORT "@status"

for z in $(seq $START $STEP $END); do 
    # converts specifications in um and turns it into mm by inserting a . 3
    # position from the end
	z=$(echo $z|sed 's/\(...\)$/\.\1/');
	
	echo $z;
	
	python "$GOTOPY" $SERIAL $z && \
	./client localhost $PORT "${DATADIR}/$z.bmp"
done

./client localhost $PORT "@exit"
sleep 1
pkill capture

