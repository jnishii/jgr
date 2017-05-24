# C++ graphic library JGR

## What is JGR?
JGR is a simple C++ class library for graphics on X11 with following features.

## Characteristics of JGR

-JGR can handle multiple windows with multiple graphs by easy opearation.

-JGR can draw axis, points, lines, rectangles, circles and so on.

-JGR has an automatic scaling function. When we draw a graph, the
real values, e.g. [m] and [kg], which we want to draw are different
from the value [dots] on the screen. Jgr automatically convert the 
real value to the dots and draws items by setting the relation as a
initial configuration.

-JGR uses Xlib library, but no need to know about it.

-JGR also outputs EPS files.

-JGR can make menus... However, it requires high load on PC^^;;;

## License
Follows LGPL except the PS handling routine which follows
XeasyGraphic-Copyright. PS handling routine is defined by jgrPS.cc
XeasyGraphic-Copyright permits the use on individual, education and
academic research. 

## Manual
No English manual has not been prepared yet. But, I think (hope)
you'll understand the basic programming method from sample programs.

## Acknowledgement
PS handling routine is modified from XeasyGraphic (Ver.1.01,
ogihara@itc.aist-nara.ac.jp). Thank you very much!

## Bugs!?

Welcome to your bug report and patches!

## Contact
Welcome to your bug report and patches!
Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 
http://bcl.sci.yamaguchi-u.ac.jp/~jun/en/

