# js6chars
Transform a JavaScript code into an equivalent JavaScript code using only 6 characters.

Output has been tested against Google Chrome web-browser. Implementation relies on string generated by it. It is hightly possible that the code does not work on other web-browsers.

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
