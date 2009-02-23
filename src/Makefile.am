MAINTAINERCLEANFILES 		= Makefile.in

bin_PROGRAMS 			= nflick

nflick_SOURCES			= nflick-worker.c 			\
				  nflick-auth-worker.c 			\
				  nflick-set-list-worker.c 		\
				  nflick-photo-list-worker.c 		\
				  nflick-window.c 			\
				  nflick-get-sizes-response.c 		\
				  nflick-photo-list-response.c 		\
				  nflick-photo-data.c 			\
				  nflick-thmb-image.c 			\
				  nflick-processor.c 			\
				  nflick-show-vbox.c 			\
				  nflick-thmb-table.c 			\
				  nflick-pixbuf-fetch.c 		\
				  nflick-set-combo.c 			\
				  nflick-token-dialog.c 		\
				  nflick-show-worker.c 			\
				  nflick-photo-set.c 			\
				  nflick-api-response.c 		\
				  nflick-api-request.c 			\
				  nflick-wait-dialog.c 			\
				  nflick-gft-response.c 		\
				  nflick-set-list-response.c 		\
				  nflick-photos-vbox.c 			\
				  nflick-model.c 			\
				  nflick-no-set-response.c 		\
				  nflick-welcome-vbox.c 		\
				  nflick-cache-dialog.c 		\
				  nflick.c

nflick_CFLAGS 			= $(GTK_CFLAGS) 			\
				  $(OSSO_CFLAGS) 			\
				  $(NEON_CFLAGS) 			\
				  $(XML_CFLAGS) 			\
				  $(GCONF_CFLAGS) 			\
				  $(GTHREAD_CFLAGS) 			\
				  $(HILDON_CFLAGS) 			\
				  $(OSSOMIME_CFLAGS)			\
				  $(LAUNCHER_CFLAGS)

nflick_LDADD 			= $(GTK_LIBS) 				\
				  $(OSSO_LIBS) 				\
				  $(NEON_LIBS) 				\
				  $(GCONF_LIBS) 			\
				  $(XML_LIBS) 				\
				  $(GTHREAD_LIBS) 			\
				  $(HILDON_LIBS) 			\
				  $(OSSOMIME_LIBS)

nflick_LDFLAGS			= $(LAUNCHER_LDFLAGS)

EXTRA_DIST 			= nflick-behaviour.h.in 		

noinst_HEADERS			= nflick-api-request.h 			\
				  nflick-api-request-private.h 		\
				  nflick-api-response.h 		\
				  nflick-api-response-private.h 	\
				  nflick-auth-worker.h 			\
				  nflick-auth-worker-private.h 		\
				  nflick-flickr.h 			\
				  nflick-get-sizes-response.h 		\
				  nflick-get-sizes-response-private.h 	\
				  nflick-gft-response.h 		\
				  nflick-gft-response-private.h 	\
				  nflick-model.h nflick-model-private.h \
				  nflick-photo-data.h 			\
				  nflick-photo-list-response.h 		\
				  nflick-photo-list-response-private.h 	\
				  nflick-photo-list-worker.h 		\
				  nflick-photo-list-worker-private.h 	\
				  nflick-photo-set.h 			\
				  nflick-photo-set-private.h 		\
				  nflick-photos-vbox.h 			\
				  nflick-photos-vbox-private.h 		\
				  nflick-pixbuf-fetch.h 		\
				  nflick-pixbuf-fetch-private.h 	\
				  nflick-processor.h 			\
				  nflick-processor-private.h 		\
				  nflick-set-combo.h 			\
				  nflick-set-combo-private.h 		\
				  nflick-set-list-response.h 		\
				  nflick-set-list-response-private.h 	\
				  nflick-set-list-worker.h 		\
				  nflick-set-list-worker-private.h 	\
				  nflick-show-vbox.h 			\
				  nflick-show-vbox-private.h 		\
				  nflick-show-worker.h 			\
				  nflick-show-worker-private.h 		\
				  nflick-thmb-image.h 			\
				  nflick-thmb-image-private.h 		\
				  nflick-thmb-table.h			\
				  nflick-thmb-table-private.h 		\
				  nflick-token-dialog.h 		\
				  nflick-token-dialog-private.h 	\
				  nflick-types.h 			\
				  nflick-wait-dialog.h 			\
				  nflick-wait-dialog-private.h 		\
				  nflick-welcome-vbox.h 		\
				  nflick-welcome-vbox-private.h		\
				  nflick-window.h 			\
				  nflick-window-private.h 		\
				  nflick-worker.h 			\
				  nflick-worker-private.h 		\
				  nflick-cache-dialog.h 		\
				  nflick-cache-dialog-private.h 	\
				  nflick-no-set-response.h 		\
				  nflick-no-set-response-private.h

