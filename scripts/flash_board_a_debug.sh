arduino-cli compile --upload \
    --fqbn esp32:esp32:esp32 \
    --port "/dev/cu.usbserial-023BE30F" \
    --build-property 'compiler.cpp.extra_flags=-DSO_LONG_BOARD_ID=1 -DSO_LONG_APP_DEBUG=1' \
    --build-path build/board-b-app-debug \
    ../firmware/so_long
