all: ht_front.pdf ht_body.pdf title.pdf

clean:
	rm *.pdf
	rm *.aux

ht_body.pdf: ht_body.aux honors_thesis.bib
	bibtex ht_body
	pdflatex ht_body
	pdflatex ht_body

ht_body.aux: ht_body.tex ht_content.tex honors_thesis.bib
	pdflatex ht_body

ht_front.pdf: ht_front.aux honors_thesis.bib
	bibtex ht_front
	pdflatex ht_front
	pdflatex ht_front

ht_front.aux: ht_front.tex ht_content.tex honors_thesis.bib
	pdflatex ht_front

title.pdf: title.aux
	pdflatex title

title.aux: title.tex
	pdflatex title
