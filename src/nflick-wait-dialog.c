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

#include "nflick-wait-dialog.h"
#include "nflick-wait-dialog-private.h"

GType                           nflick_wait_dialog_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickWaitDialogClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_wait_dialog_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickWaitDialog), 
                        4, 
                        (GInstanceInitFunc) nflick_wait_dialog_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_DIALOG, "NFlickWaitDialog",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_wait_dialog_class_init (NFlickWaitDialogClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_wait_dialog_dispose;
        gobjectclass->finalize = (gpointer) nflick_wait_dialog_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_DIALOG);
}

static void                     nflick_wait_dialog_init (NFlickWaitDialog *self)
{
        g_return_if_fail (NFLICK_IS_WAIT_DIALOG (self));

        self->Private = NULL;

        NFlickWaitDialogPrivate *priv = g_new0 (NFlickWaitDialogPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }


}

static gboolean                 private_init (NFlickWaitDialog *self, NFlickWaitDialogPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        gtk_window_set_title (GTK_WINDOW (self), gettext ("Please wait"));
        gtk_container_set_border_width (GTK_CONTAINER (self), 16);
        gtk_window_set_default_size (GTK_WINDOW (self), 350, -1);
        gtk_dialog_set_has_separator (GTK_DIALOG (self), FALSE);
       
        /* Label */
        private->Label = GTK_LABEL (gtk_label_new (NULL));
        g_return_val_if_fail (private->Label != NULL, FALSE);
        gtk_label_set_markup (private->Label, "<i> </i>");
        gtk_misc_set_alignment (GTK_MISC (private->Label), 0.0, 0.5);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->vbox), GTK_WIDGET (private->Label));

        /* Progressbar */
        private->Progress = GTK_PROGRESS_BAR (gtk_progress_bar_new ());
        g_return_val_if_fail (private->Progress != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->vbox), GTK_WIDGET (private->Progress));
     
        /* Button */
        private->AbortButton = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_CANCEL));
        g_return_val_if_fail (private->AbortButton != NULL, FALSE);
        g_signal_connect (G_OBJECT (private->AbortButton), "clicked", G_CALLBACK (on_abort_clicked), self);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->action_area), GTK_WIDGET (private->AbortButton));

        private->PulseTimeout = 0;
        private->Aborting = FALSE;

        return TRUE;
}

static void                     private_dispose (NFlickWaitDialogPrivate *private)
{
        g_return_if_fail (private != NULL);

        if (private->PulseTimeout != 0) {
                g_source_remove (private->PulseTimeout);
                private->PulseTimeout = 0;
        }
        
        if (private->Worker != NULL) {
                g_object_unref (private->Worker);
                private->Worker = NULL;
        }
}

static void                     nflick_wait_dialog_dispose (NFlickWaitDialog *self)
{
        g_return_if_fail (NFLICK_IS_WAIT_DIALOG (self));

        if (self->Private != NULL) {
                private_dispose (self->Private);
        }

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_wait_dialog_finalize (NFlickWaitDialog *self)
{
        g_return_if_fail (NFLICK_IS_WAIT_DIALOG (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickWaitDialog*               nflick_wait_dialog_new (NFlickWorker *worker, GtkWindow *parent)
{
        g_return_val_if_fail (worker != NULL, NULL);
        
        NFlickWaitDialog *self = g_object_new (NFLICK_TYPE_WAIT_DIALOG, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }
        
        /* Check if the worker is in the right state */
        NFlickWorkerStatus status;
        g_object_get (G_OBJECT (worker), "status", &status, NULL);
        
        if (status != NFLICK_WORKER_STATUS_IDLE) {
                g_warning ("Bad worker status");
                g_object_unref (self);
                return NULL;
        }
        
        g_object_ref (worker);
        self->Private->Worker = worker;
        
        /* Get the initial message */
        gchar *msg = NULL;
        gchar *msg_i = NULL;
        g_object_get (G_OBJECT (self->Private->Worker), "message", &msg, NULL);
        if (msg != NULL) {
                /* FIXME Escape markup */
                msg_i = g_strdup_printf ("<i>%s</i>", msg);
                gtk_label_set_markup (self->Private->Label, msg_i);
        }
        
        /* Set the functions */
        nflick_worker_set_custom_data (worker, self);
        nflick_worker_set_aborted_idle (worker, (NFlickWorkerIdleFunc) on_thread_abort_idle);
        nflick_worker_set_error_idle (worker, (NFlickWorkerIdleFunc) on_thread_error_idle);
        nflick_worker_set_ok_idle (worker, (NFlickWorkerIdleFunc) on_thread_ok_idle);
        nflick_worker_set_msg_change_idle (worker, (NFlickWorkerIdleFunc) on_thread_msg_change_idle);
                                        
        /* Free */
        if (msg != NULL)
                g_free (msg);
        if (msg_i != NULL)
                g_free (msg_i);

        /* Set transiency if needed */
        if (parent != NULL)
                gtk_window_set_transient_for (GTK_WINDOW (self), parent);
        
        return self;
}

static void                     start_pulser (NFlickWaitDialog *self)
{
        g_return_if_fail (NFLICK_IS_WAIT_DIALOG (self));

        g_return_if_fail (self->Private->PulseTimeout == 0);

        /* FIXME Move interval into define */
        self->Private->PulseTimeout = g_timeout_add (50, (GSourceFunc) on_pulse_timeout, self);
}

static void                     kill_pulser (NFlickWaitDialog *self)
{
        g_return_if_fail (NFLICK_IS_WAIT_DIALOG (self));

        if (self->Private->PulseTimeout != 0) {
                g_source_remove (self->Private->PulseTimeout);
                self->Private->PulseTimeout = 0;
        }
}

static gboolean                 on_pulse_timeout (NFlickWaitDialog *self)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), FALSE);

        gtk_progress_bar_pulse (self->Private->Progress);

        return TRUE;
}

static void                     on_abort_clicked (GtkWidget *widget, NFlickWaitDialog *self)
{
        g_return_if_fail (NFLICK_IS_WAIT_DIALOG (self));
        
        /* Ignore multiple calls */
        if (self->Private->Aborting == TRUE)
                return;
        
        gtk_widget_set_sensitive (GTK_WIDGET (self->Private->AbortButton), FALSE);
        gtk_label_set_markup (self->Private->Label, gettext ("<i>Aborting...</i>"));
        nflick_worker_request_abort (self->Private->Worker);
        self->Private->Aborting = TRUE;
}

static gboolean                 on_thread_abort_idle (NFlickWaitDialog *self)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), FALSE);

        kill_pulser (self);
        gtk_dialog_response (GTK_DIALOG (self), NFLICK_WAIT_DIALOG_RESPONSE_ABORTED);

        return FALSE;
}

static gboolean                 on_thread_ok_idle (NFlickWaitDialog *self)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), FALSE);

        kill_pulser (self);
        gtk_dialog_response (GTK_DIALOG (self), NFLICK_WAIT_DIALOG_RESPONSE_OK);

        return FALSE;
}

static gboolean                 on_thread_error_idle (NFlickWaitDialog *self)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), FALSE);
        
        kill_pulser (self);
        
        /* Get the actual error */
        gchar *error = NULL;
        g_object_get (G_OBJECT (self->Private->Worker), "error", &error, NULL);
        if (error == NULL) {
                error = g_strdup_printf (gettext ("Internal error. "));
                g_warning ("No error set on worker!");
        }

        gtk_widget_hide_all (GTK_WIDGET (self));

        GtkDialog *error_dialog = GTK_DIALOG (hildon_note_new_information (GTK_WINDOW(self), error));
        gtk_dialog_run (error_dialog);
        gtk_widget_destroy (GTK_WIDGET (error_dialog));

        gtk_dialog_response (GTK_DIALOG (self), NFLICK_WAIT_DIALOG_RESPONSE_ERROR);

        if (error != NULL)
                g_free (error);

        return FALSE;
}

static gboolean                 on_thread_msg_change_idle (NFlickWaitDialog *self)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), FALSE);
        
        /* Get the new message */
        gchar *msg = NULL;
        gchar *msg_i = NULL;
        g_object_get (G_OBJECT (self->Private->Worker), "message", &msg, NULL);
        if (msg != NULL) {
                /* FIXME Escape markup */
                msg_i = g_strdup_printf ("<i>%s</i>", msg);
                gtk_label_set_markup (self->Private->Label, msg_i);
        } else 
                gtk_label_set_markup (self->Private->Label, "<i> </i>");

        /* Free */
        if (msg != NULL)
                g_free (msg);
        if (msg_i != NULL)
                g_free (msg_i);

        return FALSE;
}

gint                            nflick_wait_dialog_show_and_run (NFlickWaitDialog *self)
{
        g_return_val_if_fail (NFLICK_IS_WAIT_DIALOG (self), NFLICK_WAIT_DIALOG_RESPONSE_ERROR);
        
        gtk_widget_show_all (GTK_WIDGET (self));
        
        start_pulser (self);
        nflick_worker_start (self->Private->Worker);
        return gtk_dialog_run (GTK_DIALOG (self));
}
