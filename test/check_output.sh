#!/bin/sh
option=$1
code=$2

nodecmd=`which node || which nodejs`

echo ""
echo "Command: check_output.sh $1 \"$2\""
js6code=`./build/js6chars "$option" "$code"`

echo "Length : ${#js6code}"
echo "Array.prototype.find = function find() { return this.indexOf(lookfor) !== -1; }; \
    function ArrayIterator() {}; \
    ArrayIterator.prototype.inject = function() { return '[object Array Iterator]'; }; \
    ArrayIterator.prototype.toString = function() { return '[object Array Iterator]'; }; \
    Array.prototype.entries = function(lookfor) { return new ArrayIterator(); }; \
    Function.prototype.inspect = function() { return 'function ' + this.name + '() {}'; }; \
    Function.prototype.toString = function() { return 'function ' + this.name + '() {}'; }; \
    atob = function() { return '?C'; }; \
    console.log(typeof ($js6code)); console.log($js6code)" | $nodecmd > js6.out.txt;
if [ "$option" = "-n" ]
then
  echo "console.log(typeof $code); console.log($code)" | $nodecmd > expected.out.txt;
elif [ "$option" = "-s" ]
then
  echo "console.log(typeof ($code)); console.log($code)" | $nodecmd > expected.out.txt;
else
  echo "console.log(typeof \"$code\"); console.log(\"$code\")" | $nodecmd > expected.out.txt;
fi
git diff --no-index js6.out.txt expected.out.txt

