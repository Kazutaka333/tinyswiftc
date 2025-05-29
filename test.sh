#!/bin/bash
assert() {
  input="$1"
  expected="$2"
  

  ./build/tinyswiftc "$input" || exit
  ./output.o
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert "func main() -> Int { return 0 }" 0
assert "func main() -> Int { return 1 }" 42

echo OK
