#!/bin/bash

set -e

if [ "$(uname)" == "Linux" ]; then
    LIBS=(-lglfw -lGLEW -lGL -lX11 -lpthread -ldl)
    CXX="g++"
elif [ "$(uname)" == "Darwin" ]; then
    LIBS=(-lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo)
    CXX="clang++"
else
    echo "UNSUPPORTED OS"
    exit 1
fi

mkdir -p bin

MODE="$1"

case "$MODE" in
    debug)
        FLAGS=(-g -O0 -DDEBUG -Wall -Wextra -std=c++17)
        ;;
    dev)
        FLAGS=(-g -O0 -Wall -Wextra -std=c++17)
        ;;
    release)
        FLAGS=(-g -O3 -Wall -Wextra -std=c++17)
        ;;
    *)
        echo "BUILD AVAILABLE OPTIONS: debug, dev, release"
        exit 1
        ;;
esac

rm -rf build
mkdir -p build

$CXX "${FLAGS[@]}" \
    -I./lib \
    src/*.cpp \
    "${LIBS[@]}" \
    -o build/engine

rm -rf bin/engine
rm -rf bin/res

if [ "$MODE" = "release" ]; then
    cp -f build/engine bin/
    if [ -d "res" ]; then
        if command -v rsync >/dev/null 2>&1; then
            rsync -av --delete res/ bin/res/ >/dev/null
        else
            cp -R res bin/res
        fi
    fi
else
    ln -sf ../build/engine bin/engine
    if [ -d "res" ]; then
        ln -s ../res bin/res
    fi
fi

echo "BUILD COMPLETE"