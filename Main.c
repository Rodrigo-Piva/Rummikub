#include <stdio.h>
#include <string.h>

int ler_deck(char deck[]){
	char path[128],buffer[128],c;
	FILE *arq=NULL;
	char ask[4];
	int i,j;
	ask[0]='~';
	while(ask[0]!='1'&&ask[0]!='0'){
		printf("Modo do deck:\n");
		printf("(1)Controlado   (0)Aleatorio\n");
		fgets(ask,3,stdin);
		if(ask[0]=='1'){
			i=1;
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
			return 1;
		}else if(ask[0]=='0'){
			return 0;
		}else{
			printf("###Comando Invalido###\n");
		}
	}
	return -1;
}
void remove_top(char deck[]){
	int length,i;
	length=strlen(deck);
	for(i=0;i<length-2;i++){
		deck[i]=deck[i+2];
	}
	deck[length-2]='\0';
	deck[length-1]='\0';
	deck[length]='\0';
}

void game(int mode,int num_jogs, char Root_deck[]){
	char hand[num_jogs][214];
	char deck[214];
	int i;
	strncpy(deck,Root_deck,212);
	for(i=0;i<num_jogs*14;i++){
		hand[i%num_jogs][(2*i/num_jogs)]=deck[0];
		hand[i%num_jogs][(2*i/num_jogs)+1]=deck[1];
		remove_top(deck);
	}
}

int main(){
	int mode,num_jogs,i;
	char ask[4],deck[214];
	strncpy(deck,"!1@1#1$1!2@2#2$2!3@3#3$3!4@4#4$4!5@5#5$5!6@6#6$6!7@7#7$7!8@8#8$8!9@9#9$9!A@A#A$A!B@B#B$B!C@C#C$C!D@D#D$D!1@1#1$1!2@2#2$2!3@3#3$3!4@4#4$4!5@5#5$5!6@6#6$6!7@7#7$7!8@8#8$8!9@9#9$9!A@A#A$A!B@B#B$B!C@C#C$C!D@D#D$D****\0",213);
	ask[0]='0';
	num_jogs=-1;
	while(i){
		if(ask[0]=='0'){
			mode=ler_deck(deck);
			num_jogs=-1;
			while(num_jogs==-1){
				printf("Qual o numero de jogadores\n");
				printf("(1 - 5)\n");
				scanf("%d",&num_jogs);
				while(getchar()!='\n'){}
				if(num_jogs<1||num_jogs>5){
					printf("###Quantidade invalida###\n");
					num_jogs=-1;
				}else if(num_jogs==-1){
					printf("###Comando Invalido###\n");	
				}
			}
		}
		game(mode,num_jogs,deck);
		ask[0]='~';
		while(ask[0]!='0'&&ask[0]!='1'){
			printf("Jogar novamente?\n");
			printf("(1)Sim   (0)Nao\n");
			fgets(ask,3,stdin);
			if(ask[0]=='1'){
				ask[0]='~';
				while(ask[0]!='0'&&ask[0]!='1'){
					printf("Utilizar mesma configurações?\n");
					printf("(1)Sim   (0)Nao\n");
					fgets(ask,3,stdin);
					if(ask[0]!='0'&&ask[0]!='1'){
						printf("###Comando Invalido###\n");		
					}
				}
			}else if(ask[0]=='0'){
				printf("Finalizando...\n");
				i=0;
			}else{
				printf("###Comando Invalido###\n");
			}
		}
	}
	printf("\n%s\n",deck);
}