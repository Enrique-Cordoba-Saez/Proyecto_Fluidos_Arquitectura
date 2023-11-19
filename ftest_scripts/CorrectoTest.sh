#!/bin/sh
rm ../files/small_out.fld
../cmake-build-release/fluid/fluid 5 ../files/small.fld ../files/small_out.fld
diff -s ../files/small_out.fld ../files/small-5_CORRECTO.fld
