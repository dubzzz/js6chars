err=0

time ./test/check_output.sh -n 5  # number_repr is the base case
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c 5  # char_repr @ case # 1 -- from number_repr
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c f  # char_repr @ case # 2 -- from undefined
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c a  # char_repr @ case # 3 -- from NaN
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c +  # char_repr @ case # 4 -- from 1e+308
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c c  # char_repr @ case # 5 -- from function
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c r  # char_repr @ case # 6 -- from true
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c s  # char_repr @ case # 7 -- from false
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c B  # char_repr @ case # 8 -- from constructor name
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c z  # char_repr @ case # 9 -- from a-z
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c C  # char_repr @ case #10 -- from atob("20N")
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -c Z  # char_repr @ case #11 -- from fromCharCode
if (( $? != 0 )); then err=1; fi;

time ./test/check_output.sh -t "Hello"
if (( $? != 0 )); then err=1; fi;


exit $err
