How to compile smallsh

1.  From within a terminal window, change to the directory where smallsh.c is located.

2.  Load the bash shell by running the command bash

3.  To compile, type the following command (case sensitive):
	gcc -Wall shellsh.c -o shellsh

	3a.  If you don't have permission to access, run chmod on shellsh.c:
		chmod 755 shellsh.c
		Then retry gcc command above.

4.  Check to ensure the file smallsh was created using the ls command.
