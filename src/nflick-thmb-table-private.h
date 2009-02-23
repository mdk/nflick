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

static GtkTableClass*           ParentClass = NULL;

struct                          _NFlickThmbTablePrivate
{
        GList *Thmbs;
        GList *ImageIds;
};

static void                     nflick_thmb_table_class_init (NFlickThmbTableClass *klass);

static void                     nflick_thmb_table_init (NFlickThmbTable *self);

static gboolean                 private_init (NFlickThmbTable *self, NFlickThmbTablePrivate *private);

static void                     private_dispose (NFlickThmbTablePrivate *private);

static void                     nflick_thmb_table_dispose (NFlickThmbTable *self);

static void                     nflick_thmb_table_finalize (NFlickThmbTable *self);

static void                     on_thmb_button_released (GtkWidget *widget, gpointer b, NFlickThmbTable *self);
