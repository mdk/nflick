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

#include "nflick-thmb-table.h"
#include "nflick-thmb-table-private.h"

GType                           nflick_thmb_table_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickThmbTableClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_thmb_table_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickThmbTable), 
                        4, 
                        (GInstanceInitFunc) nflick_thmb_table_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_TABLE, "NFlickThmbTable",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_thmb_table_class_init (NFlickThmbTableClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_thmb_table_dispose;
        gobjectclass->finalize = (gpointer) nflick_thmb_table_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_TABLE);
}

static void                     nflick_thmb_table_init (NFlickThmbTable *self)
{
        g_return_if_fail (NFLICK_IS_THMB_TABLE (self));

        self->Private = NULL;

        NFlickThmbTablePrivate *priv = g_new0 (NFlickThmbTablePrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static gboolean                 private_init (NFlickThmbTable *self, NFlickThmbTablePrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_THMB_TABLE (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        g_object_set (G_OBJECT (self), "homogeneous", TRUE, NULL);
        g_object_set (G_OBJECT (self), "n-rows", NFLICK_THUMBS_PER_COLUMN, "n-columns", NFLICK_THUMBS_PER_ROW, NULL);
        private->Thmbs = NULL;

        return TRUE;
}

static void                     private_dispose (NFlickThmbTablePrivate *private)
{
        g_return_if_fail (private != NULL);
}


static void                     nflick_thmb_table_dispose (NFlickThmbTable *self)
{
        g_return_if_fail (NFLICK_IS_THMB_TABLE (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_thmb_table_finalize (NFlickThmbTable *self)
{
        g_return_if_fail (NFLICK_IS_THMB_TABLE (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

static void                     on_thmb_button_released (GtkWidget *box, gpointer b, NFlickThmbTable *self)
{
        g_return_if_fail (NFLICK_IS_THMB_TABLE (self));
        
        NFlickThmbImage *image = gtk_bin_get_child ((GtkBin *) box);
        g_return_if_fail (NFLICK_IS_THMB_IMAGE (image));

        gchar *photo_id = NULL;
        g_object_get (G_OBJECT (image), "photoid", &photo_id, NULL);

        /* Iterate through our list to find the position */
        GList *list = self->Private->ImageIds;
        while (list != NULL &&
               strcmp (((NFlickPhotoData *) list->data)->Id, photo_id) != 0) {
                list = g_list_next (list);
        }

        nflick_model_show_photo (photo_id, list);

        if (photo_id != NULL)
                g_free (photo_id);
}

void                            nflick_thmb_table_resume_fetching (NFlickThmbTable *self)
{
        g_return_if_fail (NFLICK_IS_THMB_TABLE (self));

        GList *iterator;
        for (iterator = self->Private->Thmbs; iterator; iterator = g_list_next (iterator)) {
                NFlickThmbImage *image = iterator->data;

                gboolean fetched = FALSE;
                g_object_get (G_OBJECT (image), "fetched", &fetched, NULL);
                if (fetched == FALSE) 
                        nflick_thmb_image_resume_fetching (image);
        }
}

NFlickThmbTable*                nflick_thmb_table_new (NFlickPhotoSet *set, gint32 page)
{
        g_return_val_if_fail (NFLICK_IS_PHOTO_SET (set), NULL);
        g_return_val_if_fail (page >= 1, NULL);

        gboolean fetched;
        g_object_get (G_OBJECT (set), "fetched", &fetched, NULL);
        g_return_val_if_fail (fetched == TRUE, NULL);

        NFlickThmbTable *self = g_object_new (NFLICK_TYPE_THMB_TABLE, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }

        gint32 start = NFLICK_THUMBS_PER_PAGE * (page - 1);
        GList *list = NULL;
        gint32 count = 0;
        g_object_get (G_OBJECT (set), "count", &count, "list", &list, NULL);
        if (count == 0)
                return self;
        if (start >= count)
                return self;
        gint32 max_reach = MIN (start + NFLICK_THUMBS_PER_PAGE, count);

        gint32 y = 0;
        gint32 x = 0;
        gint32 current = 0;

        self->Private->ImageIds = list;

        /* Skip a bit to reach the page cound. 
         * Perhaps there is a better way to do this? */
        while (current < start) {
                list = g_list_next (list);
                current++;
        }

        while (current < max_reach) {
                g_return_val_if_fail (list != NULL, self);
                NFlickPhotoData *photo_data = (NFlickPhotoData *) list->data;
       
                GtkEventBox *box = GTK_EVENT_BOX (gtk_event_box_new ());
                g_return_val_if_fail (box != NULL, self);

                NFlickThmbImage *image = nflick_thmb_image_new (photo_data);
                g_return_val_if_fail (image != NULL, self);
                
                g_signal_connect (G_OBJECT (box), "button-release-event", (gpointer) on_thmb_button_released, self);

                gtk_container_add (GTK_CONTAINER (box), GTK_WIDGET (image));
                gtk_table_attach (GTK_TABLE (self), GTK_WIDGET (box), x, x + 1, y, y + 1, GTK_EXPAND, GTK_EXPAND, 0, 0);

                self->Private->Thmbs = g_list_append (self->Private->Thmbs, image);

                x++;

                if (x == 6) {
                        x = 0;
                        y++;
                }
                
                list = g_list_next (list);
                current++;
        }

        return self;
}

