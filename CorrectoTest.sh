#!/bin/sh

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 <build_directory>"
  exit 1
fi

BUILD_DIR="$1"

. /etc/profile
module avail
module load gcc/12.1.0

rm ./files/small_out.fld
"${BUILD_DIR}/fluid/fluid" 5 ./files/small.fld ./files/small_out.fld
diff -s ./files/small_out.fld ./files/small-5_CORRECTO.fld
