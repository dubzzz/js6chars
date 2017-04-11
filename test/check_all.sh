err=0

# Generate number

time ./test/check_output.sh -n 5    # one figure number is the base case for all the remaining
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -n 2017 # multiple figures numbers can be generated in various ways, it must remains a number
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -n -5   # negative numbers require a conversion to String to append '-'
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -n -271   # large negative number
if (( $? != 0 )); then err=1; fi;

# Generate character
# Various one-character tests to check various milestone values

time ./test/check_output.sh -c 5
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c f
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c a
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c e
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c +
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c c
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c r
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c s
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c B
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c z
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c C
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c Z
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c {
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c }
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c @
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c $
if (( $? != 0 )); then err=1; fi;

# Generate strings

time ./test/check_output.sh -t "30"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "1e30"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "find"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "constructor"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "fromCharCode"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "abcdefghijklmnopqrstuvwxyz"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "Hello"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t ",;:!?./*-+^_|@()[]{}#~&\$"
if (( $? != 0 )); then err=1; fi;

# Generate scripts

time ./test/check_output.sh -s "(function(){console.log('Hello');})()"
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -s "(function(name) { var today = new Date(); var cmas_yr = today.getMonth() == 11 && today.getDate() > 25 ? today.getFullYear()+1 : today.getFullYear(); var cmas = new Date(cmas_yr, 11, 25); console.log('Hello ' + name + ', today we are the ' + today.toLocaleDateString('en-US') + '. Time to wait before Christmas: ' + Math.ceil((cmas.getTime() - today.getTime())/(1000*60*60*24)) + ' days'); })('Test')"
if (( $? != 0 )); then err=1; fi;

exit $err
