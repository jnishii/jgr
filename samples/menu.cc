//
// メニューウィンドウ、入力ウィンドウの使い方の例
//
//   ３つのアイテムを持つメインメニューを開く。
//   １つめのアイテムで入力を受け付けるための一時的なサブメニューを開く
//     サブメニューの１つめのアイテムで、文字列を受け付ける。
//     サブメニューの２つめのアイテムで、数値を受け付ける。
//   ２つめのアイテムで変数を標準出力に表示するための静的なサブメニューを開く
//     サブメニューの１つめのアイテムで、文字列を標準出力に出す。
//     サブメニューの２つめのアイテムで、数値を標準出力に出す。
//   ３つめのアイテムで終了する。
//
//
#include <iostream.h>
#include <stdlib.h>
#include <jkit/jgr.h>

Menu main_menu(Horizontal),
	sub_menuA(Vertical,Static),
	sub_menuB(Vertical,Static);

char str[MAX_STR_LEN+1];
//char str[256];
double v;

///////////////////////// サブメニューのためのハンドラ ////////////////////

void handler_subB1(void* dummy){         //文字列を表示する
  cout << "str= " << str << "\n";
}
void handler_subB2(void* dummy){         //数値を表示する
  cout << "v= " << v << "\n";
}

////////////////////// メインルーチン //////////////////////

int main(){
  sub_menuA.open()
	  .addGetString(str,"Input string","aaa")
	  .addGetValue(v,"Input value",10.0)
	  .addClose();

  sub_menuB.open()
	  .addItem("string",handler_subB1)
	  .addItem("value", handler_subB2)
	  .addClose();

  main_menu.open(100,50)
	  .addSubmenu("input",sub_menuA)
	  .addSubmenu("print",sub_menuB)
	  .addExit()
	  .map();

  while(1) {main_menu.act();}                   // メインメニューを走らせる
}
