#include <stdio.h>
#include <string.h>
#include <ctype.h>

void splitCamelCase(char *s,int isMethod){

    char out[1000]="";
    int outIndex=0;

    for(int i=0;s[i] != '\0';++i){

        if(isupper(s[i])){
            if(i!=0){
             out[outIndex++]=' ';
            }
        }
        out[outIndex++]=tolower(s[i]);
    }

}




int main(){

    char test1[]="HelloWorldTest";
    printf("Entrada: %s\n", test1);
    printf("Salida esperada: HelloWorldTest\n");
    printf("Salida obtenida: ");
    splitCamelCase(test1, 0);
    printf("\n");

    return 0;
}