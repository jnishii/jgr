#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgrクラスを使うのに必要

JWindow win[2];             //開くウィンドウを宣言
Graph   g1,g2;               //ウィンドウに描画するグラフを宣言

int main(void)
{
  int xmax, ymax;
  int xmax2, ymax2;
  XEvent ev;
  char *msg="Click the Mouse, Please!!";
  char xy[20];
  int  FNUM=8;
  int  defsize[]={ 8,10,12,14,17,18,20,24 };
  
  // ウィンドウと、そこに描くグラフを登録-----------------------
  xmax = 1000;
  ymax = 150;
  win[0].open(10,10)  // ウィンドウを(10,10)の位置に開く宣言
    .graph(g1.open( xmax, 0, ymax, 0 ))
    .setTextFont(Courier,Roman,14) // default font
    .map(); //default fontは各グラフにmap時に設定される。
  
  ymax2 = 300;
  win[1].open(10, 280)
    .graph(g2.open( xmax, 0, ymax2, 0 )) 
    .map();

  //ここからは、グラフ g の属性設定------------------------------
  g1.setRatio(300, 65, xmax, ymax )
    .text( 100, 30, "Click the mouse!!" );
    // xmax, ymax の値をそれぞれ、画面上では300, 65の長さで描画
  g2.setRatio(350, 150, xmax, ymax2 ) ;

  Xwin.mouse_wait();    // マウス・クリックの入力を待つ
  g1.clear();
  
  int x=20, xstep=50;
  g1.text( x, 0, "Courier, Roman, 14" )
    .setTextFont(Courier,Italic,14)
    .text( x, 20, "Courier, Italic, 14" ) 
    .setTextFont(Courier, Bold, 14)
    .text( x, 40, "Courier,Bold, 14" ) 
    .setTextFont(Courier, Oblique, 14)
    .text( x, 60, "Courier,Oblique, 14" );

  x+=xstep; 
  g1.setTextFont(Helvetica,Roman, 14) 
    .text( x, 00, "Helvetica, Roman, 14" ) 
    .setTextFont(Helvetica,Italic, 14) 
    .text( x, 20, "Helvetica, Italic, 14" ) 
    .setTextFont(Helvetica,Bold, 14) 
    .text( x, 40, "Helvetica, Bold, 14" ) 
    .setTextFont(Helvetica,Oblique, 14) 
    .text( x, 60, "Helvetica, Oblique, 14" );

  x+=xstep;
  g1.setTextFont(Times,Roman, 14) 
    .text( x, 0, "Times, Roman, 14" )
    .setTextFont(Times,Italic, 14) 
    .text( x, 20, "Times, Italic, 14" )
    .setTextFont(Times,Bold, 14) 
    .text( x, 40, "Times, Bold, 14" )
    .setTextFont(Times,Oblique, 14) 
    .text( x, 60, "Times, Oblique, 14" );
  
  x+=xstep;
  g1.setTextFont(Gothic,Roman, 14) 
    .text( x, 0, "ゴシック, Roman, 14" )
    .setTextFont(Gothic,Italic, 14) 
    .text( x, 20, "ゴシック, Italic, 14" )
    .setTextFont(Gothic,Bold, 14) 
     .text( x, 40, "ゴシック, Bold, 14" ) 
     .setTextFont(Gothic,Oblique, 14) 
     .text( x, 60, "ゴシック, Oblique, 14" ) 
    .flush();

  x+=xstep;
  g1.setTextFont(Mincho,Roman, 14) 
    .text( x, 0, "明朝, Roman, 14" )
    .setTextFont(Mincho,Italic, 14) 
    .text( x, 20, "明朝, Italic, 14" )
    .setTextFont(Mincho,Bold, 14) 
     .text( x, 40, "明朝, Bold, 14" ) 
     .setTextFont(Mincho,Oblique, 14) 
     .text( x, 60, "明朝, Oblique, 14" ) 
    .flush();

  x+=xstep;
  g1.setTextFont(Symbol,Roman, 14) 
    .text( x, 0, "Symbol, Roman, 14" )
    .setTextFont(Symbol,Italic, 14) 
    .text( x, 20, "Symbol, Italic, 14" )
    .setTextFont(Symbol,Bold, 14) 
     .text( x, 40, "Symbol, Bold, 14" ) 
     .setTextFont(Symbol,Oblique, 14) 
     .text( x, 60, "Symbol, Oblique, 14" ) 
    .flush();

  int y=5, ystep=20;
  char fntmsg[32];
  for(int i=0;i<FNUM;i++){
    x=40, xstep=55;
    sprintf(fntmsg,"Courier, Roman, %d", defsize[i]);
    g2.setTextFont(Courier, Roman, defsize[i])
      .text( x, y, fntmsg );

    x+=xstep;
    sprintf(fntmsg,"Helvetica, Roman, %d", defsize[FNUM-i-1]);
    g2.setTextFont(Helvetica, Roman, defsize[FNUM-i-1])
      .text( x, y, fntmsg );

    x+=xstep;
    sprintf(fntmsg,"Times, Roman, %d", defsize[i]);
    g2.setTextFont(Times, Roman, defsize[i])
      .text( x, y, fntmsg );

    x+=xstep;
    sprintf(fntmsg,"ゴシック, Roman, %d", defsize[FNUM-i-1]);
    g2.setTextFont(Gothic, Roman, defsize[FNUM-i-1])
      .text( x, y, fntmsg );

    x+=xstep;
    sprintf(fntmsg,"明朝 , Roman, %d", defsize[i]);
    g2.setTextFont(Mincho, Roman, defsize[i])
      .text( x, y, fntmsg );

    x+=xstep;
    sprintf(fntmsg,"Symbol, Roman, %d", defsize[FNUM-i-1]);
    g2.setTextFont(Symbol, Roman, defsize[FNUM-i-1])
      .text( x, y, fntmsg );

    y+=ystep;
  }

  // end  --------------------------------------------------------
  Xwin.mouseWait()
    .close();          // 終了の儀式
}
