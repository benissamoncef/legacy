FROM docker-repo.vinci-autoroutes.com:18440/migrazur/legacy_migrazur:2.8

RUN yum -y install epel-release wget && \
    wget https://packages.erlang-solutions.com/erlang-solutions-2.0-1.noarch.rpm && \
    rpm -Uvh erlang-solutions-2.0-1.noarch.rpm && \
    yum -y install erlang && \
    yum -y install sudo && \
    curl -s https://packagecloud.io/install/repositories/rabbitmq/rabbitmq-server/script.rpm.sh | sudo bash && \
    yum clean all && \
    yum makecache && \
    yum repolist && \
    yum -y install rabbitmq-server && \
    chkconfig rabbitmq-server on && \
    chkconfig --list | grep rabbitmq && \
    rabbitmq-plugins enable rabbitmq_management && \
    service rabbitmq-server start && \
    rabbitmqctl add_user admin magistra && \
    rabbitmqctl set_user_tags admin administrator && \
    rabbitmqctl set_permissions -p / admin ".*" ".*" ".*" && \
    rabbitmqctl add_user magistra magistra && \
    rabbitmqctl set_permissions -p / magistra ".*" ".*" ".*" && \
    service rabbitmq-server restart && \
    yum -y remove cmake && \
    yum -y install gcc-c++ wget perl make zlib-devel && \
    wget --no-check-certificate https://www.openssl.org/source/openssl-1.1.1l.tar.gz && \
    tar xzf openssl-1.1.1l.tar.gz && \
    cd openssl-1.1.1l && \
    ./config --prefix=/usr/local/openssl --openssldir=/usr/local/openssl shared zlib && \
    make && \
    make install && \
    cd .. && \
    rm -rf openssl-1.1.1l openssl-1.1.1l.tar.gz && \
    wget https://github.com/Kitware/CMake/releases/download/v3.21.3/cmake-3.21.3-linux-x86_64.tar.gz && \
    tar xzf cmake-3.21.3-linux-x86_64.tar.gz && \
    mv cmake-3.21.3-linux-x86_64 /opt/cmake-3.21.3 && \
    ln -s /opt/cmake-3.21.3/bin/cmake /usr/local/bin/cmake && \
    rm cmake-3.21.3-linux-x86_64.tar.gz && \
    yum -y install git && \
    mkdir ~/development && \
    cd ~/development && \
    git clone https://github.com/alanxz/rabbitmq-c.git && \
    cd rabbitmq-c && \
    git checkout v0.13.0 && \
    export OPENSSL_ROOT_DIR=/usr/local/openssl && \
    cmake -H. -Bbuild && \
    cmake --build build && \
    cmake --build build --target install && \
    git submodule init && \
    git submodule update
CMD ["/bin/bash"]
