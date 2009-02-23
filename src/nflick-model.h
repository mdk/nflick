/******************************************************************************/
/*                                                                            */
/* GPL license, Copyright (c) 2005-2006 by:                                   */
/*                                                                            */
/* Authors:                                                                   */
/*      Michael Dominic K. <michaldominik@gmail.com>                          */
/*                                                                            */
/* This program is free software; you can redistribute it and/or modify it    */
/* under the terms of the GNU General Public License as published by the      */
/* Free Software Foundation; either version 2, or (at your option) any later  */
/* version.                                                                   */
/*                                                                            */
/* This program is distributed in the hope that it will be useful, but        */
/* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY */
/* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License   */
/* for more details.                                                          */
/*                                                                            */
/* You should have received a copy of the GNU General Public License along    */
/* with this program; if not, write to the Free Software Foundation, Inc., 59 */
/* Temple Place - Suite 330, Boston, MA 02111-1307, USA.                      */
/*                                                                            */
/******************************************************************************/

#ifndef __NFLICKMODEL_H__
#define __NFLICKMODEL_H__

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libintl.h>
#include <gconf/gconf-client.h>
#include <hildon-widgets/hildon-program.h>
#include "nflick-window.h"
#include "nflick-show-worker.h"
#include "nflick-window.h"
#include "nflick-show-vbox.h"
#include "nflick-behaviour.h"
#include "nflick-processor.h"


struct                          _NFlickModel
{
        HildonProgram *Program;
        NFlickWindow *Window;
        GdkPixbuf *FlickrPixbuf;
        GdkPixbuf *LoadingPixbuf;
        GConfClient *GConf;
        gchar *UserName;
        gchar *FullName;
        gchar *UserNsid;
        gchar *Token;
        gchar *CacheDir;
        gboolean UseCache;
};

NFlickModel*                    nflick_model_get (void);

void                            nflick_model_destroy (void);

gboolean                        nflick_model_init (int *argc, char ***argv);

void                            nflick_model_pop_from_gconf (void);

void                            nflick_model_reset_gconf (void);

void                            nflick_model_settings_reset (void);

void                            nflick_model_push_to_gconf (void);

gboolean                        nflick_model_settings_are_valid (void);

void                            nflick_model_settings_set (const gchar *username, const gchar *fullname, 
                                                           const gchar *token, const gchar  *usernsid);

void                            nflick_model_show_photo (const gchar *id, GList *list);

void                            nflick_model_switch_photo (const gchar *id, GList *list);

void                            nflick_model_hide_photo (void);

void                            nflick_model_fullscreen (void);

void                            nflick_model_unfullscreen (void);

void                            nflick_model_clear_cache (void);

void                            nflick_model_enable_cache (gboolean val);

#define                         NFLICK_MODEL (nflick_model_get ())

#define                         NFLICK_MODEL_WINDOW (((NFlickModel *) NFLICK_MODEL)->Window)

#define                         NFLICK_MODEL_USERNAME (((NFlickModel *) NFLICK_MODEL)->UserName)

#define                         NFLICK_MODEL_FULLNAME (((NFlickModel *) NFLICK_MODEL)->FullName)

#define                         NFLICK_MODEL_TOKEN (((NFlickModel *) NFLICK_MODEL)->Token)

#define                         NFLICK_MODEL_USERNSID (((NFlickModel *) NFLICK_MODEL)->UserNsid)

#define                         NFLICK_MODEL_FLICKR_LOGO (((NFlickModel *) NFLICK_MODEL)->FlickrPixbuf)

#define                         NFLICK_MODEL_LOADING (((NFlickModel *) NFLICK_MODEL)->LoadingPixbuf)

#define                         NFLICK_MODEL_CACHE_DIR (((NFlickModel *) NFLICK_MODEL)->CacheDir)

#define                         NFLICK_MODEL_USE_CACHE (((NFlickModel *) NFLICK_MODEL)->UseCache)

#endif
