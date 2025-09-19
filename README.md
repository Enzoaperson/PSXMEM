# PSXMEM
A Playstation Memory Card viewer for your terminal!

Compiling it is as easy as: "gcc -o psxmem psxmem.c ./shiftjs/shiftjs.c ./shiftjs/shiftjs.h"
Pretty neat

Using it is as simple as running the command in the terminal and pointing it to a valid PS1 memory card. Currently it supports raw memory card files (such as ones that come from a PS3) and PSP memory card files.

You can use some command line arguments after the base command to do some extra neat stuff:
D - Gives you some extra debug information (mostly usesful for development)
G - Disables showing the icons in the terminal
C - Along with the icon, a color palette strip is shown with all the 16 custom colors
S - Shows blocks that have been marked as deleted.

Currently the ShiftJS support is incomplete, so characters that aren't part of the latin alphabet and some symbols will not display in the names of certain save files :(. Although in the future I would like to either find a good library to replace my code or write my own if none exist.
