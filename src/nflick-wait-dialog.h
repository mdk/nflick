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

#ifndef __NFLICKWAITDIALOG_H__
#define __NFLICKWAITDIALOG_H__
 
#include <gtk/gtk.h>
#include <libintl.h>
#include <hildon-widgets/hildon-note.h>
#include "nflick-worker.h"
#include "nflick-types.h"

struct                          _NFlickWaitDialog
{
        GtkDialog Parent;
        NFlickWaitDialogPrivate *Private;
};

struct                          _NFlickWaitDialogClass 
{
        GtkDialogClass ParentClass;
};

GType                           nflick_wait_dialog_get_type (void);

NFlickWaitDialog*               nflick_wait_dialog_new (NFlickWorker *worker, GtkWindow *parent);

gint                            nflick_wait_dialog_show_and_run (NFlickWaitDialog *self);

#endif
