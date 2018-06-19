#include <stdio.h>
#include <string.h>

void ler_deck(char deck[]){
	char path[128],buffer[128],c;
	FILE *arq=NULL;
	int i=1,j;

	while(i){
		printf("Endereço do arquivo:\n");
		fgets(buffer,127,stdin);
		strncpy(path,buffer,strlen(buffer)-1);
		path[strlen(buffer)-1]='\0';
		arq=fopen(path,"rt");
		if(arq==NULL){
			printf("###Arquivo Inexistente###\n");
		}else{
			for(j=0,c=fgetc(arq) ; j<212 && c!=EOF ;c=fgetc(arq)){
				if(c!='\n' && c!=' '){	
					deck[j]=c;
					j++;
				}
			}
			if(j!=212){
				printf("###Arquivo Invalido###\n");
			}else{
				deck[212]='\0';
				printf("\n%s\n",deck);
				printf("\n>Deck carregado.\n");
				i=0;
			}
		}
	}
}

int main(){
	int i,mode;
	char ask[4],deck[214];

	i=1;
	while(i){
		printf("Modo do deck:\n");
		printf("(1)Controlado   (0)Aleatorio\n");
		fgets(ask,3,stdin);
		if(ask[0]=='1'){
			mode=1;
			i=0;
			ler_deck(deck);
		}else if(ask[0]=='0'){
			mode=0;
			i=0;
		}else{
			printf("###Comando Invalido###\n");
		}
	}
}