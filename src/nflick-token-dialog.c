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

#include "nflick-token-dialog.h"
#include "nflick-token-dialog-private.h"

GType                           nflick_token_dialog_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickTokenDialogClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_token_dialog_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickTokenDialog), 
                        4, 
                        (GInstanceInitFunc) nflick_token_dialog_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_DIALOG, "NFlickTokenDialog",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static gboolean                 private_init (NFlickTokenDialog *self, NFlickTokenDialogPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_TOKEN_DIALOG (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        gtk_window_set_title (GTK_WINDOW (self), gettext ("Enter token"));
        gtk_container_set_border_width (GTK_CONTAINER (self), 16);
        gtk_dialog_set_has_separator (GTK_DIALOG (self), FALSE);

        /* Entry */
        private->TokenEntry = GTK_ENTRY (gtk_entry_new ());
        g_return_val_if_fail (private->TokenEntry != NULL, FALSE);

        /* Caption */
        HildonCaption *caption = HILDON_CAPTION (hildon_caption_new (NULL, "Mini token", private->TokenEntry, NULL, HILDON_CAPTION_MANDATORY));
        
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (self)->vbox), GTK_WIDGET (caption));

        gtk_dialog_add_buttons (GTK_DIALOG (self), GTK_STOCK_OK, GTK_RESPONSE_OK, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, NULL);

        return TRUE;
}

static void                     nflick_token_dialog_dispose (NFlickTokenDialog *self)
{
        g_return_if_fail (NFLICK_IS_TOKEN_DIALOG (self));

        if (self->Private != NULL) {
                private_dispose (self->Private);
        }

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_token_dialog_finalize (NFlickTokenDialog *self)
{
        g_return_if_fail (NFLICK_IS_TOKEN_DIALOG (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

static void                     private_dispose (NFlickTokenDialogPrivate *private)
{
        g_return_if_fail (private != NULL);
}

static void                     nflick_token_dialog_class_init (NFlickTokenDialogClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_token_dialog_dispose;
        gobjectclass->finalize = (gpointer) nflick_token_dialog_finalize;
        gobjectclass->get_property = (gpointer) nflick_token_dialog_get_property;

        g_object_class_install_property (gobjectclass, ARG_MINI_TOKEN,
                                         g_param_spec_string
                                         ("minitoken", "MiniToken", "User-entered mini token",
                                         NULL, G_PARAM_READABLE));

        ParentClass = g_type_class_ref (GTK_TYPE_DIALOG);
}

static void                     nflick_token_dialog_init (NFlickTokenDialog *self)
{
        g_return_if_fail (NFLICK_IS_TOKEN_DIALOG (self));

        self->Private = NULL;

        NFlickTokenDialogPrivate *priv = g_new0 (NFlickTokenDialogPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

NFlickTokenDialog*               nflick_token_dialog_new (GtkWindow *parent)
{
        NFlickTokenDialog *self = g_object_new (NFLICK_TYPE_TOKEN_DIALOG, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        /* Set transiency if needed */
        if (parent != NULL)
                gtk_window_set_transient_for (GTK_WINDOW (self), parent);
 
        return self;
}

static void                     nflick_token_dialog_get_property (NFlickTokenDialog *self, guint propid, 
                                                                  GValue *value, GParamSpec *pspec)
{
        g_return_if_fail (NFLICK_IS_TOKEN_DIALOG (self));
        g_assert (self->Private != NULL);
                
        switch (propid) {
                
                case ARG_MINI_TOKEN:
                        g_value_set_string (value, gtk_entry_get_text (self->Private->TokenEntry));
                break;
        
                default:
                        G_OBJECT_WARN_INVALID_PROPERTY_ID (G_OBJECT (self), propid, pspec);
                break;
        }
}
