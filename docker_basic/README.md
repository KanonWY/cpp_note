## Docker基础
### 1.dds_dockerfile
该镜像为`fast-dds`的开发环境镜像，用于测试DSS和研究DDS源码用。
### 2.ubuntu_ssh_dockerfile 
该镜像为Ubuntu20的开发环境镜像，用于基本开发，开箱即用。
### 3、build env
`build_env`是一个基于Centos7.9.2009的镜像构建文件，该镜像是为了快速构建内网C++开发环境，也是CI/CD中的项目构建镜像基础，该镜像中内置了以下基本内容：  
- llvm15套件
- oh-my-zsh
- vim8.2以及常见插件（包括YCM等自动补全插件）
- cmake、ninja等CPP构建套件

### Windows上Docker相关
（1）在windows10上不使用WSL的Docker性能比较低，如果不使用WSL的情况下，在构建容器的时候进行消耗CPU的操作（比如在一个镜像中源码构建llvm），会导致镜像构建时间过长。  
（2）Docker默认的安装位置为C盘，对于一些C盘不是很充足的情况下，可能会导致C盘资源紧张，因此需要将Docker从C盘迁移。  
（3）迁移C盘的Docker本质是构建软链接到其他盘。  
（4）如使用了wsl的Docker，会将wsl的默认镜像安装在`C:\Users\username\AppData\Local\Docker\wsl`下，这样也会导致C盘过于紧张。可以使用wsl的导出和导入指令，将Docker基于的wsl迁移。  
**管理员用户打开`cmd`执行以下操作可以切换wsl的路径**
```bash
# 查看docker所用的wsl
# 一般包含两个，且处于运行。
wsl -l -v
  NAME                   STATE           VERSION
* docker-desktop         Running         2
  docker-desktop-data    Running         2
  
# 1、首先停止Docker，然后停止wsl
wsl --shutdown

# 2、将两个wsl导出为tar包到指定目录
wsl --export 虚拟机名称 文件导出路径/虚拟机名称.tar

# 3、注销两个wsl，此操作会删除现有虚拟机的空间
wsl --unregister 虚拟机名称 

# 4、导入wsl到指令目录
wsl --import 虚拟机名称 目标路径 虚拟机文件路径 --version 2

# 5、重启docker即可。
```
（5）Docker的`daemon.json`文件位于`C:\Users\username\.docker\daemon.json`
