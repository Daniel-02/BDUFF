#include <stdio.h>
#include <stdlib.h>
typedef struct {
   char id_tab_origem[1000];    //nome da tabela de origem
   char id_atr[1000];           //nome do atributo      
   char val_atr[1000];          //valor do atributo
   char id_tab_saida[1000];     //nome da tabela de saida
} com_selecao;                  //guardará as leituras dos parâmetros dos comandos de seleção

/*int e_chave(char atr[1000]){    //retorna 1 se um atributo é chave
	int i = 0;
	while(atr[i]!=','){
		i = i+1;
	}
	char temp[2];
	while(atr[i]!='\0'){
		temp[0] = atr[i];
		temp[1] = atr[i+1];
		temp[2] = atr[i+2];
		if(strcmp(temp,"chv")== 0) return 1;
		i = i+1;
	}
	return 0;
}*/
int chega_coluna(char tup[1000], int atr){  //percorre a tupla e retorna a posição de começo do atributo
	if(atr == 0) return 0; int i = 0; int atual = 0; while(atual < atr){
		if(tup[i] == ',') atual = atual + 1;
		i = i+1;
	}
	return i;
}

int libera(com_selecao * com){               //zera os vetores e da free no malloc
	int i;
	for(i=0;i<1000;i++){
		com->id_tab_origem[i]='\0';
		com->id_atr[i]='\0';
		com->val_atr[i]='\0';
		com->id_tab_saida[i]='\0';
	}
	free(com);
}

int main(void){
 char nome_arq[1000];                   //receberá nome do arquivo a ser aberto
 FILE * f_alg = fopen("T1.alg", "r");   //abre o arquivo com os comandos 
 char c[3];                             //receberá a tag do comando
 char temp;                             //guardará o caracter do arquivo temporariamente
 while(1){
    if(feof(f_alg)) break;
    fread(&c, 3,1,f_alg);               //lê e guarda a tag do comando
    c[2] = '\0';
   // printf("\n$$%s$$",c);
    if(c[0]=='S'){                      //se é seleção
      com_selecao* comando;
      comando =  (com_selecao*)malloc(sizeof(com_selecao));
      int i = 0;				   //inicializa contador 
      while(temp!=','){            //pega a tabela de origem      
        fread(&temp,1, 1, f_alg);  //pega um caracter do arquivo
        if(temp!=','){                    
     	  comando->id_tab_origem[i] = temp;   //guarda o caracter se ainda não chegou na vírgula  
		  i = i+1;
		}
	  }
	  comando->id_tab_origem[i+1] = '\0';
	  printf("%s ",comando->id_tab_origem);
	  i = 0;
      temp ='\0';
      while(temp!=','){             //pega o nome do atributo
        fread(&temp,1,1,f_alg);
        if(temp!=','){
		  comando->id_atr[i] = temp;
		  i = i+1;
		}
      }
      comando->id_atr[i+1] = '\0';
	  printf("%s ",comando->id_atr);
	  i = 0;
      temp ='\0';
      while(temp!=','){             //pega o valor do atributo
        fread(&temp,1,1,f_alg);
        if(temp!=','){
		  comando->val_atr[i] = temp;
		  i = i+1;
		}
      }
      comando->val_atr[i+1] = '\0';
	  printf("%s ",comando->val_atr);
	  i = 0;
      temp ='\0';
      while(temp!=')'){             //pega o nome da tabela de saída
        fread(&temp,1,1,f_alg);
        if(temp!=')'){
		  comando->id_tab_saida[i] = temp;
		  i = i+1;
		}
	  }
	  fread(&temp,1,1,f_alg);
	  comando->id_tab_saida[i+1] = '\0';
	  printf("%s\n",comando->id_tab_saida);
	  snprintf(nome_arq,sizeof(nome_arq),"%s.ctl",comando->id_tab_origem);   //bota o nome do arquivo no formato do fopen
	  FILE * f_origem_ctl = fopen(nome_arq,"r");                             //abre o arquivo de origem.ctl
	  int n_colunas = 0;
	  fread(&temp,1,1,f_origem_ctl);
	  n_colunas = temp - '0';                   //grau da relação
	  fread(&temp,1,1,f_origem_ctl);
  	  fread(&temp,1,1,f_origem_ctl);
      char atributo[1000];                     //guarda uma linha de atributo do ctl
      int col_atr;                             //coluna em que o atributo se encontra na relação
      for (i=0;i<=n_colunas;i++){              //percorre a relação de origem
		fgets(atributo,1000,f_origem_ctl);
		if(strncmp(comando->id_atr,atributo,strlen(comando->id_atr))== 0){      //compara se o atributo do comando é igual ao atributo lido
			col_atr = i-1;                     //salva a coluna do atributo do comando
			break;
		}
	  }
//	  int chv = e_chave(atributo);
	  rewind(f_origem_ctl);
	  fgets(atributo,1000,f_origem_ctl);	
	  snprintf(nome_arq,sizeof(nome_arq),"%s.dad",comando->id_tab_origem);   //bota o nome do arquivo no formato do fopen
	  FILE * f_origem_dad = fopen(nome_arq,"r");                             //abre o arquivo de origem.dad
	  snprintf(nome_arq,sizeof(nome_arq),"%s.dad",comando->id_tab_saida);    
	  FILE * f_destino_dad = fopen(nome_arq,"w");	                         //abre o arquivo de saída.dad
	  char tupla[1000];                         //salva uma tupla da relação origem
	  char valor_atributo_atual[1000];
	  int card_saida = 0;
	  if(c[1] == '1'){ 							//atributo = valor do atributo do comando
		while(1){ 
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);                                     //pega a posição em que o atributo começa na tupla
			strncpy(valor_atributo_atual,tupla+i,strlen(comando->val_atr));    //copia o atributo para um outro vetor
			if(feof(f_origem_dad)) break;
			if(strncmp(comando->val_atr,valor_atributo_atual)==0){             //compara a cópia com a valor do comando
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
	  }
	  if(c[1] == '2'){		                    //atributo < valor do atributo do comando
		  while(1){
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,strlen(comando->val_atr));
			if(feof(f_origem_dad)) break;
			if(strncmp(valor_atributo_atual,comando->val_atr)<0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		 }
	  }
	  if(c[1] == '3'){                          //atributo <= valor do atributo do comando
		  while(1){
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,strlen(comando->val_atr));
			if(feof(f_origem_dad)) break;
			if(strncmp(valor_atributo_atual,comando->val_atr)<=0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		 } 
	  }  
	  if(c[1] == '4'){                           //atributo > valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,strlen(comando->val_atr));
			if(feof(f_origem_dad)) break;
			if(strncmp(valor_atributo_atual,comando->val_atr)>0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
	  }  
	  if(c[1] == '5'){                         //atributo >= valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,strlen(comando->val_atr));
			if(feof(f_origem_dad)) break;
			if(strncmp(valor_atributo_atual,comando->val_atr)>=0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
      }
	  if(c[1] == '6'){                       //atributo != valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,strlen(comando->val_atr));
			if(feof(f_origem_dad)) break;
			if(strncmp(comando->val_atr,valor_atributo_atual)!=0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
	  }
      snprintf(nome_arq,sizeof(nome_arq),"%s.ctl",comando->id_tab_saida);    //bota o nome do arquivo no formato do fopen
	  FILE * f_saida_ctl = fopen(nome_arq,"w");                              //abre o arquivo de saida.ctl
      fputc((char)('0'+n_colunas),f_saida_ctl);								 //escreve o grau da relação em saida.ctl
      fputc(',',f_saida_ctl);
      fputc((char)('0'+card_saida),f_saida_ctl);                             //escreve a cardinalidade da relação em saida.ctl
      fputc('\n',f_saida_ctl);
      for(i=0;i<n_colunas;i++){              //percorre a relação de origem.ctl e preenche saida.ctl
		fgets(atributo,1000,f_origem_ctl);
	    fputs(atributo,f_saida_ctl);
	  }
	  fclose(f_origem_ctl);
      fclose(f_origem_dad);
      fclose(f_destino_dad);
      fclose(f_saida_ctl);
      libera(comando);                      
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
 fclose(f_alg);
 printf("\nsucesso\n");
 return 0;
}
