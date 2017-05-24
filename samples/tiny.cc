#include <stream.h>
#include <iostream.h>
#include <jkit/jgr.h>    //jgr���饹��Ȥ��Τ�ɬ��

JWindow win;             //����������ɥ������
Graph   g;               //������ɥ������褹�륰��դ����

main(){
  extern JWindow win;             //����������ɥ������
  extern Graph   g;               //������ɥ������褹�륰��դ����
  double xmax, ymax;
  char *msg="Click the Mouse, Please!!";
  
  // ������ɥ��ȡ���������������դ���Ͽ-----------------------
  win.open(10,10)  // ������ɥ���(10,10)�ΰ��֤˳������
    .graph(g.open( 500, 50, 200, 40 )) 
    // ������������ɥ��ˡ�x��������������(500,50)��
    //y������������(200,40)�Υ���դ���Ͽ
    .map();            // ������ɥ�����̾�˥ޥåפ��롣
  
  //��������ϡ������ g ��°������------------------------------
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 ) 
    // xmax, ymax ���ͤ򤽤줾�졢���̾�Ǥ�500dot, 200dot��Ĺ��������
    .axis() // X��, Y��������
    .text( 5, -15, msg )  // ��å������򥰥�վ��(5,-15)�ΰ��֤˽���
    .flush()          // ����ե��å����Хåե��ˤ��ޤäƤ�ǡ�����ե�å���
    .mouseWait();    // �ޥ���������å������Ϥ��Ԥ�
  
  //�ᥤ��롼���󡧥����g��������.....
  for( double i=1.0; i<=xmax; i+=0.1 ){
    g.color("blue") // ����� g �����迧���Ĥˤ���
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      // (i,0.0) ���� (0.0, (xmax-(double)i)*ymax/xmax)���������
      .flush();   
    mouseBreak();  // �ޥ���������å������ä���for�롼�פ���ȴ����
  }
  
  g.color("black")
    .text( 5, -15, msg) // �ƥ�����ɽ��
    .mouseWait();      // �ޥ���������å����Ԥ�
  
  Xwin.close();          // ��λ�ε���
}
