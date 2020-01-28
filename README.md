# COOP

**COOP** is **C** **O**bject **O**riented **P**rogramming framework, developed by FineALGs (www.fineAlgs.com) with real time embedded systems in mind. 

COOP supports **Classes** (and auto destruction at end of scope), **Inheritance** (including virtual functions), **Exceptions**, and **Memory management** infrastructure that allows you to switch between heap and **pure stack** based memory management seamlessly.

COOP aims to give lightweight syntax and minimal performance overhead for achieving efficient and **safe** embedded code.

Currently this repo features a Visual Studio 2019 solution with 3 projects:
1. **COOP** : The COOP infrastructure
2. **COOP_C_TestProxy** : Since C and C++ are compiled differently, there are parts of the COOP infrastructure that can’t be used form within a C++ project. Therefore this “Proxy” project was created for writing some of the tests.
3. **Object_unitTest**: The COOP Unit testing project.

You can find usage tutorials in the wiki.

If you’d like to support future development or to get our help with your project, feel free to contact us at Dream@fineALGs.com

Have fun!

