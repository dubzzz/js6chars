# js6chars [![Build Status](https://travis-ci.org/dubzzz/js6chars.svg?branch=master)](https://travis-ci.org/dubzzz/js6chars)
Transform a JavaScript code into an equivalent JavaScript code using only 6 characters.

Project implementation should be compatible with multiple web-browsers as long as they fit with the tricks decribed below.

## How to use it?

js6chars makes it possible to convert JavaScript data into
their equivalent versions using only a 6-character subset: []+()!

js6chars syntax:
```
-c <character> : string of one character
-n <number>    : number
-s <script>    : script
-t <text>      : string
```

Example:
```
:$ ./build/js6chars -c 0
 > +[]
```

## How does it work?

### Basic conversions in JavaScript

- conversion to Boolean: add ! in front of the data -- eg.: ```!1 === false```
- conversion to Number: add + in front of the data -- eg.: ```+"3" === 3```
- conversion to String: add +[] to dta -- eg.: ```undefined+[] === "undefined"```

### Trick by trick

- Numbers from 0 to 9:
 - ```+[] === 0```
 - ```++[[]][+[]] === 1```
 - ```++[++[[]][+[]]][+[]] === 2```
 - ...
 - ```++[N][+[]] === N+1```
- Numbers' string representation: ```++[[]][+[]]+[] === "1"```
- undefined: ```[][0] === undefined``` -- d,e,f,i,n,u
 - ```[][+[]] === undefined```
 - ```[][+[]]+[] === "undefined"```
 - ```([][+[]]+[])[+[]] === "u"...```
- ```NaN === +("u")``` -- a,N
- ```1e+308 === 1e308``` -- +
- ```function find().. { exact display depends on the browser } === []["find"]``` -- c,o,t,v, 
- ```true === !![]``` -- r
- ```false === ![]``` -- l,s
- ```function Number().. === (0).constructor === (0)["constructor"]``` -- b,m,r
- ```function Array().. === (0).constructor === (0)["constructor"]``` -- y,A
- ```function String().. === (0).constructor === (0)["constructor"]``` -- g,S
- ```function Function().. === (0).constructor === (0)["constructor"]``` -- F
- ```function Boolean().. === (0).constructor === (0)["constructor"]``` -- B
- Characters from a to z
 - ```"a" === 10["toString"](11)```
 - ```"b" === 11["toString"](12)```
 - ...
- ```"C" === Function("return atob(\"20N\")[1]")()```
- All other characters: ```([]+[])["constructor"]["fromCharCode"](\d+)```


## Sources

Project inspired by:
- http://slides.com/sylvainpv/xchars-js/
- https://syllab.fr/projets/experiments/sixcharsjs/
