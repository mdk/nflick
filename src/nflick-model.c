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

#include "nflick-model.h"
#include "nflick-model-private.h"

NFlickModel*                    nflick_model_get (void)
{
        check_if_initialized ();
        return ModelGlobal;
}

static void                     check_if_initialized (void)
{
        if (ModelGlobal == NULL) {
                g_warning ("Model was not initialized!");
                exit (128);
        }
}

gboolean                         nflick_model_init (int *argc, char ***argv)
{
        if (ModelGlobal != NULL) {
                g_warning ("Model was already initialized!");
                return FALSE; 
        }

        /* Inits */
        LIBXML_TEST_VERSION;
        g_thread_init (NULL);
        gtk_init (argc, argv);
        ne_sock_init ();

        ModelGlobal = g_new0 (NFlickModel, 1);
        g_return_val_if_fail (ModelGlobal != NULL, FALSE);

        ModelGlobal->Program = HILDON_PROGRAM (hildon_program_get_instance ());
        g_return_val_if_fail (ModelGlobal->Program != NULL, FALSE);

        g_set_application_name ("NFlick");

        ModelGlobal->GConf = gconf_client_get_default ();
        g_return_val_if_fail (ModelGlobal->GConf != NULL, FALSE);
        
        nflick_model_pop_from_gconf ();
        if (nflick_model_settings_are_valid () == FALSE)
                nflick_model_settings_reset ();

        ModelGlobal->FlickrPixbuf = gdk_pixbuf_new_from_file (NFLICK_LOGO_PNG, NULL);
        g_return_val_if_fail (ModelGlobal->FlickrPixbuf != NULL, FALSE);

        ModelGlobal->LoadingPixbuf = gdk_pixbuf_new_from_file (NFLICK_STUB_PNG, NULL);
        g_return_val_if_fail (ModelGlobal->LoadingPixbuf != NULL, FALSE);

        ModelGlobal->Window = nflick_window_new ();
        g_return_val_if_fail (ModelGlobal->Window != NULL, FALSE);

        hildon_program_add_window (ModelGlobal->Program, HILDON_WINDOW (ModelGlobal->Window));

        g_return_val_if_fail (nflick_processor_init (), FALSE);

        // Prepare the cache dir
        ModelGlobal->CacheDir = g_build_filename (g_get_user_cache_dir (), "nflick", NULL);
        g_return_val_if_fail (ModelGlobal->CacheDir != NULL, FALSE);
        g_return_val_if_fail (g_mkdir_with_parents (ModelGlobal->CacheDir, 493) == 0, FALSE);

        return TRUE;
}

void                            nflick_model_destroy (void)
{
        if (ModelGlobal == NULL)
                return;

        nflick_processor_kill ();

        if (ModelGlobal->CacheDir != NULL) {
                g_free (ModelGlobal->CacheDir);
                ModelGlobal->CacheDir = NULL;
        }

        if (ModelGlobal->Window != NULL) {
                gtk_widget_destroy (GTK_WIDGET (ModelGlobal->Window));
                ModelGlobal->Window = NULL;
        }

        if (ModelGlobal->FlickrPixbuf != NULL) {
                g_object_unref (ModelGlobal->FlickrPixbuf);
                ModelGlobal->FlickrPixbuf = NULL;
        }

        if (ModelGlobal->LoadingPixbuf != NULL) {
                g_object_unref (ModelGlobal->LoadingPixbuf);
                ModelGlobal->LoadingPixbuf = NULL;
        }

        if (ModelGlobal->GConf != NULL) {
                g_object_unref (ModelGlobal->GConf);
                ModelGlobal->GConf = NULL;
        }

        if (ModelGlobal->UserName != NULL) {
                g_free (ModelGlobal->UserName);
                ModelGlobal->UserName = NULL;
        }

        if (ModelGlobal->FullName != NULL) {
                g_free (ModelGlobal->FullName);
                ModelGlobal->FullName = NULL;
        }

        if (ModelGlobal->Token != NULL) {
                g_free (ModelGlobal->Token);
                ModelGlobal->Token = NULL;
        }

        if (ModelGlobal->UserNsid != NULL) {
                g_free (ModelGlobal->UserNsid);
                ModelGlobal->UserNsid = NULL;
        }

        /* Cleanup some global possible things */
        xmlCleanupParser();

        g_free (ModelGlobal);
        ModelGlobal = NULL;
}

void                            nflick_model_settings_set (const gchar *username, const gchar *fullname, 
                                                           const gchar *token, const gchar  *usernsid)
{
        check_if_initialized ();

        nflick_model_settings_reset ();

        ModelGlobal->UserName = g_strdup (username);
        ModelGlobal->FullName = g_strdup (fullname);
        ModelGlobal->Token = g_strdup (token);
        ModelGlobal->UserNsid = g_strdup (usernsid);
}

void                            nflick_model_hide_photo (void)
{
        check_if_initialized ();

        nflick_model_unfullscreen ();
        nflick_window_pop_vbox (NFLICK_MODEL_WINDOW);
        nflick_window_resume_fetching (NFLICK_MODEL_WINDOW);
}

void                            nflick_model_show_photo (const gchar *id, GList *list)
{
        check_if_initialized ();

        /* Stop all current work */
        nflick_processor_cancel_all ();

        /* Now let's see what kind of sizing we need... */
        gint32 vbox_width = -1;
        gint32 vbox_height = -1;
        nflick_window_probe_vbox_size (ModelGlobal->Window, &vbox_width, &vbox_height);
        g_return_if_fail (vbox_width != -1 && vbox_height != -1);

        NFlickShowWorker *worker = NULL;
        NFlickWaitDialog *dialog = NULL; 
        GdkPixbuf *pixbuf = NULL;

        // FIXME: That should be detected from window size!
        worker = nflick_show_worker_new (id, 800, 480, ModelGlobal->Token);
        if (worker == NULL)
                goto Done;

        dialog = nflick_wait_dialog_new ((NFlickWorker *) worker, GTK_WINDOW (ModelGlobal->Window));
        if (dialog == NULL)
                goto Done;

        int res = nflick_wait_dialog_show_and_run (dialog);
        gtk_widget_destroy (GTK_WIDGET (dialog));
        dialog = NULL;

        if (res == NFLICK_WAIT_DIALOG_RESPONSE_OK) {
                g_object_get (G_OBJECT (worker), "pixbuf", &pixbuf, NULL);
                if (pixbuf == NULL)
                        goto Done;

                GtkVBox *vbox = GTK_VBOX (nflick_show_vbox_new (pixbuf, list));
                if (vbox == NULL)
                        goto Done;

                nflick_window_push_vbox (NFLICK_MODEL_WINDOW, vbox);
                nflick_model_fullscreen ();
                g_object_unref (pixbuf);
        }

Done:
        if (worker != NULL)
                g_object_unref (worker);

        if (pixbuf != NULL)
                g_return_if_fail (pixbuf);
}

void                            nflick_model_switch_photo (const gchar *id, GList *list)
{
        check_if_initialized ();

        /* Stop all current work */
        nflick_processor_cancel_all ();

        /* Now let's see what kind of sizing we need... */
        gint32 vbox_width = -1;
        gint32 vbox_height = -1;
        nflick_window_probe_vbox_size (ModelGlobal->Window, &vbox_width, &vbox_height);
        g_return_if_fail (vbox_width != -1 && vbox_height != -1);

        NFlickShowWorker *worker = NULL;
        NFlickWaitDialog *dialog = NULL; 
        GdkPixbuf *pixbuf = NULL;

        // FIXME: That should be detected from window size!
        worker = nflick_show_worker_new (id, 800, 480, ModelGlobal->Token);
        if (worker == NULL)
                goto Done;

        dialog = nflick_wait_dialog_new ((NFlickWorker *) worker, GTK_WINDOW (ModelGlobal->Window));
        if (dialog == NULL)
                goto Done;

        int res = nflick_wait_dialog_show_and_run (dialog);
        gtk_widget_destroy (GTK_WIDGET (dialog));
        dialog = NULL;

        if (res == NFLICK_WAIT_DIALOG_RESPONSE_OK) {
                g_object_get (G_OBJECT (worker), "pixbuf", &pixbuf, NULL);
                if (pixbuf == NULL)
                        goto Done;

                GtkVBox *vbox = GTK_VBOX (nflick_show_vbox_new (pixbuf, list));
                if (vbox == NULL)
                        goto Done;

                nflick_window_change_to_vbox (NFLICK_MODEL_WINDOW, vbox);
                g_object_unref (pixbuf);
        }

Done:
        if (worker != NULL)
                g_object_unref (worker);

        if (pixbuf != NULL)
                g_return_if_fail (pixbuf);
}

void                            nflick_model_settings_reset (void)
{
        check_if_initialized ();

        if (ModelGlobal->UserName != NULL) {
                g_free (ModelGlobal->UserName);
                ModelGlobal->UserName = NULL;
        }

        if (ModelGlobal->FullName != NULL) {
                g_free (ModelGlobal->FullName);
                ModelGlobal->FullName = NULL;
        }

        if (ModelGlobal->Token != NULL) {
                g_free (ModelGlobal->Token);
                ModelGlobal->Token = NULL;
        }

        if (ModelGlobal->UserNsid != NULL) {
                g_free (ModelGlobal->UserNsid);
                ModelGlobal->UserNsid = NULL;
        }

        ModelGlobal->UseCache = TRUE;
}

void                            nflick_model_pop_from_gconf (void)
{
        check_if_initialized ();
        
        nflick_model_settings_reset ();

        GError *error = NULL;
        gchar *string_val = NULL;
        gchar bool_val = FALSE;

        /* Hmm, this is a bit primitive what follows... */
        string_val = gconf_client_get_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_USERNAME, &error);
        if (error == NULL)
                ModelGlobal->UserName = string_val;
        else {
                g_error_free (error);
                error = NULL;
        }

        string_val = gconf_client_get_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_FULLNAME, &error);
        if (error == NULL)
                ModelGlobal->FullName = string_val;
        else {
                g_error_free (error);
                error = NULL;
        }

        string_val = gconf_client_get_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_USERNSID, &error);
        if (error == NULL)
                ModelGlobal->UserNsid = string_val;
        else {
                g_error_free (error);
                error = NULL;
        }

        string_val = gconf_client_get_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_TOKEN, &error);
        if (error == NULL)
                ModelGlobal->Token = string_val;
        else {
                g_error_free (error);
                error = NULL;
        }

        bool_val = gconf_client_get_bool (ModelGlobal->GConf, NFLICK_GCONF_PATH_USECACHE, &error);
        if (error == NULL)
                ModelGlobal->UseCache = bool_val;
        else {
                g_error_free (error);
                error = NULL;
                ModelGlobal->UseCache = TRUE;
        }
}

void                            nflick_model_push_to_gconf (void)
{
        check_if_initialized ();

        gconf_client_set_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_USERNAME, ModelGlobal->UserName, NULL);
        gconf_client_set_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_FULLNAME, ModelGlobal->FullName, NULL);
        gconf_client_set_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_USERNSID, ModelGlobal->UserNsid, NULL);
        gconf_client_set_string (ModelGlobal->GConf, NFLICK_GCONF_PATH_TOKEN, ModelGlobal->Token, NULL);
        gconf_client_set_bool (ModelGlobal->GConf, NFLICK_GCONF_PATH_USECACHE, ModelGlobal->UseCache, NULL);
}

void                            nflick_model_reset_gconf (void)
{
        gconf_client_unset (ModelGlobal->GConf, NFLICK_GCONF_PATH_USERNAME, NULL);
        gconf_client_unset (ModelGlobal->GConf, NFLICK_GCONF_PATH_FULLNAME, NULL);
        gconf_client_unset (ModelGlobal->GConf, NFLICK_GCONF_PATH_USERNSID, NULL);
        gconf_client_unset (ModelGlobal->GConf, NFLICK_GCONF_PATH_TOKEN, NULL);
        gconf_client_unset (ModelGlobal->GConf, NFLICK_GCONF_PATH_USECACHE, NULL);
}

void                            nflick_model_fullscreen (void)
{
        check_if_initialized ();

        gtk_window_fullscreen (GTK_WINDOW (ModelGlobal->Window));
        gtk_container_set_border_width (GTK_CONTAINER (ModelGlobal->Window), 0);
}

void                            nflick_model_unfullscreen (void)
{
        check_if_initialized ();

        gtk_window_unfullscreen (GTK_WINDOW (ModelGlobal->Window));
        gtk_container_set_border_width (GTK_CONTAINER (ModelGlobal->Window), 16);
}

gboolean                        nflick_model_settings_are_valid (void)
{
        check_if_initialized ();

        if (ModelGlobal->UserName != NULL &&
            ModelGlobal->FullName != NULL &&
            ModelGlobal->UserNsid != NULL &&
            ModelGlobal->Token != NULL)
                return TRUE;
        else
                return FALSE;
}

void                            nflick_model_enable_cache (gboolean val)
{
        check_if_initialized ();
        ModelGlobal->UseCache = val;
        gconf_client_set_bool (ModelGlobal->GConf, NFLICK_GCONF_PATH_USECACHE, val, NULL);
}

void                            nflick_model_clear_cache (void)
{
        GDir *cache_dir = g_dir_open (NFLICK_MODEL_CACHE_DIR, 0, NULL);
        if (cache_dir == NULL)
                goto Done;

        const gchar *file_name = NULL;

        /* FIXME: We should first copy all the file names to a list... */
        while ((file_name = g_dir_read_name (cache_dir)) != NULL) {
                gchar *path = g_build_filename (NFLICK_MODEL_CACHE_DIR, file_name, NULL);
                if (path != NULL) {
                        g_remove (path);
                        g_free (path);
                }
        }

Done:
        if (cache_dir != NULL)
                g_dir_close (cache_dir);
}

