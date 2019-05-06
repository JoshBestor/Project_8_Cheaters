To use the makefile, type make into the console. It will create a program titled "plagiarismCatcher". To use this program, type ./plagiarismCatcher 'path/to/files' 'x' 'y'. The path to files is how to reach the files that you want to test for plagiarism. The variable 'x' is how many words you want per chunk of text to test and the variable 'y' is the minimum amount of collisions that you concider plagiarism.

The output is in the form of:
file1, file2 - num
where file1 and file2 are the plagiarising files and num is the number of collisions between them.

It currently works for the small document set but goes into an infinite loop during the medium set.