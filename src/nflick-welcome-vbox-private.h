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

#define                         FLICKR_LOGO_FILE "flickrlogo.png"

static GtkVBoxClass*            ParentClass = NULL;

struct                          _NFlickWelcomeVBoxPrivate
{
        GtkRadioButton *PrevUserRadioButton;
        GtkLabel *PrevUserLabel;
        GtkRadioButton *MiniTokenRadioButton;
};

static void                     nflick_welcome_vbox_class_init (NFlickWelcomeVBoxClass *klass);

static void                     nflick_welcome_vbox_init (NFlickWelcomeVBox *self);

static gboolean                 private_init (NFlickWelcomeVBox *self, NFlickWelcomeVBoxPrivate *private);

static void                     private_dispose (NFlickWelcomeVBoxPrivate *private);

static void                     nflick_welcome_vbox_dispose (NFlickWelcomeVBox *self);

static void                     nflick_welcome_vbox_finalize (NFlickWelcomeVBox *self);

static void                     on_ok_clicked (GtkWidget *widget, NFlickWelcomeVBox *self);

static void                     activate_prev_user (NFlickWelcomeVBox *self, const gchar *user);

static void                     activate_mini_token (NFlickWelcomeVBox *self);

static void                     activate_from_gconf (NFlickWelcomeVBox *self);

static gboolean                 authorize_mini_token (NFlickWelcomeVBox *self, const gchar *token);

static gchar*                   ask_user_for_mini_token (NFlickWelcomeVBox *self);

static void			on_link_button_released (void);

