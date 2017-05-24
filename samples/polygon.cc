#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgrクラスを使うのに必要

JWindow win;             //開くウィンドウを宣言
Graph   g;               //ウィンドウに描画するグラフを宣言

main(){
  extern JWindow win;             //開くウィンドウを宣言
  extern Graph   g;               //ウィンドウに描画するグラフを宣言
  double xmax, ymax;
  char  *msg="Click the mouse!";
  
  // ウィンドウと、そこに描くグラフを登録-----------------------
  win.open(10,10)  // ウィンドウを(10,10)の位置に開く宣言
    .graph(g.open( 300, 0, 250, 50 )) 
    .map();            // ウィンドウを画面上にマップする。
  
  //ここからは、グラフ g の属性設定------------------------------
  xmax = 10.0;
  ymax = 10.0;
  g.setRatio( xmax, ymax, 300.0, 300.0 ) 
    // xmax, ymax の値をそれぞれ、画面上では300dot, 300dotの長さで描画
    .flush();          // グラフィック・バッファにたまってるデータをフラッシュ
  
  //メインルーチン：グラフg上に線を書く.....
  g.clear()
    .EPSOn()
    .color("red")
    .fillCircle(4,2.5,1.2);
  
  g.color("blue")
    .poly(0,0)
    .poly(7,4.5)
    .poly(10,2.5)
    .fillPoly(10,0);
  
  g.color("pink")
    .poly(0,4.5)
    .poly(3,5)
    .poly(2,5.6)
    .poly(8,5.3)
    .closePoly(5,4.7);
  
  g.color("green")
    .fillRectangle(5,-0.2,1,0.7)
    .color("brown")
    .poly(4.5,-0.2)
    .poly(6.5,-0.2)
    .poly(6,0.3)
    .fillPoly(5,0.3)
    .EPSOff()
    .flush();
  
  g.color("green")
    .text( 4, 7, msg) // テキスト表示
    .EPSOut("testpoly.eps")
    .mouseWait();      // マウス・クリックを待つ
  
  Xwin.close();          // 終了の儀式
}
