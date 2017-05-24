#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgrクラスを使うのに必要

JWindow win;             //開くウィンドウを宣言
Graph   g;               //ウィンドウに描画するグラフを宣言

main(){
  extern JWindow win;             //開くウィンドウを宣言
  extern Graph   g;               //ウィンドウに描画するグラフを宣言
  double xmax, ymax;
  char *msg="Click the Mouse, Please!!";
  
  // ウィンドウと、そこに描くグラフを登録-----------------------
  win.open(10,10)  // ウィンドウを(10,10)の位置に開く宣言
    .graph(g.open( 500, 50, 200, 40 )) 
    // 開いたウィンドウに、x軸の正負方向が(500,50)、
    //y軸の正負方向(200,40)のグラフを登録
    .map();            // ウィンドウを画面上にマップする。
  
  //ここからは、グラフ g の属性設定------------------------------
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 ) 
    // xmax, ymax の値をそれぞれ、画面上では500dot, 200dotの長さで描画
    .axis() // X軸, Y軸を描画
    .text( 5, -15, msg )  // メッセージをグラフ上の(5,-15)の位置に出力
    .flush()          // グラフィック・バッファにたまってるデータをフラッシュ
    .mouseWait();    // マウス・クリックの入力を待つ
  
  //メインルーチン：グラフg上に線を書く.....
  for( double i=1.0; i<=xmax; i+=0.1 ){
    g.color("blue") // グラフ g の描画色を青にする
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      // (i,0.0) から (0.0, (xmax-(double)i)*ymax/xmax)に線を引く
      .flush();   
    mouseBreak();  // マウス・クリックがあったらforループから抜ける
  }
  
  g.color("black")
    .text( 5, -15, msg) // テキスト表示
    .mouseWait();      // マウス・クリックを待つ
  
  Xwin.close();          // 終了の儀式
}
