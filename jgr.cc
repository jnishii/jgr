// *********************************************************************
// jgr.cc:  Graphic Library ver.2.0 Time-stamp: <08/11/14 17:31:35 jun> 
// *********************************************************************

//#define DEBUG

#ifdef ICC
#include <iostream.h>
#include <iostream>
#include <fstream>
#include <new.h>
#include <cmath>
#else
#include <iostream>
//#include <stream>
#include <fstream>
#include <new>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
//extern "C" int printf (std::string, const std::string, ...);
//extern "C" int sprintf (std::string, const std::string, ...);
#endif

#include "jgr.h"

// ******************* Global Variables ******************
XController	Xwin;
Display*       	Disp;
Colormap 	Cmap;
int		Nwin=-1;
Window**	Win;
int		WMFlag;
int	XMargin=30;
int	YMargin=30;
int	EPSXMargin=30;
int	EPSYMargin=30;

const int MaxColor	= 15;
extern const int MaxColorVal	= 65536;
FontDB MenuFont;

template <class T> T maxof(T x, T y) { return ((x>y) ? x:y); }

// ******************* prototype declaration ******************
class	XController;
class	Graph;
class	JWindow;

// ******************* for default color mapping **************
std::string color_index[]={"black", "blue", "red", "yellow", "green", "cyan",
		     "magenta", "brown", "lightgray", "darkgray",
		     "lightblue", "lightgreen", "lightcyan",
		     "lightred", "lightmagenta"};


//obtain geometry of the window in main Display
void getWinGeometry(Window Wind, int& MainWinX, int& MainWinY)
{
  int win_x, win_y, done = False;
  unsigned int win_w, win_h, win_brdr_w, win_d, num_child;
  Window win, root_win, parent_win, *child_wins;
  
  win=Wind;
  MainWinX = MainWinY = 0;
  while (!done) {
    XGetGeometry(Disp, win, &root_win, &win_x, &win_y, &win_w,
		 &win_h, &win_brdr_w, &win_d);
    MainWinX += win_x;
    MainWinY += win_y;
    if (XQueryTree(Disp, win, &root_win, &parent_win, &child_wins,
		   &num_child) == 0) {
      return;
    }
    if (child_wins != NULL) XFree((void *)child_wins);
    if (parent_win == DefaultRootWindow(Disp)) {
      done = True;
    } else {
      win = parent_win;
    }
  }
}


void setFont(int type, int style, int size, FontDB& fontdb)
{
  char	fontname[256];
  int         i;
  const int   Nsize=11;
  int         defsize[Nsize]={ 8,10,11,12,14,17,18,20,24,25,34 };
  
  if (fontdb.initFlag==False)	fontdb.initFlag=True;
  //check size -------------------------------------------------
  for (i=0; i<Nsize; i++) if (size <= defsize[i]) break;
  if (size != defsize[i] )
    {
      printf("Non-supported font size: %d replaced by %d\n",size,defsize[i]);
      size=defsize[i];
    }
  
  // loading font -----------------------------------------------
#ifdef DEBUG
  printf("[Setting font] %s%s%d-*-*-*-*-*-*-*\n",
	  FontType[type].xfont.c_str(),FontStyle[style].xfont.c_str(),size);
#endif
  
  sprintf(fontname,"%s%s%d-*-*-*-*-*-*-*",
	  FontType[type].xfont.c_str(),FontStyle[style].xfont.c_str(),size);
  fontdb.xfstruct = XLoadQueryFont( Disp, fontname );
  
  int cnt=0;
  int tmpstyle;
  while(fontdb.xfstruct == NULL){
    tmpstyle=style;
    cnt++;
    switch(cnt){
    case 1: // Try other style
      if(cnt==1){
	if(style==Italic) {
	  fprintf(stderr, "Trying to exchange Italic into Oblique: ");
	  tmpstyle=style=Oblique;
	}
	else if(style==Oblique) {
	  fprintf(stderr, "Trying to exchange Oblique into Italic: ");
	  tmpstyle=style=Italic;
	}
	else {
	  fprintf(stderr, "Trying anonymous font style: ");
	  tmpstyle=NONE; cnt++; //next try is cnt==3
	}
      }
      else {
	fprintf(stderr, "Trying anonymous font style: ");
	tmpstyle=NONE; // if cnt==2
      }
      sprintf(fontname,"%s%s%d-*-*-*-*-*-*-*",
	      FontType[type].xfont.c_str(),FontStyle[tmpstyle].xfont.c_str(),size);
      break;
    case 3: // Try other size
      fprintf(stderr, "Trying anonymous font size: ");
      sprintf(fontname,"%s%s-*-*-*-*-*-*-*-*",
	      FontType[type].xfont.c_str(),FontStyle[tmpstyle].xfont.c_str());
      break;
    case 4: // Specify only type
      fprintf(stderr, "Trying anonymous font style and size: ");
      sprintf(fontname,"%s-*-*-*-*-*-*-*-*-*",
	      FontType[type].xfont.c_str());
      break;
    case 5:
      fprintf(stderr, "Trying anonymous font: ");
      sprintf(fontname,"-*-*-*-*-*-*-*-*-*-*");
      break;
    default:
      fprintf(stderr, "giving up font selection...");
    }
    fprintf(stderr, "%s \n",fontname);
    fontdb.xfstruct = XLoadQueryFont( Disp, fontname );
  }
  if(cnt>0) fprintf(stderr, "OK!\n");

  sprintf(fontdb.psfont,"%s%s",
	  FontType[type].psfont.c_str(),FontStyle[style].psfont.c_str());
  fontdb.size=size;

#ifdef DEBUG
  printf("Created fontset: %s%s:%s\n",
	  FontType[type].psfont.c_str(),FontStyle[style].psfont.c_str(),fontname);
#endif
}

void setFontSet(int type, int style, int size, FontDB& fontdb)
{
  char	fontname[256];
  int         i;
  const int   Nsize=11;
  int         defsize[Nsize]={ 8,10,11,12,14,17,18,20,24,25,34 };
  int         missing_count;
  char**      missing_list;
  char*      def_string;
  
  if (fontdb.initFlag==False)	fontdb.initFlag=True;
  //check size -------------------------------------------------
  for (i=0; i<Nsize; i++) if (size <= defsize[i]) break;
  if (size != defsize[i] )
    {
      printf("Non-supported font size: %d replaced by %d\n",size,defsize[i]);
      size=defsize[i];
    }
  
  // loading font -----------------------------------------------
#ifdef DEBUG
  printf("[Creating fontset] %s%s%d-*-*-*-*-*-*-*\n",
	  FontType[type].xfont.c_str(),FontStyle[style].xfont.c_str(),size);
#endif
  
  sprintf(fontname,"%s%s%d-*-*-*-*-*-*-*",
	  FontType[type].xfont.c_str(),FontStyle[style].xfont.c_str(),size);
  //  fontdb.xfstruct = XLoadQueryFont( Disp, fontname );
  fontdb.xfset = XCreateFontSet( Disp, fontname,
				 &missing_list, &missing_count, &def_string );
  if( missing_count != 0 ){
     for( i=0; i<missing_count; i++ ){
       fprintf(stderr, "missing %d/%d: %s\n", i, missing_count, missing_list[i] );
     }
   }
  XFreeStringList(missing_list);
  
  int cnt=0;
  int tmpstyle;
  while(fontdb.xfset == NULL){
    tmpstyle=style;
    cnt++;
    switch(cnt){
    case 1: // Try other style
      if(cnt==1){
	if(style==Italic) {
	  fprintf(stderr, "Trying to exchange Italic into Oblique: ");
	  tmpstyle=style=Oblique;
	}
	else if(style==Oblique) {
	  fprintf(stderr, "Trying to exchange Oblique into Italic: ");
	  tmpstyle=style=Italic;
	}
	else {
	  fprintf(stderr, "Trying anonymous font style: ");
	  tmpstyle=NONE; cnt++; //next try is cnt==3
	}
      }
      else {
	fprintf(stderr, "Trying anonymous font style: ");
	tmpstyle=NONE; // if cnt==2
      }
      sprintf(fontname,"%s%s%d-*-*-*-*-*-*-*",
	      FontType[type].xfont.c_str(),FontStyle[tmpstyle].xfont.c_str(),size);
      break;
    case 3: // Try other size
      fprintf(stderr, "Trying anonymous font size: ");
      sprintf(fontname,"%s%s-*-*-*-*-*-*-*-*",
	      FontType[type].xfont.c_str(),FontStyle[tmpstyle].xfont.c_str());
      break;
    case 4: // Specify only type
      fprintf(stderr, "Trying anonymous font style and size: ");
      sprintf(fontname,"%s-*-*-*-*-*-*-*-*-*",
	      FontType[type].xfont.c_str());
      break;
    case 5:
      fprintf(stderr, "Trying anonymous font: ");
      sprintf(fontname,"-*-*-*-*-*-*-*-*-*-*");
      break;
    default:
      fprintf(stderr, "giving up font selection...");
    }
    fprintf(stderr, "%s \n",fontname);
    //    fontdb.xfstruct = XLoadQueryFont( Disp, fontname );
    fontdb.xfset = XCreateFontSet( Disp, fontname,
				   &missing_list, &missing_count, &def_string );
    XFreeStringList(missing_list);
  }
  if(cnt>0) fprintf(stderr, "OK!\n");

  sprintf(fontdb.psfont,"%s%s",
	  FontType[type].psfont.c_str(),FontStyle[style].psfont.c_str());
  fontdb.size=size;

#ifdef DEBUG
  printf("Created fontset: %s%s:%s\n",
	  FontType[type].psfont.c_str(),FontStyle[style].psfont.c_str(),fontname);
#endif
}

XColor Color( const char *color )
    {
    XColor   cr, ce;
    XAllocNamedColor( Disp, Cmap, color, &cr, &ce );
    return( cr );
    }

XColor Color( const std::string color )
    {
    XColor   cr, ce;
    XAllocNamedColor( Disp, Cmap, color.c_str(), &cr, &ce );
    return( cr );
    }

// ********************* Classes *******************************
// ------------------ public fn of XController -----------------
XController::XController(void)
{
#ifdef DEBUG
  puts("[XController::XController] setting locale...");
#endif
  if ( setlocale( LC_CTYPE, "" ) == NULL ) {
    printf( "Can't set locale\n" );
  }
  
  if ( ! XSupportsLocale() ) {
    printf( "Current locale is not supported\n" );
    printf( "Ignore the locale information...\n" );
  }
  
  Disp = XOpenDisplay( NULL );
  Cmap = DefaultColormap( Disp, 0 );
  WMFlag = True;
}

//XController& XController::depend_wm(int flag=True)
XController& XController::depend_wm(int flag)
    { WMFlag = flag; return *this; }

int  XController::mouse_press( void )
    {
    int		mod;
    XEvent	ev;
    mod = 0;
    for(int i=0;i<=Nwin;++i)
	mod+=XCheckTypedWindowEvent( Disp,*Win[i],ButtonPress,&ev );
    return(mod);
    }

int  XController::expose( void )
    {
    int		mod;
    XEvent	ev;
    mod = 0;
    for(int i=0;i<=Nwin;++i)
        mod+=XCheckTypedWindowEvent(Disp,*Win[i],Expose,&ev);
    return(mod);
    }

// ------------------ private fn of Graph ------------------

void Graph::xyscale( double x, double y, double& x2, double& y2 )
    { x2 = X.scaling(x); y2 = Y.scaling(y); }

// ------------------ public fn of Graph ------------------
Graph::Graph( void )
    {
    isManualyLocatedFlag=False;
    AxisWithDigitFlag	=True;
    LastPointFlag	=False;
    PolyStartFlag	=False;
    Index=0; 	Nchild=0;
    getNewGraph();
    ChildGraph[Nchild]=this;
    PSflag=False;
    PSrecorded=False;
    }

Graph& Graph::open( int left, int top, 
		 int x_pos, int x_neg, int y_pos, int y_neg, 
		 std::string clr )
//		 std::string clr="black" )
    {
    isManualyLocatedFlag=True;
    isNewGraphFlag = True;
    fxcolor=Color(clr.c_str());
    axiscolor=Color("black");

    LeftPosition = left;	TopPosition = top; 
    BottomPosition= top+y_pos+y_neg;
    X.StPoint	= left+x_neg;	Y.StPoint= top+y_pos;
    X.len	= x_pos;	Y.len	= y_pos; 
    X.lenN	= x_neg;	Y.lenN	= y_neg;
    return *this;
    }

Graph& Graph::open( int x_pos, int x_neg, int y_pos, int y_neg, std::string clr )
//		 std::string clr="black" )
    {
    isNewGraphFlag = True;
    fxcolor=Color(clr.c_str());
    axiscolor=Color("black");

    X.StPoint	= XMargin+x_neg;	Y.StPoint= YMargin+y_pos;
    X.len	= x_pos;		Y.len	 = y_pos; 
    X.lenN	= x_neg;		Y.lenN	 = y_neg;
    return *this;
    }

//Graph& Graph::open( Graph& g, std::string clr="black" )
Graph& Graph::open( Graph& g, std::string clr )
{
    ParentGraph = &g;
    hasParentFlag=True;
    isNewGraphFlag = True;
    fxcolor=Color(clr.c_str());
    axiscolor=Color("black");
    
    X.StPoint	= g.X.StPoint;	Y.StPoint= g.Y.StPoint;
    X.len	= g.X.len;	Y.len	= g.Y.len;
    X.lenN	= g.X.lenN;	Y.lenN	= g.Y.lenN;
    return *this;
    }

void Graph::getNewGraph(void)
    {
		//    set_new_handler(0);
    ChildGraph=new Graph* [Nchild+1];
    //    catch(xalloc) { std::cout<<"failed to obtain new ChildGraph"<<std::endl; exit(1);}
    if(ChildGraph==NULL){ std::cout<<"failed to obtain new ChildGraph"<<std::endl; exit(1);}
    }

int Graph::TellBirth(Graph& child)
    {
    Graph **tmp=new Graph* [Nchild];
    ++Nchild;
    for(int i=0;i<Nchild; ++i) tmp[i]=ChildGraph[i];
    delete ChildGraph;
    getNewGraph();
    for(int i=0;i<Nchild; ++i) ChildGraph[i]=tmp[i];
    ChildGraph[Nchild]=&child;
    delete [] tmp;

    return (Nchild);
    }

Graph& Graph::child( Graph& g, std::string clr )
    {
    ParentGraph = &g;
    hasParentFlag=True;
    Index=g.TellBirth(*this);

    isNewGraphFlag = False;
    if(clr.empty())
      fxcolor=Color(color_index[Index%MaxColor]);
    else
      fxcolor=Color(clr.c_str());
    axiscolor=g.axiscolor;

    return *this;
    }

void Graph::set_win_attribute( JWindow* pwind ) //called from class JWindow
    { 
    XGCValues	values;

    Pwind=pwind;
    values.foreground=fxcolor.pixel;
    //    values.background=Pwind->(bgcolor.pixel);
    values.background=Color("white").pixel;
    Gc = XCreateGC(Disp,Pwind->get_window(),
		   GCForeground|GCBackground,&values);

    Ps.init(width(),height(),fxcolor);

    ScaleFont=Pwind->ScaleFont;
    TextFont=Pwind->TextFont;

    useFont(ScaleFont);
    }

void Graph::set_location( int x, int y )
    { LeftPosition = x; TopPosition = y; 
    BottomPosition = y+Y.len+Y.lenN;
    X.StPoint = x+X.lenN; Y.StPoint = y+Y.len; 
}

void Graph::set_child_location( void )
    {
    LeftPosition   = ParentGraph->LeftPosition;
    TopPosition    = ParentGraph->TopPosition;
    BottomPosition = ParentGraph->BottomPosition;
    X.StPoint = ParentGraph->X.StPoint; 
    Y.StPoint = ParentGraph->Y.StPoint; 
    set_child_size();
    }

void Graph::set_child_size( void )
    {
    X.len  = ParentGraph->X.len;	
    Y.len  = ParentGraph->Y.len;
    X.lenN = ParentGraph->X.lenN;
    Y.lenN = ParentGraph->Y.lenN;
    }

Graph& Graph::set_ratio( double rx, double ry, double scrx, double scry )
   {
   X.ratio = scrx/rx;	Y.ratio= scry/ry;
   X.autoScale();       Y.autoScale();
   return *this; 
   }

Graph& Graph::set_ratio( Graph& g )
   {
   X.ratio = g.X.ratio;	Y.ratio= g.Y.ratio;
   X.autoScale();       Y.autoScale();
   return *this;
   }

Graph& Graph::set_ratio( void ) // for chid graph
   {
   X.ratio = ParentGraph->X.ratio;
   Y.ratio = ParentGraph->Y.ratio;
   X.autoScale();       Y.autoScale();
   return *this;
   }

//Graph& Graph::color( std::string clr="black" )
Graph& Graph::color( std::string clr )
    {
    fxcolor=Color(clr.c_str());
    XSetForeground( Disp, Gc, fxcolor.pixel ); 

    if(PSflag) Ps.newColor(fxcolor);
    return *this;
    }

Graph& Graph::setAxisColor( std::string clr )
{
  axiscolor=Color(clr.c_str());
  return *this;
}

Graph& Graph::pushColor( std::string clr )
{
  fxcolor_push=fxcolor;
  fxcolor=Color(clr.c_str());
  XSetForeground( Disp, Gc, fxcolor.pixel ); 

  if(PSflag) Ps.newColor(fxcolor);
  return *this;
}

Graph& Graph::pushColor( XColor xclr )
{
  fxcolor_push=fxcolor;
  fxcolor=xclr;
  XSetForeground( Disp, Gc, fxcolor.pixel ); 

  if(PSflag) Ps.newColor(fxcolor);
  return *this;
}

Graph& Graph::popColor(void)
{
  fxcolor=fxcolor_push;
  XSetForeground( Disp, Gc, fxcolor.pixel ); 

  if(PSflag) Ps.newColor(fxcolor);
  return *this;
}

void Graph::_scale( int direction, double x, double y,
		double x2, double y2, double blank )
{    double i;
 if( direction == HORIZONTAL )
   {
     _line( x, y, x2, y );
     for( i = x ; i <= x2 ; i += X.scaling(blank) )
       _line( i, y-2, i, y+2 );
   }
 if ( direction == VERTICAL )
   {
     _line( x, y, x, y2 );
     for( i = y ; i <= y2 ; i += Y.scaling(blank) )
       _line( x+2, i, x-2, i );
   }
}

Graph& Graph::scale( int direction, double x, double y, double blank )
{
  _scale( direction, (int)floor(X.scaling(x)), (int)floor(Y.scaling(y)),
	  X.len, Y.len, blank );
  return *this;
}

//Graph& Graph::set_axis_with_digit( int flag=True )
Graph& Graph::set_axis_with_digit( int flag )
{ AxisWithDigitFlag=flag; return *this; }

Graph& Graph::set_scale_full( int flag ) {
  X.prop = ((flag==True) ? WHOLE : SHORT );
  Y.prop = ((flag==True) ? WHOLE : SHORT ); return *this; }

Graph& Graph::set_xscale_full( int flag )
{ X.prop = ((flag==True) ? WHOLE : SHORT ); return *this; }

//Graph& Graph::set_yscale_full( int flag=True )
Graph& Graph::set_yscale_full( int flag )
{ Y.prop = ((flag==True) ? WHOLE : SHORT ); return *this; }
/*
Graph& Graph::set_xscale_dash( int flag=True )
    { X.prop = ((flag==True) ? Dash : Solid ); return *this; }

Graph& Graph::set_yscale_dash_full( int flag=True )
    { X.propProp = ((flag==True) ? Dash : Solid ); return *this; }
*/
void Axis::autoScale( void )
{
  double Rmax = maxof((double)len/ratio,(double)lenN/ratio);    
  double Rlen = (double)length()/ratio;
  //    double temp = pow10(floor(log10(Rmax)));
  double temp = pow(10,floor(log10(Rmax)));
  blank = ((temp==Rmax) ? temp/5.0 : temp );
  if( rint(Rlen/blank)==10 ) blank = temp/2.0;
}

//Graph& Graph::_xaxis( double blank, int y0=0 )
Graph& Graph::_xaxis( double blank, int y0 ){

  if(PSflag) Ps.newStatus(Begin,Xaxis);

  double iblank = X.scaling(blank);
    // use X.len+1 instead of X.len to avoid some trouble
  int	   i, imax = (int)floor(((double)X.len+1)/iblank);
  double x =(double)imax*iblank;
  
  i = imax;
  if( X.prop==WHOLE ){
    // Whole length of axis
    set_dash_line();
    while(x >= -X.lenN-1){
      if(x!=0.0) _line( (int)x, -Y.lenN-2, (int)x, Y.len+2 ); 
      --i; x=(double)i*iblank;
    }
    set_solid_line();
  }
  else while(x >= -X.lenN-1) // normal axis
    { _line( (int)x, y0-2, (int)x, y0+2 );  --i;  x=(double)i*iblank; }
  
  if( AxisWithDigitFlag==True ){
    char buf[10];
    useFont(ScaleFont);
    i = imax;
    x = (double)i*iblank;
    
    while(x >= -X.lenN-1){ 
      if(!(x==0.0 && Y.lenN!=0 && X.offset==0.0)) 
	_text(x,0,gcvt(X.offset+X.blank*i,5,buf), CenterTop, ScaleFont );
      --i;  x=(double)i*iblank; }
  }
  _line( -X.lenN, y0, X.len, y0 ); // x-axis line

  if(PSflag) Ps.newStatus(End,Xaxis);

  return *this;
}

Graph& Graph::_yaxis( double blank, int x0 )// x0=0
{

  if(PSflag) Ps.newStatus(Begin,Yaxis);

  double iblank = Y.scaling(blank);
  // use Y.len+1 instead of Y.len to avoid some trouble
  int    i, imax = (int)floor(((double)Y.len+1)/iblank); 
  double y = (double)imax*iblank;
  
  i=imax;
  if ( Y.prop==WHOLE ){
    set_dash_line();
    while(y >= -Y.lenN-1){
      if(y!=0.0) _line( -X.lenN-2, (int)y, X.len+2, (int)y );
      --i; y=i*iblank;}
    set_solid_line();
  }
  else while(y >= -Y.lenN-1)
    { _line( x0-2, (int)y, x0+2, (int)y ); --i; y=i*iblank; }
  
  if( AxisWithDigitFlag==True )
    { char buf[10]; int xoffset=-4;
    useFont(ScaleFont);
    i = imax;
    y = (double)i*iblank;
    
    while(y >= -Y.lenN-1)
      { 
	if(y==0.0 && X.lenN!=0) 
	  _text(xoffset, y, gcvt(Y.offset+Y.blank*i,5,buf),RightTop, ScaleFont );
	else
	  _text(xoffset, y,
		gcvt(Y.offset+Y.blank*i,5,buf),RightCenter, ScaleFont );
	--i;  y=(double)i*iblank; }
    }
  
  _line( x0, -Y.lenN, x0, Y.len ); // y-axis line
  
  if(PSflag) Ps.newStatus(End,Yaxis);

  return *this;
}

Graph& Graph::xaxis( void )
{
  pushColor(axiscolor);
  _xaxis(X.blank);
  popColor();
  return *this;
}

Graph& Graph::yaxis( void )
{
  pushColor(axiscolor);
  _yaxis(Y.blank);
  popColor();
 return *this;
}

Graph& Graph::xaxis( double blank )
{
  pushColor(axiscolor);
  X.blank=blank;
  _xaxis(blank);
  popColor();
  return *this;
}

Graph& Graph::yaxis( double blank )
{
  pushColor(axiscolor);
  Y.blank=blank;
  _yaxis(blank);
  popColor();
  return *this;
}

Graph& Graph::axis( void )
{
  pushColor(axiscolor);
  _xaxis(X.blank); _yaxis(Y.blank);
  popColor();
 return *this;
}

Graph& Graph::axis_box( void )
{
  axis();
  
  pushColor(axiscolor);
  if (X.len>0)  _yaxis(Y.blank,X.len);
  if (X.lenN>0) _yaxis(Y.blank,-X.lenN);
  if (Y.len>0)  _xaxis(X.blank,Y.len);
  if (Y.lenN>0) _xaxis(X.blank,-Y.lenN);

  if(PSflag) Ps.newStatus(Begin,AxisBox);
  _rectangle( -X.lenN, Y.len, width(), height() );
  if(PSflag) Ps.newStatus(End,AxisBox);
  popColor();
  
  return *this;
}

void Graph::_pset( double x, double y )
{
  XDrawPoint( Disp, Pwind->get_window(), Gc, ScrX(x), ScrY(y)); 
  
  // Ps description of pset command is imitated by line.
  if(PSflag) Ps.newLine(X.ps(x),Y.ps(y),X.ps(x),Y.ps(y));
}

Graph& Graph::pset( double x, double y )
{ 
  _pset(X.scaling(x),Y.scaling(y));
  return *this;
} 

Graph& Graph::set_solid_line( void )
{
  XSetLineAttributes( Disp, Gc, 0, LineSolid, CapButt, JoinMiter );
  if(PSflag) Ps.setLineType(SOLID_LINE);
  return *this;
}

Graph& Graph::set_dash_line( void )
{
  XSetLineAttributes( Disp, Gc, 0, LineOnOffDash, CapButt, JoinMiter );
  if(PSflag) Ps.setLineType(DASHED_LINE);
  return *this;
}

Graph& Graph::set_double_dash_line( void )
{
  XSetLineAttributes( Disp, Gc, 0, LineDoubleDash, CapButt, JoinMiter );
  return *this;
}

void Graph::_line(double x, double y, double x2, double y2)
{
  XDrawLine( Disp, Pwind->get_window(), Gc, 
	     ScrX(x), ScrY(y), ScrX(x2), ScrY(y2));
  if(PSflag) Ps.newLine(X.ps(x),Y.ps(y),X.ps(x2),Y.ps(y2));
} 

void Graph::_line(double x, double y)
{
  XDrawLine( Disp, Pwind->get_window(), Gc, 
	     ScrX(LastX), ScrY(LastY), ScrX(x), ScrY(y));
  if(PSflag) Ps.newLline(X.ps(x),Y.ps(y));
} 

Graph& Graph::line(double x, double y, double x2, double y2)
{
  xyscale( x, y, x, y );  xyscale( x2, y2, x2, y2 );
  _line( x, y, x2, y2 );
  LastX = x2;	LastY = y2;
  return *this;
} 

Graph& Graph::dash_line( double x, double y, double x2, double y2 )
{
  xyscale( x, y, x, y );  xyscale( x2, y2, x2, y2 );
  set_dash_line();
  _line( x, y , x2, y2 );
  set_solid_line();
  LastX = x2;	LastY = y2;
  return *this;
}

Graph& Graph::line(double x2, double y2)
{
  xyscale( x2, y2, x2, y2 );
  if (LastPointFlag == True)	_line( x2, y2 );
  else { LastPointFlag=True;	_pset( x2, y2 );	}
  LastX = x2;	LastY = y2;
  return *this;
} 

Graph& Graph::dash_line( double x2, double y2 )
{
  xyscale( x2, y2, x2, y2 );
  if (LastPointFlag == True)
    {
      set_dash_line();
      _line( x2, y2 );
      set_solid_line();
    }
  else { LastPointFlag=True;	_pset( x2, y2 );	}
  LastX = x2;	LastY = y2;
  return *this;
}

void Graph::_rectangle( double x, double y, double width, double height )
{
  XDrawRectangle( Disp, Pwind->get_window(), Gc, 
		  ScrX(x), ScrY(y), (int)width, (int)height );
  if(PSflag) Ps.newBox(ps_box,X.ps(x),Y.ps(y-height),width,height);
}

Graph& Graph::rectangle( double x, double y, double width, double height )
{
  xyscale( x, y, x, y );
  xyscale( width, height, width, height );
  _rectangle( x, y, width, height );
  return *this;
}

void Graph::_fill_rectangle( double x, double y, 
			     double width, double height )
{ 
  XFillRectangle( Disp, Pwind->get_window(), 
		  Gc, ScrX(x), ScrY(y), (int)width, (int)height);
  if(PSflag) Ps.newBox(ps_boxf,X.ps(x),Y.ps(y-height),width,height);
}

Graph& Graph::fill_rectangle( double x, double y, double width, double height )
{ 
  xyscale( x, y, x, y );
  xyscale( width, height, width, height );
  _fill_rectangle(x, y, width, height  );
  return *this;
}

// flag=APPEND(Default),HEAD,TAIL
Graph& Graph::_poly( double x, double y, int flag )
{
  static PolyPoint *head, *cur;
  static int n=0;
  
  if(flag==Head){
    head=new PolyPoint;
    cur=head;
  } else {
    cur->p=new PolyPoint;
    cur=cur->p;
  }
  
  cur->x=x;  cur->y=y;
  cur->p=NULL;
  n++;
  
  //  std::cout<<"HEAD: "<<head->x<<" "<<head->y<<std::endl;
  std::cout<<"Appended: "<<cur->x<<" "<<cur->y<<std::endl;

  if(flag==Tail){
    PolyPoint* np;
    PolyPoint* tmp;

    cur=head;
    np=head->p;
    while (np!=NULL){
      std::cout << cur->x <<","<<cur->y<<std::endl;
      _line(cur->x,cur->y,np->x,np->y);
      tmp=cur;
      cur=np;
      np=cur->p;
      delete tmp;
    }
      std::cout << cur->x <<","<<cur->y<<std::endl;
    if(head->x!=cur->x || head->y!=cur->y)
      _line(cur->x,cur->y,head->x,head->y);
    PolyStartFlag=False;
    n=0;
  }
  
  if(flag==Fill) {
    PolyPoint* tmp;
    int i=0;
    
    XPoint* xp=new XPoint [n];
    double* x=new double [n];
    double* y=new double [n];

    cur=head;
    while (cur!=NULL){
      xp[i].x=ScrX(cur->x);
      xp[i].y=ScrY(cur->y);
      if(PSflag){
	x[i]=X.ps(cur->x);
	y[i]=Y.ps(cur->y);
      }
      tmp=cur;
      cur=cur->p;
      delete tmp;
      i++;
    }
    XFillPolygon( Disp, Pwind->get_window(), Gc,
		  xp, n, Complex, CoordModeOrigin );

    if(PSflag){ Ps.newPolygon(ps_polyf,x,y,n); }
    else {
      delete [] x;
      delete [] y;
    }

    delete [] xp;
    PolyStartFlag=False;
    n=0;
  }
  return *this;
}

Graph& Graph::poly( double x, double y )
{
  xyscale( x, y, x, y );
  if(PolyStartFlag==False){
    PolyStartFlag=True;
    _poly(x,y,Head);
  } else {
    _poly(x,y,Append);
  }
  return *this;
}

Graph& Graph::closePoly( double x, double y )
{
  xyscale( x, y, x, y );
  _poly(x,y,Tail);
  return *this;
}


Graph& Graph::fillPoly( double x, double y )
{
  xyscale( x, y, x, y );
  _poly(x,y,Fill);
  return *this;
}

void Graph::_cross( double x, double y, double r )
{
  XDrawLine( Disp, Pwind->get_window(), Gc, 
	     ScrX(x-r), ScrY(y), ScrX(x+r), ScrY(y));
  XDrawLine( Disp, Pwind->get_window(), Gc, 
	     ScrX(x), ScrY(y+r), ScrX(x), ScrY(y-r));
  if(PSflag){
    Ps.newLine(X.ps(x-r),Y.ps(y),X.ps(x+r),Y.ps(y));
    Ps.newLine(X.ps(x),Y.ps(y-r),X.ps(x),Y.ps(y+r));
  }
}

Graph& Graph::cross( double x, double y, double r )
{
  r = (int)X.scaling(r);
  xyscale( x, y, x, y );
  _cross( x, y, r );
  return *this;
}

void Graph::_circle( double x, double y, double r )
{
  XDrawArc( Disp, Pwind->get_window(), Gc, 
	    ScrX(x-r), ScrY(y+r), int(r)*2, int(r)*2, 0, 360*64);
  if(PSflag) Ps.newCircle(ps_circle,X.ps(x),Y.ps(y),r);
}

void Graph::_fill_circle( double x, double y, double r )
{
  XFillArc( Disp, Pwind->get_window(), Gc, 
	    ScrX(x-r), ScrY(y+r), int(r)*2, int(r)*2, 0, 360*64);
  if(PSflag) Ps.newCircle(ps_circlef,X.ps(x),Y.ps(y),r);
}

Graph& Graph::circle( double x, double y, double r )
{
  r = (int)X.scaling(r);
  xyscale( x, y, x, y );
  _circle( x, y, r );
  return *this;
}

Graph& Graph::fill_circle( double x, double y, double r )
{
  r = X.scaling(r);
  xyscale( x, y, x, y );
  _fill_circle(x, y, r);
  return *this;
}

Graph& Graph::forget(void)
{
  LastPointFlag=False;
  for (int i=1;i<=Nchild;++i) ChildGraph[i]->forget();
  return *this;
}

Graph& Graph::setTextFont( int ftype, int fstyle, int size )
{
  if(TextFont.xfset!=NULL) XFreeFontSet(Disp, TextFont.xfset);
  setFontSet(ftype, fstyle, size, TextFont);
  return(*this);
}

Graph& Graph::setScaleFont( int ftype, int fstyle, int size )
{
  if(TextFont.xfset!=NULL) XFreeFontSet(Disp, ScaleFont.xfset);
  setFontSet(ftype, fstyle, size, ScaleFont);
  return(*this);
}

Graph& Graph::useFont(FontDB& fontdb) // call to change the PS font setting
{
  //  XSetFont( Disp, Gc, fontdb.xfstruct->fid );
  if(PSflag) Ps.newFont(fontdb.psfont,fontdb.size);
  return *this;
}

void Graph::_text( double x, double y, char *msg, int position, FontDB& font )
{
  int	width, height, scrx, scry;
  //  int dir, ascent, descent;
  double  psx, psy;
  //XCharStruct cs;
  XRectangle overall_ink_return;
  XRectangle overall_logical_return;
  
  //  width = XTextWidth( font.xfstruct, msg, strlen(msg) );
  //  XTextExtents( font.xfstruct, msg, strlen(msg), 
  //		&dir, &ascent, &descent, &cs );
  // height = (int)((double)ascent*1.3);

  XmbTextExtents( font.xfset, msg, strlen(msg),
		  &overall_ink_return, &overall_logical_return);
  width  = overall_ink_return.width;
  height = int((double)overall_ink_return.height * 1.3);

  switch(position)
    {
    case Left:
      scrx = ScrX(x); 		scry = ScrY(y);	
      psx  = X.ps(x);		psy  = Y.ps(y);	break;
    case Center:
      scrx = ScrX(x-width/2);	scry = ScrY(y);	
      psx  = X.ps(x-width/2);	psy  = Y.ps(y);	break;
    case Right:
      scrx = ScrX(x-width);	scry = ScrY(y);	
      psx  = X.ps(x-width);	psy  = Y.ps(y);	break;
    case LeftTop:
      scrx = ScrX(x); 		scry = ScrY(y-height);	
      psx  = X.ps(x);		psy  = Y.ps(y-height);	break;
    case CenterTop:
      scrx = ScrX(x-width/2);	scry = ScrY(y-height);	
      psx  = X.ps(x-width/2);	psy  = Y.ps(y-height);	break;
    case RightTop:
      scrx = ScrX(x-width);	scry = ScrY(y-height);	
      psx  = X.ps(x-width);	psy  = Y.ps(y-height);	break;
    case RightCenter:
      scrx = ScrX(x-width);	scry = ScrY(y-height/2);	
      psx  = X.ps(x-width);	psy  = Y.ps(y-height/2); break;
    }
#ifdef DEBUG
    printf("_text: (width,height)=(%d,%d) at (%d,%d): %s\n", width, height, scrx, scry, msg);
#endif
  XmbDrawString( Disp, Pwind->get_window(), font.xfset, Gc, 
		 scrx, scry, msg, strlen(msg) );
  if(PSflag) Ps.newText(ps_newtext,psx,psy,msg);
}

Graph& Graph::text( double x, double y, char* msg, int position )//position=Center
{
  useFont(TextFont);
  xyscale( x, y, x, y );
  _text( x, y, msg, position, TextFont );
  return *this;
}

Graph& Graph::clear( void )
{ Pwind->clear(); return *this; }

int Graph:: mouse_press( void )
    {    XEvent	ev;
    return XCheckTypedWindowEvent( Disp,Pwind->get_window(),ButtonPress,&ev );
    }

int Graph:: mouse_press( XEvent &ev )
    {
    return XCheckTypedWindowEvent( Disp,Pwind->get_window(),ButtonPress,&ev );
    }
// ------------------ public fn of JWindow ------------------

void JWindow::getNewWindow(void)
    {
    std::set_new_handler(0);
    Win=new Window* [Nwin+1];
    if(Win==NULL){ std::cout<<"failed to obtain new Window"<<std::endl;exit(1);}
    }

JWindow::JWindow()
{
  Ngrp=0; Nmenu=0; BSFlag=True; ++Nwin; Index=Nwin; 
  MenuMargin=0;
#ifdef DEBUG
  std::cout << "Open window " << Index << std::endl;
#endif  
  if (Nwin==0) {getNewWindow();}
  else {
    Window** tmp=new Window* [Nwin];
    int	i;
    for(i=0;i<Nwin;++i) tmp[i]=Win[i];
    delete [] Win;
    getNewWindow();
    for(i=0;i<Nwin;++i) Win[i]=tmp[i];
    delete [] tmp;
  }
}

JWindow& JWindow::open( unsigned int left, unsigned int top,
			unsigned int width, unsigned int height,
			int bsflag )// int bsflag=True
{
  unsigned int		bd_width;
  XSetWindowAttributes	atr;
  
  //--------  setting window size  ---------------------
  bd_width 	= 2;
  WHints.flags = PPosition;
  WHints.x = left;    	WHints.y = top;
  WHints.width = width;	WHints.height = height;
  BSFlag=bsflag;
  //-----------------------------------------------------
  setFontSet(Gothic,Roman,17,ScaleFont);
  setFontSet(Gothic,Roman,14,TextFont);
  bgcolor=Color("white");
  Wind=XCreateSimpleWindow( Disp, DefaultRootWindow(Disp), 
			    left, top, width, height, bd_width,
			    Color("peru").pixel, bgcolor.pixel );
  Win[Index]=&Wind;
  //------- setting name? -------------------------------
  char wname[10], tmp[10];
  strcpy(wname,"Window");
  strcat(wname,gcvt((double)Index,3,tmp));
  XSetStandardProperties( Disp, Wind, wname, wname, None, 0, 0, &WHints );
  //    XStoreName( Disp, Wind, wname );
  //-------  setting window manager dependancy----------
  atr.save_under = True;			// save hide region
  atr.override_redirect = ((WMFlag==True) ? False : True );
  if (BSFlag==True) {
    atr.backing_store = Always;// save hidden region
    XChangeWindowAttributes( Disp, Wind, 
			     CWBackingStore | CWOverrideRedirect | CWSaveUnder, &atr );
  }
  else {
    XChangeWindowAttributes( Disp, Wind, 
			     CWOverrideRedirect | CWSaveUnder, &atr );}
  //----------------------------------------------------
  Xwin.flush();
  XSelectInput( Disp, Wind, ExposureMask|ButtonPressMask );
  //----------------------------------------------------
#ifdef DEBUG
  std::cout << " Registered property of Window " << Index << std::endl;
#endif
  return *this;
}

enum {OnlyMenu,WithGraph};

JWindow& JWindow::setPosition()
{
  int flag, xmax=0, ymax=0;
  for (int i=0;i<Nmenu;++i){
    //		Pmenu[i]->setSize();
    if (Pmenu[i]->getDirection()==HORIZONTAL){
      WHints.height += Pmenu[i]->height();
    }else {
      MenuMargin += Pmenu[i]->width();
      WHints.width += Pmenu[i]->width();
    }
    xmax= ((xmax > Pmenu[i]->width()) ?  xmax : Pmenu[i]->width() );
    ymax= ((ymax > Pmenu[i]->height()) ? ymax : Pmenu[i]->height() );
  }
  
  if (Ngrp>=0) {
    flag=WithGraph;
    for (int i=0;i<Ngrp;++i){
      Pgrp[i]->set_win_attribute(this);
      
      if ( Ngrp==0 ){
	Pgrp[i]->set_location( MenuMargin+XMargin, WHints.height );
	WHints.width += Pgrp[i]->width() + XMargin;
	WHints.height += Pgrp[i]->height() + YMargin;
      } else {
	if (Pgrp[i]->isNewGraph() == True){
	  if (Pgrp[i]->hasParent() == True) Pgrp[i]->set_child_size();
	  Pgrp[i]->set_location( MenuMargin+XMargin, WHints.height );
	  
	  WHints.height += Pgrp[i]->height() + YMargin;
	  WHints.width = maxof( WHints.width, (int)Pgrp[i]->width()+MenuMargin+2*XMargin);
	} else { Pgrp[i]->set_child_location(); }
      }
    }
  } else {
    flag=OnlyMenu;
    WHints.width=xmax;
    WHints.height=ymax;
  }
  
  for (int i=0;i<Nmenu;++i){
    Pmenu[i]->fitMenuInWindow(WHints.width,WHints.height,flag);
  }
  
  XResizeWindow( Disp, Wind, WHints.width, WHints.height );
  return *this;
}

JWindow& JWindow::setBgColor(std::string bgclr)
{
  bgcolor=Color(bgclr.c_str());
  XSetWindowBackground( Disp, Wind, bgcolor.pixel );
  XClearWindow( Disp, Wind );
  /*
  if(PSflag) Ps.newColor(fxcolor); // How define background color in EPS???
  */
  return *this;
}

JWindow& JWindow::open( unsigned int left, unsigned int top,
			int bsflag )//bsflag=True
{
#ifdef DEBUG
  std::cout <<"JWindow::map [start]"<<std::endl;
#endif
  WHints.width = MenuMargin+XMargin;	WHints.height = YMargin;
  WHints.x = left;		WHints.y = top;
  
  open( WHints.x, WHints.y, WHints.width, WHints.height, bsflag );
#ifdef DEBUG
  std::cout <<"JWindow::map [end]"<<std::endl;
#endif
  
  return *this;
}

JWindow& JWindow::setTextFont( int ftype, int fstyle, int size )
  {
    setFontSet(ftype, fstyle, size, TextFont);
    return(*this);
  }

JWindow& JWindow::setScaleFont( int ftype, int fstyle, int size )
  {
    setFontSet(ftype, fstyle, size, ScaleFont);
    return(*this);
  }

JWindow& JWindow::map( void )
{
#ifdef DEBUG
  std::cout <<"JWindow::map[start]"<<std::endl;
#endif
  setPosition();
  XMapRaised(Disp,Wind);
  XMapSubwindows(Disp,Wind);
  Xwin.sync();
  return *this;
}

void JWindow::getNewPgrp(void)
    { 
    std::set_new_handler(0);	
    Pgrp=new Graph* [Ngrp+1];
    if(Pgrp==NULL){ std::cout<<"failed to obtain new Pgrp"<<std::endl;exit(1);}
    }

JWindow& JWindow::graph( Graph &g )
{
#ifdef DEBUG
  std::cout <<"JWindow::graph[start]"<<std::endl;
#endif
  if (Ngrp>0){
    int i;
    Graph** tmp=new Graph* [Ngrp];
    for(i=0;i<Ngrp; ++i) tmp[i]=Pgrp[i];
    delete  [] Pgrp;
    getNewPgrp();
    for(i=0;i<Ngrp; ++i) Pgrp[i]=tmp[i];
    delete [] tmp;
  } else getNewPgrp();
  
  Pgrp[Ngrp] = &g;
  
#ifdef DEBUG
  std::cout << " Registered Graph " << Ngrp
       << " in Window " << Index << std::endl;
#endif
  ++Ngrp;
  return *this;
}

void JWindow::getNewPmenu(void)
{ 
  std::set_new_handler(0);	
  Pmenu=new Menu* [Nmenu+1];
  if(Pmenu==NULL){ std::cout<<"failed to obtain new menu"<<std::endl;exit(1);}
}

JWindow& JWindow::menu( Menu &m )
{
  if (Nmenu>0){
    int i;
    Menu** tmp=new Menu* [Nmenu];
    for(i=0;i<Nmenu; ++i) tmp[i]=Pmenu[i];
    delete [] Pmenu;
    getNewPmenu();
    for(i=0;i<Nmenu; ++i) Pmenu[i]=tmp[i];
    delete [] tmp;
  }
  else { getNewPmenu(); }
  
  Pmenu[Nmenu] = &m;
  
  m.setSize();
  if (Nmenu>0) {m.setWinAttribute(Wind,0,Pmenu[Nmenu-1]->height()); }
  else  m.setWinAttribute(Wind,0,0); 
  
  m.mapped=True;
  m.selected=False;
  
  std::cout << " Registered Menu " << Nmenu
       << " in Window" << Index << std::endl;
  ++Nmenu;
  return *this;
}

JWindow& JWindow::axis( void )
{
  for(int i=0;i<Ngrp;++i) Pgrp[i]->axis();
  return *this;
}

JWindow& JWindow::clear( void )
{
  XClearWindow( Disp, Wind );
  Xwin.flush(); 
  for(int i=0;i<Ngrp;++i) {
    Pgrp[i]->forget();
    if(Pgrp[i]->PSflag) Pgrp[i]->resetEPS();
  }
  return *this;
}

int JWindow:: mouse_press( void )
{
  XEvent ev;
  return XCheckTypedWindowEvent( Disp,Wind,ButtonPress,&ev );
}

int JWindow:: mouse_press( XEvent& ev )
{
  return XCheckTypedWindowEvent( Disp,Wind,ButtonPress,&ev );
}

int JWindow:: expose( void )
{    XEvent	ev;
 return XCheckTypedWindowEvent(Disp,Wind,Expose,&ev);
}

//---------------------------------------------------------------

const int WIND_BD_WIDTH       =  0;
const int ITEM_HBD_WIDTH       = 2;
const int ITEM_LBD_WIDTH       = 2;
const int ITEM_STR_MARGIN     =  10; 
const int ITEM_STR_ORIGIN = ITEM_HBD_WIDTH + ITEM_STR_MARGIN;

std::string ITEM_FORE_COLOR    =  "black";
std::string ITEM_BACK_COLOR    =  "gray";   //#bebebe 190//
std::string ITEM_HBD_COLOR     =  "gray90"; //#e7e7e7 229//
std::string ITEM_LBD_COLOR     =  "gray44"; //#717171 112//

//for input window
const int INPUT_STR_MARGIN    =  3;
const int INPUT_DIGIT_LEN     = 15;

//---------------- fns. of MenuItem and Menu ------------------------

MenuItem& MenuItem::setTitle(const std::string str) {
  title=str;
  return *this;
}

void MenuItem::drawFrame(Menu* menu, int flag)
{
#ifdef DEBUG
  std::cout <<"MenuItem::drawFrame"<<std::endl;
#endif
  menu->drawFrame(left,top,width,height,flag);
}

MenuItem& MenuItem::drawString(Menu* menu, int flag=Normal)
{
#ifdef DEBUG
  std::cout <<"MenuItem::drawString"<<std::endl;
#endif
  menu->drawString(left+ITEM_STR_ORIGIN,top,title.c_str(),flag);
  
  if ((Type!=GetString)&&(Type!=GetDouble)) return *this;
  
  menu->Rectangle(left+inmargin,top+ITEM_HBD_WIDTH,
		  width-inmargin-ITEM_LBD_WIDTH,
		  height-ITEM_LBD_WIDTH-ITEM_HBD_WIDTH,
		  InputArea);
  menu->drawFrame(left+inmargin,top+ITEM_HBD_WIDTH,
		  width-inmargin-ITEM_LBD_WIDTH,
		  height-ITEM_LBD_WIDTH-ITEM_HBD_WIDTH,
		  InputArea);
  
  if (Type==GetString){
    ItemGetString* getitem;
    getitem=(ItemGetString *)cell;
    
    menu->drawString(INPUT_STR_MARGIN+inmargin,top,
		     getitem->iw->input,getitem->iw->status);
    getitem->iw->setCursor(menu);
  } else if (Type==GetDouble){
    ItemGetDouble* getitem;
    getitem=(ItemGetDouble *)cell;
    
    menu->drawString(INPUT_STR_MARGIN+inmargin,top,
		     getitem->iw->input,getitem->iw->status);
    getitem->iw->setCursor(menu,Erase);
  }
  return *this;
}

void MenuItem::act(){
#ifdef DEBUG
	//	cout <<"MenuItem::act[start]"<<std::endl;
#endif
	  if (Type==Submenu){
		  ItemMenu* submenu;
		  submenu=(ItemMenu *)cell;
		  submenu->menu->act();
	  }
}

void MenuItem::drawStatus(Menu* menu, int flag=Normal)
{
#ifdef DEBUG
	cout <<"MenuItem::drawStatus"<<std::endl;
#endif
	if((Type!=GetDouble)&&(Type!=GetString)){
		menu->drawStatus(left,top,flag);
		drawString(menu,flag);
		if(menu->style()==Separate)
			drawFrame(menu,flag);
	}
}

void MenuItem::exec(Menu* menu){
#ifdef DEBUG
	cout <<"MenuItem::exec[start]"<<std::endl;
#endif
	switch (Type) {
	case Func:
		ItemFunc* ifunc;
		ifunc=(ItemFunc *)cell;
		(ifunc->handler)(ifunc->arg);
	  break;
	case Submenu:
		ItemMenu* submenu;
		submenu=(ItemMenu *)cell;
		if(!submenu->menu->mapped)
			submenu->menu->map();
		break;
	}
}

void MenuItem::checkMySize(Menu* menu,
			   unsigned int& width, unsigned int& height, int& margin)
{
  menu->retStrLen(title.c_str(),width,height);
	margin=0;
	if ((Type==GetString) || (Type==GetDouble)){
		margin=width+ITEM_STR_MARGIN*2+ITEM_HBD_WIDTH+ITEM_LBD_WIDTH;

		unsigned int awidth,aheight;
		menu->retStrLen("x",awidth,aheight);
		width = margin
			+awidth*MAX_STR_LEN+ITEM_STR_MARGIN*2+ITEM_LBD_WIDTH;
	} else {
		width+=ITEM_STR_MARGIN*2+ITEM_HBD_WIDTH+ITEM_LBD_WIDTH;
	}
	height+=ITEM_HBD_WIDTH+2*ITEM_LBD_WIDTH;
}

Menu& Menu::fitMenuInWindow( int width, int height, int flag ){
	if (flag==WithGraph){
		if (Direction==HORIZONTAL){
			WHints.width = width;
		} else {
			WHints.height = height;
		}
	} else {
			WHints.width = width;
	}
	XResizeWindow( Disp, Wind, WHints.width, WHints.height );

	return *this;
}

Menu& Menu::useFont(FontDB& fontdb, GC& gc)
    {
    XSetFont( Disp, gc, fontdb.xfstruct->fid );
    return *this;
    }

Menu::Menu(int direction, int maptype, int style)
  //(int direction=VERTICAL, int maptype=Static, int style)
{
    lastItem=-1;
    Direction=direction;
    Inum=0; 
    Style=style;
    hasParentFlag=False;
    mapType=maptype;
    notMappedYet=True;
    selected=mapped=False;
    title="Menu";
}

Menu& Menu::setWinAttribute( Window& wind, int x, int y ){ // called from open and JWindow 
#ifdef DEBUG
  std::cout <<" setting attribute..."<<std::endl;
  std::cout << WHints.x<<" "<< WHints.y<<" "<<WHints.width<<" "<<WHints.height<<"\n";
#endif
  WHints.x = x;       WHints.y = y;
  Wind=XCreateSimpleWindow(Disp, wind, WHints.x, WHints.y,
			   WHints.width, WHints.height, WIND_BD_WIDTH,
			   Color(ITEM_FORE_COLOR).pixel,
			   Color(ITEM_BACK_COLOR).pixel);
  XSetStandardProperties
    (Disp, Wind, title.c_str(), title.c_str(), None, 0, 0, &WHints);
  
  XSetWindowAttributes	atr;
  atr.save_under = True;
  
  //	atr.override_redirect = ((mapType==Temporal) ? True:False);
  atr.override_redirect = False;
  XChangeWindowAttributes
    (Disp, Wind, CWOverrideRedirect | CWSaveUnder, &atr);
  
  //#define DDD
#ifdef DDD
  ev_mask= ExposureMask
    |PointerMotionMask
    |LeaveWindowMask
    |ButtonPressMask
    |ButtonReleaseMask
    |KeyPressMask
    |StructureNotifyMask;
#else
  ev_mask= ExposureMask
    |PointerMotionMask
    |LeaveWindowMask
    |ButtonPressMask
    |ButtonReleaseMask
    |KeyPressMask;
#endif
  XSelectInput(Disp,Wind,ev_mask);
  
  ////////////////// setting Menu Font ////////////////
  setFont(Helvetica,Roman,14,MenuFont);
  
#ifdef DEBUG
  std::cout <<" setting Gc...\n";
#endif
  ////////////////// setting GC ////////////////
  XGCValues values;
  
  values.foreground=Color(ITEM_BACK_COLOR).pixel;
  values.background=Color(ITEM_FORE_COLOR).pixel;
  Gc_normal =XCreateGC(Disp,Wind,GCForeground|GCBackground,&values); 
  useFont( MenuFont, Gc_normal );
  
  values.foreground=Color(ITEM_FORE_COLOR).pixel;
  values.background=Color(ITEM_BACK_COLOR).pixel;
  Gc_normal_Text =XCreateGC(Disp,Wind,GCForeground|GCBackground,&values); 
  useFont( MenuFont, Gc_normal_Text );
  
  values.foreground=Color(ITEM_HBD_COLOR).pixel;
  values.background=Color(ITEM_BACK_COLOR).pixel;
  Gc_HI=XCreateGC(Disp,Wind,GCForeground|GCBackground,&values);
  
  values.foreground=Color(ITEM_LBD_COLOR).pixel;
  values.background=Color(ITEM_HBD_COLOR).pixel;
  Gc_HI_Text=XCreateGC(Disp,Wind,GCForeground|GCBackground,&values);
  useFont( MenuFont, Gc_HI_Text );
  
  values.foreground=Color(ITEM_LBD_COLOR).pixel;
  values.background=Color(ITEM_HBD_COLOR).pixel;
  Gc_LO=XCreateGC(Disp,Wind,GCForeground|GCBackground,&values);  
  
  values.foreground=Color(ITEM_HBD_COLOR).pixel;
  values.background=Color(ITEM_LBD_COLOR).pixel;
  Gc_LO_Text=XCreateGC(Disp,Wind,GCForeground|GCBackground,&values);  
  useFont( MenuFont, Gc_LO_Text );
  
  return *this;
}

Menu& Menu::retStrLen(std::string msg, unsigned int& width, unsigned int& height)
{
  int dir;
  XCharStruct cstruct;
  XTextExtents(MenuFont.xfstruct,msg.c_str(),msg.size(),
	       &dir,&ascent,&descent,&cstruct);
  height= ascent+descent;
  width=cstruct.lbearing+cstruct.rbearing;
  
  return *this;
}


Menu& Menu::setSize(){
  unsigned int width,height;
  int margin,inmargin;
  margin=0; inmargin=0;
 
#ifdef DEBUG
  std::cout <<"setSize..."<<std::endl;
#endif
  
  for (int i=0;i<Inum;++i){
    item[i].checkMySize(this,width,height,margin);
    
    item_w = (item_w < width ) ? width  : item_w;
    item_h = (item_h < height) ? height : item_h;
    inmargin = (inmargin < margin) ? margin : inmargin;
    if(Direction==HORIZONTAL){
      WHints.width=Inum*item_w; WHints.height=item_h;
    }
    else{
      WHints.width=item_w; WHints.height=Inum*item_h;
    }
  }
  
  for (int i=0;i<Inum;++i){
    if(Direction==HORIZONTAL){
      item[i].setPosition(i*item_w,0,item_w,item_h,inmargin);
    }
    else{
      item[i].setPosition(0,i*item_h,item_w,item_h,inmargin);
    }
  }
  
  XResizeWindow( Disp, Wind, WHints.width, WHints.height );
#ifdef DEBUG
  std::cout <<"setSize:: width: "<<WHints.width<<" height: "<<WHints.height
       <<" inmargin: "<<inmargin<<std::endl;
#endif

  return *this;
}

Menu& Menu::setTitle(const std::string str){
  title=str;
  XStoreName(Disp,Wind,title.c_str());
  return *this;
}

void Menu::makeNewItem(void)
{
  if (Inum!=0) {
    MenuItem* tmp=new MenuItem[Inum];
    for (int i=0;i<Inum;i++) {tmp[i]=item[i];}
    
    delete[] item;
    item=new MenuItem[Inum+1];
    for (int i=0;i<Inum;i++) {item[i]=tmp[i];}
    delete [] tmp;
  }
  else {
    item=new MenuItem[1];
  }
}

void handler_quit_menu(void* arg) {
  Menu* menu;
  menu=(Menu*)arg;
  menu->unmap();
}

void handler_exit(void* dummy){       //½ªÎ»¤¹¤ë
  Xwin.close();
  exit(1);
}

//Menu& Menu::addItem(const std::string title, void (*handler)(Menu& menu))
Menu& Menu::addItem(const std::string title, void (*handler)(void*), void* arg)// arg=NULL
{
  makeNewItem();
  item[Inum].Type=Func;
  item[Inum].setTitle(title);
  item[Inum].index=Inum;
  
  ItemFunc* func=new ItemFunc;
  func->handler=handler;
  func->arg=arg;
  item[Inum].cell=(void *)func;
  
  Inum++;
  
  return *this;
}

Menu& Menu::addItem( const std::string title, Menu& child )
{
  addSubmenu(title,child);
  return *this;
}

Menu& Menu::addClose(void)
{
	addItem("Close", handler_quit_menu,(void*)(this));
	return *this;
}

Menu& Menu::addExit(void)
{
	addItem("end", handler_exit,NULL);
	return *this;
}

Menu& Menu::addSubmenu( const std::string title, Menu& child )
{
  child.hasParentFlag=True;
  child.ParentMenu=(this);
  
  makeNewItem();
  item[Inum].Type=Submenu;
  item[Inum].setTitle(title);
  item[Inum].index=Inum;
  
  ItemMenu* submenu= new ItemMenu;
  submenu->menu=&child;
  submenu->menu->itemIndex=Inum;
  item[Inum].cell=(void *)submenu;
  
  Inum++;
  
  return *this;
}

Menu& Menu::addGetString(std::string value, std::string title, std::string dflt)
{
  value=dflt;
  
  makeNewItem();
  
  item[Inum].Type=GetString;
  item[Inum].index=Inum;
  item[Inum].setTitle(title);
  
  ItemGetString* func=new ItemGetString;
  func->dflt=dflt;
  func->value=value;
  
  InputItem* iwin=new InputItem;
  func->iw=iwin;
  if (!dflt.empty()) func->iw->setDefault(dflt);
  func->iw->index=Inum;
  
  item[Inum].cell=(void *)func;
  Inum++;
  
  return *this;
}

Menu& Menu::addGetValue(double& value, std::string title, double dflt)
{
	value=dflt;
	makeNewItem();
	item[Inum].Type=GetDouble;
	item[Inum].index=Inum;
	item[Inum].setTitle(title);

	ItemGetDouble* func=new ItemGetDouble;
	func->dflt=dflt;
	func->value=&value;
	
	InputItem* iwin=new InputItem;
	func->iw=iwin;
	func->iw->index=Inum;

	char dflt_str[MAX_STR_LEN+1];
	gcvt(dflt,INPUT_DIGIT_LEN,dflt_str);
	if (dflt_str!=NULL) func->iw->setDefault(dflt_str);

	item[Inum].cell=(void *)func;
	Inum++;

	return *this;
}

Menu& Menu::open(int x,int y)//int x=0,int y=0
{
#ifdef DEBUG
	cout <<"open menu...\n";
#endif
  //////////////// Setting Window ///////////////
	win_x=x; win_y=y;

	int left=win_x, top=win_y;

	WHints.flags = PPosition;
	WHints.x = left;       WHints.y = top;
	WHints.width = 10;     WHints.height = 10; // Initial value...will be resized
  
	setWinAttribute(DefaultRootWindow(Disp),WHints.x, WHints.y);
	item_w= WHints.width+ITEM_STR_MARGIN*2+ITEM_HBD_WIDTH+ITEM_LBD_WIDTH;
	item_h= ITEM_HBD_WIDTH+ITEM_LBD_WIDTH;

	return *this;
}

//Get geometry of the menu item in the main Display
//Returns the position of the window which does not include taskbar.
void MenuItem::getGeometry(Menu* menu, int& x, int& y){
	unsigned int w,h,bd;
	int px,py,dir;

	menu->getGeometry(px,py,w,h,bd,dir);
	x=px+left;
	y=py+top;
	if (dir==HORIZONTAL) y+=h+bd+2;
	else x+=w;
}

//Get geometry of the menu in the main Display and in the parent window
Menu& Menu::getGeometry(int& left, int& top,
						unsigned int& width, unsigned int& height,
						unsigned int& bd, int& direction)
{
	Window root;
	unsigned int depth;
	int wx,wy;

	getWinGeometry(Wind,left,top);

	XGetGeometry(Disp,Wind,&root,
				 &wx,&wy,&width,&height,&bd,&depth);
	direction=Direction;
	return *this;
}

//get Geometry of parent window (window includes menubar and without border)
Menu& Menu::getPGeometry(int& left, int& top)
{
	Window root;
	unsigned int depth,width,height,bd;
	int wx,wy;

	getWinGeometry(Wind,left,top);

	XGetGeometry(Disp,Wind,&root,
				 &wx,&wy,&width,&height,&bd,&depth);

#ifdef DEBUG
	cout << left <<":" << top <<"::" << wx <<":" << wy <<std::endl;
#endif
	left-=wx; top-=wy;
	return *this;
}

Menu& Menu::map(int x, int y) {
#ifdef DEBUG
  std::cout <<"Menu:map...\n";
#endif
  setSize();
  
  // set position
  int left,top;
  if(hasParentFlag){ 	// Get menu position.....
    if(((notMappedYet==False) && (mapType==Static))){
      left=win_x;
      top =win_y;
    }else {
      ParentMenu->item[itemIndex].getGeometry(ParentMenu,left,top);
    }
  } else {
    left=x; top=y;
  }
  
  WHints.x=win_x=left;	WHints.y=win_y=top;
  XMoveWindow(Disp,Wind,WHints.x,WHints.y);
  XSetNormalHints(Disp,Wind,&WHints);
  
  XMapRaised(Disp,Wind);
  Xwin.flush();
  
  moved=False;
  mapped=True;
  selected=False;
  notMappedYet=False;
  
  return *this;
}

Menu& Menu::unmap()
{
  if (mapped==True){
	mapped=False;
	getPGeometry(win_x,win_y);
	XUnmapWindow(Disp,Wind);
  }
	return *this;
}


void Menu::drawFrame(int x, int y, int width, int height, int flag)//flag=Normal
{
	XPoint points[6];
	GC gch, gcl;
	
	if ((flag==Selected)||(flag==InputArea)){ gch=Gc_LO; gcl=Gc_HI;}
	else {gch=Gc_HI; gcl=Gc_LO;}
	points[0].x=x;               points[0].y=y; 
	points[1].x=width;           points[1].y=0; 
	points[2].x=-ITEM_HBD_WIDTH; points[2].y=ITEM_HBD_WIDTH;
	points[3].x=-(width-ITEM_LBD_WIDTH); points[3].y=0;
	XFillPolygon( Disp, Wind, gch, points, 4, CoordModeOrigin, Nonconvex);

	points[0].x=x;               points[0].y=y; 
	points[1].x=ITEM_HBD_WIDTH;  points[1].y=0; 
	points[2].x=0;               points[2].y=height-ITEM_HBD_WIDTH;
	points[3].x=-ITEM_HBD_WIDTH; points[3].y=+ITEM_HBD_WIDTH;
	XFillPolygon( Disp, Wind, gch, points, 4, CoordModeOrigin, Nonconvex);

	points[0].x=x+width-ITEM_LBD_WIDTH; points[0].y=y+ITEM_LBD_WIDTH; 
	points[1].x=0;               points[1].y=height-ITEM_LBD_WIDTH; 
	points[2].x=ITEM_LBD_WIDTH;  points[2].y=0;
	points[3].x=0;               points[3].y=-height;
	XFillPolygon( Disp, Wind, gcl, points, 4, CoordModeOrigin, Nonconvex);
			
	points[0].x=x;               points[0].y=y+height; 
	points[1].x=width; points[1].y=0;
	points[2].x=0;               points[2].y=-ITEM_LBD_WIDTH;
	points[3].x=-(width-ITEM_LBD_WIDTH); points[3].y=0;
	XFillPolygon( Disp, Wind, gcl, points, 4, CoordModeOrigin, Nonconvex);
}

void Menu::redraw() {
#ifdef DEBUG
	cout <<"Menu::redraw...\n";
#endif

	drawFrame(0,0,WHints.width,WHints.height);
	if(Style==Separate)
		for (int i=0;i<Inum;i++) {
			item[i].drawFrame(this,Normal);
		}

	for (int i=0;i<Inum;i++) {
		item[i].drawString(this,Normal);
	}
}

void Menu::drawString(int x, int y, std::string msg, int flag)// flag=Normal
{
	GC gc;
	if ((flag==Normal || flag==Modified)) {gc=Gc_normal_Text;}
	else {gc=Gc_HI_Text;}

	int y0=item_h/2+(ascent-descent)/2;
	XDrawString(Disp,Wind, gc, x, y+y0, msg.c_str(), msg.size());
}

void Menu::drawStatus(int x, int y, int flag)//flag=Normal)
{
	GC gc;
	
	if (flag==Normal) {gc=Gc_normal;}
	else {gc=Gc_HI;}
		
	x+=ITEM_HBD_WIDTH;
	y+=ITEM_HBD_WIDTH;

	unsigned int width =item_w-ITEM_LBD_WIDTH-ITEM_HBD_WIDTH;
	unsigned int height=item_h-ITEM_LBD_WIDTH-ITEM_HBD_WIDTH;
  
	XFillRectangle(Disp,Wind,gc,x,y,width,height);
}

int Menu::getItemNum(XEvent ev){
	int pos,width;
	if (Direction==HORIZONTAL) { pos= ev.xbutton.x; width=item_w; }
		else { pos=ev.xbutton.y; width=item_h; }
#ifdef DEBUG
	cout <<"Menu::getItemNum: dir "<<Direction<<"::"<<
		pos<<":"<<item_h<<":"<<pos/width<<std::endl;
#endif
	return pos/width;
}

void Menu::execItem(int i) {
#ifdef DEBUG
	cout <<"Menu::execItem"<<std::endl;
#endif

	selected=True;
	//	XSelectInput(Disp,Wind,ExposureMask);
	item[i].exec(this);
	if (mapType==Temporal) unmap();
}

void MenuItem::actUnmapMenu(){
	if (Type==Submenu){
		ItemMenu* submenu;
		submenu=(ItemMenu *)cell;
#ifdef DDD
		if(submenu->menu->moved==False)
		submenu->menu->unmap();
#else
		if(submenu->menu->mapType==Temporal)
			submenu->menu->unmap();
#endif
	}
}

void MenuItem::actLeaveNotify(Menu* menu)
{
	if((Type==GetDouble)||(Type==GetString)){
	} else {
		drawStatus(menu,Normal);
	}
}

void MenuItem::actSetNormal(Menu* menu)
{
#ifdef DEBUG
	cout<<"MenuItem::actSetNormal"<<std::endl;
#endif
	if (Type==GetString){
		ItemGetString* getitem;
		getitem=(ItemGetString *)cell;
		getitem->iw->setCursor(menu,Erase);
	} else if (Type==GetDouble){
		ItemGetDouble* getitem;
		getitem=(ItemGetDouble *)cell;
		getitem->iw->setCursor(menu,Erase);
	} else {
		drawStatus(menu,Normal);
	}
}

void MenuItem::actButtonPress(Menu* menu)
{
  	if ((Type!=GetString)&&(Type==GetString))
		drawStatus(menu,Selected);
}

void MenuItem::actMotionNotify(Menu* menu)
{
#ifdef DEBUG
	cout<<"MenuItem::actMotionNotify:"<<index<<":"<<Type<<std::endl;
#endif
	if (Type==GetString){
		ItemGetString* getitem;
		getitem=(ItemGetString *)cell;
		getitem->iw->setCursor(menu);
	} else if (Type==GetDouble){
		ItemGetDouble* getitem;
		getitem=(ItemGetDouble *)cell;
		getitem->iw->setCursor(menu);
	} else {
		drawStatus(menu,Focused);
	}
}

void MenuItem::actKeyPress(Menu* menu, XEvent ev)
{
#ifdef DEBUG
	cout<<"MenuItem::actKeyPress:"<<Type<<std::endl;
#endif
	if (Type==GetString){
		  ItemGetString* getitem;
		  getitem=(ItemGetString *)cell;
		  getitem->iw->getString(ev,getitem->value);
		  drawString(menu,Normal);
		  getitem->iw->setCursor(menu);
	} else if (Type==GetDouble){
		  ItemGetDouble* getitem;
		  double val;
		  getitem=(ItemGetDouble *)cell;
		  getitem->iw->getValue(ev,val);
		  getitem->setValue(val);
		  drawString(menu,Normal);
		  getitem->iw->setCursor(menu);
	}
}

Menu& Menu::act() {
	XEvent  ev;
  
#ifdef DEBUG
	//	cout <<"Menu::act"<<std::endl;
#endif
	if (XCheckWindowEvent(Disp,Wind,ev_mask,&ev)) {
		switch (ev.type) {
			int i;

		case Expose: this->redraw(); break;

		case ButtonPress:
			for (int i=0;i<Inum;++i){
				item[i].actUnmapMenu();
			}

			lastItem=getItemNum(ev);
			if (lastItem>=Inum) { lastItem=-1; }
			else {
				item[lastItem].actButtonPress(this);
				while(XCheckTypedWindowEvent(Disp,Wind,ButtonRelease,&ev)==0){}

				i=getItemNum(ev);
				if (i==lastItem) execItem(i);
				item[lastItem].drawStatus(this,Focused);
				XSelectInput(Disp,Wind,ev_mask);
			}
			break;

		case MotionNotify:
			if (lastItem!=-1){
				item[lastItem].actSetNormal(this);
			}
			i=getItemNum(ev);
			if (i>=Inum) { lastItem=-1; }
			else {
				item[i].actMotionNotify(this);
				lastItem=i;
			}
			break;

		case LeaveNotify:
			if (lastItem!=-1) {
				item[lastItem].actSetNormal(this);
			}
			lastItem=-1;
			break;

		case KeyPress:
			i=getItemNum(ev);
			if (lastItem!=-1) {
				item[lastItem].actSetNormal(this);
			}
			if (i>=Inum) { lastItem=-1; }
			else {
				item[i].actKeyPress(this,ev);
				lastItem=i;
			}
			break;
#ifdef DDD
		case ConfigureNotify:
			if (mapped){
				int x,y;
				getPGeometry(x,y);
//				cout << win_x <<":" << x <<":" << win_y <<":" << y <<std::endl;
				if (win_x!=x || win_y!=y) moved=True;
			}
			break;
#endif
		}
	}

	for (int i=0;i<Inum;++i) item[i].act();

	return *this;
}

Menu& Menu::close() {
	selected=False;
	XDestroyWindow(Disp,Wind);
 
	return *this;
}

//------------------- common handler for static menu ----------------------
Menu& Menu::Rectangle(int x,int y, unsigned int w,unsigned int h, int status)
{
	GC gc;
	if (status==Draw){gc=Gc_LO;}
	else gc=Gc_HI;
	
	XFillRectangle(Disp,Wind,gc,x,y,w,h);
	return *this;
}

void MenuItem::drawCursor(int pos, Menu* menu, int status)// status=Draw
{
	unsigned int w,h;
	menu->retStrLen("z",w,h);
	menu->Rectangle(left+pos+inmargin+INPUT_STR_MARGIN,
					top+ITEM_HBD_WIDTH+ITEM_LBD_WIDTH,
				   w, h,status);
}

void InputItem::setCursor(Menu* menu, int status)// status=Draw)
{
#ifdef DEBUG
	cout << "InputItem::setCusor: setting cursor..."<<std::endl;
#endif
	unsigned int w,h;
	menu->retStrLen(input,w,h);
	menu->item[index].drawCursor(w,menu,status);
}

InputItem& InputItem::setDefault(const std::string dflt)
{
  input=dflt;
  retval=dflt;
  return *this;
}
  
InputItem& InputItem::exec(XEvent ev)
{
  char buf[32];
  int num;
  int index;
  XKeyEvent* kev;
  
#ifdef DEBUG
  std::cout <<"InputItem::exec"<<std::endl;
#endif
  index=input.size();
  status=Modified;
  
  kev=&ev.xkey;
  KeySym key;
  num=XLookupString(kev,buf,MAX_STR_LEN,&key,NULL);
  if (key==XK_Return) { retval=input; status=InputArea; }
  else {
    if (key==XK_BackSpace||key==XK_Delete) {
      if (index>0) input[--index]='\0';
    }else if(key==XK_Escape) {
      index=0; input[index]='\0';
    }else if (num==1) {
      if (index<MAX_STR_LEN)
	{input[index]=buf[0]; input[++index]='\0';}
    }
  }
  return *this;
}
		  
InputItem& InputItem::getString(XEvent ev, std::string str)
{
  exec(ev);
  if(!retval.empty()) str=retval;
  
  return *this;
}

InputItem& InputItem::getValue(XEvent ev, double& value)
{
  exec(ev);
  //if(retval!=NULL) gcvt(atof(input),INPUT_DIGIT_LEN,input);
  input.resize(INPUT_DIGIT_LEN);
  if(!retval.empty()) value=std::stod(retval);
  
  return *this;
}
