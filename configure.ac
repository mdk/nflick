AC_INIT([NFlick photo browser], [0.4.0], [mdk@mdk.am], [nflick])
AC_CANONICAL_SYSTEM

AM_CONFIG_HEADER(config.h)
AC_CONFIG_SRCDIR([src/nflick-flickr.h])
AM_MAINTAINER_MODE

AM_INIT_AUTOMAKE

AC_PROG_CC
AM_PROG_LIBTOOL
AC_HEADER_STDC

PKG_CHECK_MODULES(GTK, gtk+-2.0)
AC_SUBST(GTK_LIBS)
AC_SUBST(GTK_CFLAGS)

PKG_CHECK_MODULES(NEON, neon >= 0.26)
AC_SUBST(NEON_LIBS)
AC_SUBST(NEON_CFLAGS)

PKG_CHECK_MODULES(OSSO, libosso)
AC_SUBST(OSSO_LIBS)
AC_SUBST(OSSO_CFLAGS)

PKG_CHECK_MODULES(OSSOMIME, libossomime)
AC_SUBST(OSSOMIME_LIBS)
AC_SUBST(OSSOMIME_CFLAGS)

PKG_CHECK_MODULES(GTHREAD, gthread-2.0)
AC_SUBST(GTHREAD_LIBS)
AC_SUBST(GTHREAD_CFLAGS)

PKG_CHECK_MODULES(XML, libxml-2.0)
AC_SUBST(XML_LIBS)
AC_SUBST(XML_CFLAGS)

PKG_CHECK_MODULES(GCONF, gconf-2.0)
AC_SUBST(GCONF_LIBS)
AC_SUBST(GCONF_CFLAGS)

PKG_CHECK_MODULES(HILDON, hildon-libs)
AC_SUBST(HILDON_LIBS)
AC_SUBST(HILDON_CFLAGS)

# Maemo launcher support (or not)
AC_ARG_ENABLE(launcher, 
               AC_HELP_STRING([--enable-launcher], [Enable the maemo-launcher support]),
               [
                case "${enableval}" in
                        yes) LAUNCHER_SUPPORT=yes ;;
                        no)  LAUNCHER_SUPPORT=no ;;
                        *)   AC_MSG_ERROR(bad value ${enableval} for --enable-launcher) ;;
                esac],
               [LAUNCHER_SUPPORT=no])

if test x$LAUNCHER_SUPPORT = xyes; then
        AC_MSG_NOTICE(Will build with maemo-launcher support)
        LAUNCHER_CFLAGS="-shared"
        LAUNCHER_LDFLAGS="-shared -export-dynamic"
else
        AC_MSG_NOTICE(Will build without maemo-launcher support)
        LAUNCHER_CFLAGS=""
        LAUNCHER_LDFLAGS=""
fi

AC_SUBST(LAUNCHER_CFLAGS)
AC_SUBST(LAUNCHER_LDFLAGS)
AC_DEFINE_UNQUOTED(LAUNCHER_CFLAGS, "$LAUNCHER_CFLAGS", [Extra cflags introduced by launcher])
AC_DEFINE_UNQUOTED(LAUNCHER_LDFLAGS, "$LAUNCHER_LDFLAGS", [Extra ld flags introduced by launcher])

AC_CONFIG_FILES([Makefile			\
                 src/Makefile			\
                 data/Makefile			\
                 data/nflick.desktop		\
                 src/nflick-behaviour.h		\
                 data/com.nokia.nflick.service])

AC_OUTPUT

# Result
AC_MSG_RESULT([
${PACKAGE} ${VERSION}

        Compiler.................: ${CC}
        Maemo-launcher support...: ${LAUNCHER_SUPPORT}
        ])

