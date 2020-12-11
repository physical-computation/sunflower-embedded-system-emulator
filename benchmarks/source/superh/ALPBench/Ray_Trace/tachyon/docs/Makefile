#
# Makefile for Tachyon documentation
#

LATEX=latex
DVIPS=dvips
LATEX2HTML=latex2html
PDFLATEX=pdflatex
DISTILLER=distiller
MAKEINDEX=makeindex

default:
	@echo "Select documentation to build:"
	@echo "------------------------------"
	@echo " all - All supported file formats"
	@echo "html - HTML"
	@echo "  ps - Adobe PostScript"
	@echo " pdf - Adobe PDF "

all: tachyon.html tachyon.ps tachyon.pdf

ps: tachyon.ps

pdf: tachyon.pdf

html: tachyon.html

tachyon.html : 
	rm -rf ./tachyon
	$(LATEX2HTML) \
		-local_icons \
		-split 5 \
		-toc_depth 4 \
		-toc_stars \
		-t "Tachyon Docs" \
		-address "johns@megapixel.com" \
		-up_url "http://www.megapixel.com/" \
		-up_title "Megapixel.com, home of Tachyon" \
		tachyon

#	tar -cvf - tachyon | gzip > tachyon.html.tar.gz
#	rm -rf ./tachyon


tachyon.ps : tachyon.dvi
	$(DVIPS) tachyon.dvi -o tachyon.ps 

tachyon.dvi :
	$(LATEX) tachyon
	$(MAKEINDEX) tachyon
	$(LATEX) tachyon
	$(MAKEINDEX) tachyon
	$(LATEX) tachyon
	$(MAKEINDEX) tachyon
	$(LATEX) tachyon

tachyon.pdf :
	$(PDFLATEX) tachyon
	$(MAKEINDEX) tachyon
	$(PDFLATEX) tachyon
	$(MAKEINDEX) tachyon
	$(PDFLATEX) tachyon
	$(MAKEINDEX) tachyon
	$(PDFLATEX) tachyon

clean :
	rm -f *.aux *.dvi *.idx *.lof *.log *.lot *.pdf *.ps *.toc *.ilg *.ind



