CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(netlink_socket)

set(CMAKE_C_STANDARD 11)


set(SRC src/main.c)

add_executable(${PROJECT_NAME} ${SRC})