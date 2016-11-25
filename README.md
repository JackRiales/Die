# Die #
__A Language For the Aggressive Programmer__

Die is a language you can use to tell other programmers what you think of them, as well
as make them confused and angry as they try and parse out your code.
It's a simple language, based on the ideas of Brainfuck.

I don't personally recommend using this language. But you're certainly welcome to do so!

## Building ##

This interpreter is written from scratch in C. Any standard compiler should do the job without
any issue. I use build scripts in the form of build.sh and build.bat myself.

## Specification ##

Below is the language specification, as I've designed it. Some features in this specification __have not been implemented yet__. They're merely guidelines for what I want to have the language do. Just be aware!

### Cells ###
Programs in Die are made up of an array of _Cells_ and a _Pointer_ to those cells. Each cell starts out with a value of 0 and can be incremented and decremented. The pointer can be moved left and right, and values can be printed. That's all!

### Comments ###
Comments are written like you're talking under your breath.

`... This is a comment`

_Line comments are the only ones that are supported right now._
For funsies, how about just don't use comments?

### Increments & Decrements ###

The central command of the language is die, which is used to increment the current cell value. Various forms of die increment the currently pointed value in certain different ways.

```
die => +1
Die => +10
DIE => +100
```

Optionally, increasing the number of E's multiplies the incrementing value.
`dieeee => +4`

To decrement, the same principals are used but the word is `please`

```
please => -1
Please => -10
PLEASE => -100
```
and it can be multipled again with the number of E's
`pleeeease => -4`

### The Pointer ###
You can move the pointer left and right by using `.` and `go`.

```
go die. => Move to cell 1 and increment by 1, then go back to cell 0.
```

### Loops ###

~Not yet implemented~

While loops are done by using `ok` and `stop`. Whiles are terminated when the current cell is 0. Be aware, this makes it really easy to make infinite loops, by accident.

```
ok
please
die die die
stop
```

For loops are done using `ok so` and `stop`. The number of o's in `so` are how many times it loops.

```
... loop 4 times
ok soooo
die die die die die
stop
```

### Printing ###
Printing can be done in two ways:

`sorry` prints the ascii char of the currently pointed-to variable,
`Sorry` (capitalized) prints the integer value.

### Debug Mode ###
Running die and passing in `--debug` activates debug mode. This will show the read-in source and each operation that is run, in order, as they are done. Excellent for seeing what's really going on! (thanks to [darkfire000](https://github.com/darkfire000) for adding this).

## Example Program ##
See "basic.die"
