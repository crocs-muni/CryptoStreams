# Building with Docker

Docker enables to build Cryptostreams in the virtualized container in a deterministic manner without need to install and configure build environment.

The build consists of 2 phases:

- Image build
- Image run

## Building the image

Building the Cryptostreams for ubuntu 16.04.

```bash
docker build --build-arg BASE_IMAGE=ubuntu:16.04 -t="cryptostreams"  .
```

## Running the image

```bash
docker run -i -t cryptostreams:latest
```

This runs built cryptostreams binary.

### Generating the data

```bash
docker run -i --mount type=bind,src=`pwd`,dst=/data -t cryptostreams:latest -c crypto-streams-config.json
```
With this command the current working directory of the crypto-streams running in the docker image `/data` is mounted to the current working directory of the shell running the docker command. Thus you can pass a configuration file to the crypto-streams and it will produce the data file to the current directory.

### Working inside the container

If you want to get an interactive shell instead of running cryptostreams do:

```bash
docker run -i --mount type=bind,src=`pwd`,dst=/data --entrypoint bash -t cryptostreams:latest
```
