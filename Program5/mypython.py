#!/usr/bin/python

#Joshua Sears
#CS 344, Fall 2016
#Project 5 - Python

import random

choiceCtr = 0   #counters for while loops
count = 0
fname = ["file1.txt", "file2.txt", "file3.txt"] #array of file names
inputString = ""
int1 = 0
int2 = 0
product = 0

#while loop to create 3 files
while (count < 3):
    fo = open(fname[count], "w+")

    #while loops to create 10 random lower case characters and create inputString
    while (choiceCtr < 10):
        inputString = inputString + (random.choice('abcdefghijklmnopqrstuvwxyz'))  #https://docs.python.org/2/library/random.html
        choiceCtr += 1
    
    #Add "\n" to string, then write to and close file
    inputString = inputString + "\n"    
    fo.write(inputString);
    fo.close()
    
    #Print random letter strings to screen, with file name to compare with
    print "%s: %s" % (fname[count], inputString)
    
    #Increment outer loop counter, reset inner loop counter and inputString
    count+=1
    choiceCtr = 0
    inputString = ""

#Create two random integers between 1 and 42, then store multiplication product
int1 = random.randint(1, 42)
int2 = random.randint(1, 42)
product = int1 * int2

#Print values to screen
print "Integer 1: %d\n" % int1
print "Integer 2: %d\n" % int2 
print "Product of Int 1 and Int 2: %d\n" % product 
