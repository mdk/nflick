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

static GtkDialogClass*          ParentClass = NULL;

struct                          _NFlickTokenDialogPrivate
{
        GtkEntry *TokenEntry;
};

enum
{
        ARG_0,
        ARG_MINI_TOKEN
};

static void                     nflick_token_dialog_class_init (NFlickTokenDialogClass *klass);

static void                     nflick_token_dialog_init (NFlickTokenDialog *self);

static gboolean                 private_init (NFlickTokenDialog *self, NFlickTokenDialogPrivate *private);

static void                     private_dispose (NFlickTokenDialogPrivate *private);

static void                     nflick_token_dialog_dispose (NFlickTokenDialog *self);

static void                     nflick_token_dialog_finalize (NFlickTokenDialog *self);

static void                     nflick_token_dialog_get_property (NFlickTokenDialog *self, guint propid, 
                                                                  GValue *value, GParamSpec *pspec);


