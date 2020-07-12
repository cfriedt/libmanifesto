# LibManifesto

C and C++ bindings for parsing [Greybus](https://lwn.net/Articles/715955/) Manifest source files (.mnfs) and generating Greybus Manifest binary files (.mnfb).

The C and C++ APIs are meant to complement the [existing manifesto tool written in Python from Project Ara](https://github.com/projectara/manifesto).

# Code Health

[![Build Status](https://travis-ci.com/cfriedt/libmanifesto.svg?branch=master)](https://travis-ci.com/cfriedt/libmanifesto)
[![Codecov Status](https://codecov.io/gh/cfriedt/libmanifesto/branch/master/graph/badge.svg)](https://codecov.io/gh/cfriedt/libmanifesto)
[![Code Quality](https://api.codacy.com/project/badge/Grade/2591b5d32ac84f1897b4a7e8d45d1544)](https://www.codacy.com/app/cfriedt/libmanifesto?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=cfriedt/libmanifesto&amp;utm_campaign=Badge_Grade)

## Build Matrix

| (OS, Compiler)  | amd64             | ppc64le            | s390x             | arm64             |
|-----------------|-------------------|--------------------|-------------------|-------------------|
| (Linux, gcc)    | [![amd64][2]][1]  | [![ppc64le][3]][1] | [![s390x][4]][1]  | [![arm64][5]][1]  |
| (Linux, clang)  | [![amd64][6]][1]  | [![ppc64le][7]][1] | [![s390x][8]][1]  | [![arm64][9]][1]  |
| (macOS, clang)  | [![amd64][10]][1] |                    |                   |                   |
| (Windows, msvc) | [![amd64][11]][1] |                    |                   |                   |

[1]: https://travis-ci.com/cfriedt/libmanifesto
[2]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/1?use_travis_com=true
[3]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/2?use_travis_com=true
[4]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/3?use_travis_com=true
[5]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/4?use_travis_com=true
[6]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/5?use_travis_com=true
[7]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/6?use_travis_com=true
[8]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/7?use_travis_com=true
[9]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/8?use_travis_com=true
[10]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/9?use_travis_com=true
[11]: https://travis-matrix-badges.herokuapp.com/repos/cfriedt/libmanifesto/branches/master/10?use_travis_com=true

# Documentation

The Doxygen API docs are availble [here](https://cfriedt.github.io/libmanifesto/), or for individual language bindings

* [C](https://cfriedt.github.io/libmanifesto/libmanifesto_8h.html)
* [C++](https://cfriedt.github.io/libmanifesto/manifestoxx_8h.html)

# License

[![License: BSD](https://img.shields.io/badge/license-BSD-green)](https://opensource.org/licenses/BSD-3-Clause)

See the file [LICENSE](https://github.com/cfriedt/libmanifesto/blob/master/LICENSE)
