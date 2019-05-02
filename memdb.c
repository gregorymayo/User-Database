//Assignment 3
//CS 149 - Operating System, Professor Ben Reed
//Name : Gregory Mayo

/*
In this assignment, I want to implement a next fit policy. 
The reasons why I want to implement the next fit policy because next fit is easy to implement.
Then I believe that the next fit policy has a faster time complexity compared to the other policy.
There's a lot of advantages using the next fit policy.

In this assignment, I got help from Zizhen Huang, he helped me to have a better understanding of the add list. Hs explanation he is very clear.
Baohong Suen gives me some hints about what I miss.
Andres Cortes helped me with giving some explanation about some error, and he explain too about the add list.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <zconf.h>
#include <unistd.h>
#include <sys/types.h>
#include "memdb.h"
#define MAX 1024  //maximum characters in the word


int main(int argc, char **argv)
{
    //To check the input, if it is contain the file
    if (argc < 2) {
        printf("USAGE: %s dbfile\n", argv[0]);
        exit(1);
    }
    //Opening the with the read and write perimeter
    int fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror(argv[1]);
        exit(2);
    }
    //Declaring mmap with maximum size
    struct fhdr_s *fhdr = mmap(NULL, MAX_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (fhdr == (void *) -1) {
        perror("mmap");
        exit(3);
    }
    struct stat s;
    if (fstat(fd, &s) == -1) {
        perror("fstat");
        exit(4);
    }
    /* figure out the address of the end of the file */
    void *end_of_file = (char *) fhdr + s.st_size;
    off_t size = s.st_size;
    	if (size == 0) {
		//Making new files
		if (ftruncate(fd, 1024) == -1) {
            		//perror("ftruncate");
            		exit(8);
        	}
        	truncate(argv[1],MAX_SIZE);
		size = INIT_SIZE;
		fhdr->magic = FILE_MAGIC;
		// no strings, so point to the end of the header
        	fhdr->data_start = sizeof(*fhdr);
    	} else {
        	if (fhdr->magic != FILE_MAGIC) {
			fprintf(stderr, "bad magic: not a strings file!\n");
            		exit(7);
        	}
    	}
	//stores each word
	char word[MAX]={};
	char value[MAX]={};
	char *ptr = (char *)fhdr;
	struct entry_s *entry = (struct entry_s *) ((char *) fhdr + sizeof(*fhdr));
	//To get an infinite loop until the users quite	
	while(argc==2){
		//To get the users input
		scanf("%[^\n]%*c",word);
		//Check which option users want to use
		if(word[0] == 'l' ){
			//Going to the initial value
			ptr += fhdr->data_start;
			if(fhdr->data_start > 0){
				int check = 1;
				while(check==1){
					ptr = (char *) fhdr;
					struct entry_s *newCurr = (struct entry_s *) ptr;
				//If the file doesn't have a magic number or doesn't match					
					if(newCurr->magic != ENTRY_MAGIC_DATA)
						break;
					else {
				//Printing the string
						printf("%s\n", entry->str);
				//Going to the next pointer						
						if(entry->next == 0)
							break;
						else
							ptr += entry->next;
					}
				}
			}
		} else if (word[0] == 'a') {
			char *get = strtok(word," ");
			char *value = strtok(NULL," ");//To get the value after the space
			int new_end = fhdr->data_start + strlen(value) + 1;
			while (new_end > size) {
			    size += 1024;
			    if (ftruncate(fd, size) == -1) {
				perror("ftruncate");
				exit(7);
			    }
			}
			//To copy the string from using string tokken
			strcpy(entry->str,value);
			//Since we need to count \n, I put it + 1 when counting the length
			entry->len = strlen(entry->str) + 1;
			entry->magic = ENTRY_MAGIC_DATA;
			//This when you put the string at the first time			
			if(fhdr->data_start == 0){
				fhdr->data_start = fhdr->free_start;
				//The old free start will be replace by new one.
				fhdr->free_start = fhdr->free_start + sizeof(*entry) + entry->len;
			}
			entry->next = fhdr->free_start;
			char nowValue[50];
			char curValueInList[50];
			strcpy(nowValue,entry->str);
			while(entry->next != 0){//parameter to check the string, 
				strcpy(curValueInList,entry->str);
				//Using strcmp to compare the string				
				int result = strcmp(nowValue,curValueInList);				
				if(result < 0){ 
			//If the result is less than 0, it means that nowValue is less than the value in the curValueIn List
			//Case 1 value replace the head, we only update the data start when the head change
					fhdr->data_start = fhdr->free_start;
					entry->next = fhdr->free_start;
				} else if( result > 0){ 
			//Case 2 value is in the middle
			//If the result is bigger than 0 it means the curValueInList is bigger than the nowValue
					fhdr->free_start = fhdr->free_start + sizeof(*entry) + entry->len;
					printf("Second test case\n");
			//Case 3 when there is a same value
				} else if( result == 0 ){ 
					printf("You shouldn't put the same value\n");
					//I am plaaning to delete this value
					//delete(nowValue);
				}
			}
		} else if (word[0] == 'd') {
			char *get = strtok(word," ");
			char *value = strtok(NULL," ");//To get the value after the space
			if (ftruncate(fd, size) == -1) {
				perror("ftruncate");
				exit(7);
			}
			//
		} else {
			printf("Should be 'a' string_to_add\n 'd' string_to_delete\n 'l' \n");
		}
	}

}
