cmake_minimum_required(VERSION 3.5)

project(tristatekey-user-daemon LANGUAGES C)

add_executable(tristatekey-user-daemon main.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0 libgpiod)
target_link_libraries(tristatekey-user-daemon PkgConfig::deps)
