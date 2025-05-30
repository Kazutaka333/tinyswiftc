#!/bin/bash
assert() {
  input="$1"
  expected="$2"
  
  echo $1 > build/example.swift
  ./build/tinyswiftc build/example.swift || exit
  clang build/output.o -o build/output
  ./build/output
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert "0" 0
assert "42" 42

echo OK
