SRCS=jgr.tex
DVIS=jgr.dvi

all: jgr.dvi jgr.ps jgr.pdf
pdf: jgr.pdf

jgr.ps: jgr.dvi
	dvips jgr

jgr.pdf: jgr.dvi
	dvipdfmx $<

jgr.dvi: jgr.tex
	platex jgr
	platex jgr
	mendex jgr
	platex jgr

html: jgr.tex
	rm -rf html ||:
	latex2html -split 0 $<
	mv jgr html
	rm html/{*idx,*out,*aux,*log,*pl,WARNINGS} ||:

clean:
	rm *.aux *.log *.idx *.ilg *.ind *.toc *.haux *.html *.hind *.out *.ps *.pdf *.dvi *~ ||:

allclean: clean
	rm -rf *.ps *.dvi *.pdf html ||:


