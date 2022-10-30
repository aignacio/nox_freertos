FROM ubuntu:latest
LABEL author="Anderson Ignacio da Silva"
LABEL maintainer="anderson@aignacio.com"
ENV TZ=Europe/Dublin
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update && apt-get upgrade -y
RUN apt-get install git file gcc make time wget zip -y
RUN apt-get install python3 autoconf cmake -y
RUN apt-get install graphviz -y

# [riscv-toolchain]
WORKDIR /opt/
RUN apt-get install wget -y
RUN wget https://github.com/stnolting/riscv-gcc-prebuilt/releases/download/rv32i-4.0.0/riscv32-unknown-elf.gcc-12.1.0.tar.gz
RUN mkdir /opt/riscv
RUN tar -xzf riscv32-unknown-elf.gcc-12.1.0.tar.gz -C /opt/riscv/
RUN rm -rf riscv32-unknown-elf.gcc-12.1.0.tar.gz
#RUN echo "export PATH=$PATH:/opt/riscv/bin" >> /root/.bashrc # Doesn't need matter to update here because bashrc is only when you log through bash terminal
RUN chmod +x -R /opt/riscv/bin/*
ENV PATH "PATH=$PATH:/opt/riscv/bin"
