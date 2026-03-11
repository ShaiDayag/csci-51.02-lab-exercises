# Pete Justin Dagaraga (231874)
# Shaan Graal Dayag (231928)
# Robynne Alexa Gonzales (232944)

# March 11, 2026

# We have not discussed the bash language code in our program with anyone other than our instructor or the teaching assistants assigned to this course.
# We have not used bash language code obtained from another student, or any other unauthorized source, either modified or unmodified.
# If any bash language code or documentation used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.

#!/bin/bash

# Holds our pattern
pattern=$1

# Checks if pattern is nothing. If so, echo the error
if [ -z $pattern ]
then
    echo "Error: Provide process name - ./ourpgrep.sh [pattern]"
    exit 1
fi

# Out of all active user processes, return the PIDs of the processes that match the pattern
ps aux | grep ^$USER | grep $pattern | grep -v grep | grep -v "ourpgrep.sh" | grep -v "ourpkill.sh" | tr -s ' ' | cut -d ' ' -f2
