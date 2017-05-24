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
  
  // ウィンドウをひらいて、その中にグラフをかく
  win.open(10,10)		        // ウィンドウを開く
    .graph(g.open( 500, 50, 200, 40 ))	// ウィンドウの中にグラフを開く
    .map();				// ウィンドウ描画
  
  xmax = 10.0;	ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 );
  //上で定義したグラフのスケーリングパラメータを設定
  // x軸では, 値10.0をスクリーン上の500ドットにする
  // y軸では, 値100.0をスクリーン上の200ドットにする
  
  // 座標軸を書く --------------------------------------------------------
  g.axis()			// x,y座標軸を書く
    .text( 5, -15, msg )	// メッセージを描く
    .flush()			// グラフィックバッファをフラッシュ
    .mouseWait(); 		// マウスクリックを待つ
  
  // 四角で囲まれた座標軸を書く-------------------------------------------
  g.axisBox()			// 四角で囲まれた座標軸を書く
    .text( 5, -15, msg )	// メッセージを描く
    .flush()			// グラフィックバッファをフラッシュ
    .mouseWait(); 		// マウスクリックを待つ
  
  // 直線を描く ----------------------------------------------------------
  for( double i=1.0; i<=xmax; i+=0.1 ){
    g.color("green")
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      .text( 5, -15, msg )	// メッセージを描く
      .flush();		        // グラフィックバッファをフラッシュ
    mouseBreak();		// マウスクリックがあれば break(マクロ定義)
  }
  
  g.color("black")
    .text( 5, -15, msg )
    .mouseWait();
  
  // 円を描く -------------------------------------------------------------
  double r=1.0;			// 半径定義
  h=0.1;
  for( i=1; h*(double)i+2.0*r <=xmax; ++i ){
    g.clear()                   // グラフを消す
      .axisBox()		// 座標軸を描く(上記参照)
      .color("red")
      .circle( h*(double)i+r, 25.0, r ) //円を描く
      .flush()
      .color("black");
    for(int j=1; j<2000000; ++j ); // wait a moment...
    mouseBreak();		// マウスクリックがあれば break(マクロ定義)
  }
  
  g.color("red")
    .fillCircle( h*(double)(i-1)+r, 25.0, r ) //塗りつぶした円を描く
    .color("black")
    .text( 5, -15, msg)
    .flush()
    .mouseWait();
  
  // 四角形を描く --------------------------------------------------------
  double width = 2.0;
  double height = 20.0;
  h=0.05;
  for( i=1; h*(double)i+width <=xmax; ++i ){
    g.clear()                   // グラフを消す
      .axisBox()
      .rectangle( h*(double)i, height, width, height )//四角形を描く
      .flush();
    mouseBreak();
    for(int j=1; j<1000000; ++j ); // wait a moment...
  }
  
  g.fillRectangle( h*(double)(i-1), height, width, height )//塗りつぶした四角形を描く
    .mouseWait();
  
  // 点を描く --------------------------------------------------------
  g.clear()
    .setScaleFull()
    .axisBox();
  h=0.0002;
  for( i=1; i <=(int)(1.0/h); ++i ){
    g.color("blue")
      .pset( (1-i*h)*xmax*drand48(), (1-i*h)*ymax*drand48() )//点を描く
      .flush();
    mouseBreak();
  }
  
  g.color("black")
    .text( 5, -15, msg)
    .mouseWait();
  
  // end  --------------------------------------------------------
  Xwin.close();         	// おわり
}
