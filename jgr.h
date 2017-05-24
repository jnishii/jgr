// ********************************************************************* 
// jgr.h:  Graphic Library ver.2.0 Time-stamp: <08/11/14 12:05:11 jun> 
// *********************************************************************
#ifdef ICC
#include <ostream>
#include <iostream>
#include <fstream>
#else
#include <fstream>
#endif
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#ifndef JGR_H
#define JGR_H	1

#include "jgrPS.h"

// ---------------------------------------------------------
const int MAX_STR_LEN         = 64;
enum { VERTICAL, HORIZONTAL };
enum { Vertical, Horizontal };
enum { SHORT, WHOLE, DASH  };
enum { SOLID_LINE, DOTTED_LINE, CENTER_LINE, DASHED_LINE };
enum { LEFT, RIGHT, CENTER, LEFTTOP, RIGHTTOP, CENTERTOP };
enum { Left, Right, Center, LeftTop, RightTop, CenterTop, RightCenter };
enum { NORM_WIDTH, THICK_WIDTH };
enum { Flat, Separate };
enum { Normal, Focused, Selected, InputArea, Modified };
enum { Func, Submenu, GetString, GetDouble };
enum { Temporal, Static };
enum { Draw, Erase }; //for cursor
enum { Append,Head,Tail,Fill}; // for Polygon
#define mouse_break();  {if(Xwin.mouse_press()==True)break;}
#define mouseBreak();  {if(Xwin.mouse_press()==True)break;}
inline int    ev2button(XEvent &ev) {return(ev.xbutton.button);}

// -----------------Prototype Declarations ---------------------
class XController;
class Axis;		// used in class Graph
class Graph;
class JWindow;
class Menu;
class MenuItem;
class InputItem;

extern	Display	*Disp;
extern int XMargin;
extern int MenuMargin;

// --------------Font Data Definition---------------
enum { Courier, Helvetica, Times, Symbol, Gothic, Mincho };
enum { Roman, Bold, Italic, Oblique, NONE };
//!! size \in { 8,10,11,12,14,17,18,20,24,25,34 };

struct FName { std::string psfont, xfont; };
struct FStyle { std::string psfont, xfont; };

static FName FontType[]={
	{ "Courier",	"-adobe-courier"	},
	{ "Helvetica",	"-adobe-helvetica"	},
	{ "Times", 	"-adobe-times"		}, 
	{ "Symbol", 	"-adobe-symbol"		},
	{ "Gothic", "-*-gothic"	},
	{ "Ryumin",	"-*-mincho"	}};

static FStyle FontStyle[]={
	{ "",		"-medium-r-normal--"	},
	{ "-Bold",	"-bold-r-normal--"	},
	{ "-Italic", 	"-medium-i-normal--"	}, 
	{ "-Oblique", 	"-medium-o-normal--"	}, 
	{ "", 	"-*-*-*--"	}};

class FontDB {
    public:
	FontDB() { initFlag=False; }
	int	initFlag;
	char*	psfont;
	int	size;
	XFontStruct* xfstruct; // menu fonts ... should be obsolete and use xfontset
	XFontSet xfset;        // graph fonts
};

void setFont( int ftype, int fstyle, int size, FontDB& fontdb );

// -----------------Classes---------------------
class XController
    {
    public:
        XController(void);
	XController& depend_wm(int flag=True); //default: WMflag=False
	void   close( void )	{ XCloseDisplay(Disp);	}
	XController&	flush( void )	{ XFlush( Disp ); return *this; }
	XController&	sync( void )	{ XSync(Disp,False); return *this; }
	int	mouse_press( void );
	XController& 	mouse_wait( void )
             { while(1){if(mouse_press()==True)break;} return *this; }
	XController& 	mouseWait( void ) {return mouse_wait();}
	int    expose( void );
        int    ev2button(XEvent &ev) {return(ev.xbutton.button);}
    };

extern XController	Xwin;

struct PolyPoint{
  double x,y;
  PolyPoint* p;
};

class Axis {
 public:
  Axis(){ ratio = 1.0; prop  = SHORT; offset=0.0; }
  int   len;	// Length of positive side of axis
  int	lenN;	// Length of positive side of axis
  int	StPoint;// in JWindow
  int	length(void)		{ return len+lenN;	}
  double ratio;  // scr/real
  double blank;  // blank of scale
  double prop;   // property of scale (WHOLE,SHORT)
  void	 autoScale(void);	// calculate the scale on axis
  double scaling(double x)	{ return x*ratio;	  }
  double offset;
  double ps(double x)		{ return x+(double)lenN; }
  double ps(int x)		{ return (double)x+(double)lenN; }
};

class PSHandler {
 char* str_dup(char* s);
 public:
  jgrData* gdata;
  void init(int w, int h, XColor fcol);
  int  out(std::ofstream& f, int xoffset, int yoffset );
  void addCell(int k, void *dt);
  void freeCell(void);
  void setColor(void);
  void newColor(XColor fxcolor);
  void newWidth(void);
  void newLine(double x, double y, double x2, double y2);
  void newLline(double x, double y);
  void newBox(int kind, double x, double y, double width, double height);
  void newCircle(int kind, double x, double y, double r );
  void newPolygon(int kind, double x[], double y[], int n);
  void newFont( char* fontname, int size );
  void newText(int kind, double x, double y, char* str);
  void newStatus(int status, int mode);
  void setLineType(int type); // type=Solid, Dash
  //	void initColor(void); //XEGcolor_init
};

class Graph{
  GC	 Gc;
  XColor fxcolor;
  XColor fxcolor_push;
  //  XColor bgcolor;
  XColor axiscolor;     // color for axis (default is black)
  JWindow*	Pwind;	// pointer to parent window

  FontDB ScaleFont;     // Font for axis numbers
  FontDB TextFont;      // Font for texts
  
  int	Index;		// index in family; Parent:Index=0 
                        // (used for automatic color setting)
  int	Nchild; 	// # of children (default 0)
  int	hasParentFlag;	// Flag to know if this graph iherits 
                        // from other or not. if the flag is True,
  Graph	*ParentGraph;	// the graph must have ParentGraph.
  Graph	**ChildGraph;	// Parents must know who is their child
                        // Child Graph[0]=this;	
  int	isNewGraphFlag;	// Flag to know if this graph needs new
			// space in window or not 
  int	isManualyLocatedFlag;//default False
  int	AxisWithDigitFlag;
  int	LastPointFlag;	// Last point is defined or not
  int	PolyStartFlag;	// Polygon was started or not
  //  PolyPoint *head, *cur;
  
  Axis	X, Y;
  int	LeftPosition;   // left position in JWindow
  int	TopPosition;	// top position in JWindow not used now
  int	BottomPosition;	// top position in JWindow
  
  double LastX,	LastY;	// lastpoint after scaling
  void 	xyscale( double x, double y, double& x2, double& y2 );
  int	ScrX(double x)		{ return X.StPoint+int(floor(x));}
  int	ScrY(double y)		{ return Y.StPoint-int(floor(y));}
  void	getNewGraph(void);
  int	TellBirth(Graph& child); // tell parents the birth of child
  
  void _scale( int direction, 
	       double x, double y, double x2, double y2, double blank );
  void _line(double x, double y, double x2, double y2);
  void _line(double x, double y);
  void _pset(double x, double y );
  void _rectangle( double l, double t, double w, double h ); 
  void _fill_rectangle( double l, double t, double w, double h );
  void _cross( double x, double y, double r );
  void _circle( double x, double y, double r );
  void _fill_circle( double x, double y, double r );
  void _text( double x, double y, char* msg, int position, FontDB& font );
  Graph&	_xaxis( double blank, int yoffset=0 );
  Graph&	_yaxis( double blank, int xoffset=0 );
  // for PS handling
  PSHandler	Ps;
 public:
  Graph( void );
  // for interaction with JWindow
  int 	width()	{ return X.length();	}
  int 	height()	{ return Y.length();	}
  int	left()		{ return LeftPosition;	}
  int	top()		{ return TopPosition; 	}
  int	bottom()	{ return BottomPosition;}
  void	set_win_attribute( JWindow* pwind ); //from JWindow
  void	set_location( int x, int y );	// from JWindow
  void	set_child_location( void );	// for chid graph
  void	set_child_size( void );		// for chid graph
  int	isManualyLocated(void)	{ return isManualyLocatedFlag;	}
  int	isNewGraph(void)	{ return isNewGraphFlag;	}
  int	hasParent(void) 	{ return hasParentFlag;		}
  // for users
  // -------initialization
  Graph& open(int sx, int sy, // Manualy located in window
	      int x_pos, int x_neg, int y_pos, int y_neg,
	      std::string fgclr="black" );
  Graph& open( int x_pos, int x_neg, int y_pos, int y_neg, 
	       std::string fgclr="black" );// Automaticly located in window
  Graph& open( Graph& g, std::string fgclr="black" );//new graph but imitate
  Graph& child( Graph& g, std::string fgclr=NULL );
  Graph& set_ratio( double realx, double realy, double scrx, double scry );
  Graph& set_ratio( Graph& g );
  Graph& set_ratio( void );
  //aliases
  Graph& setRatio( double realx, double realy, double scrx, double scry )
    { return set_ratio( realx, realy, scrx, scry );}
  Graph& setRatio( Graph& g ){ return set_ratio(g); }
  Graph& setRatio( void )    { return set_ratio(); }

  // -------writing axis and scale
  Graph& setTextFont( int ftype, int fstyle, int size );
  Graph& setScaleFont( int ftype, int fstyle, int size );
  Graph& useFont( FontDB& fontdb );
  Graph& scale( int direction, double x, double y, double blank );
  Graph& axis( void );
  Graph& axis_box( void );
  Graph& xaxis( double blank );
  Graph& yaxis( double blank );
  Graph& xaxis( void );
  Graph& yaxis( void );
  Graph& xoffset( double off ) { X.offset=off; return *this; }
  Graph& yoffset( double off ) { Y.offset=off; return *this; }
  Graph& set_axis_with_digit( int flag=True );
  Graph& set_scale_full( int flag=True );
  Graph& set_xscale_full( int flag=True );
  Graph& set_yscale_full( int flag=True );
  // aliases
  Graph& axisBox( void ) { return axis_box(); }
  Graph& setAxisWithDigit( int flag=True )
    { return set_axis_with_digit( flag ); }
  Graph& setScaleFull( int flag=True )
    { return set_scale_full( flag ); }
  Graph& setXScaleFull( int flag=True )
    { return set_xscale_full( flag ); }
  Graph& setYScaleFull( int flag=True )
    { return set_yscale_full( flag ); }

  // ------drawing items
  Graph& color( std::string clr="black" );
  Graph& setFgColor( std::string clr="black" ) {color(clr);return *this; };
  Graph& setAxisColor( std::string clr="black" );
  Graph& pushColor( std::string clr="black" );
  Graph& pushColor( XColor xclr );
  Graph& popColor(void);
  Graph& pset(double x, double y);
  Graph& set_solid_line( void );
  Graph& set_dash_line( void );
  Graph& set_double_dash_line( void );
  //alises
  Graph& setLineSolid( void ){ return set_solid_line(); }
  Graph& setLineDash( void ) { return set_dash_line(); }
  Graph& setLineDDash( void ){ return set_double_dash_line(); }

  Graph& line(double x, double y, double x2, double y2);
  Graph& line(double x2, double y2);	// line from last point

  Graph& dash_line( double x, double y, double x2, double y2 );
  Graph& dash_line( double x2, double y2 );
  //aliases
  Graph& dashLine( double x, double y, double x2, double y2 ) //alias
    { return dash_line( x, y, x2, y2 ); }
  Graph& dashLine( double x2, double y2 ) { return dash_line( x2, y2 ); }

  Graph& rectangle( double x, double y, double width, double height );
  Graph& cross( double x, double y, double r );
  Graph& circle( double x, double y, double r );

  Graph& fill_rectangle( double x, double y, double width, double height );
  Graph& fill_circle( double x, double y, double r );
  //aliases
  Graph& fillRectangle( double x, double y, double width, double height )
    { return fill_rectangle( x, y, width, height ); }
  Graph& fillCircle( double x, double y, double r )
    { return fill_circle( x,y,r ); }

  Graph& _poly( double x, double y, int flag=Append );
  Graph& poly( double x, double y );
  Graph& closePoly( double x, double y );
  Graph& fillPoly( double x, double y );

  Graph& forgetLastPoint(void)   { forget(); return *this; }
  Graph& forget(void); // forget Last Point
  Graph& text( double x, double y, char* msg, int position=Center );
  Graph& clear( void );
  Graph& flush( void ) { XFlush( Disp ); return *this; }

  //------mouse handling functions
  int    ev2button(XEvent &ev) {return(ev.xbutton.button);}
  double ev2x(XEvent &ev) {return((ev.xbutton.x-X.StPoint)/X.ratio);}
  double ev2y(XEvent &ev) {return(-(ev.xbutton.y-Y.StPoint)/Y.ratio);}
  int	 mouse_press( void );
  int    mouse_press( XEvent& ev );
  Graph&  mouse_wait()  
    {while(1){if(mouse_press()==True)break;} return *this;}
  Graph& mouse_wait( XEvent& ev ) 
    { while(1){if(mouse_press(ev)==True)break;} return *this;}
  //aliases
  int	 mousePress( void ) {return mouse_press(); }
  int    mousePress( XEvent& ev ) {return mouse_press(ev); }
  Graph&  mouseWait() { return mouse_wait(); }
  Graph&  mouseWait( XEvent& ev ) { return mouse_wait(ev); }

  //EPS
  int   PSflag;    // If True, EPS data is recorded.
  int   PSrecorded;// If PSflag is turned into True, PSrecorded becomes True
                   // and shows the existence of the data even after PSflag becomes False.
  Graph& EPSOn(){PSflag=True; PSrecorded=True; return *this;}
  Graph& EPSOff(){PSflag=False; return *this;}
  void voidPSmsg(char* f){
    printf("You called %s for Graph %d but EPS is not enabled...\n", f, Index);
    printf("If you want use EPS routine, please enable it by EPSon()!\n");
  }
  void voidPSmsg(string f){
    printf("You called %s for Graph %d but EPS is not enabled...\n", f.c_str(), Index);
    printf("If you want use EPS routine, please enable it by EPSon()!\n");
  }
  Graph& EPSOut(char* epsfname, 
	       int lmargin=LEFT_MARGIN, int bmargin=BOTTOM_MARGIN);
  void  EPSAppend(std::ofstream& epsf, 
		  int lmargin=LEFT_MARGIN, int bmargin=BOTTOM_MARGIN);
  void  resetEPS();
};

class JWindow
{
  XSizeHints	WHints;
  Window	Wind;
  int		Ngrp;
  int		Nmenu;
  Graph**	Pgrp;	// pointer to graph in the window
  Menu**	Pmenu;	// pointer to menu in the window
  int		Index;	// index of this JWindow
  int		BSFlag;	// Backing Store (Default True)
  void getNewWindow(void);
  void getNewPgrp(void);
  void getNewPmenu(void);
 public:
  JWindow();
  int	MenuMargin;
  
  //------for initialization
  JWindow& addLeftMargin(long xmargin) { XMargin+=xmargin; return *this;}
  JWindow& open( unsigned int sx, unsigned int sy, //Manualy design
		 unsigned int width, unsigned int height,
		 int bsflag=True );
  JWindow& open( unsigned int sx, unsigned int sy, //Automaticaly
		 int bsflag=True );
  JWindow& setPosition();

  XColor bgcolor;
  JWindow& setBgColor(std::string bgcolor);   // not supported for EPS output

  JWindow& depend_wm(int wmflag=False); //default: WMflag=True
  JWindow& graph( Graph& g );
  JWindow& menu( Menu& m );

  FontDB ScaleFont;     // Font for axis numbers
  FontDB TextFont;      // Font for texts

  JWindow& setTextFont( int ftype, int fstyle, int size );
  JWindow& setScaleFont( int ftype, int fstyle, int size );
  //------for window management
  JWindow& map( void );
  JWindow& unmap( void )  { XUnmapWindow(Disp,Wind); return *this;}
  JWindow& axis( void );
  JWindow& clear( void );
  JWindow& flush( void )  { XFlush( Disp ); return *this; }

  //------mouse handling functions
  int     mouse_press( void );
  int     mouse_press( XEvent& ev );
  JWindow& mouse_wait()  
    { while(1){if(mouse_press()==True)break;} return *this;}
  JWindow& mouse_wait( XEvent& ev ) 
    { while(1){if(mouse_press(ev)==True)break;} return *this;}
  //aliases
  int  ev2button(XEvent &ev) {return(ev.xbutton.button);}
  int  mousePress() { return mouse_press(); }
  int  mousePress( XEvent& ev ) { return mouse_press(ev); }
  JWindow&  mouseWait() { return mouse_wait(); }
  JWindow&  mouseWait( XEvent& ev ) { return mouse_wait(ev); }

  int expose( void );
  //------return information about this window
  Window getWindow( void ) 	{ return Wind;	}
  Window get_window( void ) 	{ return Wind;	}
  int	getIndex( void ) 	{ return Index; }
  int	get_index( void ) 	{ return Index; }

  void voidPSmsg(char* f){
    printf("You called %s for Window %d but EPS has not been recorded...\n", f, Index);
    printf("If you want use EPS routine, please enable it by EPSOn()!\n");
  }
  JWindow& EPSOn();
  JWindow& EPSOff();
  JWindow& EPSOut(char* epsfname);
};

//---------------------------- Definition for Menu ---------------------
#include <X11/keysym.h>
#include <string.h>

//---------------------------- MenuItem class -------------------------
// Data Member of Menu Class //
//
//
class ItemMenu{
 public:
  Menu  *menu;
};

class ItemFunc{
 public:
  void (*handler)(void*);
  void* arg;
};

class ItemGetString{
 public:
  InputItem* iw;
  std::string msg;
  std::string dflt;
  std::string value;
  ItemGetString()
    { /* msg=NULL; dflt=NULL; value=NULL; */ }
};

class ItemGetDouble{
 public:
  InputItem* iw;
  std::string msg;
  double dflt;
  double* value;

  ItemGetDouble(){/*msg=NULL;*/}
  void setValue(double val){*value=val;}
  double getValue(void){return *value;}
};

class MenuItem {
  friend class Menu;
  
  int  index; // index in parent menu
  int  left,top,width,height;
  int  inmargin;
  void *cell;
  std::string title;
  int Type; //Submenu,Func,...
  
 public:
  MenuItem(){left=0;top=0;width=0;height=0;inmargin=0;}
  MenuItem& setTitle(const std::string str);
  void checkMySize(Menu* menu, unsigned int& width, unsigned int& height,
		   int& margin);
  void setPosition(int x, int y, int w, int h, int m) //set position data in menu
    { left=x; top=y; width=w; height=h; inmargin=m; }
  void drawFrame(Menu* menu, int flag);
  void drawCursor(int pos,Menu* menu,int status=Draw);
  MenuItem& drawString(Menu* menu,int flag);
  void drawStatus(Menu* menu, int flag);
  void getGeometry(Menu* menu, int& x, int& y);//get geometry in Display
  void exec(Menu* menu);
  void act();
  void actUnmapMenu();
  void actButtonPress(Menu* menu);
  void actMotionNotify(Menu* menu);
  void actLeaveNotify(Menu* menu);
  void actSetNormal(Menu* menu);
  void actKeyPress(Menu* menu,XEvent ev);
  void actButtonPress();
};

//--------------------------- Menu class -----------------------------
// The object of this class is Menu Window object of X based on XLib.
// It show menu items and watches and handles button press event 
// on the own window.

extern FontDB MenuFont;

class Menu {
  friend class   MenuItem;
  friend class   InputItem;
  
  MenuItem*     item;
  int		hasParentFlag;	  // Flag to know if this menu is submenu or not
  int           Inum;             // Number of MenuItem in the menu
  int           ascent, descent;  // info about text
  
  int           lastItem;         // item previously focused
  int           notMappedYet;
  int           win_x, win_y;     // default geometry
  
  //Attributes of the menu
  int           Direction;    // Horizontal or Vertical
  int           mapType;      // Temporal,Static
  int           Style;        // Flat, Separate
  
  // for submenu
  Menu*         ParentMenu;   // Pointer to parent menu Menu[0]=this
  int           itemIndex;    // for submenu...: index in parent menu item
  
  //Attributes of the menu items
  unsigned int  item_w, item_h;
  std::string        title;
  
  //Attributes of menu Window
  JWindow*	Pwind;		// pointer to parent window
  Window        Wind;
  GC            Gc_normal, Gc_normal_Text, Gc_HI, Gc_LO, Gc_HI_Text, Gc_LO_Text;
  XSizeHints    WHints;
  long          ev_mask;
  
  // Handle Items...
  void makeNewItem(void);
  void execItem(int item);
  
  // Draw Menu Items...
  void drawStatus(int x, int y, int flag=Normal);
  void drawFrame(int x, int y, int width, int height, int flag=Normal);
  void drawString(int x, int y, std::string msg, int flag=Normal);
  void redraw();
  
 public:
  int  selected, mapped, moved;
  // for interaction with JWindow
  int 	width()	{ return WHints.width;	}
  int 	height()	{ return WHints.height;	}
  int   style()   { return Style; }
  
  Menu&	fitMenuInWindow( int width, int height, int flag );
  Menu& setSize();
  Menu&	setWinAttribute( Window& wind, int x, int y  );
  Menu& getPGeometry(int& left, int& top);
  Menu& getGeometry(int& left, int& top,
		    unsigned int& width, unsigned int& height,
		    unsigned int& border, int& direction);
  Menu& getItemPosition(int inum, int& x, int& y);
  Menu& retStrLen(std::string msg, unsigned int& width, unsigned int& height);
  Menu& Rectangle(int x,int y, unsigned int w,unsigned int h, int status);
  
  // for users
  Menu(int direction=VERTICAL, int maptype=Static, int style=Separate);
  ~Menu(){delete[] item;}
  Menu& open(int x=0, int y=0);
  
  Menu& setTitle(const std::string str);
  Menu& setType(int type=Static)     {mapType=type; return *this;}
  Menu& setStyle(int style=Separate) {Style=style; return *this;}
  Menu& setDirection(int direction=VERTICAL) {Direction=direction; return *this;}
  
  Menu& addItem(const std::string title, void (*func)(void*),void* arg=NULL);
  Menu& addItem(const std::string title, Menu& child);
  Menu& addSubmenu(const std::string title, Menu& child);
  Menu& addGetString(std::string value, std::string title, std::string dflt);
  Menu& addGetValue(double& value, std::string title, double dflt);
  Menu& addClose(void);
  Menu& addExit(void);
  
  int getItemNum(XEvent ev);
  int getDirection(void){return Direction;}
  
  Menu& map(int x,int y);
  Menu& map()       {map(win_x,win_y); return *this;}
  Menu& unmap();
  Menu& useFont( FontDB& fontdb, GC& gc );
  Menu& act();
  Menu& close();
};

extern void handler_exit(void* menu);
extern void handler_quit_menu(void* menu);

//---------------------- InputItem class --------------------------
class InputItem {
	friend class Menu;
	friend class MenuItem;

	std::string     message;
	unsigned int   width, height;
	void setCursor(Menu* menu, int status=Draw);

 public:
	InputItem()
	  {/*message=NULL; status=InputArea; input=NULL;retval=NULL;*/}

	int index;
	int     status;
	std::string   input;
	std::string   retval;

	InputItem& exec(XEvent ev);
	InputItem& getString(XEvent ev,std::string str);
	InputItem& getValue(XEvent ev,double& val);
	InputItem& setDefault(const std::string dflt);
};

#endif // end of #ifndef JGR_H
