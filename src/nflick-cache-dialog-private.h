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

static GtkDialogClass*          ParentClass = NULL;

struct                          _NFlickCacheDialogPrivate
{
        GtkLabel *ItemsLabel;
        GtkLabel *SizeLabel;
        GtkButton *ClearButton;
        GtkCheckButton *UseCacheCheckButton;
};

static void                     nflick_cache_dialog_class_init (NFlickCacheDialogClass *klass);

static void                     nflick_cache_dialog_init (NFlickCacheDialog *self);

static gboolean                 private_init (NFlickCacheDialog *self, NFlickCacheDialogPrivate *priv);

static void                     private_dispose (NFlickCacheDialogPrivate *priv);

static void                     nflick_cache_dialog_dispose (NFlickCacheDialog *self);

static void                     nflick_cache_dialog_finalize (NFlickCacheDialog *self);

static void                     refresh_values (NFlickCacheDialog *self);

static void                     on_close_clicked (GtkWidget *widget, NFlickCacheDialog *self);

static void                     on_clear_clicked (GtkWidget *widget, NFlickCacheDialog *self);

static void                     on_use_cache_toggled (GtkWidget *widget, NFlickCacheDialog *self);

