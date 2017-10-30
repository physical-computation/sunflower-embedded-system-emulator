# Find The Bug Example
This program has a subtle bug that is outside the C code. The exercise is to find it.

First, use the commands `pcbt` to get a program counter backtrace to see if you can figure out what is going wrong.

You can use the command `valuestats` to get a report of the history of values taken on by all variables known in the debugging information.

The program will work if you:

1.	Change printf to print

2.	Change the linking of libmOS to link libeOS

What is the root cause of the bug?
