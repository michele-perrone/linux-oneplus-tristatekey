cmake_minimum_required(VERSION 3.5)

project(tristatekey-user-daemon LANGUAGES C)
add_executable(tristatekey-user-daemon tristatekey-user-daemon.c)

project(tristatekey-root-daemon LANGUAGES C)
add_executable(tristatekey-root-daemon tristatekey-root-daemon.c)

find_package(PkgConfig REQUIRED)
pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0 gio-2.0 libgpiod)
target_link_libraries(tristatekey-user-daemon PkgConfig::deps)
target_link_libraries(tristatekey-root-daemon PkgConfig::deps)
