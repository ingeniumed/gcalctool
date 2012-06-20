/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * history-panel-prototype
 * Copyright (C) Gopal Krishnan 2012 <>
 * 
history-panel-prototype is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * history-panel-prototype is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gi18n.h>

#include "math-history.h"
#include "math-buttons.h"

struct MathHistoryPrivate
{   
	/*the equation being displayed*/
	MathEquation *equation;

	/*the equals button pressed instead of enter key for solving*/
	MathButtons *equals_key;
	
	/*History Panel*/
    GtkWidget *history_panel;

	/*Keeps track of number of equations and answers added so far*/
	gint index_tracker;
};

G_DEFINE_TYPE (MathHistory, math_history, GTK_TYPE_WIDGET);

MathHistory *
math_history_new(MathEquation *equation)
{
	MathHistory *history = g_object_new(math_history_get_type(), NULL);

}
	
                 
static void
math_history_class_init (MathHistoryClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	g_type_class_add_private(klass, sizeof(MathHistoryPrivate));
	//something else to add here
	//object_class->finalize = math_history_finalize;
}

static void
math_history_init (MathHistory *history)
{
history->priv = G_TYPE_INSTANCE_GET_PRIVATE(history, math_history_get_type(), MathHistoryPrivate);
//initialize the gui here along with the interactions when the combo box is changed 
}


