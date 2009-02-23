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

#ifndef __NFLICKWELCOMEVBOX_H__
#define __NFLICKWELCOMEVBOX_H__
 
#include <gtk/gtk.h>
#include <libintl.h>
#include <osso-uri.h>
#include "nflick-wait-dialog.h"
#include "nflick-auth-worker.h"
#include "nflick-set-list-worker.h"
#include "nflick-photo-list-worker.h"
#include "nflick-window.h"
#include "nflick-model.h"
#include "nflick-types.h"
#include "nflick-model.h"
#include "nflick-token-dialog.h"
#include "nflick-photos-vbox.h"

struct                          _NFlickWelcomeVBox
{
        GtkVBox Parent;
        NFlickWelcomeVBoxPrivate *Private;
};

struct                          _NFlickWelcomeVBoxClass 
{
        GtkVBoxClass ParentClass;
};

GType                           nflick_welcome_vbox_get_type (void);

NFlickWelcomeVBox*              nflick_welcome_vbox_new (void);

#endif
