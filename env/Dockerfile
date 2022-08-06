FROM --platform=linux/amd64 debian

RUN apt-get update && apt-get install -y vim     \
                                         gcc     \
                                         make    \
                                         python3

WORKDIR /ft_ping
COPY env/vimrc /etc/vim/vimrc.local

CMD ["/bin/bash"]
