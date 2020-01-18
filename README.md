# linux-keylogger
Linux keylogger written in C

This program captures every keystroke and log it to 'keylogs' file.
Using this, everything can be captured, including PASSWORDS IN PLAINTEXT :)

Compile it as:
"gcc keylogger.c -o <executable_file>"

And then run it as:
"./<executable_file> &"  (& is for running the task in background)
We need to run it as root. 

Logging file 'keylogs' will be stored in same directory.

NOTE: Copy the header file input-codes-to-array.h in same directory, this header file is for mapping the input codes to in array, so we can play with indices.

To quit the program, enter ALT+CTRL+SHIFT+ESC keys together from anywhere!!

Please use this tool for EDUCATIONAL PURPOSE ONLY!!
