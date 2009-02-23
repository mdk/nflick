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

#ifndef __NFLICKSETCOMBO_H__
#define __NFLICKSETCOMBO_H__
 
#include <gtk/gtk.h>
#include "nflick-types.h"
#include "nflick-photo-set.h"

struct                          _NFlickSetCombo
{
        GtkComboBox Parent;
        NFlickSetComboPrivate *Private;
};

struct                          _NFlickSetComboClass 
{
        GtkComboBoxClass ParentClass;
};

GType                           nflick_set_combo_get_type (void);

NFlickSetCombo*                 nflick_set_combo_new (void);

NFlickPhotoSet*                 nflick_set_combo_get_current_set (NFlickSetCombo *self);

void                            nflick_set_combo_load_list (NFlickSetCombo *self, GList *photosets);

#endif
