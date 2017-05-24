#include <stream.h>
#include <iostream.h>
#include <math.h>
#include <jkit/jgr.h>

extern "C" { double drand48(); }
JWindow win;
Graph	g;

main(){
  long i;
  double xmax, ymax, h;
  char *msg="Click the Mouse, Please!!";
  
  // open window and put a graph on it.
  win.open(10,10)		        // open window
    .graph(g.open( 500, 50, 200, 40 ))	// open graph on window
    .map()				// map window
    .mouseWait(); 		// wait for mouse click
  
  xmax = 10.0;	ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 );
  // set the scaling parameters of the above graph
  // On x-axis, real value 10.0 is 500 dots on screen
  // On y-axis, real value 100.0 is 200 dots on screen
  
  // write axis --------------------------------------------------------
  g.axis()			// draw the x,y axis
    .text( 5, -15, msg )	// put message
    .flush()			// flush the graphic buffer.
    .mouseWait(); 		// wait for mouse click
  
  // draw axis with box -----------------------------------------------------
  g.axis_box()			// draw axis sourouded by box
    .text( 5, -15, msg )	// put message
    .flush()			// flush the graphic buffer.
    .mouseWait(); 		// wait for mouse
  
  // draw lines -----------------------------------------------------
  for( double i=1.0; i<=xmax; i+=0.1 ){
    g.color("green")
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      .text( 5, -15, msg )	// put message
      .flush();		        // wait until flushing the graphic buffer
    mouseBreak();		// break when mouse is clicked. this is MACRO
  }
  
  g.color("black")
    .text( 5, -15, msg )	// put text
    .mouseWait(); 		// wait for mouse
  
  // draw circle --------------------------------------------------------
  double r=1.0;			// set radius
  h=0.1;
  for( i=1; h*(double)i+2.0*r <=xmax; ++i ){
    g.clear()
      .axisBox()		// draw axis (see above subroutine)
      .color("red")
      .circle( h*(double)i+r, 25.0, r )
      .flush()		// wait until flushing the graphic buffer
      .color("black");
    for(int j=1; j<2000000; ++j ); // wait a moment...
    mouseBreak();		// break when mouse is clicked
  }
  
  g.color("red")
    .fillCircle( h*(double)(i-1)+r, 25.0, r )
    .color("black")
    .text( 5, -15, msg)		// put text
    .flush()
    .mouseWait(); 		// wait for mouse
  
  // draw rectangle --------------------------------------------------------
  double width = 2.0;
  double height = 20.0;
  h=0.05;
  for( i=1; h*(double)i+width <=xmax; ++i ){
    g.clear()
      .axisBox()		// draw axis (see above subroutine)
      .rectangle( h*(double)i, height, width, height )
      .flush();		        // wait until flushing the graphic buffer
    mouseBreak();		// break when mouse is clicked
    for(int j=1; j<1000000; ++j ); // wait a moment...
  }
  
  g.fillRectangle( h*(double)(i-1), height, width, height )
    .mouseWait(); 		// wait for mouse
  
  // draw pixels --------------------------------------------------------
  g.clear()
    .setScaleFull()
    .axisBox();		// draw axis with box
  h=0.0002;
  for( i=1; i <=(int)(1.0/h); ++i ){
    g.color("blue")
      .pset( (1-i*h)*xmax*drand48(), (1-i*h)*ymax*drand48() )
      .flush();			// wait until flushing the graphic buffer
    mouseBreak();		// break when mouse is clicked
  }
  
  g.color("black")
    .text( 5, -15, msg)		// put text
    .mouseWait();		// wait for mouse
  
  // end  --------------------------------------------------------
  Xwin.close();         	// end graphic
}
