# Contents

[[Interpreting stages]]

# Info
**Recursion parsing stage** is a stage of parsing a tree of modules.

**Parsing a tree of modules** involve a parsing a module and its dependents of other modules.

## Principles of recursion parsing stage
- It's **starting from main module** (with module id 1) to its dependents;
- [[Tables]] will **contain name of modules, methods, global variables and e.t.c.**;
- Calling **this stage for a specific module is accessed for interpreting stage**;
- It will **add removing variables line action** that are out of scope;
- It will **convert conditional and cyclic constructions to algorithms with jumping** between line operators;
- It will **convert expressins with calling a method to few line operators** where calling a method will separated into separate line operator without removing a stack returned variable.
