# Operating-Systems-Processes
Process creation and communication in Unix using fork, wait, pipe, kill, signal.

Files are used to pass information from child processes to parent process. Program is executed as *part1 n* to use the first *n* input files in the file order, each named as input[i].txt. Parent process creates *n* children processes. Each child process sorts the numbers in the input file and writes the intermediate output file. 
  
## Input File Format:
[m] //number of numbers <br />
number_1 number_2 number_3 ... number_m  //unsorted numbers
  
## Output File Format:
[m] //number of numbers <br />
sortednumber_1sortednumber_2 sortednumber_3 ... sortednumber_m  //sorted numbers <br />
[execution time in seconds] //execution time of the process <br />
[name of the signal received, e.g., SIGUSR2, and receive time, e.g., 11:37:44] //signal received information

