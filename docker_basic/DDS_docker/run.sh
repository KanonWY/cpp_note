#!/bin/bash

/usr/sbin/sshd -D

#安装插件
function install_vim_plugin()
{
    vim -c "PlugInstall" -c "q" -c "q"
}

install_vim_plugin()
ehoc "End if install"
