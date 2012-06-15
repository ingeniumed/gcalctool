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

#ifndef _MATH_HISTORY_H_
#define _MATH_HISTORY_H_

#include <glib-object.h>
#include <gtk/gtk.h>

#include "math-equation.h"

G_BEGIN_DECLS

#define MATH_TYPE_HISTORY             (math_history_get_type ())
#define MATH_HISTORY(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), MATH_TYPE_HISTORY, MathHistory))
#define MATH_HISTORY_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), MATH_TYPE_HISTORY, MathHistoryClass))
#define MATH_IS_HISTORY(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MATH_TYPE_HISTORY))
#define MATH_IS_HISTORY_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), MATH_TYPE_HISTORY))
#define MATH_HISTORY_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), MATH_TYPE_HISTORY, MathHistoryClass))

typedef struct MathHistoryPrivate MathHistoryPrivate;

typedef struct
{
    GtkWidget parent_instance;
    MathHistoryPrivate *priv;
} MathHistory;

typedef struct
{
    GtkWidgetClass parent_class;
} MathHistoryClass;

GType math_history_get_type (void);

G_END_DECLS

#endif /* _MATH_HISTORY_H_ */
