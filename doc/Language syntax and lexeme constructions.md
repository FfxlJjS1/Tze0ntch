# Contents

[[Reflective (programming language)]]

# Info
**Language syntax and lexeme constructions can be separated to tree.**

Program consists of modules of code.

A module have global area that can have code for importing module; define global variable, structure.

## Language syntaxes
\<Module\>:
```
{[<Module importino>] | [<Variable definition>] | [<Method definition>] | [<Structure difinition>] | [...]]}
```

\<Module importing\>:
```
{using "{<Identifier>}"}
```

\<Variable definition\>:
```
{<Data type> <Identifier> [<Assignment definition>] [, <Variable assigment>];}
```

\<Method definition\>:
```
{<Method head definition> <Method body definition>}
```

\<Structure definition\>:
```
{<Struct head definition> <Struct body definition>}
```

\<Assignment definition\>
```
{= {<Identifier> | <Literal>}}
```

\<Data type\>:
```
{<Elementary data type> | <Identifier> /*Struct identifier*/}
```

\<Identifier\>:
```
{{_ | <Number | String>} [...]}
```

\<Literal\>:
```
{Number | Text | Symbol | Boolean}
```

\<Number\>:
```
{0-9...[, 0-9...]}
```

\<Text\>:
```
{\"{<String>}\"}
```

\<Symbol\>:
```
{'{<Char>}'}
```

\<String\>:
$$
x \in Chars \times Chars
$$

\<Boolean\>:
```
{true | false}
```

Code example:
```js
using "Basic DBMS"

/*
Global variable
*/
bool t = false, b = false;

struct GG {
	int32 i = 0; // Default value
	intInf r = 0;
	char8 sym = 'c';
	char16 sym2 = 2023;
	string res = "ffff";
}

string process(string text) {
	return text;
}

void main() {
	string f = "ssft";

	if (t && !b){
		f = "f";
	}
	elif (b || f == "") {
		f += "32";
	}
	else {
		t = toBoolean(f);
	}
	
	GG g;
	
	g.res = process(f);
	
	return;
}
```