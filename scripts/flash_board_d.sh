arduino-cli compile --upload \
    --fqbn esp32:esp32:esp32 \
    --port "/dev/cu.usbserial-023BE30F" \
    --build-property compiler.cpp.extra_flags=-DSO_LONG_BOARD_ID=4 \
    firmware/so_long
