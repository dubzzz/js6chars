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

## Sources

Project is inspired from:
- http://slides.com/sylvainpv/xchars-js/
- https://syllab.fr/projets/experiments/sixcharsjs/
