#!/bin/bash

# Function to get the environment name based on the current directory
get_environment_name() {
    # Customize this function based on your directory structure
    # For example, you can check for the existence of certain files or directories
    # to determine the environment name
    if [ -d "/path/to/environment1" ]; then
        echo "environment1"
    elif [ -d "/path/to/environment2" ]; then
        echo "environment2"
    else
        echo "unknown_environment"
    fi
}

# Set PS1 with the environment name
export PS1='RAZY \w $ '
