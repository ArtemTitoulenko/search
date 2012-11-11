# Indexer

This is a rudimentary indexer. It recursively crawls directories, looking
for files, and indexes them in an index file. If only a single file is
given, then it indexes just that file.

The most important part to analyse is probably the way that the indexer
finds words and stores them, along with where other occurrences of that
word are found.

The main data structures that are used are a hash table and linked lists.
The hash table has, naturally, constant time inserts and look ups for
cases involving little to no colissions. If a collision were to occurr in
the keys, linked lists are employed to resolve the collission. This makes
the insert and look up times, theoretically, O(n). The hashing function
is adapted from the source code of the Lua language interpreter/JIT
compiler. It provides a fairly uniform distribution of keys so colissions
occurr rarely.

Writing to file is a linear operation, since it crawls through every
non-NULL node in the storage of the hash table and prints the entries as
it discovers them.

#### Authors: Artem Titoulenko, Jeff Adler
