ff-tools
========

**A collection of farbfeld tools**

This is a collection of tools working with the
[farbfeld](https://tools.suckless.org/farbfeld/) image file format.

Generators
----------

- `ff-cosgen` A cos-wave generator (x-direction)
- `ff-singen` A sinewave generator (x-direction)

Functions
---------

- `ff-mul` A multiplication tool

Corrections
-----------

- `ff-gamma` A gamma tool

Manual
======

General Usage
-------------

All of these tools have their input via `stdin` and output to `stdout`. So you
should use the like this:

   cat image.ff | ff-tool [parameters] | viewer

For example to view a `.png` file with lel and basic gamma applied:

   cat file.png | png2ff | ff-gamma | lel

(assuming you have [farbfeld](https://tools.suckless.org/farbfeld/) and
[lel](http://git.2f30.org/lel/) installed)

Generators
----------

Generators do not have input images, they just produce images from scratch
(and a few parameters).

- `ff-cosgen width height steps`. Output: image (width x height)
- `ff-singen width height steps`. Output: image (width x height)

Functions
---------

- `ff-mul`.  
  Input: Two images with the exact same size.  
  Output: Multiplication result.  
  Example: `cat a.ff b.ff | ff-mul > result.ff`

Corrections
-----------

- `ff-gamma [gamma]`.  
  Input: Image  
  Output: Image with gamma (default: 2.2)  
  Examples:
  - `cat image.ff | ff-gamma > result.ff`  
  - `cat image.ff | ff-gamma 3.4 > result.ff`
