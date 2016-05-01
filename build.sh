#!/bin/sh

third_party=$(dirname "$(readlink -f "$0")")
cd "${third_party}" || exit 1

install() {
  mkdir build.tmp && cd build.tmp || exit 1
  if [ "$OSTYPE" = "msys" ]; then
    cmake -G "Visual Studio 14 2015 Win64" \
      -DCMAKE_INSTALL_PREFIX:PATH="$(readlink -f $(pwd)/../..)/$1" .. || return 1
    cmake --build . --target install --config Debug || return 1
    cmake --build . --target install --config Release || return 1
    rm -rf *
    cmake -G "Visual Studio 14 2015" -T v140_xp \
      -DCMAKE_INSTALL_PREFIX:PATH="$(readlink -f $(pwd)/../..)/$1" .. || return 1
    cmake --build . --target install --config Debug || return 1
    cmake --build . --target install --config Release || return 1
  else
    cmake -G "Unix Makefiles" \
      -DCMAKE_C_COMPILER=clang-3.7 -DCMAKE_CXX_COMPILER=clang++-3.7 \
      -DCMAKE_INSTALL_PREFIX:PATH="$(readlink -f $(pwd)/../..)/$1" \
      -DCMAKE_BUILD_TYPE=Release .. || return 1
    cmake --build . --target install || return 1
    rm -rf *
    cmake -G "Unix Makefiles" \
      -DCMAKE_C_COMPILER=clang-3.7 -DCMAKE_CXX_COMPILER=clang++-3.7 \
      -DCMAKE_INSTALL_PREFIX:PATH="$(readlink -f $(pwd)/../..)/$1" \
      -DCMAKE_BUILD_TYPE=Debug .. || return 1
    cmake --build . --target install || return 1
  fi
  return 0
}

build() {
  case "$1" in
    zlib)
      cd "${third_party}" && rm -rf zlib-1.2.8
      tar xvf zlib-1.2.8.tar.gz && cd zlib-1.2.8 || exit 1
      7zr x -y ../zlib-patch.7z
      install "zlib" || exit 1
      cd "${third_party}" && rm -rf zlib-1.2.8
      ;;
    re2)
      cd "${third_party}" && rm -rf re2-2016-04-01
      tar xvzf re2-2016-04-01.tar.gz && cd re2-2016-04-01 || exit 1
      7zr x -y ../re2-patch.7z
      install "re2" || exit 1
      cd "${third_party}" && rm -rf re2-2016-04-01
      ;;
    libressl)
      cd "${third_party}" && rm -rf libressl-2.3.3
      tar xvzf libressl-2.3.3.tar.gz && cd libressl-2.3.3 || exit 1
      7zr x -y ../libressl-patch.7z
      install "libressl" || exit 1
      cd "${third_party}" && rm -rf libressl-2.3.3
      ;;
    asio)
      cd "${third_party}" && rm -rf asio-1.11.0
      tar xvjf asio-1.11.0.tar.bz2 && cd asio-1.11.0 || exit 1
      7zr x -y ../asio-patch.7z
      install "asio" || exit 1
      cd "${third_party}" && rm -rf asio-1.11.0
      ;;
    ice)
      cd "${third_party}/ice" && rm -rf build.tmp
      install "ice" || exit 1
      cd "${third_party}/ice" && rm -rf build.tmp
      ;;
    http)
      cd "${third_party}/http" && rm -rf build.tmp
      install "http" || exit 1
      cd "${third_party}/http" && rm -rf build.tmp
      ;;
    date)
      cd "${third_party}/date" && rm -rf build.tmp
      install "date" || exit 1
      cd "${third_party}/date" && rm -rf build.tmp
      ;;
    sqlite)
      cd "${third_party}/sqlite" && rm -rf build.tmp
      install "sqlite" || exit 1
      cd "${third_party}/sqlite" && rm -rf build.tmp
      ;;
    *)
      echo "error: unknown library \"$1\""
  esac
}

if [ -n "$1" ]; then
  build "$1"
else
  rm -rf zlib re2 libressl asio ice/lib http/lib sqlite/lib
  build zlib
  build re2
  build libressl
  build asio
  build ice
  build date
  build http
  build sqlite
fi

exit 0
