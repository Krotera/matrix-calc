# Matrix Calculator

## Background
This command line utility adds (`+`), subtracts (`-`), or multiplies (`*`) two matrices.

## Instructions
In Linux, with g++ installed, run `make` in the `src` directory and then either:
- Run `./matrix_calc "<arg>"` (with the quotes!) for a single calculation where `<arg>` is a matrix expression
- Run `./matrix_calc` to enter several matrix expressions continuously

A matrix expression looks like
```
1 2, 3 4 + 5 6, 7 8
```
and is interpreted as an addition of two matrices:
```
[ 1 2 ]   [ 5 6 ]   [  6  8 ]
[ 3 4 ] + [ 7 8 ] = [ 10 12 ]
```

## License
[GNU General Public License v3.0](https://www.gnu.org/licenses/gpl-3.0.html)
