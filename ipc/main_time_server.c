#include <gio/gio.h>
#include <glib.h>
#include "time_service.h"

static gboolean
handle_get_time_of_day(TimeServiceComExampleTimeService *object,
                       GDBusMethodInvocation *invocation,
                       gpointer user_data)
{
    // Get the current time
    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);

    // Format the time as a string
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", local_time);

    // Return the time to the client
    time_service_com_example_time_service_complete_get_time_of_day(object, invocation, time_str);
    return TRUE;
}

static void
on_name_acquired(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    g_print("Acquired name: %s\n", name);
}

static void
on_name_lost(GDBusConnection *connection, const gchar *name, gpointer user_data)
{
    if (connection == NULL) {
        g_printerr("Failed to connect to the bus\n");
    } else {
        g_printerr("Failed to acquire the name: %s\n", name);
    }
}

int main(int argc, char *argv[])
{
    GMainLoop *loop;
    GDBusConnection *connection;
    GError *error = NULL;
    TimeServiceComExampleTimeService *skeleton;

    // Create a main loop
    loop = g_main_loop_new(NULL, FALSE);

    // Get a connection to the session bus
    connection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &error);
    if (error) {
        g_printerr("Error connecting to the bus: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Create the skeleton object
    skeleton = time_service_com_example_time_service_skeleton_new();

    // Connect the method handler
    g_signal_connect(skeleton, "handle-get-time-of-day",
                     G_CALLBACK(handle_get_time_of_day), NULL);

    // Export the object on the bus
    if (!g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(skeleton),
                                          connection,
                                          "/com/example/TimeService",
                                          &error)) {
        g_printerr("Error exporting object: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

	guint owner_id;
	// Request the name explicitly
	owner_id = g_bus_own_name(
        G_BUS_TYPE_SESSION,
        "com.example.TimeService",
        G_BUS_NAME_OWNER_FLAGS_NONE,
        NULL,                   // Bus acquired callback (optional)
        on_name_acquired,       // Name acquired callback
        on_name_lost,           // Name lost callback
        NULL,                   // User data
        NULL                    // User data free function
    );

    if (owner_id == 0) {
        g_printerr("Failed to acquire the name com.example.TimeService\n");
        return 1;
    }

    g_print("Service running at com.example.TimeService\n");

    // Run the main loop
    g_main_loop_run(loop);

    // Cleanup
    g_object_unref(skeleton);
    g_main_loop_unref(loop);

    return 0;
}

