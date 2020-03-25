#!/bin/bash

clang++ -Xclang -ast-print -fsyntax-only $@ | less
