/*
 * Copyright (C) 1987-2008 Sun Microsystems, Inc. All Rights Reserved.
 * Copyright (C) 2008-2011 Robert Ancell
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 2 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <glib/gi18n.h>

#include "math-window.h"
#include "mp-equation.h"
#include "unit-manager.h"

static GSettings *settings = NULL;

static MathWindow *window;

static void
version(const gchar *progname)
{
    /* NOTE: Is not translated so can be easily parsed */
    fprintf(stderr, "%1$s %2$s\n", progname, VERSION);
}


static int
do_convert(const MPNumber *x, const char *x_units, const char *z_units, MPNumber *z, void *data)
{
    return unit_manager_convert_by_symbol(unit_manager_get_default(), x, x_units, z_units, z);
}


static void
solve(const char *equation)
{
    MPEquationOptions options;
    MPErrorCode error;
    MPNumber result;
    char *result_str;

    memset(&options, 0, sizeof(options));
    options.base = 10;
    options.wordlen = 32;
    options.angle_units = MP_DEGREES;
    options.convert = do_convert;

    error = mp_equation_parse(equation, &options, &result, NULL);
    if(error == PARSER_ERR_MP) {
        fprintf(stderr, "Error: %s\n", mp_get_error());
        exit(1);
    }
    else if(error != 0) {
        fprintf(stderr, "Error: %s\n", mp_error_code_to_string(error));
        exit(1);
    }
    else {
        result_str = mp_serializer_to_string(mp_serializer_new(MP_DISPLAY_FORMAT_AUTOMATIC, 10, 9), &result);
        printf("%s\n", result_str);
        exit(0);
    }
}


static void
usage(const gchar *progname, gboolean show_application, gboolean show_gtk)
{
    fprintf(stderr,
            /* Description on how to use gcalctool displayed on command-line */
            _("Usage:\n"
              "  %s — Perform mathematical calculations"), progname);

    fprintf(stderr,
            "\n\n");

    fprintf(stderr,
            /* Description on gcalctool command-line help options displayed on command-line */
            _("Help Options:\n"
              "  -v, --version                   Show release version\n"
              "  -h, -?, --help                  Show help options\n"
              "  --help-all                      Show all help options\n"
              "  --help-gtk                      Show GTK+ options"));
    fprintf(stderr,
            "\n\n");

    if (show_gtk) {
        fprintf(stderr,
                /* Description on gcalctool command-line GTK+ options displayed on command-line */
                _("GTK+ Options:\n"
                  "  --class=CLASS                   Program class as used by the window manager\n"
                  "  --name=NAME                     Program name as used by the window manager\n"
                  "  --screen=SCREEN                 X screen to use\n"
                  "  --sync                          Make X calls synchronous\n"
                  "  --gtk-module=MODULES            Load additional GTK+ modules\n"
                  "  --g-fatal-warnings              Make all warnings fatal"));
        fprintf(stderr,
                "\n\n");
    }

    if (show_application) {
        fprintf(stderr,
                /* Description on gcalctool application options displayed on command-line */
                _("Application Options:\n"
                  "  -s, --solve <equation>          Solve the given equation"));
        fprintf(stderr,
                "\n\n");
    }
}


static void
get_options(int argc, char *argv[])
{
    int i;
    char *progname, *arg;

    progname = g_path_get_basename(argv[0]);

    for (i = 1; i < argc; i++) {
        arg = argv[i];

        if (strcmp(arg, "-v") == 0 ||
            strcmp(arg, "--version") == 0) {
            version(progname);
            exit(0);
        }
        else if (strcmp(arg, "-h") == 0 ||
                 strcmp(arg, "-?") == 0 ||
                 strcmp(arg, "--help") == 0) {
            usage(progname, TRUE, FALSE);
            exit(0);
        }
        else if (strcmp(arg, "--help-all") == 0) {
            usage(progname, TRUE, TRUE);
            exit(0);
        }
        else if (strcmp(arg, "--help-gtk") == 0) {
            usage(progname, FALSE, TRUE);
            exit(0);
        }
        else if (strcmp(arg, "-s") == 0 ||
            strcmp(arg, "--solve") == 0) {
            i++;
            if (i >= argc) {
                fprintf(stderr,
                        /* Error printed to stderr when user uses --solve argument without an equation */
                        _("Argument --solve requires an equation to solve"));
                fprintf(stderr, "\n");
                exit(1);
            }
            else
                solve(argv[i]);
        }
        else {
            fprintf(stderr,
                    /* Error printed to stderr when user provides an unknown command-line argument */
                    _("Unknown argument '%s'"), arg);
            fprintf(stderr, "\n");
            usage(progname, TRUE, FALSE);
            exit(1);
        }
    }
}


static void
accuracy_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_int(settings, "accuracy", math_equation_get_accuracy(equation));
}


static void
word_size_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_int(settings, "word-size", math_equation_get_word_size(equation));
}


static void
show_thousands_separators_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_boolean(settings, "show-thousands", math_equation_get_show_thousands_separators(equation));
}


static void
show_trailing_zeroes_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_boolean(settings, "show-zeroes", math_equation_get_show_trailing_zeroes(equation));
}


static void
number_format_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_enum(settings, "number-format", math_equation_get_number_format(equation));
}


static void
angle_unit_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_enum(settings, "angle-units", math_equation_get_angle_units(equation));
}


static void
source_currency_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_string(settings, "source-currency", math_equation_get_source_currency(equation));
}


static void
target_currency_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_string(settings, "target-currency", math_equation_get_target_currency(equation));
}


static void
source_units_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_string(settings, "source-units", math_equation_get_source_units(equation));
}


static void
target_units_cb(MathEquation *equation, GParamSpec *spec)
{
    g_settings_set_string(settings, "target-units", math_equation_get_target_units(equation));
}


static void
programming_base_cb(MathButtons *buttons, GParamSpec *spec)
{
    g_settings_set_int(settings, "base", math_buttons_get_programming_base(buttons));
}


static void
mode_cb(MathButtons *buttons, GParamSpec *spec)
{
    g_settings_set_enum(settings, "button-mode", math_buttons_get_mode(buttons));
}


static void
startup_cb(GApplication *application)
{
    MathEquation *equation;
    MathButtons *buttons;
    int accuracy = 9, word_size = 64, base = 10;
    gboolean show_tsep = FALSE, show_zeroes = FALSE;
    MpDisplayFormat number_format;
    MPAngleUnit angle_units;
    ButtonMode button_mode;
    gchar *source_currency, *target_currency;
    gchar *source_units, *target_units;

    settings = g_settings_new ("org.gnome.gcalctool");
    accuracy = g_settings_get_int(settings, "accuracy");
    word_size = g_settings_get_int(settings, "word-size");
    base = g_settings_get_int(settings, "base");
    show_tsep = g_settings_get_boolean(settings, "show-thousands");
    show_zeroes = g_settings_get_boolean(settings, "show-zeroes");
    number_format = g_settings_get_enum(settings, "number-format");
    angle_units = g_settings_get_enum(settings, "angle-units");
    button_mode = g_settings_get_enum(settings, "button-mode");
    source_currency = g_settings_get_string(settings, "source-currency");
    target_currency = g_settings_get_string(settings, "target-currency");
    source_units = g_settings_get_string(settings, "source-units");
    target_units = g_settings_get_string(settings, "target-units");

    equation = math_equation_new();
    math_equation_set_accuracy(equation, accuracy);
    math_equation_set_word_size(equation, word_size);
    math_equation_set_show_thousands_separators(equation, show_tsep);
    math_equation_set_show_trailing_zeroes(equation, show_zeroes);
    math_equation_set_number_format(equation, number_format);
    math_equation_set_angle_units(equation, angle_units);
    math_equation_set_source_currency(equation, source_currency);
    math_equation_set_target_currency(equation, target_currency);
    math_equation_set_source_units(equation, source_units);
    math_equation_set_target_units(equation, target_units);
    g_free(source_currency);
    g_free(target_currency);
    g_free(source_units);
    g_free(target_units);

    g_signal_connect(equation, "notify::accuracy", G_CALLBACK(accuracy_cb), NULL);
    g_signal_connect(equation, "notify::word-size", G_CALLBACK(word_size_cb), NULL);
    g_signal_connect(equation, "notify::show-thousands-separators", G_CALLBACK(show_thousands_separators_cb), NULL);
    g_signal_connect(equation, "notify::show-trailing-zeroes", G_CALLBACK(show_trailing_zeroes_cb), NULL);
    g_signal_connect(equation, "notify::number-format", G_CALLBACK(number_format_cb), NULL);
    g_signal_connect(equation, "notify::angle-units", G_CALLBACK(angle_unit_cb), NULL);
    g_signal_connect(equation, "notify::source-currency", G_CALLBACK(source_currency_cb), NULL);
    g_signal_connect(equation, "notify::target-currency", G_CALLBACK(target_currency_cb), NULL);
    g_signal_connect(equation, "notify::source-units", G_CALLBACK(source_units_cb), NULL);
    g_signal_connect(equation, "notify::target-units", G_CALLBACK(target_units_cb), NULL);

    window = math_window_new(GTK_APPLICATION(application), equation);
    buttons = math_window_get_buttons(window);
    math_buttons_set_programming_base(buttons, base);
    math_buttons_set_mode(buttons, button_mode); // FIXME: We load the basic buttons even if we immediately switch to the next type
    g_signal_connect(buttons, "notify::programming-base", G_CALLBACK(programming_base_cb), NULL);
    g_signal_connect(buttons, "notify::mode", G_CALLBACK(mode_cb), NULL);

    gtk_widget_show(GTK_WIDGET(window));
}


static void
activate_cb(GApplication *application)
{
    gtk_window_present(GTK_WINDOW(window));
}


int
main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    setlocale(LC_ALL, "");
    bindtextdomain(GETTEXT_PACKAGE, LOCALE_DIR);
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain(GETTEXT_PACKAGE);

    /* Seed random number generator. */
    srand48((long) time((time_t *) 0));

    g_type_init();

    get_options(argc, argv);

    gtk_init(&argc, &argv);

    gtk_window_set_default_icon_name("accessories-calculator");

    app = gtk_application_new("org.gnome.gcalctool", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "startup", G_CALLBACK(startup_cb), NULL);
    g_signal_connect(app, "activate", G_CALLBACK(activate_cb), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);

    return status;
}
