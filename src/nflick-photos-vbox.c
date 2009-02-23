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

#include "nflick-photos-vbox.h"
#include "nflick-photos-vbox-private.h"

GType                           nflick_photos_vbox_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickPhotosVBoxClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_photos_vbox_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickPhotosVBox), 
                        4, 
                        (GInstanceInitFunc) nflick_photos_vbox_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_VBOX, "NFlickPhotosVBox",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_photos_vbox_class_init (NFlickPhotosVBoxClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_photos_vbox_dispose;
        gobjectclass->finalize = (gpointer) nflick_photos_vbox_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_VBOX);
}

static void                     nflick_photos_vbox_init (NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));

        self->Private = NULL;

        NFlickPhotosVBoxPrivate *priv = g_new0 (NFlickPhotosVBoxPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static gboolean                 private_init (NFlickPhotosVBox *self, NFlickPhotosVBoxPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_PHOTOS_VBOX (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        gtk_box_set_spacing (GTK_BOX (self), 12);

        /* Create the main upper hbox container (label + combo) */
        GtkHBox *upper_hbox = GTK_HBOX (gtk_hbox_new (FALSE, 16));
        g_return_val_if_fail (upper_hbox != NULL, FALSE);
        gtk_box_pack_start (GTK_BOX (self), GTK_WIDGET (upper_hbox), FALSE, FALSE, 0);

        /* Name label */
        GtkLabel *name_label = GTK_LABEL (gtk_label_new (NULL));
        g_return_val_if_fail (name_label != NULL, FALSE);
        gtk_misc_set_alignment (GTK_MISC (name_label), 0.0, 0.5);
        
        gchar *mrkp = NULL; 
        
        if (NFLICK_MODEL_FULLNAME != NULL && *NFLICK_MODEL_FULLNAME != 0)
                mrkp = g_strdup_printf (gettext ("<big><b>%s</b></big>"), NFLICK_MODEL_FULLNAME);
        else
                mrkp = g_strdup (gettext ("<big><b>Your photos</b></big>"));

        g_return_val_if_fail (mrkp != NULL, FALSE);
        
        gtk_label_set_markup (GTK_LABEL (name_label), mrkp);
        g_free (mrkp);

        gtk_box_pack_start (GTK_BOX (upper_hbox), GTK_WIDGET (name_label), TRUE, TRUE, 0);

        /* Combo */
        private->Combo = nflick_set_combo_new ();
        g_return_val_if_fail (private->Combo != NULL, FALSE);
        g_signal_connect (G_OBJECT (private->Combo), "changed", (gpointer) on_combo_changed, self);
        gtk_box_pack_end (GTK_BOX (upper_hbox), GTK_WIDGET (private->Combo), FALSE, FALSE, 0);

        /* Horizontal separator */
        GtkHSeparator *separator = GTK_HSEPARATOR (gtk_hseparator_new ());
        g_return_val_if_fail (separator != NULL, FALSE);
        gtk_box_pack_start (GTK_BOX (self), GTK_WIDGET (separator), FALSE, FALSE, 0);
        
        /* Bottom hbox */
        GtkHButtonBox *bottom_box = GTK_HBUTTON_BOX (gtk_hbutton_box_new ());
        g_return_val_if_fail (bottom_box != NULL, FALSE);
        gtk_box_set_spacing (GTK_BOX (bottom_box), 12);
        gtk_box_pack_end (GTK_BOX (self), GTK_WIDGET (bottom_box), FALSE, FALSE, 0);
        
        /* Previous button */
        private->PrevButton = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_GO_BACK));
        g_return_val_if_fail (private->PrevButton != NULL, FALSE);
        g_signal_connect (G_OBJECT (private->PrevButton), "clicked", G_CALLBACK (on_prev_clicked), self);
        gtk_container_add (GTK_CONTAINER (bottom_box), GTK_WIDGET (private->PrevButton));

        /* Paging label */
        private->PageLabel = GTK_LABEL (gtk_label_new (NULL));
        g_return_val_if_fail (private->PageLabel != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (bottom_box), GTK_WIDGET (private->PageLabel));

        /* Next button */
        private->NextButton = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_GO_FORWARD));
        g_return_val_if_fail (private->NextButton != NULL, FALSE);
        g_signal_connect (G_OBJECT (private->NextButton), "clicked", G_CALLBACK (on_next_clicked), self);
        gtk_container_add (GTK_CONTAINER (bottom_box), GTK_WIDGET (private->NextButton));

        /* Button size group */
        GtkSizeGroup *size_group = gtk_size_group_new (GTK_SIZE_GROUP_HORIZONTAL);
        gtk_size_group_add_widget (size_group, GTK_WIDGET (private->NextButton));
        gtk_size_group_add_widget (size_group, GTK_WIDGET (private->PrevButton));
        g_object_unref (G_OBJECT (size_group));

        private->Table = NULL;
        private->CurrentPage = 0;

        return TRUE;
}

static void                     set_paging (NFlickPhotosVBox *self, int page, int max)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));

        gchar *str = g_strdup_printf (gettext ("Page %d/%d"), page, max);
        g_return_if_fail (str != NULL);
        
        gtk_label_set_text (self->Private->PageLabel, str);

        if (page > 1)
                gtk_widget_set_sensitive (GTK_WIDGET (self->Private->PrevButton), TRUE);
        else
                gtk_widget_set_sensitive (GTK_WIDGET (self->Private->PrevButton), FALSE);

        if (max > 1 && page != max)
                gtk_widget_set_sensitive (GTK_WIDGET (self->Private->NextButton), TRUE);
        else
                gtk_widget_set_sensitive (GTK_WIDGET (self->Private->NextButton), FALSE);

        g_free (str);
}

static void                     on_next_clicked (GtkWidget *widget, NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));

        /* Abort all pending operations, if we have any */
        nflick_processor_cancel_all ();

        NFlickPhotoSet *set = nflick_set_combo_get_current_set (self->Private->Combo);
        /* FIXME Make this returned a ref set, do a dispose */
        g_return_if_fail (set != NULL);

        /* Now let's see if we need to fetch it...*/
        gboolean fetched = FALSE;
        g_object_get (G_OBJECT (set), "fetched", &fetched, NULL);
        g_return_if_fail (fetched == TRUE);

        /* Increase the page */
        self->Private->CurrentPage++;

        /* Create new table */
        NFlickThmbTable *table = nflick_thmb_table_new (set, self->Private->CurrentPage);
        g_return_if_fail (table != NULL);

        int count = 0;
        g_object_get (G_OBJECT (set), "count", &count, NULL);
        set_paging (self, self->Private->CurrentPage, 
                    (count % NFLICK_THUMBS_PER_PAGE == 0) ? 
                    (count / NFLICK_THUMBS_PER_PAGE) :
                    (count / NFLICK_THUMBS_PER_PAGE) + 1);

        set_table (self, table);
        
        gtk_widget_show_all (GTK_WIDGET (table));
}

static void                     on_prev_clicked (GtkWidget *widget, NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));

        /* Abort all pending operations, if we have any */
        nflick_processor_cancel_all ();

        NFlickPhotoSet *set = nflick_set_combo_get_current_set (self->Private->Combo);
        /* FIXME Make this returned a ref set, do a dispose */
        g_return_if_fail (set != NULL);

        /* Now let's see if we need to fetch it...*/
        gboolean fetched = FALSE;
        g_object_get (G_OBJECT (set), "fetched", &fetched, NULL);
        g_return_if_fail (fetched == TRUE);

        /* Increase the page */
        self->Private->CurrentPage--;
        g_return_if_fail (self->Private->CurrentPage >= 1);

        /* Create new table */
        NFlickThmbTable *table = nflick_thmb_table_new (set, self->Private->CurrentPage);
        g_return_if_fail (table != NULL);

        int count = 0;
        g_object_get (G_OBJECT (set), "count", &count, NULL);
        set_paging (self, self->Private->CurrentPage, (count / NFLICK_THUMBS_PER_PAGE) + 1);

        set_table (self, table);
        
        gtk_widget_show_all (GTK_WIDGET (table));
}

static void                     private_dispose (NFlickPhotosVBoxPrivate *private)
{
        g_return_if_fail (private != NULL);
}


static void                     nflick_photos_vbox_dispose (NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

void                            nflick_photos_resume_fetching (NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));
        nflick_thmb_table_resume_fetching (self->Private->Table); 
}

static void                     nflick_photos_vbox_finalize (NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickPhotosVBox*              nflick_photos_vbox_new (GList *list)
{
        NFlickPhotosVBox *self = g_object_new (NFLICK_TYPE_PHOTOS_VBOX, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }

        nflick_set_combo_load_list (self->Private->Combo, list);
        
        return self;
}

static void                     set_table (NFlickPhotosVBox *self, NFlickThmbTable *table)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));
        g_return_if_fail (NFLICK_IS_THMB_TABLE (table));

        if (self->Private->Table == NULL) 
                gtk_box_pack_start (GTK_BOX (self), GTK_WIDGET (table), TRUE, TRUE, 0);
        else {
                gtk_container_remove (GTK_CONTAINER (self), 
                                      GTK_WIDGET (self->Private->Table));
                gtk_box_pack_start (GTK_BOX (self), GTK_WIDGET (table), TRUE, TRUE, 0);
        }

        self->Private->Table = table;
}

static void                     on_combo_changed (GtkWidget *widget, NFlickPhotosVBox *self)
{
        g_return_if_fail (NFLICK_IS_PHOTOS_VBOX (self));

        /* Abort all pending operations, if we have any */
        nflick_processor_cancel_all ();

        NFlickPhotoSet *set = nflick_set_combo_get_current_set (self->Private->Combo);
        /* FIXME Make this returned a ref set, do a dispose */
        g_return_if_fail (set != NULL);

        /* Now let's see if we need to fetch it...*/
        gboolean fetched = FALSE;
        g_object_get (G_OBJECT (set), "fetched", &fetched, NULL);

        if (fetched == FALSE) {
                gchar *id = NULL;
                g_object_get (G_OBJECT (set), "id", &id, NULL);
                g_return_if_fail (id != NULL);

                NFlickPhotoListWorker *worker = nflick_photo_list_worker_new (id, NFLICK_MODEL_TOKEN);
                g_free (id);

                NFlickWaitDialog *dialog = nflick_wait_dialog_new ((NFlickWorker *) worker, (GtkWindow *) NFLICK_MODEL_WINDOW);
                int result = nflick_wait_dialog_show_and_run (dialog);
                gtk_widget_destroy (GTK_WIDGET (dialog));

                if (result != NFLICK_WAIT_DIALOG_RESPONSE_OK) {
                        g_object_unref (worker);
                        return;
                } else {
                        GList *list = nflick_photo_list_worker_take_list (worker);
                        nflick_photo_set_give_list (set, list);
                        g_object_unref (worker);
                }
        }

        /* Create new table */
        NFlickThmbTable *table = nflick_thmb_table_new (set, 1);
        g_return_if_fail (table != NULL);

        int count = 0;
        g_object_get (G_OBJECT (set), "count", &count, NULL);

        int pages = (count / NFLICK_THUMBS_PER_PAGE);
        if (count % NFLICK_THUMBS_PER_PAGE > 0)
                pages++;

        set_paging (self, 1, pages);
        self->Private->CurrentPage = 1;

        set_table (self, table);
        
        gtk_widget_show_all (GTK_WIDGET (table));
}

