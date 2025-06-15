#!/usr/bin/env bash
set -eu -o pipefail

pid="$(ps aux | grep ./main- | grep -v -e rg -e grep | awk '{print $2}')"

cat << EOF > gdbinit
b __evilcc_init
c
EOF

sudo gdb -x gdbinit -p "$pid" || true

rm gdbinit
