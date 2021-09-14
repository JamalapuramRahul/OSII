#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/* Name: Rahul Jamalapuram
* email: rahul.jamalapuram@okstate.edu 
* id: A20303740
*/
int main(int argc,char* argv[]){

	  if(argc!=4){
		printf("not enough arguments");
		exit(0);
	  }
	  else{
      //Declaration of variables reference string and pages and modes
      char referenceString[100],testString[100],modes[100],mode;
      int count=0, pageFaults = 0, m, n, s,repAlgo,nowrites;
      int  pages,pageRef,frames;
      int c=0,cref,d,f,j,k=0;
      int r,t,temp2[20],temp1[20];   //temporary variables to hold the counter values 
      
      FILE *test_file = fopen(argv[1], "r");  //open the file in read mode

      frames=atoi(argv[2]);                  //getting the number of frames from CLA's

    //getting the type of algorithm from CLA
      if (strcmp(argv[3], "lru") == 0) {
		repAlgo = 1;
    }
    
      else if (strcmp(argv[3], "fifo") == 0) {
		repAlgo = 0;
	}

      int i=0;
      //storing the values from file to the arrays.
      while (fscanf(test_file, "%d %c\n",&pageRef, &mode) != EOF){

	    referenceString[i]=pageRef;
	    modes[i]=mode;
	    
	    if(mode=='W'){
	       nowrites++;
	    }
	    i++;
	    count++;
      }

      pages=count;

      int temp[frames];                                             //creating a frame array to store the frames
      //this is for the FIFO page replacement algorithm
      if(repAlgo==0){
      for(m = 0; m < frames; m++){
            temp[m] = -1;                                           //the initial values be -1
      }
      
      for(m = 0; m < pages; m++){
        s = 0;
        for(n = 0; n < frames; n++){
                
            if(referenceString[m] == temp[n]){                      //if the part of reference string is already present reducing the page fault.
                s++;
                pageFaults--;
            }
        }
            
        pageFaults++;                                               //incrementing page faults for every insertion into the frame array
            
        if((pageFaults <= frames) && (s == 0)){
            temp[m] = referenceString[m];
        }   
            
        else if(s == 0){
            temp[(pageFaults - 1) % frames] = referenceString[m];    //tvictim part of the array to be removed from the frame array as it is FIFO.
        }
            
      } 

      printf("the final frame is:");                                    //output
      
      for(int i=0;i<frames;i++){
          printf("%d",temp[i]);
      }

      }
      
      //this part goes for lRU page replacement algorithm
      else{

      	testString[k]=referenceString[k];                   //making a copy of the given reference string for comparison
	
	   // printf("\n\t%d\n",testString[k]);
	
	    pageFaults++;
	
	    k++;

	    for(i=1;i<pages;i++){
               
	       	cref=0;
               
	       	for(j=0;j<frames;j++){
	       	    
			if(referenceString[i]!=testString[j])               //ccomparing both the strings based on no of pages and no of frames
                cref++;
            }
                       
	       	if(cref==frames){
                    
                pageFaults++;                                   //incrementing the number of page faults
                     
		     	if(k<frames){
                           	
                    testString[k]=referenceString[i];           //if there is no element insert it into frames
                    k++;
                }
                    
		    	else{
                     
			   	   for(r=0;r<frames;r++){
			   	       
                    temp1[r]=0;
                  	
                  	for(j=i-1;j<pages;j--){
                                
                        if(testString[r]!=referenceString[j])     //comparing both the elements and storing in the temporary array as to be included into the frames array.
                            temp1[r]++;
                                
                        else
                            break;
                    }
                }
                     
		        for(r=0;r<frames;r++)
                    temp2[r]=temp1[r];
                    
		   		for(r=0;r<frames;r++){
                   
		   			for(j=r;j<frames;j++){
                   
			   			if(temp2[r]<temp2[j]){                  //Swapping the element with other so as to include the page to the frame and find the victim page in the frame
							
							t=temp2[r];
                            temp2[r]=temp2[j];
                            temp2[j]=t;
                        }
                    }
                }
                           
			  	for(r=0;r<frames;r++){
                          
			   		if(temp1[r]==temp2[0])                      //swapping the victim page in frames.
                           
				  		testString[r]=referenceString[i];
                }
                                
            }
        }
	}
	
	printf("the final frame is:");                      //printing the frame.
	for(int k=0;k<frames;k++){
		printf("%d",testString[k]);
      }
	printf("\n");
      }
	printf("total number of reads: %d", pageFaults);        //printing the number of reads and writes
	printf("total number of writes: %d",nowrites);
    return 0;
}
}
