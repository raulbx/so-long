#!/bin/sh
set -eu

DW3000_LIBRARY_PATH="${DW3000_LIBRARY_PATH:-$HOME/Documents/Arduino/libraries/Dw3000}"
DW3000_MUTEX_FILE="$DW3000_LIBRARY_PATH/src/dw3000_mutex.cpp"

test -f "$DW3000_MUTEX_FILE"
test -f patches/makerfabs-dw3000-decamutexon-return.patch

sed -n '/decaIrqStatus_t decamutexon/,/^}/p' "$DW3000_MUTEX_FILE" |
  rg "return 0;" >/dev/null
