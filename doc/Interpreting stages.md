# Contents
**Stages:**
- [[Recursion parsing interpreting stage]];
- [[Semantic checking and interpreting stage]];
- [[Optimizing stage]];
- [[Interpreting stage]];
- [[Clearing stage]].

[[Compiler type interpreter]]

# Info
**Interpreting stages** is stages, in which code is transforming from current form to other form.

## Interpreting stages
***Additional stages can be avoided.***

- **Recursion parsing interpreting stage**. In this state all source code is transforming to structure of modules consists of tables with intermediate code:
	- **Parsing interpreting stage**. In this stage one module of source code is transforming to module consists of tables with intermediate code.
- **Semantic checking and interpreting stage** (*additional*). In this stage intermediate code is checking for semantic error with data types, function names, arguments, availability, and its;
- **Optimizing stage** (*additional*). In this stage intermediate code is optimizing for more faster executing;
- **Interpreting stage**. In this stage intermediate code is executing by interpreter.

After all stages last stage is **clearing stage**, in which data for interpreting is clearing from memory;