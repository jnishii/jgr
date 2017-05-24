#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgr���饹��Ȥ��Τ�ɬ��

JWindow win;             //����������ɥ������
Graph   g;               //������ɥ������褹�륰��դ����

main(){
  double xmax, ymax;
  XEvent ev;
  char *msg="Click the Mouse, Please!!";
  char xy[20];
  
  // ������ɥ��ȡ���������������դ���Ͽ-----------------------
  win.open(10,10)  // ������ɥ���(10,10)�ΰ��֤˳������
    .graph(g.open( 800, 50, 400, 40 )) 
    // ������������ɥ��ˡ�x��������������(500,50)��
    //y������������(200,40)�Υ���դ���Ͽ
    .map()            // ������ɥ�����̾�˥ޥåפ��롣
    .mouseWait();    // �ޥ���������å������Ϥ��Ԥ�
  
  //��������ϡ������ g ��°������------------------------------
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 ) ;
    // xmax, ymax ���ͤ򤽤줾�졢���̾�Ǥ�500dot, 200dot��Ĺ��������

  while(1){
    g.color("red")
      .axis() // X��, Y��������
      .text( 7, -15, msg )  // ��å������򥰥�վ��(5,-15)�ΰ��֤˽���
      .flush()          // ����ե��å����Хåե��ˤ��ޤäƤ�ǡ�����ե�å���
      .mouseWait(ev);
    
    //�ɤΥܥ��󤬥���å�����ɽ�����ʲ��Τ褦�˸ƤӽФ����Ȥ�Ǥ��롣
    //Xwin.ev2button(ev); win.ev2button(ev); g.ev2button(ev); 
    printf("mouse button # is %d\n", ev2button(ev));

    //window��κ�ɸ�͡����夬������ñ�̤ϲ��ǿ�
    printf("mouse action at (%d,%d) on the window coordinate\n", ev.xbutton.x, ev.xbutton.y);

    //����վ�κ�ɸ��
    printf("mouse action at (%lf,%lf) on the graph  coordinate\n", g.ev2x(ev), g.ev2y(ev));
    sprintf(xy,"(%3.1lf, %3.1lf)", g.ev2x(ev), g.ev2y(ev));
    g.color("red")
      .cross(g.ev2x(ev),g.ev2y(ev),0.1)
      .text(g.ev2x(ev),g.ev2y(ev),xy,LeftTop)
      .flush();
  }

  
  // end  --------------------------------------------------------
  Xwin.close();          // ��λ�ε���
}
