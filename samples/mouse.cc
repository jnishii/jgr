#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgrクラスを使うのに必要

JWindow win;             //開くウィンドウを宣言
Graph   g;               //ウィンドウに描画するグラフを宣言

main(){
  double xmax, ymax;
  XEvent ev;
  char *msg="Click the Mouse, Please!!";
  char xy[20];
  
  // ウィンドウと、そこに描くグラフを登録-----------------------
  win.open(10,10)  // ウィンドウを(10,10)の位置に開く宣言
    .graph(g.open( 800, 50, 400, 40 )) 
    // 開いたウィンドウに、x軸の正負方向が(500,50)、
    //y軸の正負方向(200,40)のグラフを登録
    .map()            // ウィンドウを画面上にマップする。
    .mouseWait();    // マウス・クリックの入力を待つ
  
  //ここからは、グラフ g の属性設定------------------------------
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 ) ;
    // xmax, ymax の値をそれぞれ、画面上では500dot, 200dotの長さで描画

  while(1){
    g.color("red")
      .axis() // X軸, Y軸を描画
      .text( 7, -15, msg )  // メッセージをグラフ上の(5,-15)の位置に出力
      .flush()          // グラフィック・バッファにたまってるデータをフラッシュ
      .mouseWait(ev);
    
    //どのボタンがクリックかを表示。以下のように呼び出すこともできる。
    //Xwin.ev2button(ev); win.ev2button(ev); g.ev2button(ev); 
    printf("mouse button # is %d\n", ev2button(ev));

    //window上の座標値。左上が原点。単位は画素数
    printf("mouse action at (%d,%d) on the window coordinate\n", ev.xbutton.x, ev.xbutton.y);

    //グラフ上の座標値
    printf("mouse action at (%lf,%lf) on the graph  coordinate\n", g.ev2x(ev), g.ev2y(ev));
    sprintf(xy,"(%3.1lf, %3.1lf)", g.ev2x(ev), g.ev2y(ev));
    g.color("red")
      .cross(g.ev2x(ev),g.ev2y(ev),0.1)
      .text(g.ev2x(ev),g.ev2y(ev),xy,LeftTop)
      .flush();
  }

  
  // end  --------------------------------------------------------
  Xwin.close();          // 終了の儀式
}
