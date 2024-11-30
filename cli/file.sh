#!/bin/bash

# Ask the user for the directory to check
read -p "Enter the directory path: " DIR

# Check if the directory exists
if [ ! -d "$DIR" ]; then
    echo "The directory $DIR does not exist."
    exit 1
fi


# Count the number of files in the directory
FILE_COUNT=$(ls -1q "$DIR" | wc -l)

# Display a message based on the file count
if [ "$FILE_COUNT" -gt 0 ]; then
    echo "There are $FILE_COUNT files in $DIR."
else
    echo "No files found in $DIR."
fi

