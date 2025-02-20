#include<stdio.h>

//Exercise one
void plusMinus(int arr_count, int *arr){
  
  int posCount=0;//3.PositiveCounter
  int negCount=0;
  int zeroCount=0;
  float resultPos=0;
  float resultNeg=0;
  float resultZero=0;
  
  //1. print all the elements 
  for(int i = 0 ; i<arr_count;i++){
    printf("This is the values %d", arr[i]);
    //2. Check if mayor to cero
    if(arr[i] > 0){
      posCount++;
    }
    else if(arr[i] < 0){
      negCount++;
    }
    else if(arr[i] == 0){
      zeroCount++;
    }
    
    
  }
  resultPos = (float)posCount/arr_count;
  resultNeg = (float)negCount/arr_count;
  resultZero = (float)zeroCount/arr_count;
  
}


//Exercise two 
// Function to get the min and max sum of 4 elements in an array
void minMaxSum(int *array, int length) {
    if (length < 2) {
        printf("At least 2 elements are required.\n");
        return;
    }

    // Bubble Sort to sort the array
    for (int i = 0; i < length - 1; ++i) {
        for (int j = 0; j < length - i - 1; ++j) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
        }
    }

    // Calculate the min sum (sum of the first 4 elements)
    long int minSum = 0, maxSum = 0;
    for (int i = 0; i < 4; ++i) {
        minSum += array[i];
    }

    // Calculate the max sum (sum of the last 4 elements)
    for (int i = length - 4; i < length; ++i) {
        maxSum += array[i];
    }

    // Print results
    printf("Min sum: %ld, Max sum: %ld\n", minSum, maxSum);
}



//Exercise tree just study all 
/* 
-----------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char* timeConversion(char* s) {
    char time;
    //. Define the cases
    switch(time){
      case '0':
          s = s + 'AM';
      case 'PM':
          s = s + 12 + 'PM';
          
    }
  
  return s;    
}


int main() {
    
  //1. Create the input 
  char exampTime = {'0','7','0','5','4','5','P','M'};
  //2. how to use the function?
  timeConversion(exampTime);
  
  return 0;
}

---------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>


void convertTo24HourFormat(int *hh , int *mm  ,int *ss , const char *a){
  
  if(*hh>=1 && *hh <=12 && *mm>=0 && *mm<=59 && *ss >=0 && *ss<=59){
    //AM or PM
    if( (strcmp(a,"PM")== 0 || strcmp(a,"pm")==0) && *hh !=12){
     *hh = *hh + 12;
    }
    
     if( (strcmp(a,"AM")== 0 || strcmp(a,"am")==0) && *hh ==12){
     *hh = 0;
    }
    
    printf("The obtained 24-hour format of input is: \t");
    printf("%02d:%02d:%02d\n", *hh, *mm, *ss);
    
  }
  
  else{
    printf("Invalid input for time values.\n");  
  }

}

int main() {
  //to can work from main , we use pointers
  int hh = 7,mm=5,ss=45;
  char a[]="PM";
  convertTo24HourFormat(&hh,&mm,&ss,a);
  
     
  return 0;
}

---------------------------------------------------------------------------

char* timeConversion(char* s) {
    static char result[9];  
    int hh, mm, ss;
    char period[3];  

    hh = (s[0] - '0') * 10 + (s[1] - '0');  
    mm = (s[3] - '0') * 10 + (s[4] - '0');  
    ss = (s[6] - '0') * 10 + (s[7] - '0');  
    period[0] = s[8];  
    period[1] = s[9];  
    period[2] = '\0';  

    if (strcmp(period, "PM") == 0 && hh != 12) {
        hh += 12;  
    } else if (strcmp(period, "AM") == 0 && hh == 12) {
        hh = 0; 
    }

    result[0] = (hh / 10) + '0';  
    result[1] = (hh % 10) + '0';  
    result[2] = ':';  
    result[3] = (mm / 10) + '0';  
    result[4] = (mm % 10) + '0';  
    result[5] = ':';  
    result[6] = (ss / 10) + '0';  
    result[7] = (ss % 10) + '0';  
    result[8] = '\0';  

    return result;  
}


---------------------------------------------------------------------


*/

/*
int main() {
  
  //Ex.1
  int testVal[5]={1,1,0,-1,-1};
  int n = 5;
  plusMinus(n,testVal);

  //Ex.2
  int array[5] = {1, 3, 5, 7, 9}; // Test array
  int length = sizeof(array) / sizeof(array[0]);

  minMaxSum(array, length);

  return 0;
}

 */
