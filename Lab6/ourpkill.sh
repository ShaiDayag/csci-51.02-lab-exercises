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

if [ -z "$*" ]
then
    echo "Error: Provide process name - ./ourpgrep.sh [pattern]"
    exit 1
fi

# check for the signal
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

# call ourpgrep.sh and redirect to a temp file
./ourpgrep.sh $pattern > .tmp_pids

# loop through the PIDs
for id in $(cat .tmp_pids)
do
    # redirecting to /dev/null makes it output nothing
    kill $signal $id > /dev/null 2>&1
done

rm .tmp_pids
