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

static GtkVBoxClass*            ParentClass = NULL;

struct                          _NFlickShowVBoxPrivate
{
        GtkImage *Image;
        GList *List;
};

static void                     nflick_show_vbox_class_init (NFlickShowVBoxClass *klass);

static void                     nflick_show_vbox_init (NFlickShowVBox *self);

static gboolean                 private_init (NFlickShowVBox *self, NFlickShowVBoxPrivate *private);

static void                     private_dispose (NFlickShowVBoxPrivate *private);

static void                     nflick_show_vbox_dispose (NFlickShowVBox *self);

static gboolean                 on_button_released (gpointer a, gpointer b, NFlickShowVBox *self);

static void                     nflick_show_vbox_finalize (NFlickShowVBox *self);
