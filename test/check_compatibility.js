function log_success(property_label) {
  console.log(property_label + ": OK");
  return 0;
}
function log_failure(property_label, value, expected, operator) {
  console.log(property_label + ": ERROR");
  console.log("  should have: value " + operator + " expected");
  console.log("  value    (" + typeof(value) + "): " + value + ".");
  console.log("  expected (" + typeof(expected) + "): " + expected + ".");
  return 1;
}

function check_ok(property_label, value, expected) {
  return value === expected
      ? log_success(property_label)
      : log_failure(property_label, value, expected, "===");
}
function check_nok(property_label, value, expected) {
  return value !== expected
      ? log_success(property_label)
      : log_failure(property_label, value, expected, "!==");
}
function truncate(input, max_length) {
  return typeof(input) !== typeof("") || input.length <= max_length
      ? input
      : input.substr(0, max_length);
}

if ([].find === undefined) {
  console.log("WARN  need to inject Array.prototype.find to have the right output");
  Array.prototype.find = function find() { return this.indexOf(lookfor) !== -1; };
}
if ([].entries === undefined) {
  console.log("WARN  need to inject Array.prototype.entries to have the right output");
  function ArrayIterator() {};
  ArrayIterator.prototype.inject = function() { return "[object Array Iterator]"; };
  ArrayIterator.prototype.toString = function() { return "[object Array Iterator]"; };
  Array.prototype.entries = function(lookfor) { return new ArrayIterator(); };
}
if ([].constructor.toString().indexOf("function Array()") !== 0) {
  console.log("WARN  need to inject Function.prototype.toString to have the right output");
  Function.prototype.inspect = function() { return 'function ' + this.name + '() {}'; };
  Function.prototype.toString = function() { return 'function ' + this.name + '() {}'; };
}
if (typeof atob === 'undefined') {
  console.log("WARN  need to inject atob");
  atob = function() { return "?C"; };
}

var num_errors = 0;
num_errors += check_ok("Representation of 0 using only +[]", +[], 0);
num_errors += check_ok("Representation of 1 using only +[]", ++[[]][+[]], 1);
num_errors += check_ok("Representation of 2 using only +[]", ++[++[[]][+[]]][+[]], 2);
num_errors += check_ok("Conversion to string using +[]", 0+[], "0");
num_errors += check_ok("Basic outputs: undefined", [][+[]]+[], "undefined");
num_errors += check_ok("Basic outputs: NaN", +"u"+[], "NaN");
num_errors += check_ok("Basic outputs: 1e+30", 1e30+[], "1e+30");
num_errors += check_ok("Basic outputs: 1e+1000", 1e1000+[], "Infinity");
num_errors += check_ok("Basic outputs: true", !![]+[], "true");
num_errors += check_ok("Basic outputs: false", ![]+[], "false");
num_errors += check_ok("Basic outputs: [object Array Iterator]", []["entries"]()+[], "[object Array Iterator]");
num_errors += check_ok("Basic outputs: function find()", truncate([]["find"]+[], 15), "function find()");
num_errors += check_ok("Basic outputs: Array constructor", truncate([].constructor+[], 16), "function Array()");
num_errors += check_ok("Basic outputs: Boolean constructor", truncate((![]).constructor+[], 18), "function Boolean()");
num_errors += check_ok("Basic outputs: Function constructor", truncate([].find.constructor+[], 19), "function Function()");
num_errors += check_ok("Basic outputs: Number constructor", truncate((0).constructor+[], 17), "function Number()");
num_errors += check_ok("Basic outputs: String constructor", truncate("".constructor+[], 17), "function String()");
num_errors += check_ok("toString in non-10 base works as expected", 11["toString"](12), "b");
num_errors += check_ok("atob works as expected", atob("20N")[1], "C");
num_errors += check_nok("fromCharCode defined on String", ([]+[])["constructor"]["fromCharCode"], undefined);
num_errors += check_ok("Retrieve comma", [[]].concat(0)+[], ",0");

process.exit(num_errors);

