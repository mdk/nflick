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

#include "nflick-set-combo.h"
#include "nflick-set-combo-private.h"

GType                           nflick_set_combo_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickSetComboClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_set_combo_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickSetCombo), 
                        4, 
                        (GInstanceInitFunc) nflick_set_combo_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_COMBO_BOX, "NFlickSetCombo",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_set_combo_class_init (NFlickSetComboClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_set_combo_dispose;
        gobjectclass->finalize = (gpointer) nflick_set_combo_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_COMBO_BOX);
}

static void                     nflick_set_combo_init (NFlickSetCombo *self)
{
        g_return_if_fail (NFLICK_IS_SET_COMBO (self));

        self->Private = NULL;

        NFlickSetComboPrivate *priv = g_new0 (NFlickSetComboPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) 
                self->Private = priv;
        else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static gboolean                 private_init (NFlickSetCombo *self, NFlickSetComboPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_SET_COMBO (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        private->Store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_OBJECT);
        g_return_val_if_fail (private->Store != NULL, FALSE);

        gtk_combo_box_set_model (GTK_COMBO_BOX (self), GTK_TREE_MODEL (private->Store));
        GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
        g_return_val_if_fail (renderer != NULL, FALSE);

        gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (self), renderer, TRUE);
        gtk_cell_layout_add_attribute (GTK_CELL_LAYOUT (self), renderer, "text", 0);

        return TRUE;
}

static void                     add_photo_set (NFlickSetCombo *self, NFlickPhotoSet *set)
{
        g_return_if_fail (NFLICK_IS_SET_COMBO (self));
        g_return_if_fail (NFLICK_IS_PHOTO_SET (set));

        gchar *text = NULL; 
        g_object_get (G_OBJECT (set), "combotext", &text, NULL);
        g_return_if_fail (text != NULL);

        GtkTreeIter *iter = g_new0 (GtkTreeIter, 1);
        gtk_list_store_append (self->Private->Store, iter);

        gtk_list_store_set (self->Private->Store, iter, 0, text, 1, set, -1);

        g_free (iter);
        g_free (text);
}

static void                     private_dispose (NFlickSetComboPrivate *private)
{
        g_return_if_fail (private != NULL);

        if (private->Store != NULL) {
                g_object_unref (private->Store);
                private->Store = NULL;
        }
}

static void                     nflick_set_combo_dispose (NFlickSetCombo *self)
{
        g_return_if_fail (NFLICK_IS_SET_COMBO (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_set_combo_finalize (NFlickSetCombo *self)
{
        g_return_if_fail (NFLICK_IS_SET_COMBO (self));

        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickPhotoSet*                 nflick_set_combo_get_current_set (NFlickSetCombo *self)
{
        g_return_val_if_fail (NFLICK_IS_SET_COMBO (self), NULL);
        
        NFlickPhotoSet *set = NULL;
        
        GtkTreeIter *iter = g_new0 (GtkTreeIter, 1);
        g_return_val_if_fail (iter != NULL, NULL);

        if (gtk_combo_box_get_active_iter (GTK_COMBO_BOX (self), iter) == TRUE) 
                gtk_tree_model_get (GTK_TREE_MODEL (self->Private->Store), iter, 1, &set, -1);

        g_free (iter);
        return set;
}

NFlickSetCombo*                 nflick_set_combo_new (void)
{
        NFlickSetCombo *self = g_object_new (NFLICK_TYPE_SET_COMBO, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }
        
        return self;
}

void                            nflick_set_combo_load_list (NFlickSetCombo *self, GList *photosets)
{
        g_return_if_fail (NFLICK_IS_SET_COMBO (self));
        g_return_if_fail (photosets != NULL);

        GList *iterator;
        
        for (iterator = photosets; iterator; iterator = g_list_next (iterator))
                if (iterator->data != NULL)
                        add_photo_set (self, (NFlickPhotoSet *) iterator->data);

        /* Do a default selection */
        gtk_combo_box_set_active (GTK_COMBO_BOX (self), 0);
}
