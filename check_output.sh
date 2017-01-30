option=$1
code=$2

nodecmd=`which node || which nodejs`
js6code=`./build/js6chars "$option" "$code"`

echo "Function.prototype.inspect = function() { return 'function ' + this.name + '()'; }; console.log(typeof ($js6code)); console.log($js6code)";
echo "Function.prototype.inspect = function() { return 'function ' + this.name + '()'; }; console.log(typeof ($js6code)); console.log($js6code)" | $nodecmd > js6.out.txt;
if [ "$option" = "-n" ]
then
  echo "console.log(typeof $code); console.log($code)" | $nodecmd > expected.out.txt;
else
  echo "console.log(typeof \"$code\"); console.log(\"$code\")" | $nodecmd > expected.out.txt;
fi
diff js6.out.txt expected.out.txt

