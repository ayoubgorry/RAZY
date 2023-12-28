#!/bin/bash

# Function to check if a user exists
user_exists() {
    id "$1" &>/dev/null
}

# Function to check if a user entry already exists in passwords.txt
user_entry_exists() {
    grep -q "^$1:" passwords.txt
}

# Function to add user with hashed password
add_user() {
    username=$1
    hashed_password=$2

    if [[ -z $username || -z $hashed_password ]]; then
        echo "Usage: $0 <username> <hashed_password>"
        exit 1
    fi

    if user_exists $username; then
        echo "User $username already exists."
    else
        sudo adduser --disabled-password --gecos "" $username
        echo "$username:$hashed_password" >> etc/passwords.txt
        echo "User $username added successfully."
    fi
}

# Function to create or update passwords.txt
update_passwords_file() {
    username=$1
    hashed_password=$2

    if ! user_entry_exists $username; then
        echo "$username:$hashed_password" >> passwords.txt
    fi
}

# Prompt for username and password
read -p "Enter username: " new_username
read -s -p "Enter password: " new_password
echo
bin_directory="/home/youssef/Desktop/cc/users/$new_username/bin"
home_directory="/home/youssef/Desktop/cc/users/$new_username/home"
# Generate hashed password
hashed_password=$(openssl passwd -1 "$new_password")

# Add the new user and update passwords.txt
add_user "$new_username" "$hashed_password"
update_passwords_file "$new_username" "$hashed_password"

# Create a directory with the user's name
user_directory="/home/youssef/Desktop/cc/users/$new_username"
echo "Creating directory: $user_directory"
sudo mkdir -p "$user_directory"
sudo mkdir -p "$bin_directory"
sudo mkdir -p "$home_directory"

# Set ownership and permissions
sudo chown "$new_username" "$user_directory"
sudo chmod 700 "$user_directory"

echo "Directory $user_directory created and locked."

# Display the content of passwords.txt
echo "Contents of passwords.txt:"
cat passwords.txt

