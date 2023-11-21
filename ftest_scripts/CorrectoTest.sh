#!/bin/sh

. /etc/profile
module avail
module load gcc/12.1.0

rm ../files/small_out.fld
../build/fluid/fluid 5 ../files/small.fld ../files/small_out.fld
diff -s ../files/small_out.fld ../files/small-5_CORRECTO.fld
