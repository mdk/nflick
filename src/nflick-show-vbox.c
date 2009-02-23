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

#include "nflick-show-vbox.h"
#include "nflick-show-vbox-private.h"

GType                           nflick_show_vbox_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickShowVBoxClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_show_vbox_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickShowVBox), 
                        4, 
                        (GInstanceInitFunc) nflick_show_vbox_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_VBOX, "NFlickShowVBox",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_show_vbox_class_init (NFlickShowVBoxClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_show_vbox_dispose;
        gobjectclass->finalize = (gpointer) nflick_show_vbox_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_VBOX);
}

static void                     nflick_show_vbox_init (NFlickShowVBox *self)
{
        g_return_if_fail (NFLICK_IS_SHOW_VBOX (self));

        self->Private = NULL;

        NFlickShowVBoxPrivate *priv = g_new0 (NFlickShowVBoxPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) {
                self->Private = priv;
        } else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static gboolean                 on_button_released (gpointer a, gpointer b, NFlickShowVBox *self)
{
        g_return_val_if_fail (NFLICK_IS_SHOW_VBOX (self), FALSE);
        nflick_model_hide_photo ();
        return TRUE;
}

static gboolean                 private_init (NFlickShowVBox *self, NFlickShowVBoxPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_SHOW_VBOX (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        GtkEventBox *box = GTK_EVENT_BOX (gtk_event_box_new ());
        g_return_val_if_fail (box != NULL, FALSE);
        g_signal_connect (G_OBJECT (box), "button-release-event", (gpointer) on_button_released, self);
        gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET (box));

        /* Make the container black */
        GdkColor color;
        gdk_color_parse ("black", &color);
        gtk_widget_modify_bg (GTK_WIDGET (box), GTK_STATE_NORMAL, &color);
        gtk_widget_modify_bg (GTK_WIDGET (box), GTK_STATE_PRELIGHT, &color);

        private->Image = GTK_IMAGE (gtk_image_new ());
        g_return_val_if_fail (private->Image != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (box), GTK_WIDGET (private->Image));

        gtk_widget_grab_focus (GTK_WIDGET (box));

        return TRUE;
}

static void                     private_dispose (NFlickShowVBoxPrivate *private)
{
        g_return_if_fail (private != NULL);
}


static void                     nflick_show_vbox_dispose (NFlickShowVBox *self)
{
        g_return_if_fail (NFLICK_IS_SHOW_VBOX (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_show_vbox_finalize (NFlickShowVBox *self)
{
        g_return_if_fail (NFLICK_IS_SHOW_VBOX (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickShowVBox*                 nflick_show_vbox_new (GdkPixbuf *pixbuf, GList *list)
{
        g_return_val_if_fail (GDK_IS_PIXBUF (pixbuf), NULL);
        g_return_val_if_fail (list != NULL, NULL);

        NFlickShowVBox *self = g_object_new (NFLICK_TYPE_SHOW_VBOX, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }

        g_object_set (G_OBJECT (self->Private->Image), "pixbuf", pixbuf, NULL);
        self->Private->List = list;

        return self;
}

const GList*                    nflick_show_vbox_get_next_photo_list (NFlickShowVBox *self)
{
        g_return_val_if_fail (NFLICK_IS_SHOW_VBOX (self), NULL);
        g_return_val_if_fail (self->Private->List != NULL, NULL);

        return g_list_next (self->Private->List);
}

const GList*                    nflick_show_vbox_get_prev_photo_list (NFlickShowVBox *self)
{
        g_return_val_if_fail (NFLICK_IS_SHOW_VBOX (self), NULL);
        g_return_val_if_fail (self->Private->List != NULL, NULL);

        return g_list_previous (self->Private->List);
}
