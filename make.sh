#!/bin/bash

TOOLS="indexer search"

function make_tools
{
  for tool in $TOOLS; do
    make -C "${tool}_src"
    cp "${tool}_src/${tool}" ./
  done
}

function clean_tools
{
  for tool in $TOOLS; do
    rm -f "${tool}"
    rm -f test_index
    make -C "${tool}_src" clean
  done
}

if [ "$1" == "make" ]; then
  make_tools
elif [ "$1" == "clean" ]; then
  clean_tools
fi

