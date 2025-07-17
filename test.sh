
DEBUG_FLAG=""
if [[ "$1" == "--debug" ]]; then
    DEBUG_FLAG="--debug"
fi

if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    echo "Usage: ./test.sh [--debug] [--help]"
    echo "  --debug   Enable debug logging in tinyswiftc"
    echo "  --help    Show this help message"
    exit 0
fi

#!/bin/bash
assert() {
  input="$1"
  expected="$2"
  
  echo $1 > build/input.swift
  ./build/tinyswiftc $DEBUG_FLAG build/input.swift || exit
  clang build/output.o -o build/output
  ./build/output 
  actual="$?"
  
  if [ "$actual" = "$expected" ]; then  
    echo "🟢 $input => $actual"
  else
    echo "❌ $input => $expected expected, but got $actual"
    exit 1
  fi
  echo
}

assert "func main() -> Int {return 0}" 0
assert "func main() -> Int { return 42 }" 42
assert "func main( ) -> Int { return 255 }" 255
assert "func main() -> Int { return 1 + 2 }" 3
echo OK
