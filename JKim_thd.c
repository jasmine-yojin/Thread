
/*===============================================================
gcc JKim_thd.c -o JKim_thd.exe -lpthread					                                                  
./JKim_thd.exe 1000000 10 > JKimResultC.txt                                                                                            
./JKim_thd.exe 1000000 50 50 > JKimResultG.txt                                                                               
================================================================*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int finalSum[50];
void *childRunner (void* param);
int cadd;//this variable represents workload each child receives.

long int childSum[50];
void *slaveRunner (void *sparam);
int gadd;//this variable represents workload each grandchild receives.


int main (int argc, char *argv[]){
	if (argc < 3){
		printf("Wrong number of command line arguments. Include target number and how many threads to create\n");
		return 1;
    }
    
	printf("Parent: Start of Execution\n");
	
	//Converting string into integer and integer into string
	int target= atoi(argv[1]);
	int nthread= atoi(argv[2]);
	cadd = target/nthread;
	char strCAdd[10];
	sprintf (strCAdd, "%lld", cadd);
	
	//Creating string parameter for runner function. Then adding initial values for index, start, end, and each workload. 
	char parameter [20];
		//part2
		if (argc==3){
			strcpy (parameter, "0");
			strcat(parameter, ",");
			strcat(parameter,"1");
			strcat(parameter, ",");
			strcat(parameter, strCAdd);
		}
		//part3
		else if (argc==4){
			strcpy (parameter, "0");
			strcat(parameter, ",");
			strcat(parameter,"1");
			strcat(parameter, ",");
			strcat(parameter, strCAdd);
			strcat(parameter, ",");
			strcat(parameter, argv[3]);
		}
		
		//Creating Child Thread Array
		pthread_t tid[nthread];
		
		//Running the child threads
		int i;
		for (i=0; i<nthread; i++){
			pthread_create(&tid[i], NULL, childRunner, &parameter);
			pthread_join (tid[i], NULL);
		}
		
		//Summing up the work of child threads
		long int fsum=0;
		for (i=0; i<nthread; i++){
			fsum+= finalSum[i];
		}
		
	printf("Total sum is %ld\n", fsum);
	printf("Parent: End of Execution %ld\n", fsum);
		
	return 0;	
}

void *childRunner (void *param){
		//parsing the string by using "," as delimiter. NULL will get you the next pointer. 
		char* indexptr= strtok(param,",");//index. strtok returns a char pointer of each occurence.
		char* startptr = strtok(NULL,",");//start
		char* endptr = strtok (NULL,",");//end
		char* nGthreadptr = strtok (NULL,",");// number of grandchild thread- if it is null, then run part2. If not null, run part 3
		
		//converting each string into integer.
		int cindex = atoi (indexptr);
		int start = atoi (startptr);
		int end= atoi(endptr);	
		
		long int csum=0;
		if(nGthreadptr==NULL){ //part2
		
			//Summation	
			int i;
			for (i=start; i<=end; i++){
				csum+= i;
			}
			//update the finalSum array
			finalSum[cindex]= csum;
			printf("Child thread number %d\t child start= %d\t end= %d\t sum= %ld\n", cindex+1, start, end, csum);
			
			//Update the parameter- numerically. Math operation on index, start, and end integer values
			cindex++;
			start= (cindex*cadd)+1;
			end= (cindex+1)*cadd;
			
			//converting integer values into string to be injected into the parameter for the next child thread.
			char cindexchar[10];
			char startchar[10];
			char endchar[10];
			sprintf (cindexchar, "%lld", cindex);
			sprintf(startchar, "%lld", start);
			sprintf(endchar, "%lld", end);
			
			//Update the parameter- string-wise
			strcpy(param, "\0");//emptied the string.
			strcpy(param, cindexchar);
			strcat(param, ",");
			strcat(param, startchar);
			strcat(param,",");
			strcat(param, endchar);
			
			
		}else { //part3
			//Converting string into integer and integer into string
			int nGthread= atoi(nGthreadptr);
			gadd = cadd/nGthread;//workload each grandchild will get
			char strGAdd[10];
			sprintf (strGAdd, "%lld", gadd);
			//Creating Grandchild Thread Array
			pthread_t tid[nGthread];
			
			//Creating string parameter for grandchild/slave runner function. Setting values for start and end.
			char sparameter[20];
			int gstart=start;
			int gend=start-1+gadd;
			
			//Running the grandchild/slave threads.
			int i;
			for (i=0; i<nGthread; i++){ //i = sindex
				//int i = grandchild index
				//Converting grandchild index, start, end, and child index into string.
				char sindexchar[10];
				sprintf(sindexchar, "%lld", i);
				char startchar[10];
				sprintf(startchar, "%lld", gstart);
				char endchar[10];
				sprintf(endchar, "%lld", gend);
				char cindexchar[10];
				sprintf (cindexchar, "%lld", cindex);
				//Inserting the strings into the parameter.
				strcpy (sparameter, sindexchar);
				strcat(sparameter, ",");
				strcat(sparameter,startchar);
				strcat(sparameter, ",");
				strcat(sparameter,endchar);
				strcat(sparameter, ",");
				strcat(sparameter,cindexchar);
				
				//Run the grandchild threads
				pthread_create(&tid[i], NULL, slaveRunner, &sparameter);
				pthread_join (tid[i], NULL);
				//update the start and end values for the next grandchild thread
				gstart= gstart+gadd;
				gend=gend+gadd;
			}
			//Summing up the work of grandchild/slave threads
			for (i=0; i<nGthread; i++){
				csum+= childSum[i];//the childSum array is updated in the grandchild thread runner function.
			}
			//update the finalSum array
			finalSum[cindex]= csum;
			printf("Child thread number %d\t child start= %d\t end= %d\t sum= %ld\n", cindex+1, start, end, csum);
			
			//Update the child parameter- numerically. Math operation on index, start, and end integer values
			cindex++;
			start= (cindex*cadd)+1;
			end= (cindex+1)*cadd;
			
			//converting integer values into string to be injected into the parameter.
			char cChildchar[10]; 
			char startchar[10];
			char endchar[10];
			char nGrandchild[10];
			sprintf(cChildchar, "%lld", cindex);
			sprintf(startchar, "%lld", start);
			sprintf(endchar, "%lld", end);
			sprintf(nGrandchild, "%lld", nGthread);
			
			//Update the child parameter- string-wise
			strcpy(param, "\0");//emptied the string.
			strcpy(param, cChildchar);
			strcat(param, ",");
			strcat(param, startchar);
			strcat(param,",");
			strcat(param, endchar);
			strcat(param,",");
			strcat(param, nGrandchild);
		}
		
	pthread_exit(0);
}

void *slaveRunner (void *sparam){
		//parsing the string by using "," as delimiter
		char* sindexptr= strtok(sparam,",");//grandchild index. returns a char pointer of each occurence.
		char* startptr = strtok(NULL,",");//start
		char* endptr = strtok (NULL,",");//end
		char* cindexptr = strtok (NULL,",");//child index
	
		//converting each string into integer.
		int sindex = atoi (sindexptr);
		int start = atoi (startptr);
		int end= atoi(endptr);	
		int cindex= atoi(cindexptr);	
	
		//Summation	
		int i;
		long int gsum = 0;
		for (i=start; i<=end; i++){
			gsum+= i;
		}
		childSum[sindex]= gsum;//update the childSum array
		printf("Child thread number, %d\t GrandChild thread number:%d\t grandchild start= %d\t grandchild end= %d\t grandchild sum= %ld\n", cindex+1, sindex+1, start, end, gsum);
			
		pthread_exit(0);	
}
