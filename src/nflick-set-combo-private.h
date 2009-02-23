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

static GtkComboBoxClass*        ParentClass = NULL;

struct                          _NFlickSetComboPrivate
{
        GtkListStore *Store;
};

static void                     nflick_set_combo_class_init (NFlickSetComboClass *klass);

static void                     nflick_set_combo_init (NFlickSetCombo *self);

static gboolean                 private_init (NFlickSetCombo *self, NFlickSetComboPrivate *private);

static void                     private_dispose (NFlickSetComboPrivate *private);

static void                     nflick_set_combo_dispose (NFlickSetCombo *self);

static void                     nflick_set_combo_finalize (NFlickSetCombo *self);

static void                     add_photo_set (NFlickSetCombo *self, NFlickPhotoSet *set);
