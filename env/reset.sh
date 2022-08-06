#!/bin/sh

docker rmi -f ft_ping
docker image build -t ft_ping -f env/Dockerfile .
