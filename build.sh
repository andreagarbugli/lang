SOURCES=$(find src/ -maxdepth 1 -type f -name '*.c')
INCLUDE_DIR=src
GCC_FLAGS="-g -Wall -Wextra -Wpedantic -Wuninitialized -Wno-stringop-truncation"
OUTDIR=out
LANG_NAME=gx

gcc $GCC_FLAGS ${LANG_NAME}.c $SOURCES -I$INCLUDE_DIR -o "$OUTDIR/${LANG_NAME}"


