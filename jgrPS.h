// *****************************************************************
// jgrPS.h: PS handling Library Time-stamp: <08/11/14 12:03:27 jun> 
// *****************************************************************

// defining format of graph
const int BOTTOM_MARGIN=25;
const int LEFT_MARGIN=25;
//---------------------------
#define Version	"jgr Ver.3.4"
const int GrpMONO=0;
const int GrpRGB=1;
const int COLORGRAIN=144;

// Data type of PS
enum PStype {
	ps_status,
	ps_color, ps_width,
	ps_newtext, ps_font,
	ps_ltype,
	ps_line, ps_lline, ps_circle, ps_circlef, ps_arc, ps_fan, ps_fanf,
	ps_box, ps_boxf, ps_poly, ps_polyf
};

enum { Begin, End };		// status of drawing
enum { Xaxis, Yaxis, AxisBox };	// mode of drawing

static std::string PSstatusMsg[]={ "%%%%Begin ", "%%%%End "};
static std::string PSmodeMsg[]={ "x axis\n", "y axis\n", "axis box\n" };

typedef struct {
        XColor	val;
	char mode;	// GrpMONO or GrpRGB 
} PScolor;

typedef struct { int status, mode; }		PSstatus;
typedef struct { int width; }			PSwidth;
typedef struct { int type; }             	PSLineType;
typedef struct { double x1, y1, x2, y2; }	PSline;
typedef struct { double x, y; }			PSlline;//from last point
typedef struct { double x, y, r, ang1, ang2; }	PScircle;
typedef struct { double x, y, w, h; }		PSbox;
typedef struct { double *xp, *yp; int n; }	PSpolygon;//xp,yp:top address of an array
typedef struct { char *font;  int size;	 }	PSfont;
typedef struct { double x, y; char *str; }	PStext;
typedef struct _cel { struct _cel *next; int kind; void *data; } PScell;

/* data struct for each window */
typedef struct {
	int	width, height;
	int	cmode;		// color mode (GrMONO, GrRGB)
	XColor	color;
	unsigned long cpix;	// current pixel
	int	lwidth;		// line width
	PScell	ps, *tail;
} jgrData; 	// XEGw
//---------------------------------------------------------------------
