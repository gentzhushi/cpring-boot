# Clang Boot - A toy backend framework in C

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)

A small REST API server written from scratch in C.

It has no web framework; just POSIX sockets, a hand-written HTTP request parser, and
[cJSON](https://github.com/DaveGamble/cJSON) for JSON bodies.

## Requirements

- A C compiler (`cc`, clang, or gcc)
- MacOS, Linux, or another OS that supports POSIX sockets
- Optionally, you may want `curl` to try out the APIs you create

## Building

In order to build this project, all you need is a C compiler. The build is
defined in `nob.c`. You compile it once:

```sh
cc nob.c -o nob
```
—then run it:
```sh
./nob
```
### Debugging:

Should you want to build in debug mode:
```sh
./nob debug                     # compiles with -O0 -g, runs with lldb
```
