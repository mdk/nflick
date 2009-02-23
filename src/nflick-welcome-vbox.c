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

#include "nflick-welcome-vbox.h"
#include "nflick-welcome-vbox-private.h"

GType                           nflick_welcome_vbox_get_type (void)
{
        static GType objecttype = 0;

        if (!objecttype) {

                static const GTypeInfo objectinfo = {
                        sizeof (NFlickWelcomeVBoxClass), 
                        NULL, 
                        NULL,
                        (GClassInitFunc) nflick_welcome_vbox_class_init,
                        NULL,
                        NULL, 
                        sizeof (NFlickWelcomeVBox), 
                        4, 
                        (GInstanceInitFunc) nflick_welcome_vbox_init,
                };
                objecttype = g_type_register_static (GTK_TYPE_VBOX, "NFlickWelcomeVBox",
                                                     &objectinfo, 0);
        }
        return objecttype;
}

static void                     nflick_welcome_vbox_class_init (NFlickWelcomeVBoxClass *klass)
{
        GObjectClass *gobjectclass = (GObjectClass *) klass;

        gobjectclass->dispose = (gpointer) nflick_welcome_vbox_dispose;
        gobjectclass->finalize = (gpointer) nflick_welcome_vbox_finalize;

        ParentClass = g_type_class_ref (GTK_TYPE_VBOX);
}

static void                     nflick_welcome_vbox_init (NFlickWelcomeVBox *self)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));

        self->Private = NULL;

        NFlickWelcomeVBoxPrivate *priv = g_new0 (NFlickWelcomeVBoxPrivate, 1);
        g_return_if_fail (priv != NULL);
        
        if (private_init (self, priv) == TRUE) {
                self->Private = priv;
                activate_from_gconf (self);
        } else {
                private_dispose (priv);
                g_free (priv);
                self->Private = NULL;
        }
}

static void			on_link_button_released (void)
{
	osso_uri_open ("http://www.flickr.com/auth-21684", NULL, NULL);
	return;
}

static gboolean                 private_init (NFlickWelcomeVBox *self, NFlickWelcomeVBoxPrivate *private)
{
        g_return_val_if_fail (NFLICK_IS_WELCOME_VBOX (self), FALSE);
        g_return_val_if_fail (private != NULL, FALSE);

        gtk_box_set_spacing (GTK_BOX (self), 16);

        /* Create the main upper hbox container */
        GtkHBox *upper_hbox = GTK_HBOX (gtk_hbox_new (FALSE, 16));
        g_return_val_if_fail (upper_hbox != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (self), GTK_WIDGET (upper_hbox));

        /* Flickr logo */
        GtkImage *flickr_logo = GTK_IMAGE (gtk_image_new_from_pixbuf (NFLICK_MODEL_FLICKR_LOGO));
        g_return_val_if_fail (flickr_logo != NULL, FALSE);
        gtk_box_pack_start (GTK_BOX (upper_hbox), GTK_WIDGET (flickr_logo), FALSE, FALSE, 0);

        /* Right vbox */
        GtkVBox *right_vbox = GTK_VBOX (gtk_vbox_new (FALSE, 2));
        g_return_val_if_fail (right_vbox != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (upper_hbox), GTK_WIDGET (right_vbox));

        /* Introductionary label markup */
        gchar *msg_markup = gettext ("Welcome to NFlick!\n\n" 
                                     "This application alows you to browse your online flickr photo collection. "
                                     "To obtain the authorization mini-token for your user account please visit: ");

        GtkLabel *intro_label = GTK_LABEL (gtk_label_new (NULL));
        g_return_val_if_fail (intro_label != NULL, FALSE);
        gtk_label_set_line_wrap (intro_label, TRUE);
        gtk_label_set_markup (intro_label, msg_markup);
        gtk_box_pack_start (GTK_BOX (right_vbox), GTK_WIDGET (intro_label), FALSE, TRUE, 0);

	/* The url label */
	GtkEventBox *url_box = GTK_EVENT_BOX (gtk_event_box_new ());
	g_return_val_if_fail (url_box != NULL, FALSE);
        g_signal_connect (G_OBJECT (url_box), "button-release-event", (gpointer) on_link_button_released, NULL);
        gtk_box_pack_start (GTK_BOX (right_vbox), GTK_WIDGET (url_box), TRUE, TRUE, 0);
	
	GtkLabel *url_label = GTK_LABEL (gtk_label_new (NULL));
	g_return_val_if_fail (url_label != NULL, FALSE);
	gtk_misc_set_alignment (GTK_MISC (url_label), 0.5, 0.1);
	gtk_label_set_markup (url_label, "<span underline=\"single\" foreground=\"blue\">http://www.flickr.com/auth-21684</span>");
	gtk_container_add (GTK_CONTAINER (url_box), GTK_WIDGET (url_label));
	
        /* The VBox containing the radio selections */
        GtkVBox *radio_vbox = GTK_VBOX (gtk_vbox_new (FALSE, 6));
        g_return_val_if_fail (radio_vbox != NULL, FALSE);
        gtk_box_pack_end (GTK_BOX (right_vbox), GTK_WIDGET (radio_vbox), FALSE, TRUE, 0);

        /* Label for previous user */
        private->PrevUserLabel = GTK_LABEL (gtk_label_new (gettext ("Log in as previous user")));
        g_return_val_if_fail (private->PrevUserLabel != NULL, FALSE);
        
        /* Previous user radio button */
        private->PrevUserRadioButton = GTK_RADIO_BUTTON (gtk_radio_button_new (NULL));
        g_return_val_if_fail (private->PrevUserRadioButton != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (private->PrevUserRadioButton), GTK_WIDGET (private->PrevUserLabel));
        gtk_container_add (GTK_CONTAINER (radio_vbox), GTK_WIDGET (private->PrevUserRadioButton));

        /* From mini token label */
        GtkLabel *mini_token_label = GTK_LABEL (gtk_label_new (gettext ("Authorize from new mini-token")));
        g_return_val_if_fail (mini_token_label != NULL, FALSE);

        /* From mini token radio button */
        private->MiniTokenRadioButton = GTK_RADIO_BUTTON (gtk_radio_button_new_from_widget (private->PrevUserRadioButton));
        g_return_val_if_fail (private->MiniTokenRadioButton != NULL, FALSE);
        gtk_container_add (GTK_CONTAINER (private->MiniTokenRadioButton), GTK_WIDGET (mini_token_label));
        gtk_container_add (GTK_CONTAINER (radio_vbox), GTK_WIDGET (private->MiniTokenRadioButton));

        /* Bottom box with the main buttons */
        GtkHButtonBox *bottom_box= GTK_HBUTTON_BOX (gtk_hbutton_box_new ());
        g_return_val_if_fail (bottom_box != NULL, FALSE);
        /* FIXME Obsoleted */
        gtk_button_box_set_spacing (GTK_BUTTON_BOX (bottom_box), 6);
        gtk_box_pack_end (GTK_BOX (self), GTK_WIDGET (bottom_box), FALSE, TRUE, 0);

        /* Ok button */
        GtkButton *ok_button = GTK_BUTTON (gtk_button_new_from_stock (GTK_STOCK_OK));
        g_return_val_if_fail (ok_button != NULL, FALSE);
        g_signal_connect (G_OBJECT (ok_button), "clicked", G_CALLBACK (on_ok_clicked), self);
        gtk_container_add (GTK_CONTAINER (bottom_box), GTK_WIDGET (ok_button));

        return TRUE;
}

static void                     private_dispose (NFlickWelcomeVBoxPrivate *private)
{
        g_return_if_fail (private != NULL);
}

static void                     nflick_welcome_vbox_dispose (NFlickWelcomeVBox *self)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));

        if (self->Private != NULL)
                private_dispose (self->Private);

        G_OBJECT_CLASS (ParentClass)->dispose (G_OBJECT (self));
}

static void                     nflick_welcome_vbox_finalize (NFlickWelcomeVBox *self)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));
        
        if (self->Private != NULL) {
                g_free (self->Private);
                self->Private = NULL;
        }

        G_OBJECT_CLASS (ParentClass)->finalize (G_OBJECT (self));
}

NFlickWelcomeVBox*              nflick_welcome_vbox_new (void)
{
        NFlickWelcomeVBox *self = g_object_new (NFLICK_TYPE_WELCOME_VBOX, NULL);
        g_return_val_if_fail (self != NULL, NULL);

        if (self->Private == NULL) {
                g_object_unref (self);
                return NULL;
        }
        
        return self;
}

static gchar*                   ask_user_for_mini_token (NFlickWelcomeVBox *self)
{
        g_return_val_if_fail (NFLICK_IS_WELCOME_VBOX (self), NULL);

        NFlickTokenDialog *dialog = nflick_token_dialog_new (GTK_WINDOW (NFLICK_MODEL_WINDOW));
        g_return_val_if_fail (dialog != NULL, NULL);

        gtk_widget_show_all (GTK_WIDGET (dialog));
        int res = gtk_dialog_run (GTK_DIALOG (dialog));

        gchar *mini_token = NULL;
        if (res == GTK_RESPONSE_OK) {
                g_object_get (G_OBJECT (dialog), "minitoken", &mini_token, NULL);

                if (mini_token == NULL || strlen (mini_token) == 0) {
                        GtkDialog *warning_dialog = GTK_DIALOG (hildon_note_new_information (GTK_WINDOW (NFLICK_MODEL_WINDOW), 
                                                                                             gettext ("You need to provide a nine-digit mini token. "
                                                                                                      "You can obtain it at www.flickr.com/auth-21684")));
                        gtk_widget_hide_all (GTK_WIDGET (dialog));
                        gtk_widget_show_all (GTK_WIDGET (warning_dialog));
                        gtk_dialog_run (GTK_DIALOG (warning_dialog));
                        gtk_widget_destroy (GTK_WIDGET (warning_dialog));

                        if (mini_token != NULL)
                                g_free (mini_token);

                        mini_token = NULL;
                }
        }

        gtk_widget_destroy (GTK_WIDGET (dialog));
       
        return mini_token;
}

static gboolean                 authorize_mini_token (NFlickWelcomeVBox *self, const gchar *minitoken)
{
        g_return_val_if_fail (NFLICK_IS_WELCOME_VBOX (self), FALSE);

        gboolean result = TRUE;
        gchar *username = NULL;
        gchar *fullname = NULL;
        gchar *token = NULL;
        gchar *usernsid = NULL;
        
        NFlickAuthWorker *auth_worker = nflick_auth_worker_new (minitoken);
        if (auth_worker == NULL) {
                result = FALSE;
                goto Done;
        }
                
        NFlickWaitDialog *dialog = nflick_wait_dialog_new ((NFlickWorker *) auth_worker, 
                                                           GTK_WINDOW (NFLICK_MODEL_WINDOW));
        if (dialog == NULL) {
                result = FALSE;
                goto Done;
        }

        int res = nflick_wait_dialog_show_and_run (dialog);

        if (res == NFLICK_WAIT_DIALOG_RESPONSE_OK) {
                g_object_get (G_OBJECT (auth_worker), 
                              "username", &username, 
                              "fullname", &fullname, 
                              "token", &token, 
                              "usernsid", &usernsid, 
                              NULL);

                if (username == NULL ||
                    fullname == NULL ||
                    token == NULL ||
                    usernsid == NULL) {
                        result = FALSE;
                        goto Done;
                } else { 
                        nflick_model_settings_set (username, fullname, token, usernsid);
                        nflick_model_push_to_gconf ();

                        GtkDialog *confirmed_dialog = GTK_DIALOG (hildon_note_new_information (GTK_WINDOW (NFLICK_MODEL_WINDOW), 
                                                                                               gettext ("The token has been succesfully authorized. "
                                                                                                        "Please note that authorization tokens expire when not "
                                                                                                        " used actively. "
                                                                                                        "When that happens just obtain another mini-token. ")));
                        gtk_widget_hide_all (GTK_WIDGET (dialog));
                        gtk_widget_show_all (GTK_WIDGET (confirmed_dialog));
                        gtk_dialog_run (GTK_DIALOG (confirmed_dialog));
                        gtk_widget_destroy (GTK_WIDGET (confirmed_dialog));
                }
                
                result = TRUE;
        } else 
                result = FALSE;

Done:
        if (auth_worker != NULL)
                g_object_unref (auth_worker);

        if (dialog != NULL)
                gtk_widget_destroy (GTK_WIDGET (dialog));

        if (username != NULL)
                g_free (username);

        if (fullname != NULL)
                g_free (fullname);

        if (usernsid != NULL)
                g_free (usernsid);

        if (token != NULL)
                g_free (token);

        return result;
}

static void                     on_ok_clicked (GtkWidget *widget, NFlickWelcomeVBox *self)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));


        if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (self->Private->MiniTokenRadioButton)) == TRUE) {
                gchar *mini_token = ask_user_for_mini_token (self);
                        
                if (mini_token == NULL)
                        return;
                else {
                        gboolean cont = authorize_mini_token (self, mini_token);
                        g_free (mini_token);

                        if (cont == FALSE)
                                return;
                        else
                                activate_from_gconf (self);
                }
        }

        NFlickSetListWorker *worker = nflick_set_list_worker_new (NFLICK_MODEL_USERNSID, NFLICK_MODEL_TOKEN);
        NFlickWaitDialog *dialog = nflick_wait_dialog_new ((NFlickWorker *) worker, 
                                                           GTK_WINDOW (NFLICK_MODEL_WINDOW));
        int result = nflick_wait_dialog_show_and_run (dialog);
        gtk_widget_destroy (GTK_WIDGET (dialog));

        if (result == NFLICK_WAIT_DIALOG_RESPONSE_OK) {
                GList *list = nflick_set_list_worker_take_list ((NFlickSetListWorker *) worker);
                nflick_window_change_to_vbox (NFLICK_MODEL_WINDOW, GTK_VBOX (nflick_photos_vbox_new (list)));
        }

        g_object_unref (worker);
}

static void                     activate_from_gconf (NFlickWelcomeVBox *self)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));

        if (nflick_model_settings_are_valid () == TRUE)
                activate_prev_user (self, NFLICK_MODEL_FULLNAME);
        else
                activate_mini_token (self);
}

static void                     activate_prev_user (NFlickWelcomeVBox *self, const gchar *user)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));
        
        gchar *prepared = NULL;

        if (user != NULL && *user != 0)
                prepared = g_strdup_printf (gettext ("Log in as %s"), user);
        else
                prepared = g_strdup (gettext ("Log in"));

        g_return_if_fail (prepared != NULL);

        gtk_label_set_text (GTK_LABEL (self->Private->PrevUserLabel), prepared);
        gtk_widget_set_sensitive (GTK_WIDGET (self->Private->MiniTokenRadioButton), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET (self->Private->PrevUserRadioButton), TRUE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (self->Private->PrevUserRadioButton), TRUE);

        gtk_widget_show_all (GTK_WIDGET (self));
        g_free (prepared);
}

static void                     activate_mini_token (NFlickWelcomeVBox *self)
{
        g_return_if_fail (NFLICK_IS_WELCOME_VBOX (self));
        
        gtk_label_set_text (GTK_LABEL (self->Private->PrevUserLabel), gettext ("Log in"));
        gtk_widget_set_sensitive (GTK_WIDGET (self->Private->MiniTokenRadioButton), TRUE);
        gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (self->Private->MiniTokenRadioButton), TRUE);
        gtk_widget_set_sensitive (GTK_WIDGET (self->Private->PrevUserRadioButton), FALSE);

        gtk_widget_show_all (GTK_WIDGET (self));
}
