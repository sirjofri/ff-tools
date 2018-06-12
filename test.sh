#!/bin/bash

diff <($1/ff-singen 100 100 3) bintest/sintest.ff
diff <($1/ff-cosgen 100 100 2) bintest/costest.ff
diff <(cat bintest/sintest.ff bintest/costest.ff | $1/ff-mul) bintest/multest.ff
diff <($1/ff-color 10 10 0.9 0.8 0.7 0.6) bintest/colortest.ff
diff <(cat bintest/colortest.ff | $1/ff-chuffle g b a r) bintest/chuffletest.ff
