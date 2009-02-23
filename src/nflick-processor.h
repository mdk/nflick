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

#ifndef __NFLICKPROCESSOR_H__
#define __NFLICKPROCESSOR_H__
 
#include <gtk/gtk.h>
#include <libintl.h>
#include "nflick-types.h"
#include "nflick-api-request.h"
#include "nflick-get-sizes-response.h"
#include "nflick-pixbuf-fetch.h"
#include "nflick-flickr.h"
#include "nflick-model.h"

struct                          _NFlickProcessorResult
{
        gint32 Id;
        gpointer Custom;
        GdkPixbuf *Pixbuf;
        gboolean Valid;
        gboolean Error;
};

gboolean                        nflick_processor_init (void);

gint32                          nflick_processor_add_job (const gchar *id, gpointer custom, gint32 width, gint32 height, gpointer func);

void                            nflick_processor_cancel_all (void);

void                            nflick_processor_cancel_by_id (gint32 id);

void                            nflick_processor_loose_result (NFlickProcessorResult *result);

void                            nflick_processor_kill (void);

#endif
