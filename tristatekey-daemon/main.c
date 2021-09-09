#include <stdio.h>
#include <gpiod.h>
#include <signal.h>
#include <unistd.h>
#include <gio/gio.h>

//#define DEBUG

#define POLL_RATE_MS 20

#define LINE_KEY_BOTTOM 24
#define LINE_KEY_MIDDLE 52
#define LINE_KEY_UP     126
#define N_KEYS          3

#define VALUE_KEY_BOTTOM 5
#define VALUE_KEY_MIDDLE 4
#define VALUE_KEY_UP     3
#define VALUE_KEY_NA     6 // This occurs when switching between the keys


GDBusProxy *initialize_dbus_proxy();
struct gpiod_line_bulk initialize_GPIO_lines();
void sigint_handler(int sig_num);
void set_notify_property(int key_state);
void tristatekey_manager_loop();


// These are global so they are accessible to sigint_handler
struct gpiod_chip *chip;
struct gpiod_line_bulk lines_tristatekey_blk;
// These have to be initialized only once
GDBusConnection *conn;
GDBusProxy *proxy;


int main()
{
    // Define a sigint handler, so we can clean after ourselves in case of termination
    signal(SIGINT, sigint_handler);

    // Initialize the DBus proxy for org.sigx.Feedback
    proxy = initialize_dbus_proxy();

    // Initialize the input GPIO lines for the tri-state key
    lines_tristatekey_blk = initialize_GPIO_lines();

    // Manage the current tri-state key values
    tristatekey_manager_loop();

    return 0;
}

void tristatekey_manager_loop()
{
    int values[3];
    int key_state;
    int prev_key_state = 0; // This state can never happen

    while (true)
    {
        gpiod_line_get_value_bulk(&lines_tristatekey_blk, values);

        // Encode the values into a single state, so we can check it easily
        key_state = values[0] + 2*values[1] + 3*values[2];

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
        case VALUE_KEY_NA:
            printf("Tri-state key is set to NA\n");
            break;
        }
#endif

        // Set the property only if the state has changed.
        // By initializing prev_key_state to 0, we garantee to do it the first time we get here
        if (key_state != prev_key_state && key_state != VALUE_KEY_NA)
        {
            set_notify_property(key_state);
        }
        prev_key_state = key_state;

        usleep(1000*POLL_RATE_MS);
    }

    return;
}

void sigint_handler(int sig_num)
{
#ifdef DEBUG
    printf("\nTerminating daemon...");
#endif

    // Release lines and close chip
    gpiod_line_release_bulk(&lines_tristatekey_blk);
    gpiod_chip_close(chip);

#ifdef DEBUG
    printf(" Terminated.\n");
#endif

    exit(0);
}

struct gpiod_line_bulk initialize_GPIO_lines()
{
    // Open GPIO chip
    const char *chipname = "gpiochip2";
    chip = gpiod_chip_open_by_name(chipname);

    // Open the GPIO lines
    unsigned int offsets[3] = {LINE_KEY_BOTTOM, LINE_KEY_MIDDLE, LINE_KEY_UP};
    gpiod_chip_get_lines(chip, offsets, N_KEYS, &lines_tristatekey_blk);

    // Request the lines for input
    gpiod_line_request_bulk_input(&lines_tristatekey_blk, "Tri-state key");
    // I would prefer to name each line separately, but it does not work
    //gpiod_line_request_input(lines_tristate_blk.lines[0], "Tri-state up");
    //gpiod_line_request_input(lines_tristate_blk.lines[1], "Tri-state middle");
    //gpiod_line_request_input(lines_tristate_blk.lines[2], "Tri-state bottom");
    //lines_tristate_blk.num_lines = 3;

    return lines_tristatekey_blk;
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

#ifdef DEBUG
    // Get the "Profile" property and print it
    GVariant *profile_prop = g_dbus_proxy_get_cached_property(proxy, "Profile");
    gchar *current_prop_str;
    g_variant_get(profile_prop, "s", &current_prop_str);
    printf("Property \"Profile\" is set to: %s\n", current_prop_str);
    g_free(current_prop_str);
#endif

    return;
}











