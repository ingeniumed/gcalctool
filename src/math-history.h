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

#ifndef _MATH_HISTORY_H_
#define _MATH_HISTORY_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#include "math-equation.h"

G_BEGIN_DECLS

void math_history_append(GtkWidget *list, MathEquation *equation, gboolean append_flag);

G_END_DECLS

#endif /* _MATH_HISTORY_H_ */
