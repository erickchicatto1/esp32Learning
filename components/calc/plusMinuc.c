#include<stdio.h>


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
/*
int main() {
  
  int testVal[5]={1,1,0,-1,-1};
  int n = 5;
  plusMinus(n,testVal);
  
  return 0;
}
 */
