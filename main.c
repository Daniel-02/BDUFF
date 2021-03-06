#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
	char id_tab_origem[1000];    //nome da tabela de origem
	char id_atr[1000];           //nome do atributo      
	char val_atr[1000];          //valor do atributo
	char id_tab_saida[1000];     //nome da tabela de saída
} com_selecao;                   //guardará as leituras dos parâmetros dos comandos de seleção
typedef struct{
	char id_tab_A[1000];         //nome da tabela A
	char id_tab_B[1000];         //nome da tabela B
	char atr_A[1000];            //nome do atributo A
	char atr_B[1000];            //nome do atributo B
	char id_tab_saida[1000];     //nome da tabela de saída
} com_juncao;                    //guardará as leituras dos parâmetros dos comandos de junção

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
	if(atr == 0) return 0; 
	int i = 0;
	 int atual = 0;
	  while(atual < atr){
		if(tup[i] == ',') atual = atual + 1;
		i = i+1;
	}
	return i;
}
int pega_tam(char tup[1000],int pos){       //percorre a tupla e retorna o tamanho do valor do atributo
	int tam = 0;
	while(1){
		if(tup[pos]==',' || tup[pos]=='\n' || tup[pos]==EOF) break;
		pos = pos + 1;
		tam = tam +1;
	}
	return tam;
}

int libera_S(com_selecao * com){               //zera os vetores e da free no malloc da seleção
	memset(com->id_tab_origem,'\0',sizeof(com->id_tab_origem));
	memset(com->id_atr,'\0',sizeof(com->id_atr));
	memset(com->val_atr,'\0',sizeof(com->val_atr));
	memset(com->id_tab_saida,'\0',sizeof(com->id_tab_saida));
	free(com);
}
int libera_J(com_juncao * com){               //zera os vetores e da free no malloc da junção
	memset(com->id_tab_A,'\0',sizeof(com->id_tab_A));
	memset(com->id_tab_B,'\0',sizeof(com->id_tab_A));
	memset(com->atr_A,'\0',sizeof(com->atr_A));
	memset(com->atr_B,'\0',sizeof(com->atr_B));
	memset(com->id_tab_saida,'\0',sizeof(com->id_tab_saida));
	free(com);
}


int main(void){
 char nome_arq[1000];                   //receberá nome do arquivo a ser aberto
 FILE * f_alg = fopen("T6.alg", "r");   //abre o arquivo com os comandos 
 char c[3];                             //receberá a tag do comando
 char temp;                             //guardará o caracter do arquivo temporariamente
 while(1){
    if(feof(f_alg)) break;
    memset(c,'\0',sizeof(c));
    fread(&c, 3,1,f_alg);               //lê e guarda a tag do comando
    c[2] = '\0';
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
	  sprintf(nome_arq,"%s.ctl",comando->id_tab_origem);                     //bota o nome do arquivo no formato do fopen
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
	  sprintf(nome_arq,"%s.dad",comando->id_tab_origem);   //bota o nome do arquivo no formato do fopen
	  FILE * f_origem_dad = fopen(nome_arq,"r");                             //abre o arquivo de origem.dad
	  sprintf(nome_arq,"%s.dad",comando->id_tab_saida);    
	  FILE * f_destino_dad = fopen(nome_arq,"w");	                         //abre o arquivo de saída.dad
	  char tupla[1000];                                                      //salva uma tupla da relação origem
	  char valor_atributo_atual[1000];
	  memset(valor_atributo_atual,'\0',sizeof(valor_atributo_atual));        //zera o vetor que vai salvar o atributo lido
	  int tam;
	  tam = strlen(comando->val_atr);										 //salva o tamanho do valor do atributo
	  int card_saida = 0;
	  if(c[1] == '1'){ 							//atributo = valor do atributo do comando
		while(1){ 
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);                                     //pega a posição em que o atributo começa na tupla
			strncpy(valor_atributo_atual,tupla+i,tam);                         //copia o atributo para um outro vetor
			if(strcmp(comando->val_atr,valor_atributo_atual)==0){             //compara a cópia com a valor do comando
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
	  }
	  if(c[1] == '2'){		                    //atributo < valor do atributo do comando
		  while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)<0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		 }
	  }
	  if(c[1] == '3'){                          //atributo <= valor do atributo do comando
		  while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)<=0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		 } 
	  }  
	  if(c[1] == '4'){                          //atributo > valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)>0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
	  }  
	  if(c[1] == '5'){                          //atributo >= valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)>=0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
      }
	  if(c[1] == '6'){                          //atributo != valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(comando->val_atr,valor_atributo_atual)!=0){
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
		}
	  }
      sprintf(nome_arq,"%s.ctl",comando->id_tab_saida);    //bota o nome do arquivo no formato do fopen
	  FILE * f_saida_ctl = fopen(nome_arq,"w");                              //abre o arquivo de saida.ctl
      char temp_s[1000];
      sprintf(temp_s,"%d",n_colunas);
      fputs(temp_s,f_saida_ctl);										//escreve o grau da relação em saida.ctl
      fputc(',',f_saida_ctl);
	  sprintf(temp_s,"%d\n",card_saida);
      fputs(temp_s,f_saida_ctl);                                        //escreve a cardinalidade da relação em saida.ctl 
      for(i=0;i<n_colunas;i++){              //percorre a relação de origem.ctl e preenche saida.ctl
		fgets(atributo,1000,f_origem_ctl);
	    fputs(atributo,f_saida_ctl);
	  }
	  fclose(f_origem_ctl);
      fclose(f_origem_dad);
      fclose(f_destino_dad);
      fclose(f_saida_ctl);
      libera_S(comando);                      
    }
    if(c[0]=='O'){                 //se é seleção ordenada
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
	  sprintf(nome_arq,"%s.ctl",comando->id_tab_origem);   						//bota o nome do arquivo no formato do fopen
	  FILE * f_origem_ctl = fopen(nome_arq,"r");                             	//abre o arquivo de origem.ctl
	  int n_colunas = 0;
	  fread(&temp,1,1,f_origem_ctl);
	  n_colunas = temp - '0';                   							    //grau da relação
	  fread(&temp,1,1,f_origem_ctl);
  	  fread(&temp,1,1,f_origem_ctl);
      char atributo[1000];                     									//guarda uma linha de atributo do ctl
      int col_atr;                           								    //coluna em que o atributo se encontra na relação
      for (i=0;i<=n_colunas;i++){             									//percorre a relação de origem
		fgets(atributo,1000,f_origem_ctl);
		if(strncmp(comando->id_atr,atributo,strlen(comando->id_atr))== 0){      //compara se o atributo do comando é igual ao atributo lido
			col_atr = i-1;                                                      //salva a coluna do atributo do comando
			break;
		}
	  }
//	  int chv = e_chave(atributo);
	  rewind(f_origem_ctl);
	  fgets(atributo,1000,f_origem_ctl);	 
	  sprintf(nome_arq,"%s.dad",comando->id_tab_origem);                     //bota o nome do arquivo no formato do fopen
	  FILE * f_origem_dad = fopen(nome_arq,"r");                             //abre o arquivo de origem.dad
	  sprintf(nome_arq,"%s.dad",comando->id_tab_saida);    
	  FILE * f_destino_dad = fopen(nome_arq,"w");	                         //abre o arquivo de saída.dad
	  char tupla[1000];                                                      //salva uma tupla da relação origem
	  char valor_atributo_atual[1000];
	  memset(valor_atributo_atual,'\0',sizeof(valor_atributo_atual));        //zera o vetor que vai salvar o atributo lido
	  int tam;
	  tam = strlen(comando->val_atr);										 //salva o tamanho do valor do atributo
	  int card_saida = 0;
      if(c[1] == '1'){                                                       //atributo = valor do atributo do comando
		while(1){ 
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);                                   //pega a posição em que o atributo começa na tupla
			strncpy(valor_atributo_atual,tupla+i,tam);                       //copia o atributo para um outro vetor
			if(strcmp(comando->val_atr,valor_atributo_atual)==0){            //compara a cópia com a valor do comando
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
			if(strcmp(valor_atributo_atual,comando->val_atr)>0)break;        //como a relação está ordenada, assim que encontrar um atributo maior para de procurar
		}
	  }
      if(c[1] == '2'){													//atributo < valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)>=0)break;       //como a relação esta ordenada, assim que encontrar um atributo maior ou igual para de procurar
			fputs(tupla,f_destino_dad); 
			card_saida = card_saida+1;
		}
      }
      if(c[1] == '3'){                                                  //atributo <= valor do atributo do comando
		  while(1){
			if(feof(f_origem_dad)) break;
			fgets(tupla,1000,f_origem_dad);
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)>0)break;        //como a relação está ordenada, assim que encontrar um atributo maior para de procurar
			fputs(tupla,f_destino_dad);
			card_saida = card_saida+1;
		 } 
	  }
      if(c[1] == '4'){                                                  //atributo > valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)>0){             //como a relação está ordenada, assim que encontrar um atributo maior, copia todas as tuplas para a resposta
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
				while(1){
					fgets(tupla,1000,f_origem_dad);
					if(feof(f_origem_dad)) break;
				    fputs(tupla,f_destino_dad);
					card_saida = card_saida+1;
				}
				break;
			}
		}
	  }
      if(c[1] == '5'){                                                  //atributo >= valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)>=0){             //como a relação está ordenada, assim que encontrar um atributo maior ou igual, copia todas as tuplas para a resposta
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
				while(1){
					fgets(tupla,1000,f_origem_dad);
					if(feof(f_origem_dad)) break;
				    fputs(tupla,f_destino_dad);
					card_saida = card_saida+1;
				}
				break;
			}
		}
	  }
      if(c[1] == '6'){                                                  //atributo != valor do atributo do comando
		while(1){
			fgets(tupla,1000,f_origem_dad);
			if(feof(f_origem_dad)) break;
			i=chega_coluna(tupla,col_atr);
			strncpy(valor_atributo_atual,tupla+i,tam);
			if(strcmp(valor_atributo_atual,comando->val_atr)<0){               //se o atributo é menor, logo é diferente
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
			}
			if(strcmp(valor_atributo_atual,comando->val_atr)>0){               //como a relação está ordenada, assim que encontrar um atributo maior, copia todas as tuplas para a resposta
				fputs(tupla,f_destino_dad);
				card_saida = card_saida+1;
				while(1){
					fgets(tupla,1000,f_origem_dad);
					if(feof(f_origem_dad)) break;
					fputs(tupla,f_destino_dad);
					card_saida = card_saida+1;
				}
				break;
				}
			}
	  }
      sprintf(nome_arq,"%s.ctl",comando->id_tab_saida);    				//bota o nome do arquivo no formato do fopen
	  FILE * f_saida_ctl = fopen(nome_arq,"w");                         //abre o arquivo de saida.ctl
      char temp_s[1000];
      sprintf(temp_s,"%d",n_colunas);
      fputs(temp_s,f_saida_ctl);										//escreve o grau da relação em saida.ctl
      fputc(',',f_saida_ctl);
	  sprintf(temp_s,"%d\n",card_saida);
      fputs(temp_s,f_saida_ctl);                                        //escreve a cardinalidade da relação em saida.ctl 
      for(i=0;i<n_colunas;i++){             							//percorre a relação de origem.ctl e preenche saida.ctl
		fgets(atributo,1000,f_origem_ctl);
	    fputs(atributo,f_saida_ctl);
	  }
	  fclose(f_origem_ctl);
      fclose(f_origem_dad);
      fclose(f_destino_dad);
      fclose(f_saida_ctl);
      libera_S(comando);                      
    }  
    if(c[0]=='J'){ 
      com_juncao* comando;
      comando =  (com_juncao*)malloc(sizeof(com_juncao));
      int i = 0;				   //inicializa contador 
      while(temp!=','){            //pega a tabela A      
        fread(&temp,1, 1, f_alg);  //pega um caracter do arquivo
        if(temp!=','){                    
     	  comando->id_tab_A[i] = temp;   //guarda o caracter se ainda não chegou na vírgula  
		  i = i+1;
		}
	  }
	  comando->id_tab_A[i+1] = '\0';
	  printf("%s ",comando->id_tab_A);
	  i = 0;
      temp ='\0';
      while(temp!=','){            //pega a tabela B      
        fread(&temp,1, 1, f_alg);  //pega um caracter do arquivo
        if(temp!=','){                    
     	  comando->id_tab_B[i] = temp;   //guarda o caracter se ainda não chegou na vírgula  
		  i = i+1;
		}
	  }
	  comando->id_tab_B[i+1] = '\0';
	  printf("%s ",comando->id_tab_B);
	  i = 0;
      temp ='\0';
      while(temp!='='){            //pega o atributo A      
        fread(&temp,1, 1, f_alg);  //pega um caracter do arquivo
        if(temp!='='){                    
     	  comando->atr_A[i] = temp;   //guarda o caracter se ainda não chegou na vírgula  
		  i = i+1;
		}
	  }
	  comando->atr_A[i+1] = '\0';
	  printf("%s ",comando->atr_A);
	  i = 0;
      temp ='\0';
      while(temp!=','){            //pega o atributo B      
        fread(&temp,1, 1, f_alg);  //pega um caracter do arquivo
        if(temp!=','){                    
     	  comando->atr_B[i] = temp;   //guarda o caracter se ainda não chegou na vírgula  
		  i = i+1;
		}
	  }
	  comando->atr_B[i+1] = '\0';
	  printf("%s ",comando->atr_B);
	  i = 0;
      temp ='\0';
      while(temp!=')'){            //pega a tabela de saída      
        fread(&temp,1, 1, f_alg);  //pega um caracter do arquivo
        if(temp!=')'){                    
     	  comando->id_tab_saida[i] = temp;   //guarda o caracter se ainda não chegou na vírgula  
		  i = i+1;
		}
	  }
	  comando->id_tab_saida[i+1] = '\0';
	  printf("%s ",comando->id_tab_saida);
	  i = 0;
      temp ='\0';
	  sprintf(nome_arq,"%s.ctl",comando->id_tab_A);                     //bota o nome do arquivo no formato do fopen
	  FILE * f_A_ctl = fopen(nome_arq,"r");                             //abre o arquivo de tab_A.ctl
	  sprintf(nome_arq,"%s.ctl",comando->id_tab_B);                     //bota o nome do arquivo no formato do fopen
	  FILE * f_B_ctl = fopen(nome_arq,"r");                             //abre o arquivo de tab_B.ctl
	  int n_col_A = 0;
	  fread(&temp,1,1,f_A_ctl);
	  n_col_A = temp - '0';                   //grau da relação A
	  fread(&temp,1,1,f_A_ctl);
  	  fread(&temp,1,1,f_A_ctl);
	  int n_col_B = 0;
	  fread(&temp,1,1,f_B_ctl);
	  n_col_B = temp - '0';                   //grau da relação B
	  fread(&temp,1,1,f_B_ctl);
  	  fread(&temp,1,1,f_B_ctl);
      char atributo_A[1000];                  //guarda uma linha de atributo do ctl
      int col_atr_A;                          //coluna em que o atributo_A se encontra na relação
      for (i=0;i<=n_col_A;i++){               //percorre a relação A.ctl
		fgets(atributo_A,1000,f_A_ctl);
		if(strncmp(comando->atr_A,atributo_A,strlen(comando->atr_A))== 0){      //compara se o atributo_A é igual ao atributo lido
			col_atr_A = i-1;                                                    //salva a coluna do atributo_A
			break;
		}
	  }
      char atributo_B[1000];                  //guarda uma linha de atributo do ctl
      int col_atr_B;                          //coluna em que o atributo_B se encontra na relação
      for (i=0;i<=n_col_B;i++){               //percorre a relação B.ctl
		fgets(atributo_B,1000,f_B_ctl);
		if(strncmp(comando->atr_B,atributo_B,strlen(comando->atr_B))== 0){      //compara se o atributo_B é igual ao atributo lido
			col_atr_B = i-1;                    //salva a coluna do atributo_B
			break;
		}
	  }
	  rewind(f_A_ctl);
	  fgets(atributo_A,1000,f_A_ctl);	
	  rewind(f_B_ctl);
	  fgets(atributo_B,1000,f_B_ctl);	
	  sprintf(nome_arq,"%s.dad",comando->id_tab_A);                     //bota o nome do arquivo no formato do fopen
	  FILE * f_A_dad = fopen(nome_arq,"r");                             //abre o arquivo de A.dad
	  sprintf(nome_arq,"%s.dad",comando->id_tab_B);                     //bota o nome do arquivo no formato do fopen
	  FILE * f_B_dad = fopen(nome_arq,"r");                             //abre o arquivo de B.dad
	  sprintf(nome_arq,"%s.dad",comando->id_tab_saida);                 //bota o nome do arquivo no formato do fopen
	  FILE * f_saida_dad = fopen(nome_arq,"w");                         //abre o arquivo de Saida.dad
	  char tupla_A[1000];                                               //salva uma tupla da relação A
	  char tupla_B[1000];                                               //salva uma tupla da relação B
	  char tupla_A_nL[1000];											//salva uma tupla da relação A sem o ultimo caracter
	  char tupla_saida[1000];                                           //salva a concatenação das tuplas A e B
	  int card_saida = 0;
      int pos_A;
      int pos_B;
      int tam_A;
      if(c[1] == '1'){
		while(1){
			fgets(tupla_A,1000,f_A_dad);
			if(feof(f_A_dad)) break;
			pos_A = chega_coluna(tupla_A,col_atr_A);
			tam_A = pega_tam(tupla_A,pos_A);
			memset(tupla_A_nL,'\0',sizeof(tupla_A_nL)); 
			strncpy(tupla_A_nL,tupla_A,strlen(tupla_A)-1);              //copia a tupla_A sem o \n ou EOF
			while(1){  
				fgets(tupla_B,1000,f_B_dad);
				pos_B = chega_coluna(tupla_B,col_atr_B);
				if(strncmp(tupla_A+pos_A,tupla_B+pos_B,tam_A)==0){      //escreve em f_saida_dad sem são iguais
					memset(tupla_saida,'\0',sizeof(tupla_saida));
					sprintf(tupla_saida,"%s,%s",tupla_A_nL,tupla_B);
					fputs(tupla_saida,f_saida_dad);
					printf("\n$%s,%s",tupla_A_nL,tupla_B);
					card_saida = card_saida + 1;
				}
				if(feof(f_B_dad)) break;
		    }
         	rewind(f_B_dad);
		}
	  }
      if(c[1] == '2'){
		fgets(tupla_B,1000,f_B_dad);
		while(1){
			fgets(tupla_A,1000,f_A_dad);
			if(feof(f_A_dad)) break;
			pos_A = chega_coluna(tupla_A,col_atr_A);
			tam_A = pega_tam(tupla_A,pos_A);
			memset(tupla_A_nL,'\0',sizeof(tupla_A_nL)); 
			strncpy(tupla_A_nL,tupla_A,strlen(tupla_A)-1);              //copia a tupla_A sem o \n ou EOF
			while(strncmp(tupla_B+pos_B,tupla_A+pos_A,tam_A)<=0){  
				pos_B = chega_coluna(tupla_B,col_atr_B);
				if(strncmp(tupla_A+pos_A,tupla_B+pos_B,tam_A)==0){      //escreve em f_saida_dad sem são iguais
					memset(tupla_saida,'\0',sizeof(tupla_saida));
					sprintf(tupla_saida,"%s,%s",tupla_A_nL,tupla_B);
					fputs(tupla_saida,f_saida_dad);
					card_saida = card_saida + 1;
				}
				fgets(tupla_B,1000,f_B_dad);
				if(feof(f_B_dad)) break;
		    }
			if(strncmp(tupla_A+pos_A,tupla_B+pos_B,tam_A)==0){          //trata o caso do último elemento de b.dad
				memset(tupla_saida,'\0',sizeof(tupla_saida));
				sprintf(tupla_saida,"%s,%s",tupla_A_nL,tupla_B);
				fputs(tupla_saida,f_saida_dad);
				card_saida = card_saida + 1;
				}
		}
	  }
      sprintf(nome_arq,"%s.ctl",comando->id_tab_saida);                 //bota o nome do arquivo no formato do fopen
	  FILE * f_saida_ctl = fopen(nome_arq,"w");                         //abre o arquivo de saida.ctl
      char temp_s[1000];
      sprintf(temp_s,"%d",n_col_A+n_col_B);
      fputs(temp_s,f_saida_ctl);										//escreve o grau da relação em saida.ctl
      fputc(',',f_saida_ctl);
	  sprintf(temp_s,"%d\n",card_saida);
      fputs(temp_s,f_saida_ctl);                                        //escreve a cardinalidade da relação em saida.ctl 
      for(i=0;i<n_col_A;i++){                                           //percorre a relação de A.ctl e preenche saida.ctl
		fgets(atributo_A,1000,f_A_ctl);
	    fputs(atributo_A,f_saida_ctl);
	  }
      for(i=0;i<n_col_B;i++){                                           //percorre a relação de A.ctl e preenche saida.ctl
		fgets(atributo_B,1000,f_B_ctl);
	    fputs(atributo_B,f_saida_ctl);
	  }
	  fclose(f_A_ctl);
	  fclose(f_B_ctl);
	  fclose(f_A_dad);
	  fclose(f_B_dad);
      fclose(f_saida_dad);
	  fclose(f_saida_ctl);
	  libera_J(comando);
    }  
    if(c[0]=='P'){ 
	  fgets(nome_arq,1000,f_alg);	
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
