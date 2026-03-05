# 1. Internally, your script is required to implement the functionality by calling the default Unix
# kill command and your own ourpgrep.sh script.

# 2. ourpgrep.sh shall accept one optional argument and one required argument:
    # a. Optional argument: a signal number to pass on to the processes to kill, preceded
    # with the dash character ‘-'. Example: -9 to send the SIGKILL (force-kill) signal. If
    # this is not specified, the default signal is -15 (or SIGTERM).

    # b. Required argument: a name pattern to pass to ourpgrep.sh.

# 3. The script itself should not output anything unless there was an error (just send the
# signal to the process(es) quietly).

# _______________________________________________________________________________________________

#!/bin/bash
# Use ourpgrep.sh to find the PID then terminate them.

if [ -z $1 ]
then
    echo "Error: Provide process name - ./ourpgrep.sh [pattern]"
    exit 1
fi

# Determine if $1 is a signal or a name pattern
first_char=$(echo $1 | cut -c1)

if [ "$first_char" = "-" ]
then
    # User provided a signal (like -9)
    signal=$1
    pattern=$2
else
    # No signal provided, use default -15 (SIGTERM)
    signal="-15"
    pattern=$1
fi

# If a signal was given, make sure a pattern followed it
if [ -z $pattern ]
then
    echo Error: No process name pattern provided after signal.
    exit 1
fi

# Get the PIDs using ourpgrep.sh and Redirect to a temp file
./ourpgrep.sh $pattern > temp_pids.txt

# Use a for loop to kill each process ID
# We use $(cat ...) to turn the file content into a list.
for id in $(cat temp_pids.txt)
do
    # Call the kill command
    # Redirecting error to /dev/null ensures it stays "quiet"
    kill $signal $id 2> /dev/null
done

# Delete the temp file
rm temp_pids.txt
