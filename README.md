# lcalc
RPN calculator intented to be used for quick calculations.

## Operations and constants supported
### Binary operations
- +: a b + -> a + b
- -: a b - -> a - b
- \*: a b \* -> a\*b
- /: a b / -> a/b
- %: a b % -> a%b
- ^: a b ^ -> a^b

### Unary operations
- sin: x sin -> sin(x)
- cos: x cos -> cos(x)
- exp: x exp -> e^x
- ln: x ln   -> ln(x)

### Constants
- pi
- e

## Usage
Either by
```sh
$ echo <exp> | lcalc
```
or
```sh
$ lcalc
<exp>
```

### Example
```
$ echo "1 2 + ." | lcalc
3

# or
$ lcalc
1
2
+
.
3
```
