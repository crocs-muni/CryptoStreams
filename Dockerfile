# initialize from the image

ARG BASE_IMAGE=ubuntu:16.04
FROM ${BASE_IMAGE} AS tbuilder

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ=Europe/Prague

RUN set -ex && \
    apt-get update && \
    apt-get --no-install-recommends --yes install \
        ca-certificates \
        cmake \
        g++ \
        git \
        make \
        pkg-config \
        libboost-random-dev

## Packager cleanup, best practices
RUN set -ex && rm -rf /var/lib/apt/lists/*

ARG NBPROC
WORKDIR /src
COPY . .

ARG NPROC
FROM tbuilder AS cryptostreams

RUN set -ex \
    && git submodule update --recursive --init \
    && rm -rf build \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j${NPROC:-`nproc`} \
    && mkdir -p /data

WORKDIR /data

# In order to bypass entrypoint pass the folowing param
# to the docker run command: --entrypoint bash
ENTRYPOINT ["/src/build/crypto-streams"]
