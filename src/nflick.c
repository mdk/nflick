#include <libxml/parser.h>
#include <libxml/tree.h>
#include <gtk/gtk.h>
#include "nflick-window.h"
#include "nflick-welcome-vbox.h"
#include "nflick-api-response.h"
#include "nflick-gft-response.h"
#include "nflick-photos-vbox.h"
#include "nflick-model.h"

int                             main (int argc, char **argv)
{
        /* Poorman's way to get rid of the icons next to 
         * the buttons */
        gtk_rc_parse_string ("gtk-button-images = 0");
        gtk_rc_parse_string ("gtk-menu-images = 0");
        
        g_return_val_if_fail (nflick_model_init (&argc, &argv), 128);
     
        gtk_widget_show_all (GTK_WIDGET (NFLICK_MODEL_WINDOW));
        gtk_main();
        nflick_model_destroy ();
        
        return 0;
}

