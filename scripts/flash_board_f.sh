arduino-cli compile --upload \
    --fqbn esp32:esp32:esp32 \
    --port "/dev/cu.usbserial-02E4F819" \
    --build-property compiler.cpp.extra_flags=-DSO_LONG_BOARD_ID=6 \
    firmware/so_long
