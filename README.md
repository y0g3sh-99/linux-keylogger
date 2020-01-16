# linux-keylogger
Linux keylogger written in C

This program captures every keystroke and log it to 'keylogs' file.
Using this, everything can be captured, including PASSWORDS IN PLAINTEXT :)
Please use this for EDUCATIONAL PURPOSE ONLY!!

Compile it as:
gcc keylogger.c -o <executable_file>

And then run it as:
./<executable_file> &
We need to run it as root. 
(& is for running the task in background)

Logging file 'keylogs' will be stored in same directory.
