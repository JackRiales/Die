# Die #
__A Language For the Aggressive Programmer__

Die is a language you can use to tell other programmers what you think of them, as well
as make them confused and angry as they try and parse out your code.
It's a simple language, based on the ideas of Brain^%3@.

I don't personally recommend using this language. But you're certainly welcome to do so!

### Building ###

This interpreter is written from scratch in C. Any standard compiler should do the job without
any issue. I use build scripts in the form of build.sh and build.bat myself.

## Specification ##

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

Increasing the number of E's multiplies the incrementing value.
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

## Example Program ##
``` TODO! ```
