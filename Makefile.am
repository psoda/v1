EXTRA_DIST = ltmain.sh ltconfig CODING_STANDARDS reconf data
SUBDIRS = src

gui :
	$(MAKE) 
	$(MAKE) -C ${top_srcdir}/src/gui

cleangui :
	$(MAKE) -C ${top_srcdir}/src/gui cleangui

distribution:
	./reconf --enable-gui
	$(MAKE) clean
	$(MAKE) 
	$(MAKE) gui
	./reconf
#$(MAKE) -C ${top_srcdir}/src/gui cleangui
	$(MAKE) clean
	$(MAKE)

complete:
	$(MAKE) clean
	cd src/fann-2.0.0/; ./configure; $(MAKE)
	cd src/mrbayes-3.1.2/; $(MAKE)
	cd src/RAxML-7.0.4/; $(MAKE)
	$(MAKE) gui

