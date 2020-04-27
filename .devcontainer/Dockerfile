FROM ubuntu:bionic-20200311

ARG CMAKE_VERSION=3.15.6

# Installation of CMake
RUN apt-get update && apt install -y curl wget build-essential libcairo2-dev libgraphicsmagick1-dev libpng-dev \
    && wget -O cmake_install.sh https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION-Linux-x86_64.sh \
    && chmod +x /cmake_install.sh \
    && /cmake_install.sh --skip-license \
    && rm /cmake_install.sh

RUN apt install -y libncurses5-dev libncursesw5-dev

CMD [ "/bin/sh" ]