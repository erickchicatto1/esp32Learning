#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

void miniMaxSum(std::vector<int>arr){
  
  long int min=0,max=0;
  int lenght = arr.size();
  
  //Sort the vector
  sort(arr.begin(),arr.end());
  
  //Calculate the min
  for(int i=0;i<4;++i){
    min += arr[i];  
  }
  
  //Calculate the max
  for(int i=lenght-1;i>=lenght-4;--i){
   max += arr[i];
  }
  
}
/*
int main() {
  
  //Example test case
  std::vector<int> arr = {1,3,5,7,9};
  miniMaxSum(arr);
  
  return 0;
}
*/
