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

#include "nflick-processor.h"
#include "nflick-processor-private.h"

static void                     check_if_initialized (void)
{
        if (ProcessorGlobal == NULL) 
                g_error ("Processor not initialized!");
}

gboolean                        nflick_processor_init (void)
{
        NFlickProcessor *processor = g_new0 (NFlickProcessor, 1);
        g_return_val_if_fail (processor != NULL, FALSE);

        processor->Kill = FALSE;

        /* Create queue */
        processor->Queue = g_queue_new ();
        g_return_val_if_fail (processor->Queue != NULL, FALSE);

        /* List reset */
        processor->IdleList = NULL;

        /* Reset counter */
        processor->IdCounter = 1;

        /* Create mutex */
        processor->Mutex = g_mutex_new ();
        g_return_val_if_fail (processor->Mutex != NULL, FALSE);

        /* Create signalling condition */
        processor->Cond = g_cond_new ();
        g_return_val_if_fail (processor->Cond != NULL, FALSE);

        /* Lock the mutex to 'gate' the entry func */
        g_mutex_lock (processor->Mutex);

        /* Thread launches here */
        processor->Thread = g_thread_create ((GThreadFunc) thread_func, processor, TRUE, NULL);
        g_return_val_if_fail (processor->Thread != NULL, FALSE);

        ProcessorGlobal = processor;
        g_mutex_unlock (processor->Mutex);

        return TRUE;
}

static gchar*                   get_photo_addr (const gchar *id, gint32 width, gint32 height)
{
        NFlickApiRequest *get_sizes_request = NULL; 
        gchar *uri = NULL;
        NFlickApiResponse *get_sizes_response = NULL;

        get_sizes_request = nflick_api_request_new (NFLICK_FLICKR_API_METHOD_PHOTOS_GET_SIZES);
        g_return_val_if_fail (get_sizes_request != NULL, NULL);

        nflick_api_request_add_parameter (get_sizes_request, 
                                          NFLICK_FLICKR_API_PARAM_TOKEN, 
                                          NFLICK_MODEL_TOKEN);

        nflick_api_request_add_parameter (get_sizes_request, 
                                          NFLICK_FLICKR_API_PARAM_PHOTO_ID, 
                                          id);

        nflick_api_request_sign (get_sizes_request);
        if (nflick_api_request_exec (get_sizes_request) != TRUE)
                goto Done;

        get_sizes_response = nflick_api_response_new_from_request (NFLICK_TYPE_GET_SIZES_RESPONSE, get_sizes_request);
        if (get_sizes_response == NULL)
                goto Done;
        
        /* FIXME: Parse api response! */

        gboolean rotated = FALSE;
        uri = nflick_get_sizes_response_find_match ((NFlickGetSizesResponse *) get_sizes_response, &width, &height, &rotated);

        /* All ok */
        goto Done;

Done:
        if (get_sizes_request != NULL) 
                g_object_unref (get_sizes_request);

        if (get_sizes_response != NULL) 
                g_object_unref (get_sizes_response);

        return uri;

}

static void                     thread_func (NFlickProcessor *processor)
{
        g_return_if_fail (processor != NULL);

        /* A gate to prevent race condition during creation */
        g_mutex_lock (processor->Mutex);
        g_mutex_unlock (processor->Mutex);

        while (1) {
                
                NFlickProcessorJob *job = NULL;
                NFlickProcessorJob *job_copy = NULL;
                gchar *photo_addr = NULL;
                GdkPixbuf *pixbuf = NULL;
                gboolean error = FALSE;

                /* LOCK */
                g_mutex_lock (processor->Mutex);
               
                while (processor->Kill == FALSE && g_queue_is_empty (processor->Queue) == TRUE)
                        g_cond_wait (processor->Cond, processor->Mutex);

                /* Check if we need to bail out */
                if (processor->Kill == TRUE) {
                        g_mutex_unlock (processor->Mutex);
                        g_thread_exit (NULL);
                }

                /* Here we MUST have an item stacked */
                job = g_queue_peek_head (processor->Queue);
                g_assert (job != NULL);

                /* Copy the job */
                job_copy = copy_job (job);
                g_assert (job_copy != NULL);

                /* UNLOCK */
                g_mutex_unlock (processor->Mutex);

                /* Do the processing */
                gchar *cache_token = NULL;
                if (job_copy->Width == 75 && job_copy->Height == 75) {
                        cache_token = g_strdup_printf ("%s_%d_%d", job_copy->PhotoId, job_copy->Width, job_copy->Height);
                        pixbuf = nflick_pixbuf_fetch_try_cache (cache_token);
                }

                if (pixbuf == NULL) {

                        photo_addr = get_photo_addr (job_copy->PhotoId, job_copy->Width, job_copy->Height);
                        if (photo_addr == NULL) 
                                error = TRUE;
                        else 
                                pixbuf = nflick_pixbuf_fetch (photo_addr, job_copy->Width, job_copy->Height, cache_token);
                }

                if (cache_token != NULL)
                        g_free (cache_token);
                
                if (pixbuf == NULL)
                        error = TRUE;

                NFlickProcessorResult *result = g_new0 (NFlickProcessorResult, 1);
                g_assert (result != NULL);

                /* Common */
                result->Id = job_copy->Id;
                result->Custom = job_copy->Custom;

                if (error == FALSE) {
                        result->Pixbuf = pixbuf;
                        result->Valid = TRUE;
                        result->Error = FALSE;
                } else {
                        result->Pixbuf = NULL;
                        result->Valid = TRUE;
                        result->Error = TRUE;
                }

                /* Check if we still have it */
                g_mutex_lock (processor->Mutex);
                if (g_queue_find (processor->Queue, job) != NULL) {
                        g_queue_remove (processor->Queue, job);
                        processor->IdleList = g_list_append (processor->IdleList, result);
                        g_idle_add ((GSourceFunc) job_copy->IdleFunc, result);
                        free_job (job);
                        free_job (job_copy);
                } else {
                        free_job (job_copy);
                        free_result (result);
                }
                g_mutex_unlock (processor->Mutex);

                if (photo_addr != NULL)
                        g_free (photo_addr);
        }
}

static NFlickProcessorJob*      new_job (gchar *id, gpointer custom, gint32 width, gint32 height, gpointer func)
{
        NFlickProcessorJob *job = g_new0 (NFlickProcessorJob, 1);
        g_return_val_if_fail (job != NULL, NULL);

        job->PhotoId = id;
        job->Custom = custom;
        job->IdleFunc = func;
        job->Width = width;
        job->Height = height;
        
        return job;
}

static NFlickProcessorJob*      copy_job (const NFlickProcessorJob *job)
{
        NFlickProcessorJob *newjob = new_job (NULL, job->Custom, job->Width, job->Height, job->IdleFunc);
        newjob->Id = job->Id;
        if (job->PhotoId != NULL)
                newjob->PhotoId = g_strdup (job->PhotoId);

        return newjob;
}

static void                     free_job (NFlickProcessorJob *job)
{
        g_return_if_fail (job != NULL);

        if (job->PhotoId != NULL)
                g_free (job->PhotoId);

        g_free (job);
}

static void                     free_result (NFlickProcessorResult *result)
{
        g_return_if_fail (result != NULL);

        if (result->Pixbuf != NULL)
                g_object_unref (result->Pixbuf);

        g_free (result);
}

void                            nflick_processor_loose_result (NFlickProcessorResult *result)
{
        g_return_if_fail (result != NULL);

        check_if_initialized ();

        g_mutex_lock (ProcessorGlobal->Mutex);

        ProcessorGlobal->IdleList = g_list_remove (ProcessorGlobal->IdleList, result);
        free_result (result);

        g_mutex_unlock (ProcessorGlobal->Mutex);
}

void                            nflick_processor_kill (void)
{
        check_if_initialized ();
        nflick_processor_cancel_all ();
        
        g_mutex_lock (ProcessorGlobal->Mutex);
        ProcessorGlobal->Kill = TRUE;
        g_cond_broadcast (ProcessorGlobal->Cond);
        g_mutex_unlock (ProcessorGlobal->Mutex);

        g_thread_join (ProcessorGlobal->Thread);

        // Now we need to free all the results
        GList *iterator;
        
        for (iterator = ProcessorGlobal->IdleList; iterator; iterator = g_list_next (iterator)) {
                NFlickProcessorResult *result = (NFlickProcessorResult *) iterator->data;
                free_result (result);
        }

        g_list_free (ProcessorGlobal->IdleList);
        g_queue_free (ProcessorGlobal->Queue);
        g_cond_free (ProcessorGlobal->Cond);
        g_mutex_free (ProcessorGlobal->Mutex);
}

void                            nflick_processor_cancel_all (void)
{
        check_if_initialized ();

        g_mutex_lock (ProcessorGlobal->Mutex);

        while (g_queue_is_empty (ProcessorGlobal->Queue) == FALSE) {
                
                /* Remove the item from the queue and free it */
                NFlickProcessorJob *job = g_queue_pop_head (ProcessorGlobal->Queue);
                g_assert (job != NULL);
                free_job (job);
        }

        GList *iterator;
        
        for (iterator = ProcessorGlobal->IdleList; iterator; iterator = g_list_next (iterator)) {
                
                /* Mark as invalid */
                NFlickProcessorResult *result = (NFlickProcessorResult *) iterator->data;
                result->Valid = FALSE;
        }

        g_mutex_unlock (ProcessorGlobal->Mutex);
}

static int                      find_job_by_id (const NFlickProcessorJob *a, const gint32 *id)
{
        g_return_val_if_fail (a != NULL, -1);
        g_return_val_if_fail (id != NULL, -1);

        if (a->Id == *id)
                return 0;
        else
                return -1;
}

void                            nflick_processor_cancel_by_id (gint32 id)
{
        check_if_initialized ();

        g_mutex_lock (ProcessorGlobal->Mutex);

        NFlickProcessorJob *job = (NFlickProcessorJob *) g_queue_find_custom (ProcessorGlobal->Queue, 
                                                                              &id, 
                                                                              (GCompareFunc) find_job_by_id);

        if (job != NULL) {
                g_queue_remove (ProcessorGlobal->Queue, job);
                free_job (job);
        }

        GList *iterator;
        
        for (iterator = ProcessorGlobal->IdleList; iterator; iterator = g_list_next (iterator)) {
                
                /* Mark as invalid */
                NFlickProcessorResult *result = (NFlickProcessorResult *) iterator->data;
                if (result->Id == id)
                        result->Valid = FALSE;
        }

        g_mutex_unlock (ProcessorGlobal->Mutex);
}

gint32                          nflick_processor_add_job (const gchar *id, gpointer custom, gint32 width, gint32 height, gpointer func)
{
        g_return_val_if_fail (func != NULL, -1);
        g_return_val_if_fail (id != NULL, -1);

        check_if_initialized ();

        NFlickProcessorJob *job = new_job (g_strdup (id), custom, width, height, func);
        g_return_val_if_fail (job != NULL, -1);

        g_mutex_lock (ProcessorGlobal->Mutex);
        
        gint32 idd = ProcessorGlobal->IdCounter;
        job->Id = idd;
        g_queue_push_tail (ProcessorGlobal->Queue, job);
      
        ProcessorGlobal->IdCounter++;
        g_cond_broadcast (ProcessorGlobal->Cond);
        g_mutex_unlock (ProcessorGlobal->Mutex);

        return idd;
}
