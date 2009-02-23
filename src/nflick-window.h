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

#ifndef __NFLICKWINDOW_H__
#define __NFLICKWINDOW_H__
 
#include <gtk/gtk.h>
#include <hildon-widgets/hildon-program.h>
#include <hildon-widgets/hildon-note.h>
#include "nflick-welcome-vbox.h"
#include "nflick-types.h"
#include "nflick-thmb-table.h"
#include "nflick-cache-dialog.h"
#include "nflick-processor.h"
#include "gdk/gdkkeysyms.h"
#include "hildon-widgets/hildon-defines.h"

struct                          _NFlickWindow
{
        HildonWindow Parent;
        NFlickWindowPrivate *Private;
};

struct                          _NFlickWindowClass 
{
        HildonWindowClass ParentClass;
};

GType                           nflick_window_get_type (void);

NFlickWindow*                   nflick_window_new (void);

void                            nflick_window_change_to_vbox (NFlickWindow *self, GtkVBox *vbox);

void                            nflick_window_pop_vbox (NFlickWindow *self);

void                            nflick_window_change_to_vbox (NFlickWindow *self, GtkVBox *vbox);

void                            nflick_window_push_vbox (NFlickWindow *self, GtkVBox *vbox);

void                            nflick_window_pop_vbox (NFlickWindow *self);

void                            nflick_window_probe_vbox_size (NFlickWindow *self, gint *width, gint *height);

void                            nflick_window_resume_fetching (NFlickWindow *self);

#endif
