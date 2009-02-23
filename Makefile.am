SUBDIRS				= src data

MAINTAINERCLEANFILES		= Makefile.in aclocal.m4 configure 	\
				  config.h.in compile 			\
				  ltmain.sh depcomp missing 		\
				  install-sh config.sub 		\
				  config.guess intltool-extract.in 	\
				  intltool-merge.in intltool-update.in  \
				  mkinstalldirs build-stamp 		\
				  configure-stamp

EXTRA_DIST			= debian/changelog 			\
				  debian/control 			\
				  debian/copyright 			\
				  debian/rules				\
				  debian/postinst			\
				  HACKING				\
				  autogen.sh

deb:
	./dpkg-buildpackage -rfakeroot
