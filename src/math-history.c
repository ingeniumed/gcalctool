/*
 * Copyright (C) 1987-2008 Sun Microsystems, Inc. All Rights Reserved.
 * Copyright (C) 2008-2011 Robert Ancell
 * Copyright (C) 2012      Gopal Krishnan
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "math-history.h"
#include "mp-serializer.h"
#include "mp-equation.h"

enum
{
  LIST_ITEM = 0,
  N_COLUMNS
};

void
math_history_append(GtkWidget *list, MathEquation *equation, gboolean append_flag)
{
  	MpSerializer *result_serializer;
    int ret;
    char *equation_text;
    GtkListStore *store;
  	GtkTreeIter iter;
    MPEquationOptions options;
    /* Using the solve method in command line gcalctool to get the answer */ 
    result_serializer = mp_serializer_new(MP_DISPLAY_FORMAT_AUTOMATIC, 10, 9);
    equation_text = math_equation_get_equation(equation);
    MPNumber z;
    gchar *result_str = NULL;
    memset(&options, 0, sizeof(options));
    options.base = 10;
    options.wordlen = 32;
    options.angle_units = MP_DEGREES;
    ret = mp_equation_parse(equation_text, &options, &z, NULL);
    /* ignore errors */
    if (ret == PARSER_ERR_MP)
    {
        return;
    }
    else if (ret)        
    {
        return;
    }
    else 
    {	
	    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));
	    if (append_flag)
	    {
		    /* get the equation */
		    gtk_list_store_append(store, &iter);
  		    gtk_list_store_set(store, &iter, LIST_ITEM, math_equation_get_equation(equation), -1);
		    g_free(result_str);
		}
		else if (!append_flag)
		{
		    /* get the answer */ 
		    result_str = mp_serializer_to_string(result_serializer, &z);
		    gtk_list_store_append(store, &iter);
		    gtk_list_store_set(store, &iter, LIST_ITEM, result_str, -1);
		    g_free(result_str);
	    }
    }
}
