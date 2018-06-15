#!/bin/bash

OUT=$1

# you can play around with $SCALE and be flexible with the size
SCALE=1
SIZE=256
S=$(($SIZE*$SCALE))
# you can also play around with other variables
REP=35

# sometimes you want to create temp files
TMP=$(mktemp -d)
sin=$TMP/sin
cos=$TMP/cos
mul=$TMP/mul
color=$TMP/color
waves=$TMP/waves
glow=$TMP/glow
glowsin=$TMP/glowsin
glowresult=$TMP/glowresult
rows=$TMP/rows
addresult=$TMP/addresult
result=$TMP/result

# or use shell piping
{ { $OUT/ff-singen $S $S $REP; $OUT/ff-color $S $S 0.001 0.002 0.007; } | $OUT/ff-mul; $OUT/ff-glow $S; } | $OUT/ff-sub >$rows

$OUT/ff-singen $S $S $REP >$sin
$OUT/ff-cosgen $S $S $(($REP+4)) >$cos
$OUT/ff-color $S $S 0.8 0.7 1.0 >$color

$OUT/ff-singen $S $S 1 >$glowsin
$OUT/ff-glow $S >$glow
cat $glow $glowsin | $OUT/ff-mul >$glowresult

cat $sin $cos | $OUT/ff-mul >$mul
cat $mul $color | $OUT/ff-mul >$waves

cat $waves $glowresult | $OUT/ff-mul >$addresult
cat $addresult $rows | $OUT/ff-add >$result

cat $TMP/result
rm -Rf $TMP
