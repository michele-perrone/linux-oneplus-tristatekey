#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <gio/gio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>

#define DEBUG

#define FIFO_PATH "/tmp/tristatekey-state"

#define VALUE_KEY_BOTTOM 5
#define VALUE_KEY_MIDDLE 4
#define VALUE_KEY_UP     3


GDBusProxy * initialize_dbus_proxy();
void sigint_handler(int sig_num);
void set_notify_property(int key_state);
void tristatekey_user_daemon_loop();


// These have to be initialized only once
GDBusConnection *conn;
GDBusProxy *proxy;
// These are global so they are accessible to sigint_handler
// or other sig* handlers in case of termination
int fd_FIFO;


int main()
{
    // Define a sigint handler, so we can clean after ourselves in case of termination
    signal(SIGINT, sigint_handler);

    // Grab our end of the FIFO
    char *path = "/tmp/tristatekey-state";
    fd_FIFO = open(FIFO_PATH, O_RDONLY);
    printf("fd_FIFO: %i\n", fd_FIFO);

    // Initialize the DBus proxy for org.sigx.Feedback
    proxy = initialize_dbus_proxy();

    // Manage the current tri-state key values
    tristatekey_user_daemon_loop();

    return 0;
}

void tristatekey_user_daemon_loop()
{
    int key_state = 0;

    while (true)
    {
        // Get the tri-state key value (blocking read from FIFO)
        read(fd_FIFO, &key_state, sizeof (int));

#ifdef DEBUG
        // Print the state in a human-friendly way
        switch (key_state)
        {
        case VALUE_KEY_BOTTOM:
            printf("Tri-state key is set to BOTTOM\n");
            break;
        case VALUE_KEY_MIDDLE:
            printf("Tri-state key is set to MIDDLE\n");
            break;
        case VALUE_KEY_UP:
            printf("Tri-state key is set to UP\n");
            break;
        }
#endif

        // Set the property
        set_notify_property(key_state);

        //usleep(10000);
    }

    return;
}

void sigint_handler(int sig_num)
{
#ifdef DEBUG
    printf("\nTerminating user daemon...");
#endif

    // Close the FIFO
    close(fd_FIFO);

#ifdef DEBUG
    printf(" Terminated.\n");
#endif

    exit(0);
}


GDBusProxy* initialize_dbus_proxy()
{
    conn = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, NULL);
    return g_dbus_proxy_new_sync(conn, // connection
                                 G_DBUS_PROXY_FLAGS_NONE, // flags
                                 NULL, // info
                                 "org.sigxcpu.Feedback", // name
                                 "/org/sigxcpu/Feedback", // object_path
                                 "org.freedesktop.DBus.Properties", // interface_name
                                 NULL, // cancellable
                                 NULL // error
                                 );
}

void set_notify_property(int key_state)
{
    GVariant *silent_prop = g_variant_new("s", "silent"); // Silent
    GVariant *quiet_prop = g_variant_new("s", "quiet"); // Vibration
    GVariant *full_prop = g_variant_new("s", "full"); // Ring

    // Set the "Profile" property
    switch (key_state)
    {
    case VALUE_KEY_BOTTOM:
#ifdef DEBUG
    printf("Setting \"Profile\" property to \"silent\"\n");
#endif
        g_dbus_proxy_call(proxy, "Set", g_variant_new("(ssv)",
                                                      "org.sigxcpu.Feedback",
                                                      "Profile",
                                                      silent_prop), G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL, NULL);
        break;
    case VALUE_KEY_MIDDLE:
#ifdef DEBUG
        printf("Setting \"Profile\" property to \"quiet\"\n");
#endif
        g_dbus_proxy_call(proxy, "Set", g_variant_new("(ssv)",
                                                      "org.sigxcpu.Feedback",
                                                      "Profile",
                                                      quiet_prop), G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL, NULL);
        break;
    case VALUE_KEY_UP:
#ifdef DEBUG
        printf("Setting \"Profile\" property to \"full\"\n");
#endif
        g_dbus_proxy_call(proxy, "Set", g_variant_new("(ssv)",
                                                      "org.sigxcpu.Feedback",
                                                      "Profile",
                                                      full_prop), G_DBUS_CALL_FLAGS_NONE, -1, NULL, NULL, NULL);
        break;
    }

    return;
}

