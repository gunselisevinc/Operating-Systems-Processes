#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void swap(int*, int*);
void selectionSort(int[], int);
void insertionSort(int[], int);
void printArray(int[], int);
void signalHandler(int);

char signalSent[10];

int main(int argc, char *argv[])
{
	int i, pid;
	int numOfFiles = atoi(argv[1]);
	char inputFileNames[numOfFiles][20];
	char outputFileNames[numOfFiles][20];
	FILE *fpInput, *fpOutput;
	int m=0, size, sleep_time;
	int* arr;
	int nums[50];
	double executionTime;
	char childProcessArr[numOfFiles][4];
	
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	
	//input file names in an array
	for(i=0 ; i<numOfFiles ; i++)
	{
		sprintf(inputFileNames[i], "input%d.txt", i+1);
	}
	//output file names in an array
	for(i=0 ; i<numOfFiles ; i++)
	{
		sprintf(outputFileNames[i], "output%d.txt", i+1);
	}
	
	printf("NumOfFiles: %d\n\n", numOfFiles);
	
	//child process creation
	for(i=0 ; i<numOfFiles ; i++)
	{
		pid = fork();
		if(pid < 0)
		{
			printf("ERROR OCCURED DURING CHILD CREATION\n");
			exit(1);
		}
		else if(pid == 0) //Child process
		{
			printf("Child - %d: %d \tParent: %d\n", i+1, getpid(), getppid());
			
			fpInput = fopen(*(inputFileNames+i), "r"); //read mode
			if(fpInput == NULL)
			{
 		       perror("Error while opening the file.\n");
 		       exit(EXIT_FAILURE);
			}

			while((fscanf(fpInput,"%d",&nums[m]))!=EOF)
			{
				if(m==0)
				{
					//creation of array with given size in input file
					size = nums[m];
					arr = (int*) malloc(nums[m] * sizeof(int));
				}
				else
				{
					//filling the array with the numbers in the input file
					arr[m-1] = nums[m];
				}
				m++;
			}
		    fclose(fpInput);
			m=0;
			printf("%s\n", inputFileNames[i]);
			printArray(arr, size);
			
			srand(time(NULL) ^ (getpid()<<16));

			if(getpid()%2 == 0) //even --> Insertion Sort
			{
				sleep_time = rand()%7 + 1;
				clock_t begin = clock();
				insertionSort(arr, size);
				sleep(sleep_time);
				clock_t end = clock();
				executionTime = ((double)(end - begin) / CLOCKS_PER_SEC) + sleep_time;
				printArray(arr, size);
				printf("SleepTime: %dsec\n", sleep_time);
				printf("Insertion Sort Execution Time: %f\n\n", executionTime);
			}
			else //odd --> Selection Sort
			{
				sleep_time = rand()%7 + 1;
				clock_t begin = clock();
				selectionSort(arr, size);
				sleep(sleep_time);
				clock_t end = clock();
				executionTime = ((double)(end - begin) / CLOCKS_PER_SEC) + sleep_time;
				printArray(arr, size);
				printf("SleepTime: %dsec\n", sleep_time);
				printf("Selection Sort Execution Time: %f\n\n", executionTime);
			}

			fpOutput = fopen(*(outputFileNames+i), "w+"); //write mode
			if(fpOutput == NULL)
			{
 		       perror("Error while opening the file.\n");
 		       exit(EXIT_FAILURE);
			}
			
			fprintf(fpOutput,"%d\n", size);
			for(i=0 ; i<size ; i++)
			{
				fprintf(fpOutput,"%d ", arr[i]);
			}
			fprintf(fpOutput,"\n%f",executionTime);
			time_t now = time(NULL); //Taking the current time in format of HH:MM:SS
			struct tm *tm_struct = localtime(&now);
			int hour = tm_struct->tm_hour;
			int min = tm_struct->tm_min;
			int sec = tm_struct->tm_sec;
			fprintf (fpOutput,"\n%s\t%d:%d:%d", signalSent, hour, min, sec);
			fclose(fpOutput);
			exit(0);
		}
		else //Parent Process
		{
			if(pid%2==0)
			{
				kill(pid, SIGUSR2);
			}
			else
			{
				kill(pid,SIGUSR1);
			}
		}
	}
	for(i=0 ;i<numOfFiles ; i++)
	{
		wait(NULL);
	}
	printf("Parent: %d\n", getpid());
	
	fpOutput = fopen("parentOutput.txt", "w+");
	
	for(i=0 ; i<numOfFiles ; i++)
	{
		int lineNum=0;
		fpInput = fopen(*(outputFileNames+i), "r"); //read mode
		fprintf(fpOutput,"%s\n=============\n", *(outputFileNames+i));
		char line[100];
		while(fgets(line, sizeof(line), fpInput))
		{
			printf("%s", line);
			fprintf(fpOutput,"%s", line);
		}
		fclose(fpInput);
		fprintf(fpOutput,"\n\n");
	}
	fclose(fpOutput);
	
	return 0;
}

void signalHandler(int signalInput)
{
	char buffer[10];
	
	if(signalInput == SIGUSR1)
	{
		strcpy(buffer,"SIGUSR1");
		strcpy(signalSent, buffer);
		printf("SIGUSR1\n");
	}
	else
	{
		strcpy(buffer,"SIGUSR2");
		strcpy(signalSent, buffer);
		printf("SIGUSR2\n");
	}
}
void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
  
void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
	
    for (i = 0; i < n-1; i++)
    {
        min_idx = i;
        for (j = i+1; j < n; j++)
          if (arr[j] < arr[min_idx])
            min_idx = j;
        swap(&arr[min_idx], &arr[i]);
    }
}

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void printArray(int arr[], int size)
{
    int i;
    for (i=0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

/*
	Sort Functions are taken from internet. Below is the References:
	-Insertion Sort: https://www.geeksforgeeks.org/insertion-sort/
	-Selection Sort: https://www.geeksforgeeks.org/selection-sort/
*/