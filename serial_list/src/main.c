// fichier: list_serial_ports.c
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int is_serial_port(const char *name) {
    return (strncmp(name, "tty.", 4) == 0 || strncmp(name, "cu.", 3) == 0);
}

int main() {
    struct dirent *entry;
    DIR *dp = opendir("/dev");

    if (dp == NULL) {
        perror("opendir");
        return 1;
    }

    printf("Ports série disponibles sur macOS :\n");
    while ((entry = readdir(dp))) {
        if (is_serial_port(entry->d_name)) {
            printf("/dev/%s\n", entry->d_name);
        }
    }

    closedir(dp);
    return 0;
}

