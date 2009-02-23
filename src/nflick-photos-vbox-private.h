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

struct                          _NFlickPhotosVBoxPrivate
{
        NFlickSetCombo *Combo;
        NFlickThmbTable *Table;
        GtkLabel *PageLabel;
        GtkButton *NextButton;
        GtkButton *PrevButton;
        gint32 CurrentPage;
};

static void                     nflick_photos_vbox_class_init (NFlickPhotosVBoxClass *klass);

static void                     nflick_photos_vbox_init (NFlickPhotosVBox *self);

static gboolean                 private_init (NFlickPhotosVBox *self, NFlickPhotosVBoxPrivate *private);

static void                     private_dispose (NFlickPhotosVBoxPrivate *private);

static void                     nflick_photos_vbox_dispose (NFlickPhotosVBox *self);

static void                     nflick_photos_vbox_finalize (NFlickPhotosVBox *self);

static void                     on_combo_changed (GtkWidget *widget, NFlickPhotosVBox *self);

static void                     on_next_clicked (GtkWidget *widget, NFlickPhotosVBox *self);

static void                     on_prev_clicked (GtkWidget *widget, NFlickPhotosVBox *self);

static void                     set_paging (NFlickPhotosVBox *self, int page, int max);

static void                     set_table (NFlickPhotosVBox *self, NFlickThmbTable *table);

