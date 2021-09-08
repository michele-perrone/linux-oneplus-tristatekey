cmake_minimum_required(VERSION 3.5)

project(tristatekey-daemon LANGUAGES C)

add_executable(tristatekey-daemon main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0 libgpiod)
target_link_libraries(tristatekey-daemon PkgConfig::deps)


#find_library(GPIOD_LIBRARY NAMES libgpiod.so)

#set(LIBS ${GPIOD_LIBRARY})

#target_link_libraries(tristatekey-daemon ${LIBS})
