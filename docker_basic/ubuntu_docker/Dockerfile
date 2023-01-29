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


#安装编译环境
ADD vimconfig ~/.vimrc




#取消pam限制
RUN sed -ri 's/session required pam_loginuid.so/#session required pam_loginuid.so/g' /etc/pam.d/sshd

#复制配置文件，赋予脚本可执行权限
ADD authorized_keys  /root/.ssh/authorized_keys
ADD run.sh /run.sh
RUN chmod 755 /run.sh


EXPOSE 22

#启动命令
CMD ["/run.sh"]
