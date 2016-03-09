#include <stdio.h>
#include <stdlib.h>
typedef struct {
   char id_tab_origem[1000];
   char id_atr[1000];
   char val_atr[1000];
   char id_tab_saida[1000];
} com_selecao;

int main(void){
 FILE * f = fopen("/home/daniel/Desktop/Trabalho 2/T1.alg", "r");
   char c[3];
   char temp;
   if(!feof(f)){
      fread(&c, 3, 1, f);
      c[2] = '\0';
      if(c[0]=='S'){ 
        com_selecao* comando;
        comando =  (com_selecao*)malloc(sizeof(com_selecao));
        fread(&temp, 1, 1, f);
        int i = 0;
	do{
          fread(&temp,1, 1, f);
          comando->id_tab_origem[i] = temp;
	  i = i+1;
         }
        while(temp!=',');
        if(c[1] ='1'){}
	if(c[1] ='2'){}
	if(c[1] ='3'){}
	if(c[1] ='4'){}
	if(c[1] ='5'){}
	if(c[1] ='6'){}
      }

      if(c[0]=='O'){ 
        if(c[1] ='1'){}
        if(c[1] ='2'){}
        if(c[1] ='3'){}
        if(c[1] ='4'){}
        if(c[1] ='5'){}
        if(c[1] ='6'){}
      }  
      if(c[0]=='J'){ 
        if(c[1] ='1'){}
        if(c[1] ='2'){}
      }  
      if(c[0]=='P'){ 
        if(c[1] ='1'){}
        if(c[1] ='2'){}
        if(c[1] ='3'){}
        if(c[1] ='4'){}
      }  


  
   }
 printf(" sucesso\n");
 return 0;
}
