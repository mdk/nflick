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

#include "nflick-thmb-image.h"
#include "nflick-thmb-image-private.h"

GType                           nflick_thmb_image_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickThmbImageClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_thmb_image_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickThmbImage), 
                        4, 
                        (GInstanceInitFunc) nflick_thmb_image_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_IMAGE, "NFlickThmbImage",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_thmb_image_class_init (NFlickThmbImageClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_thmb_image_dispose;
        gobjectclass->finalize = (gpointer) nflick_thmb_image_finalize;
        gobjectclass->get_property = (gpointer) nflick_thmb_image_get_property;

        g_object_class_install_property (gobjectclass, ARG_PHOTO_ID,
                                         g_param_spec_string
                                         ("photoid", "PhotoId", "Id of the photo we're interested in",
                                         NULL, G_PARAM_READABLE));

        g_object_class_install_property (gobjectclass, ARG_TITLE,
                                         g_param_spec_string
                                         ("title", "Title", "Title of the photo",
                                         NULL, G_PARAM_READABLE));

        g_object_class_install_property (gobjectclass, ARG_FETCHED,
                                         g_param_spec_boolean
                                         ("fetched", "Fetched", "If the fetching was finished",
                                         FALSE, G_PARAM_READABLE));

        ParentClass = g_type_class_ref (GTK_TYPE_IMAGE);
}

static void                     nflick_thmb_image_init (NFlickThmbImage *self)
{
        g_return_if_fail (NFLICK_IS_THMB_IMAGE (self));

        self->Private = NULL;

        NFlickThmbImagePrivate *priv = g_new0 (NFlickThmbImagePrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static gboolean                 private_init (NFlickThmbImage *self, NFlickThmbImagePrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_THMB_IMAGE (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        g_object_set (G_OBJECT (self), "pixbuf", NFLICK_MODEL_LOADING, NULL);
        private->Data = NULL;
        private->FetcherId = -1;
        private->Fetched = FALSE;

        return TRUE;
}

static void                     private_dispose (NFlickThmbImagePrivate *private)
{
        g_return_if_fail (private != NULL);

        if (private->Data != NULL) {
                nflick_photo_data_free (private->Data);
                private->Data = NULL;
        }

        if (private->FetcherId != -1) {
                nflick_processor_cancel_by_id (private->FetcherId);
                private->FetcherId = -1;
        }
}

static void                     nflick_thmb_image_dispose (NFlickThmbImage *self)
{
        g_return_if_fail (NFLICK_IS_THMB_IMAGE (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_thmb_image_finalize (NFlickThmbImage *self)
{
        g_return_if_fail (NFLICK_IS_THMB_IMAGE (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

void                            nflick_thmb_image_resume_fetching (NFlickThmbImage *self)
{
        g_return_if_fail (NFLICK_IS_THMB_IMAGE (self));

        self->Private->Fetched = FALSE;
        self->Private->FetcherId = nflick_processor_add_job (self->Private->Data->Id, self, 75, 75, on_job_finished);
}

NFlickThmbImage*                nflick_thmb_image_new (const NFlickPhotoData *data)
{
        g_return_val_if_fail (data != NULL, NULL);
        g_return_val_if_fail (data->Id != NULL, NULL);

        NFlickThmbImage *self = g_object_new (NFLICK_TYPE_THMB_IMAGE, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }

        self->Private->Data = nflick_photo_data_copy (data);
        g_return_val_if_fail (self->Private->Data != NULL, NULL);

        self->Private->FetcherId = nflick_processor_add_job (data->Id, self, 75, 75, on_job_finished);
        
        return self;
}

static gboolean                 on_job_finished (NFlickProcessorResult *result)
{
        g_return_val_if_fail (result != NULL, FALSE);
       	
        /* Abort if this ops is no longer valid */
        if (result->Valid == FALSE) 
                goto Done;

        
        NFlickThmbImage *self = (NFlickThmbImage *) result->Custom;
        g_return_val_if_fail (NFLICK_IS_THMB_IMAGE (self), FALSE);
        g_object_set (G_OBJECT (self), "pixbuf", result->Pixbuf, NULL);

        gtk_widget_show_all (GTK_WIDGET (self));

Done:
        if (self->Private->FetcherId == -1)
                g_warning ("FetcherId == -1, we shouldn't be here!");

        self->Private->FetcherId = -1;
        self->Private->Fetched = TRUE;
        nflick_processor_loose_result (result);
        return FALSE;
}

static void                     nflick_thmb_image_get_property (NFlickThmbImage *self, guint propid, 
                                                                GValue *value, GParamSpec *pspec)
{
        g_return_if_fail (NFLICK_IS_THMB_IMAGE (self));
        g_assert (self->Private != NULL);
                
        switch (propid) {
                
                case ARG_PHOTO_ID:
                        g_value_set_string (value, self->Private->Data->Id);
                break;
       
                case ARG_TITLE:
                        g_value_set_string (value, self->Private->Data->Name);
                break;

                case ARG_FETCHED:
                        g_value_set_boolean (value, self->Private->Fetched);
                break;

                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (G_OBJECT (self), propid, pspec);
                break;
        }
}
