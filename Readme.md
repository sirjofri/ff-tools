ff-tools
========

**A collection of farbfeld tools**

This is a collection of tools working with the
[farbfeld](https://tools.suckless.org/farbfeld/) image file format.

![Demo image](demo.png)

(Code: `demo.sh`)

Generators
----------

- `ff-glow` A circle glow calculator
- `ff-color` A static color generator
- `ff-cosgen` A cos-wave generator (x-direction)
- `ff-singen` A sinewave generator (x-direction)

Functions
---------

- `ff-add` Add two images
- `ff-chuffle` A channel shuffler
- `ff-mul` A multiplication tool
- `ff-sub` Subtract: `<first image> - <second image>`

Corrections
-----------

- `ff-gamma` A gamma tool

Manual
======

General Usage
-------------

All of these tools have their input via `stdin` and output to `stdout`. So you
should use them like this:

    cat image.ff | ff-tool [parameters] | viewer

For example to view a `.png` file with lel and basic gamma applied:

    cat file.png | png2ff | ff-gamma | lel

(assuming you have [farbfeld](https://tools.suckless.org/farbfeld/) and
[lel](http://git.2f30.org/lel/) installed.)

Scripting
---------

**ff-tools** is created with scripting in mind. You can easily write scripts
to automatically perform tasks and profit from various scripting techniques
like variables, etc. Here is an example:

```bash
#!/bin/bash

if [ "$#" -eq 0 ]; then
	SCALE=4
else
	SCALE=$1
fi

SIZE=256
S=$(($SIZE*$SCALE))
REP=35

TMP=$(mktemp -d)
sin=$TMP/sin
cos=$TMP/cos
mul=$TMP/mul
col=$TMP/col
result=$TMP/result

ff-singen $S $S $REP >$sin
ff-cosgen $S $S $(($REP+4)) >$cos
ff-color $S $S 0.8 0.7 1.0 >$col

cat $sin $cos | ff-mul >$mul
cat $mul $col | ff-mul >$result

cat $TMP/result
rm -Rf $TMP
```

Generators
----------

Generators do not have input images, they just produce images from scratch
(and a few parameters).

- `ff-glow size`. Output: Image (size x size)
- `ff-color width height red green blue [alpha]`. Output: Image (width x
  height)
- `ff-cosgen width height steps`. Output: Image (width x height)
- `ff-singen width height steps`. Output: Image (width x height)

Functions
---------

- `ff-chuffle r g b a`.  
  Input: Image  
  Output: Image with shuffled rgba channels  
  Examples:
  - `cat image.ff | ff-chuffle g b r a > result.ff` (rotates rgb)
  - `cat image.ff | ff-chuffle g g r a > result.ff` (fill more channels,
    discard others)
- `ff-add`.  
  Input: Two images with the exact same size.  
  Output: Addition result.  
  Example: `cat a.ff b.ff | ff-add > result.ff`
- `ff-mul`.  
  Input: Two images with the exact same size.  
  Output: Multiplication result.  
  Example: `cat a.ff b.ff | ff-mul > result.ff`
- `ff-sub`.  
  Input: Two images with the exact same size.  
  Output: Subtraction result.  
  Example: `cat a.ff b.ff | ff-sub > result.ff`

Corrections
-----------

- `ff-gamma [gamma]`.  
  Input: Image  
  Output: Image with gamma (default: 2.2)  
  Examples:
  - `cat image.ff | ff-gamma > result.ff`  
  - `cat image.ff | ff-gamma 3.4 > result.ff`
