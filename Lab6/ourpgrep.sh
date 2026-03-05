# 1. ourpgrep.sh shall accept only one argument: a name pattern that will be used to
# match the name of the processes you want to list.

# 2. Your script should find the processes that match the pattern and belong to the current
# user. (There is an environment variable that denotes the current user name.)

# 3. You are required to use the output of the ps aux command to find the relevant
# processes. You may not modify this command (but you may redirect its output to a file or
# another program). Note that this command lists all processes by all users, so you will
# also have to filter the list by the current user name.

# 4. If there are no errors, only the matching PIDs should be reported in standard output, just
# as with pgrep. (But if there are errors, you should report them.)

# _______________________________________________________________________________________________

#!/bin/bash

pattern=$1

if [ -z $pattern ]
then
    echo "Error: Provide process name - ./ourpgrep.sh [pattern]"
    exit 1
fi

ps aux | grep ^$USER | grep $pattern | grep -v grep | grep -v ourpgrep.sh | grep -v ourpkill.sh | tr -s ' ' | cut -d' ' -f2
