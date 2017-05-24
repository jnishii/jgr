##### do not edit here, definition is automatically replaced by "make rpm"
%define version 3.4
%define soversion 3.4
%define release 0vl1
#########################################################################
Summary:    A simple C++ class library for graphics on X11
Summary(ja):X Window�� C++����ե��å������饹���饤�֥��
Name:  	   jgr
Version:   %{version}
Release:   %{release}
Source:    jgr-%{version}.tar.gz
License:   LGPL/XeasyGraphic-Copyright
Group:     Development/Libraries
Packager:  Jun Nishii <nishii@sci.yamaguchi-u.ac.jp>
Buildroot: %{_tmppath}/%{name}-%{version}-root
BuildPrereq: latex2html, tetex, dvipdfmx

%description
JGR is a simple C++ class library for graphics on X11 with following features.
-JGR can handle multiple windows with multiple graphs by easy operation.
-JGR can draw axis, points, lines, rectangles, circles and so on.
-JGR has an automatic scaling function. When we draw a graph, the
real values, e.g. [m] and [kg], which we want to draw are different from
the value [dots] on the screen. JGR automatically convert the the real
value to the dots and draws items by setting the relation as a initial 
configuration.
-JGR uses Xlib library, but we don't know about it.
-JGR also outputs EPS files. 
-JGR can make menus... However, it needs high load on PC, so I recommend
 to use some other toolkit to use the menu.

%description -l ja
JGR��X Window��ǥ���ե��å������������ C++ ���饹�饤�֥��Ǥ���
-��ñ�ʥץ���ߥ󥰤�ʣ���Υ�����ɥ�������Ǥ�, �ޤ�, ���ĤΥ������
 �����ʣ���Υ���դ�����Ǥ��ޤ����ץ����μ¹���˿���׻���̤��
 ��ɤ�ץ�åȤ��Ƥ����������������Ǥ���
-���������ͳѡ��ߤʤɤʤɤ������ޤ���
-�̾����դ��������ˤ�X���κ�ɸ��ñ�̷ϡ��㤨��m,s)�ˤ������ͤȥ���
 �ե��å����̾�Ǥ��͡ʥɥåȡˤ��㤤�ޤ��������Υ���ե��å����ġ����
 ��X��Y����Ĺ�����ºݤ��͡�m,s�ʤɡˤȲ��̾�Ǥ��͡ʥɥåȡˤδط��ʤɤ�
 �Ϥ����Ϳ���Ƥ������Ȥˤ�äơ����Ȥϼ�ưŪ�˼ºݤ��ͤ����ɸ�Ѵ����
 �ʤä��������������Ƥ���ޤ���
-Xlib �δؿ���ȤäƤޤ���, Xlib ��ռ������˻��ѤǤ��ޤ���
-EPS�ե������Ϥ��Ƥ���ޤ���(�ޤ��Х����ꤽ���Ǥ���)
-��˥塼��Ĥ���ޤ������������Ť��Ǥ�^^;;

%prep
rm -rf ${RPM_BUILD_ROOT}
%setup -n %{name}-%{version}

%build
make
make -C doc
make html -C doc

%install

make DESTDIR=$RPM_BUILD_ROOT install
(
cd ${RPM_BUILD_ROOT}/usr/lib
ln -s libjgr.so.%{soversion} libjgr.so.1
ln -s libjgr.so.%{soversion} libjgr.so
)

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean

%files
%defattr(-,root,root)
%doc README* INSTALL* ChangeLog LGPL* *Copyright
%doc doc/{*.tex,*.dvi,*.ps,*.pdf,html} samples/
/usr/include/jkit/
/usr/lib/*

%changelog 
* Fri Nov 14 2008 Jun Nishii <nishii@bcl.sci.yamaguchi-u.ac.jp>
- enable Multibyte fonts for ScaleFont and TextFont
- use Gothic as deafult font

* Tue Nov 11 2008 Jun Nishii <nishii@bcl.sci.yamaguchi-u.ac.jp> 3.3-0vl1 
- suppress verbose messages

* Tue Oct  7 2008 Jun Nishii <nishii@bcl.sci.yamaguchi-u.ac.jp> 3.2-0vl1 
- added polygon routines

* Mon Oct  6 2008 Jun Nishii <nishii@bcl.sci.yamaguchi-u.ac.jp> 3.1-0vl1
- bug fix for ev2button()
- add function ev2button as a member of XController, JWindow, Graph
- revise some documents

* Fri Oct  3 2008 Jun Nishii <nishii@bcl.sci.yamaguchi-u.ac.jp> 3.0-0vl1
- obsolete the macro PS and make flags to enable PS recording
- new functions functions for font setting
-- setTextFont, setScaleFont
- improve font search process when specifed font is not available
- new functions for color setting
-- setFgColor, setBgColor
-- bug fix for PS routine when writing rectangle
- enable compile dylib for Mac OSX (Leopard)
- new examples: font.cc

* Thu Oct  2 2008 Jun Nishii <nishii@bcl.sci.yamaguchi-u.ac.jp> 2.2-0vl1
- fix bug for EPSOut when jgr is compiled without the declaration of PS

* Wed Aug 20 2008 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 2.1-0vl1
- added function cross for Graph
- compile without PS routine
- added FontDB TextFont;
- added mouse_press( XEvent &ev ) for Graph and JWindow
- added ev2x, ev2y for Graph

* Thu Mar 31 2005 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 2.0-0vl1
- compile with Menu classes again
- separate jgrPS.cc from jgr.cc
- rename as jgr from jkit
- fix samples
- update/new documents
- change Group

* Mon Apr 19 2004 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 1.31-1
- fix minor gramatic problem (use new for dynamical allocation
  for temporal matrix in jgr.cc

* Tue Apr 13 2004 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 1.30-1
- added GausRand in jmath.h

* Wed Mar 31 2004 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 1.29-1
- build with g++-3
- remove form() from jgr 
- remove default value definition in jgr.cc

* Thu Jan 29 2004 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 1.28-2
- added JWin.forget()
- added Graph.pushcolor()
- added Graph.popcolor()
- set axis color as black
- update document

* Thu Jan 15 2004 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 1.27-1
- minor bug fix

* Thu Jun 19 2003 Jun Nishii <nishii@sci.yamaguchi-u.ac.jp> 1.26-2
- build on Seed

