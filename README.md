# Die #
__A Language For the Aggressive Programmer__

Die is a language you can use to tell other programmers what you think of them, as well
as make them confused and angry as they try and parse out your code.
It's a simple language, based on the ideas of Brain^%3@.

I don't personally recommend using this language. But you're certainly welcome to do so!

### Building ###

This interpreter is written from scratch in C. Any standard compiler should do the job without
any issue. I use build scripts in the form of build.sh and build.bat myself.

### Specification ###

Comments are written like you're talking under your breath.

`
... This is a comment
... This is another comment
`

The central command of the language is die. Various forms of die incremnt the currently pointed
value in certain different ways.

`
die => +1
Die => +10
DIE => +100
`

Increasing the number of E's multiplies the incrementing value.

`
dieeee => +4
Dieeee => +40
DIEEEE => +400
`

For loops are done using `ok so` and `stop`
The number of o's in so are how many times it loops.

`
... loop 4 times
ok soooo
die die die die die
stop
`

result: +20 @ 0
