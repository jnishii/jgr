//
// ��˥塼������ɥ������ϥ�����ɥ��λȤ�������
//
//   ���ĤΥ����ƥ����ĥᥤ���˥塼�򳫤���
//   ���Ĥ�Υ����ƥ�����Ϥ�����դ��뤿��ΰ��Ū�ʥ��֥�˥塼�򳫤�
//     ���֥�˥塼�Σ��Ĥ�Υ����ƥ�ǡ�ʸ���������դ��롣
//     ���֥�˥塼�Σ��Ĥ�Υ����ƥ�ǡ����ͤ�����դ��롣
//   ���Ĥ�Υ����ƥ���ѿ���ɸ����Ϥ�ɽ�����뤿�����Ū�ʥ��֥�˥塼�򳫤�
//     ���֥�˥塼�Σ��Ĥ�Υ����ƥ�ǡ�ʸ�����ɸ����Ϥ˽Ф���
//     ���֥�˥塼�Σ��Ĥ�Υ����ƥ�ǡ����ͤ�ɸ����Ϥ˽Ф���
//   ���Ĥ�Υ����ƥ�ǽ�λ���롣
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

///////////////////////// ���֥�˥塼�Τ���Υϥ�ɥ� ////////////////////

void handler_subB1(void* dummy){         //ʸ�����ɽ������
  cout << "str= " << str << "\n";
}
void handler_subB2(void* dummy){         //���ͤ�ɽ������
  cout << "v= " << v << "\n";
}

////////////////////// �ᥤ��롼���� //////////////////////

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

  while(1) {main_menu.act();}                   // �ᥤ���˥塼�����餻��
}
