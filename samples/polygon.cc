#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgr���饹��Ȥ��Τ�ɬ��

JWindow win;             //����������ɥ������
Graph   g;               //������ɥ������褹�륰��դ����

main(){
  extern JWindow win;             //����������ɥ������
  extern Graph   g;               //������ɥ������褹�륰��դ����
  double xmax, ymax;
  char  *msg="Click the mouse!";
  
  // ������ɥ��ȡ���������������դ���Ͽ-----------------------
  win.open(10,10)  // ������ɥ���(10,10)�ΰ��֤˳������
    .graph(g.open( 300, 0, 250, 50 )) 
    .map();            // ������ɥ�����̾�˥ޥåפ��롣
  
  //��������ϡ������ g ��°������------------------------------
  xmax = 10.0;
  ymax = 10.0;
  g.setRatio( xmax, ymax, 300.0, 300.0 ) 
    // xmax, ymax ���ͤ򤽤줾�졢���̾�Ǥ�300dot, 300dot��Ĺ��������
    .flush();          // ����ե��å����Хåե��ˤ��ޤäƤ�ǡ�����ե�å���
  
  //�ᥤ��롼���󡧥����g��������.....
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
    .text( 4, 7, msg) // �ƥ�����ɽ��
    .EPSOut("testpoly.eps")
    .mouseWait();      // �ޥ���������å����Ԥ�
  
  Xwin.close();          // ��λ�ε���
}
