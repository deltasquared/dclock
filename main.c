#include <gtk/gtk.h>
#include <time.h>
#include <math.h>

// Startup
static void destroy(GtkWidget*,  gpointer);
static gboolean delete_event (GtkWidget*, GdkEvent*, gpointer);
static gchar *timeString;
static gchar *utcString;
static gchar *beat;

gint paintLocal(gpointer data){
    GtkWidget* localLabel = (GtkWidget*) data;
    GDateTime* localTime = g_date_time_new_now_local();
    timeString = g_strdup_printf("Local Time: %02d:%02d:%02d", g_date_time_get_hour(localTime), g_date_time_get_minute(localTime), g_date_time_get_second(localTime));
    gtk_label_set_text(localLabel, timeString);
    return TRUE;
}
//
//why
gint paintUTC(gpointer data){
    GtkWidget* utcLabel = (GtkWidget*) data;
    GDateTime* utcTime = g_date_time_new_now_utc();
    utcString = g_strdup_printf("UTC Time: %02d:%02d:%02d", g_date_time_get_hour(utcTime), g_date_time_get_minute(utcTime), g_date_time_get_second(utcTime));
    gtk_label_set_text(utcLabel, utcString);
    return TRUE;
}

gint paintBeat(gpointer data){
    GtkWidget* beatLabel = (GtkWidget*) data;
    GDateTime* utcTime = g_date_time_new_now_utc();
    int hours = g_date_time_get_hour(utcTime) == 23 ? 0 : g_date_time_get_hour(utcTime) + 1;
    int minutes = g_date_time_get_minute(utcTime);
    int seconds = g_date_time_get_second(utcTime);
    int timeInSeconds = (((hours * 60) + minutes) * 60) + seconds;
    double secondsInABeat = 86.4;
    double rawBeats = timeInSeconds / secondsInABeat;
    int beatNumber = fmod(rawBeats, 1000);
    beat = g_strdup_printf("@%d", beatNumber);
    gtk_label_set_text(beatLabel, beat);
    return TRUE;
}

int main(int argc, char*argv[]){
    GtkWidget *window, *vbox, *label, *utcLabel, *beatLabel;
    GDateTime* utcTime = g_date_time_new_now_utc();
    GDateTime* localTime = g_date_time_new_now_local();

    gtk_init(&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW (window), "Desk Clock");
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    gtk_widget_set_size_request(window, 200, 100);

    // Connect the main window to the destroy and delete-event signals
    g_signal_connect (G_OBJECT (window), "destroy", G_CALLBACK(destroy), NULL);
    g_signal_connect (G_OBJECT (window), "delete_event", G_CALLBACK(delete_event), NULL);

    //timeString = g_strdup_printf("Local Time: %02d:%02d:%02d", now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);
    utcString = g_strdup_printf("UTC Time: %02d:%02d:%02d", g_date_time_get_hour(utcTime), g_date_time_get_minute(utcTime), g_date_time_get_second(utcTime));
    timeString = g_strdup_printf("Local Time: %02d:%02d:%02d", g_date_time_get_hour(localTime), g_date_time_get_minute(localTime), g_date_time_get_second(localTime));

    int hours = g_date_time_get_hour(utcTime) == 23 ? 0 : g_date_time_get_hour(utcTime) + 1;
    int minutes = g_date_time_get_minute(utcTime);
    int seconds = g_date_time_get_second(utcTime);
    int timeInSeconds = (((hours * 60) + minutes) * 60) + seconds;
    double secondsInABeat = 86.4;
    double rawBeats = timeInSeconds / secondsInABeat;
    int beatNumber = fmod(rawBeats, 1000);

    //double beatNumberRaw = (((utc_now->tm_hour + 3) * 3600) + (utc_now->tm_min * 60) + utc_now->tm_sec) / 86.4;
    //double beatNumberRaw = ((utc_now->tm_sec + (60 * utc9ow->tm_min) + ((utc_now->tm_hour + 3) * 3600)) / 86.4);
    //int beatNumber = fmod(beatNumberRaw, 1000.0);
    //g_free(utcTime);
    beat = g_strdup_printf("@%d", beatNumber);

    label = gtk_label_new (timeString);

    gtk_label_set_selectable(GTK_LABEL (label), TRUE);
    utcLabel = gtk_label_new(utcString);

    beatLabel = gtk_label_new(beat);
    vbox = gtk_vbox_new(TRUE, 5);
    gtk_box_pack_start(GTK_BOX (vbox), label, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX (vbox), utcLabel, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX (vbox), beatLabel, TRUE, TRUE, 5);
    //(GTK_BOX (vbox), label);

    gtk_container_add(GTK_CONTAINER (window), vbox);
    //gtk_container_add(GTK_CONTAINER (window), label);
    gtk_widget_show_all(window);
    g_timeout_add(1000, paintLocal, (gpointer) label);
    g_timeout_add(1000, paintUTC, (gpointer) utcLabel);
    g_timeout_add(864, paintBeat, (gpointer) beatLabel);
    gtk_main();
    return 0;
}


static void destroy (GtkWidget *window, gpointer data){
    g_free(timeString);
    g_free(utcString);
    g_free(beat);
    gtk_main_quit();
}

static gboolean delete_event(GtkWidget *window, GdkEvent *event, gpointer data){
    return FALSE;
}
