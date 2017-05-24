#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>

JWindow win;
Graph	g;

main(){
  double xmax, ymax;
  char *msg="Click the Mouse, Please!!";
  
  // open window and put a graph on it.
  win.open(10,10)		            // open window
    .graph(g.open( 500, 50, 200, 40 ))      // open graph on window
    .map()			            // map window
    .mouseWait();             // wait for mouse click
  
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 )
    .axis()			// draw the x,y axis
    .text( 5, -15, msg )	// put message
    .flush()			// flush the graphic buffer.
    .mouseWait(); 		// wait for mouse click
  
  for( double i=1.0; i<=xmax; i+=0.1 ){
    g.color("blue")
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      .flush();		// wait until flushing the graphic buffer
    mouseBreak();		// break when mouse is clicked. this is MACRO
  }
  
  g.color("black")
    .text( 5, -15, msg)		// put text
    .mouseWait();		// wait for mouse
  
  // end  --------------------------------------------------------
  Xwin.close();			// end graphic
}
