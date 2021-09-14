#include <stdio.h>
#include <gpiod.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//#define DEBUG

#define FIFO_PATH "/tmp/tristatekey-state"
#define FIFO_PERMISSIONS 666

#define POLL_RATE_MS 20

#define LINE_KEY_BOTTOM 24
#define LINE_KEY_MIDDLE 52
#define LINE_KEY_UP     126
#define N_KEYS          3

#define VALUE_KEY_BOTTOM 5
#define VALUE_KEY_MIDDLE 4
#define VALUE_KEY_UP     3
#define VALUE_KEY_NA     6 // This occurs when switching between the keys


struct gpiod_line_bulk initialize_GPIO_lines();
void sigint_handler(int sig_num);
void tristatekey_root_daemon_loop();
int create_and_open_FIFO(char *pathname, mode_t permissions);


// These are global so they are accessible to sigint_handler
// or other sig* handlers in case of termination
struct gpiod_chip *chip;
struct gpiod_line_bulk lines_tristatekey_blk;
int fd_FIFO;

int main()
{
    // Define a sigint handler, so we can clean after ourselves in case of termination
    signal(SIGINT, sigint_handler);

    // Initialize the input GPIO lines for the tri-state key
    lines_tristatekey_blk = initialize_GPIO_lines();

    // Initialize the named pipe
    fd_FIFO = create_and_open_FIFO(FIFO_PATH, FIFO_PERMISSIONS);

    // Manage the current tri-state key values
    tristatekey_root_daemon_loop();

    return 0;
}

void tristatekey_root_daemon_loop()
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
            // Write the current state into the FIFO
            write(fd_FIFO, &key_state, sizeof (int));
        }
        prev_key_state = key_state;

        usleep(1000*POLL_RATE_MS);
    }

    return;
}

void sigint_handler(int sig_num)
{
#ifdef DEBUG
    printf("\nTerminating root daemon...");
#endif

    // Release lines and close chip
    gpiod_line_release_bulk(&lines_tristatekey_blk);
    gpiod_chip_close(chip);
    // Close the pipe
    close(fd_FIFO);

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

    return lines_tristatekey_blk;
}

int create_and_open_FIFO(char *pathname, mode_t permissions)
{
    mkfifo(pathname, permissions);

    int fd = open(pathname, O_WRONLY);

    return fd;
}














