
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
# Since assert function checks exit code of compiled program, the possible exit code range is 0-255.
# If a negative value is returned as an exit code, the actual exit code will be 256 + <negative value>
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


assert "func main() -> Int { return 0 }" 0
assert "func main() -> Int {return 0}" 0
assert "func main ()->Int{return 0}" 0

# int
assert "func main() -> Int { return 42 }" 42
assert "func main( ) -> Int { return 255 }" 255

# binary op
assert "func main() -> Int { return 1 + 2 }" 3
assert "func main() -> Int { return 10 + 123 }" 133
assert "func main() -> Int { return 10 - 9 }" 1
assert "func main() -> Int { return 255 - 241 }" 14
assert "func main() -> Int { return 1 - 2 }" 255 # check comment on assert()
assert "func main() -> Int { return -2 }" 254 # check comment on assert()
assert "func main() -> Int { return 3 + -2 }" 1
assert "func main() -> Int { return 2 * 3 }" 6
assert "func main() -> Int { return 2 * 3 * 2}" 12
assert "func main() -> Int { return 2 * 3 - 2 }" 4
assert "func main() -> Int { return 10 + 2 * 3 - 2 }" 14
assert "func main() -> Int { return 10 + 2 - 3 * 2 }" 6
assert "func main() -> Int { return 10 / 2 }" 5

# comment
assert "func main() -> Int { // comment
return 3 }" 3
assert "func main() -> Int { // comment1
// comment2
return 3 }" 3
assert "func main() -> Int { // comment1
return 3 
// comment2
}" 3

# parenthese
assert "func main() -> Int { return (3) }" 3
assert "func main() -> Int { return (3 + 1) * 3 }" 12
assert "func main() -> Int { return 13 - (3 + 1) * 3 }" 1
assert "func main() -> Int { return (1 + 3) * (2 + 3 ) }" 20
assert "func main() -> Int { return 3 * (2 + (3 + 1)) }" 18

# comparison op
assert "func main() -> Int { return 1 == 1 }" 1
assert "func main() -> Int { return 1 == 0 }" 0


echo OK
