#include <iostream.h>
#include <jkit/jgr.h>    //jgr���饹��Ȥ��Τ�ɬ��

JWindow win;             //����������ɥ������
Graph   g;               //������ɥ������褹�륰��դ����
Menu main_menu,sub_menuA,sub_menuB;


///////////////////////// ���֥˥塼�Τ���Υϥ�ɥ� ////////////////////

char str[MAX_STR_LEN+1];
double v;

/////////////////////// �ᥤ���˥塼�Τ���Υϥ�ɥ� //////////////////

void handler_main1(void* dummy){        //���Ū���֥�˥塼�򳫤�
}

void handler_main2(void* dummy){        //��Ū�ʥ��֥�˥塼�򳫤�
  g.EPSOut("out.eps");
}
void handler_main3(void* dummy){       //��λ����
  Xwin.close();
  exit(1);
}

/////////////////////////// �ᥤ��롼���� /////////////////////////////

main(){
  double xmax, ymax;
  
  main_menu.open()
    .addItem("Go", handler_main1)      // �����ƥ����Ͽ
    .addItem("EPS", handler_main2)
    .addItem("end",  handler_main3);
  
  // ������ɥ��ȡ���������������դ���Ͽ-----------------------
  win.open(10,10)  // ������ɥ���(10,10)�ΰ��֤˳������
    .menu(main_menu)
    .graph(g.open( 500, 50, 200, 40 )) 
    // ������������ɥ��ˡ�x��������������(500,50)��
    //y������������(200,40)�Υ���դ���Ͽ
    .map()
    .flush();
  
  //��������ϡ������ g ��°������------------------------------
  xmax = 10.0;
  ymax = 100.0;
  g.setRatio( xmax, ymax, 500.0, 200.0 ) 
    // xmax, ymax ���ͤ򤽤줾�졢���̾�Ǥ�500dot, 200dot��Ĺ��������
    .axis() // X��, Y��������
    .flush();
  
  //�ᥤ��롼���󡧥����g��������.....
  for( double i=1.0; i<=xmax; i+=0.5 ){
    g.color("blue") // ����� g �����迧���Ĥˤ���
      .line( (double)i, 0.0, 0.0, (xmax-(double)i)*ymax/xmax )
      // (i,0.0) ���� (0.0, (xmax-(double)i)*ymax/xmax)���������
      .flush();   
  }
  
  while(1) {
    main_menu.act();                   // �ᥤ���˥塼�����餻��
  }
  // end  --------------------------------------------------------
  Xwin.close();          // ��λ�ε���
}
