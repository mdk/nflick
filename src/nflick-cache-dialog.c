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

#include "nflick-cache-dialog.h"
#include "nflick-cache-dialog-private.h"

GType                           nflick_cache_dialog_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickCacheDialogClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_cache_dialog_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickCacheDialog), 
                        4, 
                        (GInstanceInitFunc) nflick_cache_dialog_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_DIALOG, "NFlickCacheDialog",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_cache_dialog_class_init (NFlickCacheDialogClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_cache_dialog_dispose;
        gobjectclass->finalize = (gpointer) nflick_cache_dialog_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_DIALOG);
}

static void                     nflick_cache_dialog_init (NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));

        self->Private = NULL;

        NFlickCacheDialogPrivate *priv = g_new0 (NFlickCacheDialogPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) {
                self->Private = priv;
                refresh_values (self);
        } else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }


}

static gboolean                 private_init (NFlickCacheDialog *self, NFlickCacheDialogPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_CACHE_DIALOG (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        gtk_window_set_title (GTK_WINDOW (self), gettext ("Cache details"));
        gtk_container_set_border_width (GTK_CONTAINER (self), 16);
        gtk_dialog_set_has_separator (GTK_DIALOG (self), FALSE);

        /* This displays the actual count value */
        private->ItemsLabel = GTK_LABEL (gtk_label_new (gettext ("N/A")));
        g_return_val_if_fail (private->ItemsLabel != NULL, FALSE);
        gtk_misc_set_alignment (GTK_MISC (private->ItemsLabel), 0.0, 0.5);

        /* This displays the actual size value */
        private->SizeLabel = GTK_LABEL (gtk_label_new (gettext ("N/A")));
        g_return_val_if_fail (private->SizeLabel != NULL, FALSE);
        gtk_misc_set_alignment (GTK_MISC (private->SizeLabel), 0.0, 0.5);

        /* This is the check item for enablding/disabling the cache */
        private->UseCacheCheckButton = GTK_CHECK_BUTTON (gtk_check_button_new ());
        g_return_val_if_fail (private->UseCacheCheckButton != NULL, FALSE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (private->UseCacheCheckButton), 
                                      NFLICK_MODEL_USE_CACHE);
        g_signal_connect (G_OBJECT (private->UseCacheCheckButton), "toggled", 
                          G_CALLBACK (on_use_cache_toggled), self);

        /* Size group */
        GtkSizeGroup *size_group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);

        /* Count caption */
        HildonCaption *count_caption = HILDON_CAPTION (hildon_caption_new (size_group, 
                                                                           gettext ("No. of items"), 
                                                                           GTK_WIDGET (private->ItemsLabel), 
                                                                           NULL,
                                                                           HILDON_CAPTION_MANDATORY));
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->vbox), GTK_WIDGET (count_caption));

        /* Cache size caption */
        HildonCaption *size_caption = HILDON_CAPTION (hildon_caption_new (size_group, 
                                                                          gettext ("Cache total size"), 
                                                                          GTK_WIDGET (private->SizeLabel), 
                                                                          NULL,
                                                                          HILDON_CAPTION_MANDATORY));
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->vbox), GTK_WIDGET (size_caption));

        /* Use cache button */
        HildonCaption *use_caption = HILDON_CAPTION (hildon_caption_new (size_group, 
                                                                         gettext ("Use cache"), 
                                                                         GTK_WIDGET (private->UseCacheCheckButton), 
                                                                         NULL,
                                                                         HILDON_CAPTION_MANDATORY));
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->vbox), GTK_WIDGET (use_caption));

        /* We can unref the size group now... */
        g_object_unref (size_group);
        
        /* Clear Button */
        private->ClearButton = GTK_BUTTON (gtk_button_new_with_label (gettext ("Clear cache")));
        g_return_val_if_fail (private->ClearButton != NULL, FALSE);
        g_signal_connect (G_OBJECT (private->ClearButton), "clicked", G_CALLBACK (on_clear_clicked), self);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->action_area), GTK_WIDGET (private->ClearButton));

        /* Ok Button */
        GtkButton *close_button = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_CLOSE));
        g_return_val_if_fail (close_button != NULL, FALSE);
        g_signal_connect (G_OBJECT (close_button), "clicked", G_CALLBACK (on_close_clicked), self);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->action_area), GTK_WIDGET (close_button));
        
        /* Parent behaviour */
        gtk_window_set_transient_for (GTK_WINDOW (self), GTK_WINDOW (NFLICK_MODEL_WINDOW));
        gtk_window_set_destroy_with_parent (GTK_WINDOW (self), TRUE);
        gtk_widget_grab_focus (GTK_WIDGET (close_button));

        return TRUE;
}

static void                     on_close_clicked (GtkWidget *widget, NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));

        gtk_dialog_response (GTK_DIALOG (self), GTK_RESPONSE_OK);
}

static void                     on_clear_clicked (GtkWidget *widget, NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));

        nflick_model_clear_cache ();
        refresh_values (self);
}

static void                     on_use_cache_toggled (GtkWidget *widget, NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));

        nflick_model_enable_cache (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self->Private->UseCacheCheckButton)));
}

static void                     refresh_values (NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));

        int items = 0; /* Number of items */
        int size = 0;  /* Size */

        GDir *cache_dir = g_dir_open (NFLICK_MODEL_CACHE_DIR, 0, NULL);
        if (cache_dir == NULL)
                goto Done;

        const gchar *file_name = NULL;
        struct stat stat_struct;

        while ((file_name = g_dir_read_name (cache_dir)) != NULL) {
                gchar *path = g_build_filename (NFLICK_MODEL_CACHE_DIR, file_name, NULL);
                g_stat (path, &stat_struct);
                g_free (path);
                items++;
                size += stat_struct.st_size;
        }

        gchar *items_str = g_strdup_printf ("%d", items);
        if (items_str == NULL)
                goto Done;
        else
                gtk_label_set_text (self->Private->ItemsLabel, items_str);


        gchar *size_str = NULL;
        if (size > 1024 * 1024)
                size_str = g_strdup_printf ("%.1f MB", (float) size / (1024 * 1024));
        else
                size_str = g_strdup_printf ("%d KB", (int) (size / 1024));

        if (size_str == NULL)
                goto Done;
        else
                gtk_label_set_text (self->Private->SizeLabel, size_str);

        if (items > 0)
                gtk_widget_set_sensitive (GTK_WIDGET (self->Private->ClearButton), TRUE);
        else
                gtk_widget_set_sensitive (GTK_WIDGET (self->Private->ClearButton), FALSE);

Done:
        if (cache_dir != NULL)
                g_dir_close (cache_dir);

        if (items_str != NULL)
                g_free (items_str);

        if (size_str != NULL)
                g_free (size_str);
}

static void                     private_dispose (NFlickCacheDialogPrivate *private)
{
        g_return_if_fail (private != NULL);
}

static void                     nflick_cache_dialog_dispose (NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));

        if (self->Private != NULL) {
                private_dispose (self->Private);
        }

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_cache_dialog_finalize (NFlickCacheDialog *self)
{
        g_return_if_fail (NFLICK_IS_CACHE_DIALOG (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickCacheDialog*               nflick_cache_dialog_new (void)
{
        NFlickCacheDialog *self = g_object_new (NFLICK_TYPE_CACHE_DIALOG, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }
        
        return self;
}
