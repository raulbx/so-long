arduino-cli compile --upload \
    --fqbn esp32:esp32:esp32 \
    --port "/dev/cu.usbserial-023BE31B" \
    --build-property compiler.cpp.extra_flags=-DSO_LONG_BOARD_ID=2 \
    ../firmware/so_long
