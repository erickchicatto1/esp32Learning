/* Examples for practicing the esp-idf framework

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>

//1. Define a structure
typedef struct Person{
   char FirstName[20];
   char lastName[20];
   int age;
}Person;

//4. Make a function to pass the values of the structure
void PersonValue(Person *person){
  strcpy(person->FirstName,"Erick");
  stpcpy(person->lastName,"Lopez");
  person->age = 32;

   //Print in one line 
  printf("person : %s %s is %d years old \n",person->FirstName,person->lastName,person->age);
}

//5. Copy ! into the structure
void exclamIt(char* phrase){
   strcat(phrase,"!");
   printf("Function output %s\n", phrase);
}


void app_main(void)
{
   //2. Use the structure
   Person person;
   char phrase[20]={"Hello"};

   exclamIt(phrase);
   PersonValue(&person);

   //My mistakes , you need a function strcpy
   //person.age =20;
   //person.FirstName = "Erick";
   //person.LastName = "Lopez";

   //3. Print the elements of the structure , you can print in one line 
   //printf("This is the firstname  %s", person.FirstName);
   //printf("This is the last name  %s", person.LastName);
   //printf("This is the age  %d", person.age);

}
