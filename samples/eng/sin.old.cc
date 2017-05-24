//
//	Obtain Phase Response Curve.
//
#include <stream.h>
#include <fstream.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <jkit/jgr.h>

inline void     beep(void)      { cout << "\7"; }

main(){
unsigned long	t, tmax, t2;
unsigned long gwidth, gheight;
double	h=0.01;
JWindow	win[2];
Graph gxt0, gxt1[2], gxt2[2], gxy;

//   initialize
tmax=1000;
gwidth = 300;	gheight = 50;

//open 1st window with 3 graphs
win[0].open( 10, 10 )               // open the 1st window win[0]
    .graph(gxt0.open( gwidth, 0, gheight, gheight )) // 1st graph 
    .graph(gxt1[0].open( gwidth, 0, 75, gheight ))   // 2nd graph gxt1[0] 
    .graph(gxt1[1].child( gxt1[0] ))// 2nd plot gxt1[1] on gxt1[0]
    .graph(gxt2[0].open( gxt0 ))    // 3rd graph gxt2[0]
    .graph(gxt2[1].child( gxt2[0] ))// 2nd plot gxt2[1] on gxt2[0]
    .map();
   
//open the 2nd window with 1 graph
win[1].open( 400, 10 )  // open the 2nd window win[1]
  .graph(gxy.open( 150, 150, 150, 150 ))
  .map()
  .flush();
sleep(1);
 
// set properties of graphs
int axisDigit=True;
gxt0.setRatio( tmax*h, 1.0, gwidth, gheight )//The value tmx*h is gwidth on screen...
    .setAxisWithDigit(axisDigit)    //make axis digits
    .axis();

gxt1[0].setRatio( tmax*h, 1.0, gwidth, gheight )
    .setAxisWithDigit(axisDigit)
    .axis();
gxt1[1].setRatio( gxt1[0] );          //The property of gxt1[1] is the same as gxt1[0]

gxt2[0].setRatio( tmax*h, 1.0, gwidth, gheight )
    .setAxisWithDigit(axisDigit)
    .axis();
gxt2[1].setRatio( gxt2[0] );

gxy.setRatio( 2.0, 2.0, 150, 150 )
    .setAxisWithDigit(axisDigit)
    .setXScaleFull()                 // make long scale along x axis
    .setYScaleFull()
    .axisBox()
    .color("red")
    .text(0,0,"Please click the mouse ...",CenterTop)
    .color("black");

Xwin.flush().mouseWait();

// main routine...
t2=0;
while(1){
// draw axis
    gxt0.clear().forget().axis();      //clear, forget last point and draw axis
    gxt1[0].forget().axis(); 
    gxt2[0].forget().axis();
//    gxt2[1].forget();
    gxy.clear().axisBox(); 
    Xwin.flush(); 

// drawing 
	for (t=0;t<tmax;++t){
		double realt,scrt;
		realt = h*t2; ++t2;
		scrt = h*t;
		double x,y,z;
		x=sin(realt); y=cos(realt); z=sin(2*realt)+cos(3*realt);
		gxy.pset(x,z);
		gxt0.dashLine( scrt, x );
		gxt1[0].dashLine( scrt, x );
		gxt1[1].line( scrt, y );
		gxt2[0].line( scrt, z);
		Xwin.flush();
      }
	//    	mouseBreak();
	break;
}
gxy.color("red")
	.text(0,0,"Please click the mouse ...",CenterTop);

beep();
Xwin.flush().mouseWait();
 
gxy.color("red")
	.text(0,-0.5,"Saving eps file...",CenterTop);
Xwin.flush();
sleep(1);
 
win[0].EPSOut("test.eps");
gxt1[0].EPSOut("test2.eps");
}
