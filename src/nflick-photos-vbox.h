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

#ifndef __NFLICKPHOTOSVBOX_H__
#define __NFLICKPHOTOSVBOX_H__
 
#include <gtk/gtk.h>
#include <libintl.h>
#include "nflick-types.h"
#include "nflick-set-combo.h"
#include "nflick-model.h"
#include "nflick-thmb-table.h"
#include "nflick-processor.h"
#include "nflick-photos-vbox.h"

struct                          _NFlickPhotosVBox
{
        GtkVBox Parent;
        NFlickPhotosVBoxPrivate *Private;
};

struct                          _NFlickPhotosVBoxClass 
{
        GtkVBoxClass ParentClass;
};

GType                           nflick_photos_vbox_get_type (void);

NFlickPhotosVBox*               nflick_photos_vbox_new (GList *list);

void                            nflick_photos_resume_fetching (NFlickPhotosVBox *self);

#endif
