#include <iostream.h>
#include <jkit/jgr.h>    //jgrクラスを使うのに必要

JWindow win;             //開くウィンドウを宣言
Graph   g;               //ウィンドウに描画するグラフを宣言
Menu main_menu,sub_menuA,sub_menuB;


///////////////////////// サブニューのためのハンドラ ////////////////////

char str[MAX_STR_LEN+1];
double v;

/////////////////////// メインメニューのためのハンドラ //////////////////

void handler_main1(void* dummy){        //一時的サブメニューを開く
}

void handler_main2(void* dummy){        //静的なサブメニューを開く
  g.EPSOut("out.eps");
}
void handler_main3(void* dummy){       //終了する
  Xwin.close();
  exit(1);
}

/////////////////////////// メインルーチン /////////////////////////////

main(){
  double xmax, ymax;
  
  main_menu.open()
    .addItem("Go", handler_main1)      // アイテムの登録
    .addItem("EPS", handler_main2)
    .addItem("end",  handler_main3);
  
  // ウィンドウと、そこに描くグラフを登録-----------------------
  win.open(10,10)  // ウィンドウを(10,10)の位置に開く宣言
    .menu(main_menu)
    .graph(g.open( 500, 50, 200, 40 )) 
    // 開いたウィンドウに、x軸の正負方向が(500,50)、
    //y軸の正負方向(200,40)のグラフを登録
    .map()
    .flush();
  
  //ここからは、グラフ g の属性設定------------------------------
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 ) 
    // xmax, ymax の値をそれぞれ、画面上では500dot, 200dotの長さで描画
    .axis() // X軸, Y軸を描画
    .flush();
  
  //メインルーチン：グラフg上に線を書く.....
  for( double i=1.0; i<=xmax; i+=0.5 ){
    g.color("blue") // グラフ g の描画色を青にする
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      // (i,0.0) から (0.0, (xmax-(double)i)*ymax/xmax)に線を引く
      .flush();   
  }
  
  while(1) {
    main_menu.act();                   // メインメニューを走らせる
  }
  // end  --------------------------------------------------------
  Xwin.close();          // 終了の儀式
}
