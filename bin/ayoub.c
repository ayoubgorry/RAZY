#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>

#define PASSWORDS_FILE "/RAZY/etc/passwords.txt"
#define USERS_DIRECTORY "/RAZY/users/"

// Function to check if a user entry exists in /etc/passwords.txt
int user_entry_exists(const char *username) {
    FILE *file = fopen(PASSWORDS_FILE, "r");
    if (file == NULL) {
        perror("Error opening passwords file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0) {
            fclose(file);
            return 1; // User entry found
        }
    }

    fclose(file);
    return 0; // User entry not found
}

// Function to authenticate user
int authenticate_user(const char *username, const char *entered_password) {
    FILE *file = fopen(PASSWORDS_FILE, "r");
    if (file == NULL) {
        perror("Error opening passwords file");
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0) {
            char *saved_password = strtok(line, ":");
            saved_password = strtok(NULL, ":"); // Get the hashed password

            // Adjust the comparison for $1$ format
            char *salt = strtok(saved_password, "$");
            salt = strtok(NULL, "$");

            char *hashed_password = crypt(entered_password, salt);

            if (strcmp(hashed_password, saved_password) == 0) {
                fclose(file);
                return 0; // Authentication successful
            } else {
                fclose(file);
                return 1; // Authentication failed
            }
        }
    }

    fclose(file);
    return 2; // User not found
}

// Function to access the directory
void access_directory(const char *username) {
    char directory[256];
    snprintf(directory, sizeof(directory), "%s%s", USERS_DIRECTORY, username);

    if (chdir(directory) != 0) {
        perror("Error changing directory");
        exit(EXIT_FAILURE);
    }

    execl("/bin/bash", "/bin/bash", NULL);
    perror("Error executing /bin/bash");
    exit(EXIT_FAILURE);
}

int main() {
    char input_username[256];
    char input_password[256];

    // Prompt for username and password
    printf("Enter username: ");
    scanf("%s", input_username);
    printf("Enter password: ");
    scanf("%s", input_password);

    // Authenticate the user
    switch (authenticate_user(input_username, input_password)) {
        case 0:
            printf("Authentication successful. Accessing directory...\n");
            access_directory(input_username);
            break;
        case 1:
            printf("Authentication failed. Exiting.\n");
            exit(EXIT_FAILURE);
        case 2:
            printf("Error: User not found. Exiting.\n");
            exit(EXIT_FAILURE);
    }

    return 0;
}
