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

#include "nflick-window.h"
#include "nflick-window-private.h"

GType                           nflick_window_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickWindowClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_window_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickWindow), 
                        4, 
                        (GInstanceInitFunc) nflick_window_init,
                };
                objecttype = g_type_register_static (HILDON_TYPE_WINDOW, "NFlickWindow",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_window_class_init (NFlickWindowClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;
        GtkWidgetClass *widgetclass = (GtkWidgetClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_window_dispose;
        gobjectclass->finalize = (gpointer) nflick_window_finalize;

        widgetclass->delete_event = (gpointer) on_delete_event;

        ParentClass = g_type_class_ref (HILDON_TYPE_WINDOW);
}

static gboolean                 on_delete_event (NFlickWindow *self)
{
        g_return_val_if_fail (NFLICK_IS_WINDOW (self), TRUE);

        nflick_model_destroy ();
        gtk_main_quit ();

        return TRUE;
}

static void                     nflick_window_init (NFlickWindow *self)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));

        self->Private = NULL;

        NFlickWindowPrivate *priv = g_new0 (NFlickWindowPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static gboolean                 private_init (NFlickWindow *self, NFlickWindowPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_WINDOW (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        gtk_container_set_border_width (GTK_CONTAINER (self), 16);

        /* Menus */
        GtkMenu *main_menu = GTK_MENU (gtk_menu_new ());
        g_return_val_if_fail (main_menu != NULL, FALSE);
        hildon_window_set_menu (HILDON_WINDOW (self), main_menu);

        /* Cache  */
        GtkMenuItem *cache_item = GTK_MENU_ITEM (gtk_menu_item_new_with_label (gettext ("Cache details...")));
        g_return_val_if_fail (cache_item != NULL, FALSE);
        gtk_menu_append (main_menu, GTK_WIDGET (cache_item));
        g_signal_connect (G_OBJECT (cache_item), "activate", GTK_SIGNAL_FUNC (on_cache_activated), NULL);

        /* Reset item  */
        GtkMenuItem *reset_item = GTK_MENU_ITEM (gtk_menu_item_new_with_label (gettext ("Reset private data")));
        g_return_val_if_fail (reset_item != NULL, FALSE);
        gtk_menu_append (main_menu, GTK_WIDGET (reset_item));
        g_signal_connect (G_OBJECT (reset_item), "activate", GTK_SIGNAL_FUNC (on_reset_activated), NULL);

        /* Separator */
        GtkWidget *separator = gtk_separator_menu_item_new();
        g_return_val_if_fail (separator != NULL, FALSE);
        gtk_menu_append (main_menu, separator);

        /* Close */
        GtkMenuItem *close_item = GTK_MENU_ITEM (gtk_menu_item_new_with_label (gettext ("Exit")));
        g_return_val_if_fail (close_item != NULL, FALSE);
        gtk_menu_append (main_menu, GTK_WIDGET (close_item));
        g_signal_connect (G_OBJECT (close_item), "activate", GTK_SIGNAL_FUNC (on_close_activated), NULL);

        /* Is this really needed?! */
        gtk_widget_show_all (GTK_WIDGET (main_menu));

        /* Connect the kard keys */
        g_signal_connect (G_OBJECT (self), "key-release-event", (gpointer) on_key_released, NULL);

        NFlickWelcomeVBox *vbox = nflick_welcome_vbox_new ();
        private->CurrentVBox = GTK_VBOX (vbox);
        g_object_ref (vbox);
        gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET (vbox));

        return TRUE;
}

static gboolean                 on_key_released (NFlickWindow *self, GdkEventKey *event)
{
        if (! NFLICK_IS_SHOW_VBOX (self->Private->CurrentVBox))
                return FALSE;

        if (event->keyval == HILDON_HARDKEY_ESC ||
            event->keyval == HILDON_HARDKEY_FULLSCREEN)
                nflick_model_hide_photo ();

        GList *list = NULL;
        if (event->keyval == HILDON_HARDKEY_LEFT)
            list = nflick_show_vbox_get_prev_photo_list ((NFlickShowVBox *) self->Private->CurrentVBox);
        else if (event->keyval == HILDON_HARDKEY_RIGHT)
            list = nflick_show_vbox_get_next_photo_list ((NFlickShowVBox *) self->Private->CurrentVBox);

        if (list == NULL)
            return FALSE;
        else 
            nflick_model_switch_photo (((NFlickPhotoData *) list->data)->Id, list);

        return FALSE;
}

static void                     on_cache_activated (void)
{
        NFlickCacheDialog *cache_dialog = nflick_cache_dialog_new ();
        g_return_if_fail (cache_dialog != NULL);

        gtk_widget_show_all (GTK_WIDGET (cache_dialog));
        gtk_dialog_run (GTK_DIALOG (cache_dialog));
        gtk_widget_destroy (GTK_WIDGET (cache_dialog));
}

static void                     on_reset_activated (void)
{
        GtkDialog *note = GTK_DIALOG (hildon_note_new_confirmation (GTK_WINDOW (NFLICK_MODEL_WINDOW), 
                                                                    gettext ("Are you sure you want to reset all private data?")));
        gtk_widget_show_all (GTK_WIDGET (note));
        int response = gtk_dialog_run (GTK_DIALOG (note));
        gtk_widget_destroy (GTK_WIDGET (note));
        
        if (response == GTK_RESPONSE_OK) {
                /* Do the action */
                nflick_model_settings_reset ();
                nflick_model_reset_gconf ();
                nflick_model_clear_cache ();
                nflick_processor_cancel_all ();
                nflick_window_change_to_vbox (NFLICK_MODEL_WINDOW, 
                                              GTK_VBOX (nflick_welcome_vbox_new ()));
        }
}

static void                     on_close_activated (void)
{
        nflick_model_destroy ();
        gtk_main_quit ();
}

static void                     private_dispose (NFlickWindowPrivate *private)
{
        g_return_if_fail (private != NULL);

        if (private->PushedVBox != NULL) {
                g_object_unref (private->PushedVBox);
                private->PushedVBox = NULL;
        }
}

static void                     nflick_window_dispose (NFlickWindow *self)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_window_finalize (NFlickWindow *self)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));

        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickWindow*                   nflick_window_new (void)
{
        NFlickWindow *self = g_object_new (NFLICK_TYPE_WINDOW, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }
        
        return self;
}

void                            nflick_window_push_vbox (NFlickWindow *self, GtkVBox *vbox)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));
        g_return_if_fail (self->Private->CurrentVBox != NULL);
        
        self->Private->PushedVBox = self->Private->CurrentVBox;
        g_object_ref (self->Private->PushedVBox);

        nflick_window_change_to_vbox (self, vbox);
}

void                            nflick_window_pop_vbox (NFlickWindow *self)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));
        g_return_if_fail (self->Private->PushedVBox != NULL);
      
        GtkVBox *vbox = self->Private->PushedVBox;
        self->Private->PushedVBox = NULL;

        nflick_window_change_to_vbox (self, vbox);
}

void                            nflick_window_change_to_vbox (NFlickWindow *self, GtkVBox *vbox)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));

        if (self->Private->CurrentVBox != NULL) 
                gtk_container_remove (GTK_CONTAINER (self), GTK_WIDGET (self->Private->CurrentVBox));
        
        gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET (vbox));
        self->Private->CurrentVBox = vbox;
        gtk_widget_show_all (GTK_WIDGET (vbox));
}

void                            nflick_window_resume_fetching (NFlickWindow *self)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));

        if (NFLICK_IS_PHOTOS_VBOX (self->Private->CurrentVBox))
                nflick_photos_resume_fetching ((NFlickPhotosVBox *) self->Private->CurrentVBox);
}

void                            nflick_window_probe_vbox_size (NFlickWindow *self, gint *width, gint *height)
{
        g_return_if_fail (NFLICK_IS_WINDOW (self));
        g_return_if_fail (width != NULL);
        g_return_if_fail (height != NULL);
        g_return_if_fail (self->Private->CurrentVBox != NULL);

        *width = GTK_WIDGET (self)->allocation.width;
        *height = GTK_WIDGET (self)->allocation.height;
}
