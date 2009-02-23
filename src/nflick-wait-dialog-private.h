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

struct                          _NFlickWaitDialogPrivate
{
        GtkProgressBar *Progress;
        GtkLabel *Label;
        guint PulseTimeout;
        GtkButton *AbortButton;
        NFlickWorker *Worker;
        gboolean Aborting;
};

static void                     nflick_wait_dialog_class_init (NFlickWaitDialogClass *klass);

static void                     nflick_wait_dialog_init (NFlickWaitDialog *self);

static gboolean                 private_init (NFlickWaitDialog *self, NFlickWaitDialogPrivate *private);

static void                     private_dispose (NFlickWaitDialogPrivate *private);

static void                     nflick_wait_dialog_dispose (NFlickWaitDialog *self);

static void                     nflick_wait_dialog_finalize (NFlickWaitDialog *self);

static void                     start_pulser (NFlickWaitDialog *self);

static void                     kill_pulser (NFlickWaitDialog *self);

static gboolean                 on_pulse_timeout (NFlickWaitDialog *self);

static void                     on_abort_clicked (GtkWidget *widget, NFlickWaitDialog *self);

static gboolean                 on_thread_abort_idle (NFlickWaitDialog *self);

static gboolean                 on_thread_error_idle (NFlickWaitDialog *self);

static gboolean                 on_thread_ok_idle (NFlickWaitDialog *self);

static gboolean                 on_thread_msg_change_idle (NFlickWaitDialog *self);

