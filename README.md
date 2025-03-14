# TreeFormula
## Description
This is an console application that allows interpreting and calculating the values of mathematical prefix expressions through their representation in the form of trees. 
The project was implemented as part of the “Effective programming techniques” subject at Wroclaw University of Science and Technology.

TreeFormula allows you to:
- parsing mathematical expressions written in prefix notation
- storing expressions in the form of a tree
- correcting incomplete expressions by completing them
- calculating expression values based on given variables
- combining expression trees according to specified rules

## Install and open
Clone the repository. Then open project and run the application.

## Functions and options

### Supported commands

```console 
enter <formula>
```
- loads a formula in prefix notation and builds a tree
```console
vars
```
- prints the list of variables in the current tree

```console
print
```
- prints out the current tree in prefix format

```console
print_rows
```
- prints out the current tree in rows

```console
comp <var0> <var1>...<varN>
```
- calculates the value of an expression for the given variables
```console
join <formula>
```
- appends a new expression to an existing tree

### Additional capabilities
- automatic correction of incorrect expressions by filling in missing elements
- support for mathematical operators +, -, *, /, sin, cos
- storage of results in template-based structures
- implementation of smart pointers for efficient memory management
- optimization of operations on objects through move semantics


## Preview

```console
> enter + a b
> print
+ a b
> vars
a b
> comp 2 3
5
> join * 2 - c 8
> print
+ * 2 - c 8 b
> print_rows
1: +
2: * b
3: 2 -
4: c 8

```

## Stack
- C++


