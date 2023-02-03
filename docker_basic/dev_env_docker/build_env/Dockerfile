FROM centos:centos7.9.2009

USER root

WORKDIR /root/

ENV PATH /opt/rh/devtoolset-7/root/usr/bin${PATH:+:${PATH}}
# ENV PATH /opt/rh/llvm-toolset-7/root/usr/bin:/opt/rh/llvm-toolset-7/root/usr/sbin${PATH:+:${PATH}}
ENV PATH /opt/rh/rh-git227/root/usr/bin${PATH:+:${PATH}}
ENV PATH /opt/rh/httpd24/root/usr/bin:/opt/rh/httpd24/root/usr/sbin${PATH:+:${PATH}}
ENV LIBRARY_PATH /opt/rh/httpd24/root/usr/lib64${LIBRARY_PATH:+:${LIBRARY_PATH}}
ENV LD_LIBRARY_PATH /opt/rh/httpd24/root/usr/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

ENV PERL5LIB /opt/rh/devtoolset-7/root//usr/lib64/perl5/vendor_perl:/opt/rh/devtoolset-7/root/usr/lib/perl5:/opt/rh/devtoolset-7/root//usr/share/perl5/vendor_perl${PERL5LIB:+:${PERL5LIB}}

ENV LD_LIBRARY_PATH /opt/rh/devtoolset7/root$rpmlibdir$rpmlibdir32${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
ENV LD_LIBRARY_PATH /opt/rh/devtoolset-7/root$rpmlibdir$rpmlibdir32:/opt/rh/devtoolset-7/root$rpmlibdir/dyninst$rpmlibdir32/dyninst${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

# ENV LD_LIBRARY_PATH /opt/rh/llvm-toolset-7/root/usr/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
ENV PERL5LIB /opt/rh/rh-git227/root/usr/share/perl5/vendor_perl${PERL5LIB:+:${PERL5LIB}}
ENV LD_LIBRARY_PATH /opt/rh/httpd24/root/usr/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}


RUN yum install -y wget zsh make unzip && \
    yum clean all && \
    yum makecache && \
    yum install -y centos-release-scl xz-devel && \
    yum install -y devtoolset-7 rh-git227 && \
    wget http://192.168.1.11:8929/root/tools/-/raw/main/cmake-3.24.3-linux-x86_64.tar.gz && \
    tar -zxvf cmake-3.24.3-linux-x86_64.tar.gz && \
    mv cmake-3.24.3-linux-x86_64 cmake && \
    cp -r cmake /usr/local/ && \
    rm -rf ~/cmake && \
    wget https://github.com/ninja-build/ninja/releases/download/v1.11.1/ninja-linux.zip && \
    unzip ninja-linux.zip && cp ninja /usr/bin && \
    ln -s /usr/local/cmake/bin/cmake /usr/bin/cmake 

RUN sh -c "$(curl -fsSL https://gitee.com/Devkings/oh_my_zsh_install/raw/master/install.sh)" && \
    git clone http://192.168.1.11:8929/root/ka_vim.git && \
    cd  ~/ka_vim/ && git checkout v_1 && \
    ./install.sh && \
    rm -rf ~/ka_vim && \
    cd /usr/local && git clone https://gitee.com/mirrors/LLVM.git  && \
    cd LLVM && git checkout llvmorg-15.0.0 && \
    cmake -S llvm -B build \ 
    -DCMAKE_BUILD_TYPE=Release \
    -DLLVM_ENABLE_PROJECTS="clang;lldb;clang-tools-extra"  \
    -DLLVM_TEMPORARILY_ALLOW_OLD_TOOLCHAIN=true \
    -G "Ninja" && \
    cmake --build build -j16 && \
    cmake --build build --target install 

RUN rm -rf ~/cmake-3.24.3-linux-x86_64.tar.gz ninja ninja-linux.zip && \
    rm -rf /usr/local/LLVM