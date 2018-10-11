# Find The Bug Example
This program has a subtle bug that is outside the C code. The exercise is to find it.

First, use the commands `pcbt` to get a program counter backtrace to see if you can figure out what is going wrong.

You can use the command `valuestats` to get a report of the history of values taken on by all variables known in the debugging information.

The program will work if you:

1.	Change `printf` to `print` in findTheBugExample.c.

2.	Change the Makefile to link `libeOS.a` rather than linking `libmOS.a`.

From looking at the map file, you will be able to figure out where the function `print` is implemented. What is the root cause of the bug?
