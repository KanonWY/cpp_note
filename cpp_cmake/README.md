## CMake Cookbook阅读笔记

### 1、选择编译器
```bash
#1、在CLI中使用
cmake -DCMKAE_CXX_COMPILER=clang++
#2、通过导出环境变量 CXX (C++编译器)、 CC (C编译器)
env CXX=clang++ cmake ..
```
#### 编译器相关的预定义变量
`CMAKE_<LANG>_COMPILER_LOADED` :如果为项目启用了语言 `<LANG>` ，则将设置为`TRUE`。

`CMAKE__COMPILER_ID` :编译器标识字符串。`GCC`,`MSVC`,`AppleClang`。

`CMAKE__COMPILER_VERSION` :此变量包含一个字符串，该字符串给定语 言的编译器版本。

### 2、构建类型

CMAKE支持多种构建类型。

- Debug：用于在没有优化的情况下，使用带有调试符号构建库或可执行文件。 
- Release：用于构建的优化的库或可执行文件，不包含调试符号。 
- RelWithDebInfo：用于构建较少的优化库或可执行文件，包含调试符号。 
- MinSizeRel：用于不增加目标代码大小的优化方式，来构建库或可执行文件。

简单实用：使用缓存机制，在没有制定的情况下，默认生成`Release`版本。

```cmake
if(NOT CMAKE_BUILD_TYPE)
		set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE
)
```

在使用cmake指令时候切换：

```bash
cmake -D CMAKE_BUILD_TYPE=Debug ..
```

各种构建类型所对应的编译标志。

```cmake
message(STATUS "C flags, Debug configuration: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C flags, Release configuration: ${CMAKE_C_FLAGS_RELEASE}")
message(STATUS "C flags, Release configuration with Debug info:${CMAKE_C_FLAGS_RELWITHDEBINFO}")
message(STATUS "C flags, minimal Release configuration: ${CMAKE_C_FLAGS_MINSIZEREL}")
message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "C++ flags, Release configuration with Debug info:${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
```

Result:

```bash
[cmake] -- C flags, Debug configuration: -g
[cmake] -- C flags, Release configuration: -O3 -DNDEBUG
[cmake] -- C flags, Release configuration with Debug info:-O2 -g -DNDEBUG
[cmake] -- C flags, minimal Release configuration: -Os -DNDEBUG
[cmake] -- C++ flags, Debug configuration: -g
[cmake] -- C++ flags, Release configuration: -O3 -DNDEBUG
[cmake] -- C++ flags, Release configuration with Debug info:-O2 -g -DNDEBUG
[cmake] -- C++ flags, minimal Release configuration: -Os -DNDEBUG
```

### 3、设置编译选项

分为全局设置和制定目标设置。因为CMake将编译选项视为目标属性。因此，可以根据每个目标设置编译选项，而不需要覆盖CMake默认值。在编译的时候使用`cmake -D option`将影响全局。

#### 3.1 展示当前的编译选项

```cmake
message("C++ compiler flags: ${CMAKE_CXX_FLAGS}")
```

#### 3.2 针对单个目标`target_compile_options`

假如我们需要生成一个压缩静态库，然后给该库设置一些编译选项。并且有另一个文件处理程序，需要Link压缩库。

```cmake
#在变量flags中添加一些编译选项。
list(APPEND flags "-fPIC" "-Wall")
if(NOT WIN32)
		list(APPEND flags "-Wextra" "-Wpedantic")
endif()

# 构建一个静态库
add_library(kazlib STATIC
	xxx1.cpp
	xxx2.cpp
	xxx3.cpp
	xxx4.cpp
)
# 为该库设置编译选项
target_compile_options(kazlib
	PRIVATE
		${flags}
)

# 构建文件处理程序, kazlib链接到file_exe中
add_executable(file_exe file_exe.cpp)
target_compile_options(file_exe
	PROVATE
		"-fPIC"
)
target_link_libraries(file_exe kazlib)
```

可以看到，编译警告标志`-Wall`,`-Wextra`,`-Wpedantic`，`-fPIC`等标志添加到了`kazlib`的编译选项中。

针对单个目标的编译选项可以指定其可见行：

- PRIVATE：编译选项会引用给当前目标，但是不会传递跟object相关的object。
- INTERFACE：给定的编译选项会应用于指定的目标，并传递给与目标相关的目标。
- PUBLIC：编译选项将会应用于指定的目标和使用它的目标。

#### 3.3 查看构建细节

设置CMAKE的环境变量`VERBOSE=1`,在编译时指定该选项。

```bash
cmake -s . -B build
cmake --build build --VERBOSE=1
```

#### 3.4 全局指定编译选项

```bash
cmake -D CMAKE_CXX_FLAGS="-fno-exceptions -fno-rtti"
```

#### 3.5 调试数据库

```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
-DCMAKE_EXPORT_COMPILE_COMMANDS=ON
```

### 4、指定语言的标准

#### 4.1 常见全局设置：

```cmake
# 设置CXX标准
set(CMAKE_CXX_STANDARD 11)
# 不允许编译器扩展
set(CMAKE_CXX_EXTENSIONS OFF)
# 
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

#### 4.2 在生成目标时候设置目标属性

使用`set_target_properties`指定生成目标的语言标准。

```cmake
add_executable(test_model test_model.cpp)
set_target_properties(test_model
	PROPERTIES
		CXX_CTANDARD 14
		CXX_EXTENSIONS OFF
		CXX_STANDARD_REQUIRED ON
)
```

### 5、环境监测

#### 5.1 系统名称

CMakewe为目标操作系统定义了`CMAKE_SYSTEM_NAME`环境变量。对于`MacOS`该值为`Darwin`,对于`Linux`和`Windows`与其变量名称相同。

```cmake
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		message(STATUS "Configuring on/for Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		message(STATUS "Configuring on/for macOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		message(STATUS "Configuring on/for Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
		message(STATUS "Configuring on/for IBM AIX")
else()
		message(STATUS "Configuring on/for ${CMAKE_SYSTEM_NAME}")
endif()
```

#### 5.2 宏定义处理`target_compile_definitions`

可以根据该宏在C/CPP中配置跨平台代码。使用`target_compile_definitions`可以添加宏定义。

```cmake
add_executable(helloWorld helloWorld.cpp)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		target_compile_definitions(hello-world PUBLIC "IS_LINUX")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		target_compile_definitions(hello-world PUBLIC "IS_MACOS")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
		target_compile_definitions(hello-world PUBLIC "IS_WINDOWS")
endif()
```

与`target_compile_options`类似的，`target_compile_definitions`也有三个可见行的限定符：

- PRIVATE，编译定义将只应用于给定的目标，而不应用于相关的其他目标。 

- INTERFACE，对给定目标的编译定义将只应用于使用它的目标。

- PUBLIC，编译定义将应用于给定的目标和使用它的所有其他目标。

#### 5.3 处理器名称检查位数量

CMake设置了`CMAKE_HOST_SYSTEM_PROCESSOR`变量以包含当前运行的处理器的名称。

### 6、平台差异

- 要求在Windows上导出所有库符号

```cmake
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
```

### 7、引入三方库

常见的几种方式：1、使用git子模块的方式；2、使用find_package；3、使用pkg-config模块

#### 7.1 find_package

```cpp
find_package(packagename ...)
```
如果找到了设置以下几个变量:
```bash
NAME_FOUND  //true or false
NAME_INCLUDE_DIR
NAME_LIBRARIE
NAME_NEED_PREFIX
NAME_VERSION_STRING
```
以Boost为例,查看找到Boost之后的变量输出
```cmake
if(Boost_FOUND)
    message(STATUS "-----> Boost INFO:")
    # Boost头文件目录
    message(STATUS "Boost_INCLUDE_DIRS----->: ${Boost_INCLUDE_DIRS}")
    # Boost库文件目录
    message(STATUS "Boost_LIBRARY_DIRS----->: ${Boost_LIBRARY_DIRS}")
    # Boost库
    message(STATUS "Boost_LIBRARIES ----->  ${Boost_LIBRARIES}")
    # Boost 版本信息字符串
    message(STATUS "NAME_VERSION_STRING -----> ${Boost_VERSION_STRING}")

else()
    message(STATUS "-----> not found BOOST!")
endif()
```
使用`Boost`部分组件:
```cmake
# 按组件查找获取
find_package(Boost REQUIRED COMPONENTS compinentNames)
# 生成可执行目标文件
add_executable(testBoost BoostMain.cpp)
# 部分链接
target_link_libraries(testBoost
    PUBLIC
    Boost::compinentNames
)
```
讲`Boost`全部链接:
```cmake
find_package(Boost REQUIRED)
# 生成可执行目标文件
add_executable(testBoost BoostMain.cpp)
# 直接链接
target_link_libraries(testBoost ${Boost_LIBRARIES})
```
如果Boost没有安装在标准目录，需要自己指定boost安装的根目录`BOOST_ROOT`
```bash
cmake -DBOOST_ROOT=/custom/Boost
```
当然也可以直接指定BOOST的头文件目录与库文件目录:
```bash
cmake -D BOOST_INCLUDEDIR=/custom/boost/include -DBOOST_LIBRARYDIR=/custom/boost/lib
```

#### 7.2 pkg-conifg

pkg-config是向用户提供相应库的路径，版本号，头文件路径等信息的综合调用程序。
```bash
# 显示所有的可以基于此程序的库
pkg-config  --list-all
```
查看某一个库的链接信息：比如SDL2
```bash
╭─kanon@wangyudembp ~
╰─➤  pkg-config  --libs --cflags sdl2                                                           1 ↵
-D_THREAD_SAFE -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2
```
在`CMake`中同样可以基于`find_package`对`PkgConfig`进行处理。基本使用方法如下：
```cmake
# 查找PkgConfig，如果找不到会QUIET
find_package(PkgConfig REQUIRED QUIET)
```
下面以查找SDL2为例子
```cmake
find_package(PkgConfig REQUIRED QUIET)
pkg_search_module(
    SDL2
    REQUIRED
    libsdl2 SDL2
    IMPORTED_TARGET
)
if(TARGET PkgConfig::SDL2)
    message(STATUS "----> find SDL2")
    # 头文件目录
    message(STATUS "SDL2_INCLUDE_DIRS -----> ${SDL2_INCLUDE_DIRS}")
    # 库目录
    message(STATUS "SDL2_LIBRARY_DIRS-----> ${SDL2_LIBRARY_DIRS}")
    # 链接库
    message(STATUS "SDL2_LIBRARIES----->${SDL2_LIBRARIES}")
endif()

add_executable(testSDL2 testSDL2.c)
target_link_libraries(testSDL2 PkgConfig::SDL2)
```
