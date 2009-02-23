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

#ifndef __NFLICKTHMBTABLE_H__
#define __NFLICKTHMBTABLE_H__
 
#include <gtk/gtk.h>
#include <libintl.h>
#include "nflick-types.h"
#include "nflick-model.h"
#include "nflick-photo-set.h"
#include "nflick-photo-data.h"
#include "nflick-thmb-image.h"
#include "nflick-behaviour.h"

struct                          _NFlickThmbTable
{
        GtkTable Parent;
        NFlickThmbTablePrivate *Private;
};

struct                          _NFlickThmbTableClass 
{
        GtkTableClass ParentClass;
};

GType                           nflick_thmb_table_get_type (void);

NFlickThmbTable*                nflick_thmb_table_new (NFlickPhotoSet *set, gint32 page);

void                            nflick_thmb_table_resume_fetching (NFlickThmbTable *self);

#endif
