#!/bin/bash

dd bs=512 count=1 if=/dev/random of=random.bin 2>/dev/null

./base85 -e < random.bin > my_encoded.b85
python3 -c "import sys, base64; sys.stdout.buffer.write(base64.b85encode(sys.stdin.buffer.read()))" < random.bin > py_encoded.b85

if cmp -s my_encoded.b85 py_encoded.b85; then
    echo "Encoder: OK"
else
    echo "Encoder: FAIL"
    exit 1
fi

./base85 -d < my_encoded.b85 > decoded.bin

if cmp -s random.bin decoded.bin; then
    echo "Decoder: OK"
else
    echo "Decoder: FAIL"
    exit 1
fi

exit 0
