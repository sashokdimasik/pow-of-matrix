# Power of matrix
So drunk me was watching 3Blue1Brown video about [matrix exponential](https://youtu.be/O85OWBJ2ayo) and thought to myself: "Damn, you can probably perform such exponentiation with bases other than $e$ using natural logarithm properties".

So I decided to write a little program for that.

## Math
Math algorithm for this is simple:
$e$ to the power of matrix can be defined by substituting the matrix into the Taylor series:

$e^A=\sum_{k=1}^{\infty} \frac{1}{k!} A^k$, where $A$ is a $n \times n$ matrix.

Casual power operation can be written like this:

$a^b=e^{b \ln{a}}$

So, we can probably say that this should work:

$a^B=e^{B\ln{a}}=\sum_{k=1}^{\infty} \frac{1}{k!} (B \ln{a})^k$, where $B$ is a $n \times n$ matrix.

## How to use
Use your favorite compiler to compile the source code or download an executable from [releases](https://github.com/sashokdimasik/pow-of-matrix/releases). Run the program via your local cmd or shell and provide an input file. Example for latest Windows release:

```cmd
pom_win64 input.txt
```

Input file must contain the base, matrix size, precision, max amount of performed calculations and matrix. Input file example:
```txt
2.718281828 5 0.000001 25
1 0 0 0 0
0 2 0 0 0
0 0 3 0 0
0 0 0 4 0
0 0 0 0 5
```
Program will print this in cmd:
```cmd
Base: 2.718282; Matrix:
1.000000 0.000000 0.000000 0.000000 0.000000
0.000000 2.000000 0.000000 0.000000 0.000000
0.000000 0.000000 3.000000 0.000000 0.000000
0.000000 0.000000 0.000000 4.000000 0.000000
0.000000 0.000000 0.000000 0.000000 5.000000
[Reached desired precision! (0.000001)] [23 / 25 calculations performed]
The result:
2.718282 0.000000 0.000000 0.000000 0.000000
0.000000 7.389056 0.000000 0.000000 0.000000
0.000000 0.000000 20.085537 0.000000 0.000000
0.000000 0.000000 0.000000 54.598150 0.000000
0.000000 0.000000 0.000000 0.000000 148.413159
The result is saved to 'output.txt'!
```
This one is boring, as it uses $e$ as a base.

More interesting input:

```txt
1 3 0.000001 10
69 96 42
11 22 33
24 69 0.5
```
Printed output:
```cmd
Base: 1.000000; Matrix:
69.000000 96.000000 42.000000
11.000000 22.000000 33.000000
24.000000 69.000000 0.500000
[Reached desired precision! (0.000001)] [2 / 10 calculations performed]
The result:
1.000000 0.000000 0.000000
0.000000 1.000000 0.000000
0.000000 0.000000 1.000000
The result is saved to 'output.txt'!
```

After computation program will save the result into the ```output.txt``` file.

## Restrictions
You can't provide zero as a base, obviously. Also you can't provide negative numbers as a base, since natural log of a negative number is not defined. Maybe in the future I will add complex numbers support for this.
