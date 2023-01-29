FROM ubuntu:20.04
MAINTAINER kanonWY (jialomg62@gmail.com)
#RUN echo "deb http://mirrors.163.com/ubuntu/ bionic main restricted universe multiverse" > /etc/apt/sources.list
RUN apt-get update

#安装ssh服务
RUN apt-get install -y openssh-server
RUN mkdir -p /var/run/sshd
RUN mkdir -p /root/.ssh

#安装基本的环境
RUN apt-get install -y cmake
RUN apt-get install -y clang
RUN apt-get install -y ninja-build
RUN apt-get install -y git
RUN apt-get install -y vim
RUN apt-get install -y clangd



#安装ohmyzsh
RUN apt-get install -y zsh wget
RUN sh -c "$(wget -O- https://gitee.com/pocmon/ohmyzsh/raw/master/tools/install.sh)"
RUN git clone https://gitee.com/asddfdf/zsh-syntax-highlighting.git ~/.oh-my-zsh/plugins/zsh-syntax-highlighting
RUN git clone https://gitee.com/chenweizhen/zsh-autosuggestions.git ~/.oh-my-zsh/plugins/zsh-autosuggestions
RUN chsh -s /bin/zsh


#安装编译环境
ADD vimconfig ~/.vimrc

#取消pam限制
RUN sed -ri 's/session required pam_loginuid.so/#session required pam_loginuid.so/g' /etc/pam.d/sshd

#复制配置文件，赋予脚本可执行权限
ADD authorized_keys  /root/.ssh/authorized_keys
ADD run.sh /run.sh
RUN chmod 755 /run.sh

#创建fastdds环境
RUN apt-get install -y cmake g++ python3-pip wget git
RUN apt-get install -y libasio-dev libtinyxml2-dev
RUN apt-get install -y libssl-dev

# 编译 foonathan_memory_vendor
RUN mkdir -p ~/Fast-DDS
RUN cd ~/Fast-DDS && git clone https://github.com/eProsima/foonathan_memory_vendor.git && \ 
    cd foonathan_memory_vendor && \
    cmake -S . -B build -G "Ninja" && \
    cmake --build build -j2 && \
    cmake --build build --target install

# 编译Fast CDR
RUN cd ~/Fast-DDS && \
    git clone https://github.com/eProsima/Fast-CDR.git && \
    cd Fast-CDR && \ 
    cmake -S . -B build -G "Ninja" && \
    cmake --build build -j4 && \
    cmake --build build --target install

# 编译Fast DDS
RUN cd ~/Fast-DDS && \ 
    git clone https://github.com/eProsima/Fast-DDS.git && \
    cd Fast-DDS && \
    cmake -S . -B build -G "Ninja" && \
    cmake --build build -j4 && \
    cmake --build build --target install


# 安装IDL工具
RUN apt-get install -y openjdk-8-jdk unzip
RUN cd ~/Fast-DDS && \
    mkdir -p /root/Fast-DDS/tools && \
    cd /root/Fast-DDS/tools && \
    wget https://services.gradle.org/distributions/gradle-7.6-bin.zip &&\
    unzip gradle-7.6-bin.zip

ENV PATH=$PATH:/root/Fast-DDS/tools/gradle-7.6/bin

RUN cd ~/Fast-DDS && \
    git clone --recursive https://github.com/eProsima/Fast-DDS-Gen.git && \
    cd Fast-DDS-Gen && \
    gradle assemble

#add to env_path
RUN echo 'export PATH=/root/Fast-DDS/tools/gradle-7.6/bin:$PATH'  > ~/.bashrc



EXPOSE 22
#启动命令
CMD ["/run.sh"]
