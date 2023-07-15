# Contents

[[Reflective (programming language)]]

# Information
**Data types** are setting of variables, which **data type they can contain**.
It need for **syntax and semantic analyzing**.

## Elementary data types

### Variable
**Data type (var)** can contain **each other data type** but semantic and syntax analyzing **can't find bugs and errors** with them.

For define data types in var you should use general method **type_of** to variable.

```cpp
var variable;
```

### Bool
**Data type (bool)** can contain **a bit of information: true (1) or false (0)**.
```cpp
bool variable;
```

### Char
**Data type (char)** can contain a **UTF-8 symbol or integer (-128...127)**.
```cpp
char variable;
```

### UChar
**Data type (uchar)** can contain a **UTF-8 symbol or integer (0...255)**.
```cpp
uchar variable;
```


### Int
**Data type (int) (default: int32)** can contain **a integer (Default: -2 147 483 648...2 147 483 647).**
```cpp
int variable;
int32 variable;
```

### UInt
**Data type (uint) (default: uint32)** can contain **a integer (Default: 0...4 294 967 295).**
```cpp
uint variable;
uint32 variable;
```

### Double
**Data type (double) (default: double64)** can contain **a double (Default: +/- 1.7E-308 ... 1.7E+308).**
```cpp
double variable;
double64 variable;
```

### Void
**Data type (void)** using for declare method as **procedure what isn't return an any variable**.
```cpp
void f(){}
```

### IntInf
**Data type (int_inf)** can contain an integer
```cpp
int_inf variable;
```

### String
**Data type (string)** can contain a string
```cpp
string variable;
```

## Complex data types
### Array
**Data type (array)** can contain **n count of other data type**.
```cpp
int[] variable = new int[n];
```

### Tree


### Class


### Struct
