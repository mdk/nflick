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

struct                          _NFlickProcessor 
{
        GThread *Thread;
        GMutex *Mutex;
        GMutex *IdleMutex;
        GCond *Cond;
        gboolean Kill;
        GQueue *Queue;
        GList *IdleList;
        gint32 IdCounter;
} typedef NFlickProcessor;

struct                          _NFlickProcessorJob
{
        gint32 Id;
        gchar *PhotoId;
        gpointer Custom;
        gint32 Width;
        gint32 Height;
        gpointer IdleFunc;
} typedef NFlickProcessorJob;


NFlickProcessor*                ProcessorGlobal;

static void                     check_if_initialized (void);

static void                     thread_func (NFlickProcessor *processor);

static NFlickProcessorJob*      new_job (gchar *id, gpointer Custom, gint32 Width, gint32 Height, gpointer IdleFunc);

static NFlickProcessorJob*      copy_job (const NFlickProcessorJob *job);

static void                     free_job (NFlickProcessorJob *job);

static void                     free_result (NFlickProcessorResult *result);

static int                      find_job_by_id (const NFlickProcessorJob *a, const gint32 *id);

static gchar*                   get_photo_addr (const gchar *id, gint32 width, gint32 height);
