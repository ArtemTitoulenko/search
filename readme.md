# Search

This is a tool that, using an index file created with `indexer`, evaluates
queries and shows the files where words were listed. It's a search engine.

# Design

The search tool is a simple REPL that uses an index file as a source. The
source is broken up into two parts. A function to create a hash table using
the contents of a given index file, and a REPL to parse and fulfil queries
using said hash table. Since many parts of the table are already fairly
modular, the only troubles were regarding how to impliment `  AND` and `OR`.

`OR` was the simplest. For every word after `so `, the REPL would look for
said word in the hash table, go through every listed file that it had occurred
in, and add it to a sorted list if the file was not in the list already.

`AND` required the use of a second sorted list. The first word would trigger
all of it's file occurrences to be added to list 1. Then the second, and every
subsequent word, would check all the files that the word exists in, and only
add them to the second list if they exist in the first. That way, only files
that have contained all the words are remaining.

The main data structures used were a sorted linked list that could become a
set, a hash table, and several specialized linked lists. The hash table houses
nodes that can form a linked list, to handle collisions, that each contained
a pointer to a linked list that listed all of the text files that contained the
word in their contents. The sorted linked list was used to uniquely add file
names to then later print out to the user.

# Valgrind Complaints

There are no valgrind complaints. Valgrind testing is part of the build
process.

# Profiling

I used valgrind to ensure that my code ran without memory leaks and errors.
After I had ensured that everything was in line, I used the callgrind tool
provided by valgrind ($ valgrind --tool=callgrind) to find the various costs
of calling the various functions in my code.

Unsurprisingly, various ld (linker) and libc (glibc) had created a lot of
overhead and accounted for more than half of the programs running cost. The
trade off is welcome, since libc and ld perform many useful tasks that have
been heavily optimized over the decades.

One of the clear targets to further optimize the program is finding a more
efficient way to use fscanf. Changes to the index file layout had cut down the
number of calls to fscanf, though I fear that the viability of optimizing
fscanf calls has reached a plateau.

Another optimization that comes to mind is using mergesort to sort the various
linked lists used in the application. For large numbers of strings, there is a
clear bottleneck in this area. Furthermore, indexing the file names as part of
the index\_file protocol, could lead to faster sorting. Number comparisons are
usually much faster than string comparisons.

These optimizations will be kept in mind during development in the near future.

##### Authors: Artem Titoulenko, Jeffrey Adler
