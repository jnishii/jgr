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
  
  // ������ɥ���Ҥ餤�ơ�������˥���դ򤫤�
  win.open(10,10)		        // ������ɥ��򳫤�
    .graph(g.open( 500, 50, 200, 40 ))	// ������ɥ�����˥���դ򳫤�
    .map();				// ������ɥ�����
  
  xmax = 10.0;	ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 );
  //��������������դΥ�������󥰥ѥ�᡼��������
  // x���Ǥ�, ��10.0�򥹥��꡼����500�ɥåȤˤ���
  // y���Ǥ�, ��100.0�򥹥��꡼����200�ɥåȤˤ���
  
  // ��ɸ����� --------------------------------------------------------
  g.axis()			// x,y��ɸ�����
    .text( 5, -15, msg )	// ��å�����������
    .flush()			// ����ե��å��Хåե���ե�å���
    .mouseWait(); 		// �ޥ�������å����Ԥ�
  
  // �ͳѤǰϤޤ줿��ɸ�����-------------------------------------------
  g.axisBox()			// �ͳѤǰϤޤ줿��ɸ�����
    .text( 5, -15, msg )	// ��å�����������
    .flush()			// ����ե��å��Хåե���ե�å���
    .mouseWait(); 		// �ޥ�������å����Ԥ�
  
  // ľ�������� ----------------------------------------------------------
  for( double i=1.0; i<=xmax; i+=0.1 ){
    g.color("green")
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      .text( 5, -15, msg )	// ��å�����������
      .flush();		        // ����ե��å��Хåե���ե�å���
    mouseBreak();		// �ޥ�������å�������� break(�ޥ������)
  }
  
  g.color("black")
    .text( 5, -15, msg )
    .mouseWait();
  
  // �ߤ����� -------------------------------------------------------------
  double r=1.0;			// Ⱦ�����
  h=0.1;
  for( i=1; h*(double)i+2.0*r <=xmax; ++i ){
    g.clear()                   // ����դ�ä�
      .axisBox()		// ��ɸ��������(�嵭����)
      .color("red")
      .circle( h*(double)i+r, 25.0, r ) //�ߤ�����
      .flush()
      .color("black");
    for(int j=1; j<2000000; ++j ); // wait a moment...
    mouseBreak();		// �ޥ�������å�������� break(�ޥ������)
  }
  
  g.color("red")
    .fillCircle( h*(double)(i-1)+r, 25.0, r ) //�ɤ�Ĥ֤����ߤ�����
    .color("black")
    .text( 5, -15, msg)
    .flush()
    .mouseWait();
  
  // �ͳѷ������� --------------------------------------------------------
  double width = 2.0;
  double height = 20.0;
  h=0.05;
  for( i=1; h*(double)i+width <=xmax; ++i ){
    g.clear()                   // ����դ�ä�
      .axisBox()
      .rectangle( h*(double)i, height, width, height )//�ͳѷ�������
      .flush();
    mouseBreak();
    for(int j=1; j<1000000; ++j ); // wait a moment...
  }
  
  g.fillRectangle( h*(double)(i-1), height, width, height )//�ɤ�Ĥ֤����ͳѷ�������
    .mouseWait();
  
  // �������� --------------------------------------------------------
  g.clear()
    .setScaleFull()
    .axisBox();
  h=0.0002;
  for( i=1; i <=(int)(1.0/h); ++i ){
    g.color("blue")
      .pset( (1-i*h)*xmax*drand48(), (1-i*h)*ymax*drand48() )//��������
      .flush();
    mouseBreak();
  }
  
  g.color("black")
    .text( 5, -15, msg)
    .mouseWait();
  
  // end  --------------------------------------------------------
  Xwin.close();         	// �����
}
