# Contents
- [[Reflective (programming language)]];
- [[Expression and variables]];
- [[Tables]];
- [[Interpreting stages]].

[[Home]]

# Info
**Compiler type interpreter** is module for parsing, analyzing and executing algorithms.

***Compiler type interpreter*** is a system consists of ***compiler***, transforming source code to intermediate code, example bite-code or p-code, and ***interpret***, that execute got intermediate code (***virtual machine***).

## Principles of interpreter
- Has methods for modifing code in interpreting stage by **unload, changing then loading changed module of code**;
- Has way to **determinate changed of code** (may be code will have integer of changing for ***determinate current version in interpretator***);
- Has parser, syntax and semantic analyzer methods for checking correction of code;
- Has a methods for working with **an other system modules only in specific code modules**;
- Has a method for **reloading interpreter without disconnect a client**;
- Has a **static** variable of [[Database management system|DBMS class]];
- Has a variable of [[Network module|network interface]] for communicating with a client.