#!/usr/bin/ruby

output = `find . -type f`

valid_args = ["-verbose", "-convert"]

if !(ARGV - valid_args).empty? then
    puts "wrong arguments"
    exit(1)
end

if ARGV.include? "-verbose" then
    puts "verbose is true"
    verbose = true
else
    puts "verbose is false"
    verbose = false
end

if ARGV.include? "-convert" then
    puts "convert is true"
    flag = true
else
    puts "convert is false"
    flag = false
end

c_count = 0
h_count = 0
checked_c_count = 0
checked_h_count = 0

output.each_line do |line|
    if line =~ /\.\/(.*)\.checked\.c$/ then
        checked_c_count += 1
        source_file_name = $1 + ".c"
        checked_file_name = $1 + ".checked.c"
        if verbose then print "original source file name: " + source_file_name + "\n" end
        if verbose then print "checked source file name: " + checked_file_name + "\n" end
        if verbose then print "line: " + line end

        command = "mv " + checked_file_name + " " + source_file_name
        if verbose then print "call command: " + command + "\n" end
        if flag then
            system command
        end

        if verbose then print "\n" end
    
    elsif line =~ /\.\/(.*)\.checked\.h$/ then
        checked_h_count += 1
        source_file_name = $1 + ".h"
        checked_file_name = $1 + ".checked.h"
        if verbose then print "original header file name: " + source_file_name + "\n" end
        if verbose then print "checked header file name: " + checked_file_name + "\n" end
        if verbose then print "line: " + line end
    
        command = "mv " + checked_file_name + " " + source_file_name
        if verbose then print "call command: " + command + "\n" end
        if flag then
          system command
        end
    
        if verbose then print "\n" end
    
      elsif line =~ /\.\/(.*)\.c/ then
        c_count += 1

    elsif line =~ /\.\/(.*)\.h/ then
        h_count += 1
    end
          
end

puts "original source count: " + c_count.to_s
print "\n"

puts "original header count: " + h_count.to_s
print "\n"

puts "checked source count: " + checked_c_count.to_s
print "\n"

puts "checked header count: " + checked_h_count.to_s
print "\n"


