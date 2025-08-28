# Power of matrix
So drunk me was watching 3Blue1Brown [video](https://youtu.be/O85OWBJ2ayo) about matrix exponential and thought to myself: "Damn, you can probably perform such exponentiation with bases other than $e$ using natural logarithm properties".

So I decided to write a little program for that.

## Math
Math algorithm for this is simple:
$e$ to the power of matrix can be defined by substituting the matrix into the Taylor series:

$e^M=\sum_{k=1}^{\infty} \frac{1}{k!} M^k$, where $M$ is a $n \times n$ matrix.

Casual power operation can be written like this:

$a^b=e^{b \ln{a}}$

By the way, we want to use negative numbers as bases too, so instead of the natural logarithm we'll use the complex logarithm, and while we are at it, why not use complex numbers as bases too:

$z^b=e^{b Log{z}}$

So, we can probably say that this should work:

$z^M=e^{M Log{z}}=\sum_{k=1}^{\infty} \frac{1}{k!} (M Log{z})^k$, where $M$ is a $n \times n$ matrix.

## How to use
Use your favorite compiler to compile the source code or download an executable from [releases](https://github.com/sashokdimasik/pow-of-matrix/releases).

You can provide I/O files as arguments upon launching program via command line. Example for latest Windows release:

```
pom-win-2.0.1-64 tests/test_-e2.txt output.txt
```

Also you can provide file names manually when program asks you for them.

Input file must contain:

1. Base. Real and imaginary part must be separated by comma.
2. Parameters: precision, max amount of iterations (must be $\ge 3$), matrix size. Before each parameter write a minus sign.
3. Power matrix. Every element is a complex number, so you must provide real and imaginary part separated by comma for each of them.

## Examples

### tests/test_e.txt

Let's run a little test to ensure that this stuff works like it should:

```
2.718282,0 -0.000001 -20 -2
0,3.141592 1,0
0,0        0,1.570796
```

So we have here a classic $e$ base and following matrix as a power:

$$\begin{bmatrix}
\pi i & 1 \\\
0       & \frac{\pi i}{2}
\end{bmatrix}$$

Program will print this in your terminal:

```
Base: 2.718282 + 0.000000 * i;
Desired precision: 0.000001
Power matrix:
(     0.000000,     3.141592) (     1.000000,     0.000000) 
(     0.000000,     0.000000) (     0.000000,     1.570796) 

[Reached desired precision! (0.000001)] [18 / 20 iterations performed]

The result:
(    -1.000000,     0.000000) (    -0.636620,     0.636620) 
(     0.000000,     0.000000) (     0.000000,     1.000000) 

The result is saved to "dimasik.txt"!
```

Looks very belivable. Assuming "dimasik.txt" is indeed what you called your output file, you can found your result in it:

```
(    -1.000000,     0.000000) (    -0.636620,     0.636620) 
(     0.000000,     0.000000) (     0.000000,     1.000000) 
```

### tests/test_-e2.txt

Let's take $-e^2$ as a base this time. Input file:

```
-7.389056,0 -0.000001 -100 -4
0.5,0  0,0  0,0  -0.0858076113,-0.0140860896
  0,0  0,0  0,0              0,0
  0,0  0,0  2,0              0,0
  0,0  0,0  0,0              5,0
```

The result:

```
(     0.000000,     2.718282) (     0.000000,     0.000000) (     0.000000,     0.000000) (   420.000000,    69.000000) 
(     0.000000,     0.000000) (     1.000000,     0.000000) (     0.000000,     0.000000) (     0.000000,     0.000000) 
(     0.000000,     0.000000) (     0.000000,     0.000000) (    54.598149,     0.000000) (     0.000000,     0.000000) 
(     0.000000,     0.000000) (     0.000000,     0.000000) (     0.000000,     0.000000) (-2.202646e+04,     0.000000) 
```

### tests/test_base1.txt

Input file:

```
1,0 -0.000001 -4 -3
  0,0   0,100   0,200
100,0 100,100 100,200
200,0 200,100 200,200
```

So we got boring base 1 here... Result is very predictable:

```
(     1.000000,     0.000000) (     0.000000,     0.000000) (     0.000000,     0.000000) 
(     0.000000,     0.000000) (     1.000000,     0.000000) (     0.000000,     0.000000) 
(     0.000000,     0.000000) (     0.000000,     0.000000) (     1.000000,     0.000000) 
```

### tests/test_i.txt

Okay let's try something cool as a base finally, like $i$. And let's make a ring of $i$ as our power.
Input file:

```
0,1 -0.000001 -15 -3
0,0 0,1 0,0
0,1 0,0 0,1
0,0 0,1 0,0
```

The result:
```
(     2.832266,     0.000000) (    -3.221635,     0.000000) (     1.832266,     0.000000) 
(    -3.221635,     0.000000) (     4.664533,     0.000000) (    -3.221635,     0.000000) 
(     1.832266,     0.000000) (    -3.221635,     0.000000) (     2.832266,     0.000000) 
```

Cool...

## Restrictions
You can't provide zero as a base, obviously.
