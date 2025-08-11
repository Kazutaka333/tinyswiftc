
DEBUG_FLAG=""
if [[ "$1" == "--debug" ]]; then
    DEBUG_FLAG="--debug"
fi

if [[ "$1" == "--help" || "$1" == "-h" ]]; then
    echo "Usage: ./test.sh [--debug] [--help]"
    echo "  --debug   print out AST trees"
    echo "  --help    Show this help message"
    exit 0
fi

#!/bin/bash
assert() {
  input="$1"
  expected="$2"
  echo "$1" > build/input.swift
  ./build/tinyswiftc $DEBUG_FLAG build/input.swift
  clang build/output.o -o build/output
  ./build/output 
  actual="$?"
  
  if [ "$actual" = "$expected" ]; then  
    echo "🟢 $input ⇨ $actual"
  else
    echo "❌ $input ⇨ $expected expected, but got $actual"
    exit 1
  fi
  echo
}
# make clean && make
assert "func main() -> Int { return 0 }" 0
assert "func main() -> Int {return 0}" 0
assert "func main ()->Int{return 0}" 0
assert "func main() -> Int { return 42 }" 42
assert "func main( ) -> Int { return 255 }" 255
assert "func main() -> Int { return 1 + 2 }" 3
assert "func main() -> Int { return 10 + 123 }" 133
assert "func main() -> Int { return 10 - 9 }" 1
assert "func main() -> Int { return 255 - 241 }" 14
assert "func main() -> Int { return 2 * 3 }" 6
assert "func main() -> Int { return 2 * 3 * 2}" 12
assert "func main() -> Int { return 2 * 3 - 2 }" 4
assert "func main() -> Int { return 10 + 2 * 3 - 2 }" 14
assert "func main() -> Int { return 10 + 2 - 3 * 2 }" 6
assert "func main() -> Int { return 10 / 2 }" 5
assert "func main() -> Int { // comment
return 3 }" 3
assert "func main() -> Int { // comment1
// comment2
return 3 }" 3
assert "func main() -> Int { // comment1
return 3 
// comment2
}" 3


echo OK
