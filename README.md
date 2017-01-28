# js6chars
Transform a JavaScript code into an equivalent JavaScript code using only 6 characters.

Project implementation hightly relies on Google Chrome web-browser way of displaying its datatypes.
It is not supposed to work with other browsers as soon as you leave the ```-c <number>``` (with positive number) case.

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
- Letters from undefined:
 - ```[][0] === undefined```
 - ```[][+[]] === undefined```
 - ```[][+[]]+[] === "undefined"```
 - ```([][+[]]+[])[+[]] === "u"...```

## Sources

Project inspired by:
- http://slides.com/sylvainpv/xchars-js/
- https://syllab.fr/projets/experiments/sixcharsjs/
