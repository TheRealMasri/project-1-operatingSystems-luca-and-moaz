#!/usr/bin/env bash
# System Resource Analyzer
# Usage: ./system_report.sh <NUM_PROCESSES> <OUTPUT_FILE>

process = "$1"
output = "$2"


#date
echo "The current date is: " > output
date >> output


#uptime
echo \ >> output
echo "This computers uptime is: " >> output
uptime >> output


#how many users are online
echo \ >> output
echo "This is how many users are on this computer: " >> output
who | wc -l  >> output


#top N process by memory
echo \ >> output
echo "The program with the most ram usage: " >> output
ps -eo pid,comm,%mem,%cpu --sort=-%mem | head -n "$1" >> output


#disk usage
echo \ >> output
echo "The disk usage is: " >> output
df -h >> output


echo "Report saved to report.txt"
