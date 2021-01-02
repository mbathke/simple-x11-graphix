# simple-x11-graphix

Compile with:

```sh
$ gcc -o xgraphic main.c -L/usr/X11R6/lib -lX11
```

or debug with

```sh
$ gcc -g  -Wall -fPIC -o xgraphic main.c -L/usr/X11R6/lib -lX11
$ gdb xgraphic
```
