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

static HildonWindowClass*       ParentClass = NULL;

struct                          _NFlickWindowPrivate
{
        GtkVBox *PushedVBox;
        GtkVBox *CurrentVBox;
};

static void                     nflick_window_class_init (NFlickWindowClass *klass);

static void                     nflick_window_init (NFlickWindow *self);

static gboolean                 private_init (NFlickWindow *self, NFlickWindowPrivate *private);

static void                     private_dispose (NFlickWindowPrivate *private);

static void                     nflick_window_dispose (NFlickWindow *self);

static void                     nflick_window_finalize (NFlickWindow *self);

static gboolean                 on_delete_event (NFlickWindow *self);

static void                     on_reset_activated (void);

static void                     on_cache_activated (void);

static void                     on_close_activated (void);

static gboolean                 on_key_released (NFlickWindow *self, GdkEventKey *event);
