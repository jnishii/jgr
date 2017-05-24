// *****************************************************************
// jgrPS.cc: PS handling Library Time-stamp: <08/10/07 10:55:28 jun> 
//                modified from [XeasyGraphic]	XEGps
//                             	Ver. 1.01  1993-07-06	PS for JIS
//                              (ogihara@itc.aist-nara.ac.jp)
// *****************************************************************

#ifdef ICC
#include <iostream.h>
#include <iostream>
#include <fstream>
#include <new.h>
#include <cmath>
#else
#include <iostream>
//#include <stream.h>
#include <fstream>
#include <new>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#endif

#include "jgr.h"

extern const int MaxColorVal;
extern const int EPSXMargin;
extern const int EPSYMargin;

/*
const int  MAXCOLORVAL=0x10000;
const int  COLORSTEPS=16;
const int  MapScales=8;
static int isColor = 0;
static int ColorStep = COLORSTEPS;
void Graph::initColor(void)
{
    Visual *v = DefaultVisual (Disp, 0);
    int cells = v->map_entries;
    int i;
    static int step[MapScales] = { 16, 12, 8, 6, 4, 3, 2, 1 };
    static int vals[MapScales] = { 4913, 2197, 729, 343, 125, 64, 27, 8 };

    switch (v->class) {
    case StaticColor:
    case TrueColor:
	isColor = 1;  ColorStep = COLORSTEPS; break;
    case PseudoColor:
    case DirectColor:
	isColor = 1;
	for (i=0; i<MapScales; i++) if (cells >= vals[i]) break;
	if (i == MapScales) { isColor = 0; break; }
	ColorStep = step[i];
	break;
    case StaticGray:
    case GrayScale:
    default:
	isColor = 0; break;
    }
}
*/

void PSHandler::addCell( int k, void *dt)
{
    PScell* ps = new PScell;
    ps->next = NULL;
    ps->kind = k;
    ps->data = dt;
    gdata->tail->next = ps;
    gdata->tail = ps;
}

void PSHandler::freeCell()
{
    PScell *cel, *nx;
    for (cel = gdata->ps.next; cel != NULL; cel = nx) {
	nx = cel->next;
	switch (cel->kind) {
	case ps_newtext:{
		char* cp;
		cp = ((PStext *)(cel->data))->str;
		if (cp != NULL) delete [] cp;
		break;
	        }
	case ps_poly:
	case ps_polyf:{
	        PSpolygon* g; 
		g= (PSpolygon *)(cel->data);
		delete g->xp;
		delete g->yp;
		break;
	        }
	default:
		break;
	}
	delete cel->data;
	delete cel;
    }
    gdata->ps.next = NULL;
    gdata->tail = &(gdata->ps);
}

void PSHandler::newColor(XColor fxcolor)
    {
    gdata->color = fxcolor;
    gdata->cpix  = fxcolor.pixel;
    setColor();
    }

void PSHandler::setColor()
{
    PScolor* clr = new PScolor;
    clr->val = gdata->color;
    clr->mode = gdata->cmode;
    addCell(ps_color, clr);
}

void PSHandler::setLineType(int type)
{
    PSLineType* line = new PSLineType;
	line->type=type;
    addCell(ps_ltype, line);
}

void PSHandler::newStatus(int status, int mode)
{
    PSstatus* psstat = new PSstatus;
    psstat->status = status;
    psstat->mode = mode;
    addCell(ps_status, psstat);
}

void PSHandler::newWidth()
{
    PSwidth* wd = new PSwidth;
    wd->width = gdata->lwidth;
    addCell( ps_width, wd);
}

//------------------------------------------------------

void PSHandler::newLine(double x, double y, double x2, double y2)
    {
      std::set_new_handler(0);
    PSline* w=new PSline;
    if(w==NULL){std::cout<<"failed to obtain new PSline"<<std::endl;exit(1);}
    w->x1=x;    w->y1=y;    w->x2=x2;    w->y2=y2;
    addCell(ps_line,w);
    }

void PSHandler::newLline(double x, double y)
    {
    std::set_new_handler(0);
    PSlline* w=new PSlline;
    if(w==NULL){std::cout<<"failed to obtain new PSlline"<<std::endl;exit(1);}
    w->x=x;    w->y=y;
    addCell(ps_lline,w);
    }

void PSHandler::newBox(int kind, double x, double y, double w, double h)
    {
    std::set_new_handler(0);
    PSbox* box=new PSbox;
    if(box==NULL){std::cout<<"failed to obtain new PSbox"<<std::endl;exit(1);}
    box->x=x;    box->y=y;    box->w=w;    box->h=h;
    addCell(kind,box);
    }

void PSHandler::newCircle( int kind, double x, double y, double r )
    {
    std::set_new_handler(0);
    PScircle* w=new PScircle;
    if(w==NULL){std::cout<<"failed to obtain new PScircle"<<std::endl;exit(1);}
    w->x=x;    w->y=y;    w->r=r;    w->ang1=0;    w->ang2=360;
    addCell(kind,w);
    }

void PSHandler::newPolygon(int kind, double x[], double y[], int n)
    {
    std::set_new_handler(0);
    PSpolygon* w=new PSpolygon;
    if(w==NULL){std::cout<<"failed to obtain new PSpolygon"<<std::endl;exit(1);}
    w->xp=x;    w->yp=y;  w->n=n;
    addCell(kind,w);
    }

char* PSHandler::str_dup(char *s)
    {
    if (s == NULL) return NULL;
    char* p=new char [strlen(s)+1];
    strcpy(p, s);
    return p;
    }

void PSHandler::newFont(char *font, int sz )
{
    PSfont* w = new PSfont;
    w->font = font;    w->size = sz;    
    addCell(ps_font, w);
}

void PSHandler::newText(int kind, double x, double y, char *str)
{
    PStext* w = new PStext;
    w->x = x;    w->y = y;
    w->str = str_dup(str);
    addCell(kind, w);
}

void PSHandler::init(int width, int height, XColor fxcolor) //initializing PS item
    {
    std::set_new_handler(0);	
    gdata=new jgrData;
//    catch(xalloc) {std::cout<<"failed to obtain new jgrData"<<std::endl;exit(1);}
    if(gdata==NULL){std::cout<<"failed to obtain new jgrData"<<std::endl;exit(1);}

    gdata->width=width;    gdata->height=height;
    gdata->lwidth  = 1;
    gdata->ps.next = NULL;
    gdata->tail = &(gdata->ps);
    gdata->cmode = GrpRGB;
    newColor(fxcolor);
    newWidth();
    }

//---------------------------------------------------------------
//  for EPS output
//	modified from	[XeasyGraphic]	GrEPSOut.c
//	Ver. 1.02  1993-07-23	PS debug
//	(ogihara@itc.aist-nara.ac.jp)

#define prf(x)    x<<" "

int PSHandler::out(std::ofstream& f, int xoffset, int yoffset)
    {
    PScell* ps;
    if ((ps = gdata->ps.next) == NULL) return 0;

    while (ps) {
	switch (ps->kind) {
	case ps_status:{
	    PSstatus* psstat = (PSstatus *)(ps->data);
	    f << PSstatusMsg[psstat->status]
	      << PSmodeMsg[psstat->mode];
	    if (psstat->status==End) f << "stroke\n\n";
	    break;
	    }
	case ps_color:{
	    PScolor* clr = (PScolor *)(ps->data);
	    if (clr->mode == GrpMONO) f << prf(clr->val.pixel) << "setgray\n";
	    else {
	      f << "stroke\n"
		<< prf((double)clr->val.red/MaxColorVal)
		<< prf((double)clr->val.green/MaxColorVal)
		<< prf((double)clr->val.blue/MaxColorVal)
		<< "setrgbcolor\n";
	      }
	    break;
	    }
	case ps_ltype:{
		PSLineType* line = (PSLineType *)(ps->data);
		if (line->type==SOLID_LINE){
			f << "[] 0 setdash\n";
		}else if (line->type==DASHED_LINE){
			f << "[4 4] 0 setdash\n";
		}
		break;
	}
	case ps_width:{
	    PSwidth* w = (PSwidth *)(ps->data);
	    f << prf(w->width) << "setlinewidth\n";
	    break;
	    }
	case ps_font:{
	    PSfont* font = (PSfont *)(ps->data);
	    //fprintf(f,"/%s findfont ", font->font);
	    f <<"/"<<font->font<<" findfont ";
	    f  << prf(font->size)
	      << "scalefont setfont\n";
	    break;
	    }
	case ps_newtext:{
	    PStext* t = (PStext *)(ps->data);
	    f << prf(t->x+xoffset)
	      << prf(t->y+yoffset)
	      << "moveto ("
	      << t->str
	      << ") show\n";
	    break;
	    }
	case ps_line:{
	    PSline* w = (PSline *)(ps->data);
	    f << prf(w->x1+xoffset)
	      << prf(w->y1+yoffset)
	      << "moveto "
	      << prf(w->x2+xoffset)
	      << prf(w->y2+yoffset)
	      << "lineto\n";
	    break;
	    }
	case ps_lline:{
	    PSlline* w = (PSlline *)(ps->data);
	    f << prf(w->x+xoffset) << prf(w->y+yoffset) << "lineto\n";
	    break;
	    }
	case ps_circle:
	case ps_circlef:
	case ps_arc:{
	  PScircle* crc = (PScircle *)(ps->data);
	  f << "newpath "
	    << prf(crc->x+xoffset)
	    << prf(crc->y+yoffset)
	    << prf(crc->r)
	    << prf(crc->ang1)
	    << prf(crc->ang2);
	  //	  fprintf(f,"arc%s\n",(ps->kind == ps_circlef)?" closepath fill":"");
	  f << "arc"
	    << ((ps->kind == ps_circlef) ? " closepath fill" : "")
	    <<"\n";
	    break;
	    }
	case ps_fan:
	case ps_fanf:{
	    PScircle* crc = (PScircle *)(ps->data);
	    f << "newpath "
	      << prf(crc->x+xoffset)
	      << prf(crc->y+yoffset)
	      << "moveto "
	      << prf(crc->x+xoffset)
	      << prf(crc->y+yoffset)
	      << prf(crc->r)
	      << prf(crc->ang1)
	      << prf(crc->ang2);
	    //	    fprintf(f,"arc closepath %s\n",(ps->kind == ps_fan)?"":"fill");
	    f<<"arc closepath "
	     <<((ps->kind == ps_fan) ? "" : "fill")
	     <<"\n";
	    break;
	    }
	case ps_box:
	case ps_boxf:{
	    PSbox* w = (PSbox *)(ps->data);
	    f << "newpath\n"
	      << prf(w->x+xoffset)
	      << prf(w->y+yoffset)
	      << "moveto "
	      << prf(w->w)
	      << " 0 rlineto\n"
	      << "0 "
	      << prf(w->h)
	      << "rlineto "
	      << -1*prf(w->w)
	      << "0 rlineto closepath ";
	    //	    fprintf(f,"rect%s\n", (ps->kind == ps_box)?"":"fill");
	    f<<((ps->kind == ps_box)?"":"fill")
	     <<"\n"; 
	    break;
	    }
	case ps_poly:
	case ps_polyf:{
	    PSpolygon* w = (PSpolygon *)(ps->data);
	    double *xl, *yl;
	    xl = w->xp;
	    yl = w->yp;
	    int n = w->n;
	    f << "newpath " 
	      << prf(xl[0]+xoffset)
	      << prf(yl[0]+yoffset)
	      << "moveto ";
	    for (int i=1; i<n; i++) {
		f << prf(xl[i]+xoffset)
		  << prf(yl[i]+yoffset);
		//	  fprintf(f,"lineto%c", ((i & 3) == 3)?'\n':' ');
		f<<"lineto"
		 <<(((i & 3) == 3)?'\n':' ');
	        }
	    //fprintf(f,"closepath %s\n",(ps->kind == ps_poly)?"":"fill");
	    f<<"closepath "
	     <<((ps->kind == ps_poly)?"":"fill")
	     <<"\n";
	    break;
	    }
	} /* switch */
	ps = ps->next;
    }
    f << "stroke\n";
    return 1;
}

void EPSheader(std::ofstream& f, char* fname, int w, int h )
    {
    f.open(fname);
    if(!f) { std::cout <<"failed to open file"<<std::endl; exit(0); }
    f << "%!PS-Adobe-2.0 EPSF-2.0\n";
    f << "%%%%Title: " << fname << "\n";
    f << "%%%%Creator: "<< Version << "\n";
    f << "%%%%BoundingBox: 0 0 " << w+EPSXMargin << " "<<h+EPSYMargin << "\n";
    f << "%%%%EndComments\n\n"
      << "0 setlinejoin 0 setlinecap\n\n";
    }

JWindow& JWindow::EPSOn()
{
  // check if PS is enabled in child graphs
  for (int i=0;i<Ngrp;++i){
    Pgrp[i]->EPSOn();
  }
  return(*this);
}

JWindow& JWindow::EPSOff()
{
  // check if PS is enabled in child graphs
  for (int i=0;i<Ngrp;++i){
    Pgrp[i]->EPSOff();
  }
  return(*this);
}

JWindow& JWindow::EPSOut(char* fname)
{
  // check if PS is enabled in child graphs
  int flag=0;
  for (int i=0;i<Ngrp;++i){
    flag+=Pgrp[i]->PSrecorded;
  }
  if(flag==0){
    voidPSmsg("EPSOut");
    return(*this);
  }
    
  // main routine
  std::ofstream f;
  std::cout <<"Generating EPS file: "<<fname<<"\n";
  EPSheader(f, fname, WHints.width, WHints.height);
  for (int i=0;i<Ngrp;++i){
    //		fprintf(f,"%%%%Drawing Graph:%d\n\n",i); 
    if(Pgrp[i]->PSrecorded){
      f<<"%%%%Drawing Graph:"
       <<i
       <<"\n\n"; 
      Pgrp[i]->EPSAppend(f,Pgrp[i]->left(),WHints.height-Pgrp[i]->bottom());
      f << "%%%%End of Graph:" <<i<<"\n\n";
    }
  }
  (void) f.close();
  std::cout <<"Done...\n";
  return(*this); 
}

Graph& Graph::EPSOut(char* fname,
      		int xoffset, int yoffset)
//		int xoffset=LEFT_MARGIN, int yoffset=BOTTOM_MARGIN)
{
  if(!PSrecorded){
    voidPSmsg("EPSOut");
    return(*this);
  }

  std::ofstream f;
  EPSheader(f, fname, Ps.gdata->width+xoffset, Ps.gdata->height+yoffset);
  
  std::cout <<"Generating EPS file: "<<fname<<"...\n";
  for (int i=0;i<=Nchild;++i)
    {
      f<<"%%%%Drawing Child Graph:"
       <<i
       <<"\n\n"; 
      ChildGraph[i]->EPSAppend(f,xoffset,yoffset);
      f<<"%%%%End of Child Graph:"
       <<i
       <<"\n\n"; 
    }
  f.close();
  std::cout <<"Done...\n";
  return(*this);
}

void Graph::EPSAppend(std::ofstream& epsf, int lmargin, int bmargin)
{
    Ps.out(epsf, lmargin, bmargin);// Convert data to EPS format
}

void Graph::resetEPS() {
  if(!PSflag)
    voidPSmsg("resetEPS");
  else
    Ps.freeCell(); Ps.setColor(); Ps.newWidth();
}
