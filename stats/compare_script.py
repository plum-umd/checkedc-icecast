import json
import os
import re
import subprocess

# TODO: Make a list of all .h files that need to be modified and check if they have a .checked.h equivalent
# TODO: Automatically search each .c and .h file to see if there are any pointers that should have been converted, but weren't for some reason

SLASH = os.sep

output = False 

# The 'compile_commands.json' file produced by the 'bear make' command
json_cmds = json.load(open('compile_commands.json', 'r'))

json_list = []
for i in json_cmds:
    json_list.append(i['directory'] + SLASH + i['file'])

# The 'convert_all.sh' file produced by the 'convert-commands.py' script
script_all_lines = open('convert_all.sh', 'r').read().split('\n')

script_all_list = []
for line in script_all_lines:
    m = re.match(".*\.c", line)
    n = re.match(".*\.h", line)
    if m:
        script_all_list.append(m.group())
    if n:
        script_all_list.append(n.group())

# Run the 'pwd' command to find the path
path = subprocess.check_output(['pwd'])
path = path[0:len(path) - 1]

# Run the 'find' command to find all .checked.c and .checked.h files
result = subprocess.check_output(['find', '-regex', '.*/.*\.\(checked\.c\|checked\.h\)$'])

find_list = []
for i in result.split('\n'):
    j = i.replace('.checked', '')
    find_list.append(path + j[1:])

## Test if json_list and script_all_list are requal

# Make lists unique because the JSON file has more than one occurrence for some files
json_list = sorted(set(json_list))
script_all_list = sorted(set(script_all_list))
find_list = sorted(set(find_list))

if output:
    f = open('output_compile_commands.txt', 'a')
    for i in json_list:
        f.write(i + '\n')
    f.close
    
    f = open('output_convert_all.txt', 'a')
    for i in script_all_list:
        f.write(i + '\n')
    f.close

    f = open('output_find.txt', 'a')
    for i in find_list:
        f.write(i + '\n')
    f.close

# Helper function to print lists
def print_list(lst):
    for i in lst:
        print i

if json_list == script_all_list:
    print "JSON and 'convert_all.sh' are equal"
    if not list(set(json_list) - set(find_list)):
        print "All lists are equal"
    else:
        print "Something is wrong..."
        print_list(list(set(json_list) - set(find_list)))
else:
    print "The lists aren't equal"

    print "Printing difference of json - convert_all lists:"
    print_list(list(set(json_list) - set(script_all_list)))

    print "Printing other difference (i.e. convert_all - json):"
    print_list(list(set(json_list) - set(script_all_list)))

#    print "Printing json list:"
#    print_list(json_list)

#    print "Printing convert_all list:"
#    print_list(script_all_list)

