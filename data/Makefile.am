icondir 			= $(datadir)/icons/hicolor
desktopdir 			= $(datadir)/applications/hildon
pixmapdir 			= $(datadir)/pixmaps
dbusdir				= $(datadir)/dbus-1/services

MAINTAINERCLEANFILES 		= Makefile.in

desktop_DATA 			= nflick.desktop
pixmap_DATA 			= logo.png stub.png
dbus_DATA			= com.nokia.nflick.service

EXTRA_DIST 			= nflick.desktop.in 		\
				  com.nokia.nflick.service.in	\
				  logo.png			\
				  nflick-26.png 		\
				  nflick-34.png 		\
				  nflick-40.png 		\
				  nflick-64.png stub.png 

install-data-local:
	$(mkinstalldirs) $(DESTDIR)$(icondir)/26x26/hildon; 	\
	$(mkinstalldirs) $(DESTDIR)$(icondir)/34x34/hildon; 	\
	$(mkinstalldirs) $(DESTDIR)$(icondir)/40x40/hildon; 	\
	$(mkinstalldirs) $(DESTDIR)$(icondir)/scalable/hildon; 	\
	$(INSTALL_DATA) $(srcdir)/nflick-26.png $(DESTDIR)$(icondir)/26x26/hildon/nflick.png; \
	$(INSTALL_DATA) $(srcdir)/nflick-34.png $(DESTDIR)$(icondir)/34x34/hildon/nflick.png; \
	$(INSTALL_DATA) $(srcdir)/nflick-40.png $(DESTDIR)$(icondir)/40x40/hildon/nflick.png; \
	$(INSTALL_DATA) $(srcdir)/nflick-64.png $(DESTDIR)$(icondir)/scalable/hildon/nflick.png;

uninstall-local:
	rm $(DESTDIR)$(icondir)/26x26/hildon/nflick.png; \
	rm $(DESTDIR)$(icondir)/34x34/hildon/nflick.png; \
	rm $(DESTDIR)$(icondir)/40x40/hildon/nflick.png; \
	rm $(DESTDIR)$(icondir)/scalable/hildon/nflick.png;

