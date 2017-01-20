
About the Yuni project
======================

[![Join the chat at https://gitter.im/libyuni/libyuni](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/libyuni/libyuni?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[![Join the chat at https://gitter.im/libyuni/libyuni](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/libyuni/libyuni?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)


Build Status
------------

| Platform  | Build Status |
| :------------- | :------------- |
| GNU/Linux, MacOS, (via travis-ci)  |   |
| Windows, `VS2015` (via appveyor)  | [![Build Status](https://ci.appveyor.com/api/projects/status/github/libyuni/libyuni)](https://ci.appveyor.com/project/milipili/libyuni)  |


What is Yuni ?
--------------

Yuni is a cross-platform C++ framework for developing high-level, complex and
fast applications. It is meant as a reliable and simple bridge between
different worlds, allowing to write all sorts of applications, while
letting the developer focus on her real work.



Target audience
---------------

The Yuni library and tools are meant to be used by developers who
already know C++, like it and want to use it for writing any kind of
application without worrying about the low-level aspects of the
language and the trouble of mixing many kinds of libraries to do all
the usual things any application requires.

Big applications always end up requiring performance optimization at
some point, multi-threading at some other, graphics, sound, input,
command line parsing, ... Yuni intends to provide all this in a portable
way with a single, easy-to-use, object-oriented API.


Supported compilers
-------------------

| libyuni  | Minimum Compiler Requirements |
| :------------- | :------------- |
| v2.x {`master`}  | `>=VS2015`, `>=g++-4.9`, `>=clang++-3.7` {C++14 required} |
| v1.x | `>=VS2010`, `>=g++-4.4` (*) |

**Special note**: `VS2012` and `g++-4.7` are not supported due (ICE on templates). Please consider to upgrade
