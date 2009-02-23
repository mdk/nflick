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

static GtkImageClass*           ParentClass = NULL;

struct                          _NFlickThmbImagePrivate
{
        NFlickPhotoData *Data;
        gint32 FetcherId;
        gboolean Fetched;
};

enum 
{
        ARG_0,
        ARG_PHOTO_ID,
        ARG_TITLE,
        ARG_FETCHED
};

static void                     nflick_thmb_image_class_init (NFlickThmbImageClass *klass);

static void                     nflick_thmb_image_init (NFlickThmbImage *self);

static gboolean                 private_init (NFlickThmbImage *self, NFlickThmbImagePrivate *private);

static void                     private_dispose (NFlickThmbImagePrivate *private);

static void                     nflick_thmb_image_dispose (NFlickThmbImage *self);

static void                     nflick_thmb_image_finalize (NFlickThmbImage *self);

static gboolean                 on_job_finished (NFlickProcessorResult *result);

static void                     nflick_thmb_image_get_property (NFlickThmbImage *self, guint propid, 
                                                                GValue *value, GParamSpec *pspec);

