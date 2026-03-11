# Pete Justin Dagaraga (231874)
# Shaan Graal Dayag (231928)
# Robynne Alexa Gonzales (232944)

# March 11, 2026

# We have not discussed the bash language code in our program with anyone other than our instructor or the teaching assistants assigned to this course.
# We have not used bash language code obtained from another student, or any other unauthorized source, either modified or unmodified.
# If any bash language code or documentation used in our program was obtained from another source, such as a textbook or course notes, that has been clearly noted with a proper citation in the comments of our program.

#!/bin/bash

# Checks if there are no passed arguments. If so, echo the error
if [ -z "$*" ]
then
    echo "Error: Provide process name - ./ourpgrep.sh [pattern]"
    exit 1
fi

# Check for the signal
first_char=$(echo $1 | cut -c1)

if [ "$first_char" = "-" ]
then
    signal=$1
    pattern=$2
else
    # default signal
    signal="-15"
    pattern=$1
fi

# Call ourpgrep.sh and redirect to a temp file
./ourpgrep.sh $pattern > .tmp_pids

# Loop through the PIDs
for id in $(cat .tmp_pids)
do
    # Redirecting to /dev/null makes it output nothing
    kill $signal $id > /dev/null 2>&1
done

rm .tmp_pids