#include <stdio.h>
#include <string.h>


int point_card(char card[]){
	if(card[1]=='*'){
		return 20;
	}else if(card[1]>='1' && card[1]<='9'){
		return card[1]-'0';
	}else{
		return card[1]-'A'+1;
	}
}

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

int find_card(char deck[],char card[]){
	int length,i;
	length=strlen(deck);
	for(i=0;i<length;i+=2){
		if(card[0]==deck[i]&&card[1]==deck[i+1]){
			return i;
		}
	}
	return -1;
}

void print_list(char list[]){
	int i;
	for(i=0;list[i]!='\0';i+=2){
		printf("%c%c ",list[i],list[i+1]);
	}
	printf("\n");
}

void remove_pos(char deck[],int pos){
	int length,i;
	length=strlen(deck);
	for(i=pos;i<length-1;i++){
		deck[i]=deck[i+2];
	}
}

void add_in_pos(char deck[],char card[],int pos){
	int length,i;
	length=strlen(deck);
	for(i=length;i>=pos;i--){
		deck[i+2]=deck[i];
	}
	deck[pos]=card[0];
	deck[pos+1]=card[1];
}

void print_game(char hand[],char trincas[][10],char sequencias[][30]){
	int i;
	printf("Mesa:\n");
	printf(">Trincas\n");
	for(i=0;trincas[i][0]!='\0';i++){
		print_list(trincas[i]);
	}
	printf(">Sequencias\n");
	for(i=0;sequencias[i][0]!='\0';i++){
		print_list(sequencias[i]);
	}
	printf("Sua mao:\n");
	print_list(hand);
}

void game(int mode,int num_jogs, char Root_deck[]){
	char hand[num_jogs][214];
	char trincas[32][10];
	char sequencias[32][30];
	char deck[214],ask[4],card[8];
	int jogou[num_jogs];
	int i,j,counter,jog_atual,num_tri,num_seq,ponts,turn;
	strncpy(deck,Root_deck,214);
	for(i=0;i<28;i+=2){
		for(j=0;j<num_jogs;j++){
			hand[j][i]=deck[0];
			hand[j][i+1]=deck[1];
			hand[j][i+2]='\0';
			remove_pos(deck,0);
		}
	}
	for(i=0;i<16;i++){
		trincas[i][0]='\0';
		sequencias[i][0]='\0';
	}
	for(j=0;j<num_jogs;j++){
		jogou[j]=0;
	}
	jog_atual=0;
	num_seq=0;
	num_tri=0;
	turn=0;
	while(1){
		print_game(hand[jog_atual],trincas,sequencias);
		printf("\n>Turno do jogador %d\n\n",jog_atual+1);
		printf("                     O que fazer:\n");     
		printf("(0)Criar uma trinca       |   (1)Cortar uma trinca\n");
		printf("(2)Criar uma sequencia    |   (3)Substituir numa trinca\n");
		printf("(4)Cortar uma sequencia   |   (5)Dividir uma sequencia\n");
		printf("(6)Deslocar sequencia     |   (7)Substituir um coringa\n");
		printf("(8)Comprar carta          |   (9)Terminar turno\n");
		
		fgets(ask,3,stdin);
		if(ask[0]=='0'){
			counter=0;
			while(1){
				printf(">Trinca:");
				print_list(trincas[num_tri]);
				printf(">Sua mao: ");
				print_list(hand[jog_atual]);
				printf("\n                 O que fazer:\n");
				printf("(1)Adicionar carta    |  (2)Remover carta\n");
				printf("(3)Finalizar jogada   |  (4)Cancelar jogada\n");
				fgets(ask,3,stdin);
				if(ask[0]=='1'&&counter<4){
					i=1;
					while(i){
						printf(">Trinca:");
						print_list(trincas[num_tri]);
						printf(">Sua mao:");
						print_list(hand[jog_atual]);
						printf("\nDigite '0' para cancelar.\n");
						printf("Digite a carta para adicionar: ");
						fgets(card,4,stdin);
						if(card[0]=='0'){
							i=0;
						}else if(find_card(hand[jog_atual],card)!=-1){
							for(j=0;j<2*counter;j+=2){
								if(card[0]==trincas[num_tri][j]||card[1]!=trincas[num_tri][j+1]){
									break;
								}
							}
							if(j==2*counter){
								i=0;
							}else{
								printf("\n\n\n###Carta Invalida###\n");	
							}
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
					i=1;
					while(i&&card[0]!='0'){
						print_list(trincas[num_tri]);
						printf("Posicionar %c%c no:\n",card[0],card[1]);
						printf("(0)Início   (1)Fim\n");
						fgets(ask,3,stdin);
						if(ask[0]=='0'){
							add_in_pos(trincas[num_tri],card,0);
							i=0;
						}else if(ask[0]=='1'){
							add_in_pos(trincas[num_tri],card,2*counter);
							i=0;
						}else{
							printf("###Comando Invalido###\n");
						}
					}
					remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
					counter++;
				}else if(ask[0]=='2'&&counter>0){
					i=1;
					while(i){
						printf(">Trinca:\n");
						print_list(trincas[num_tri]);
						printf("Que carta remover: ");
						fgets(card,4,stdin);
						j=find_card(trincas[num_tri],card);
						if(j!=-1){
							remove_pos(trincas[num_tri],j);
							add_in_pos(hand[jog_atual],card,0);
							i=0;
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
					counter--;
				}else if(ask[0]=='3'&&counter>=3){
					num_tri++;
					break;
				}else if(ask[0]=='4'){
					trincas[num_tri][0]='\0';
					break;
				}else{
					printf("###Comando Invalido###\n");
				}

			}
		}else if(ask[0]=='1'){
			
		}else if(ask[0]=='2'){

		}else if(ask[0]=='3'){

		}else if(ask[0]=='4'){

		}else if(ask[0]=='5'){

		}else if(ask[0]=='6'){

		}else if(ask[0]=='7'){
			
		}else if(ask[0]=='8'){
			
		}else if(ask[0]=='9'&&(turn!=0||ponts>=30)){
			ponts=0;
			i++;
			if(i==num_jogs){
				turn++;
				i%=num_jogs;
			}
		}else if(ask[0]=='0'){
			return;
		}else{
			printf("###Comando Invalido###\n");
		}
	}
}

int main(){
	int mode,num_jogs,i;
	char ask[4],deck[214];
	strncpy(deck,"!1@1#1$1!2@2#2$2!3@3#3$3!4@4#4$4!5@5#5$5!6@6#6$6!7@7#7$7!8@8#8$8!9@9#9$9!A@A#A$A!B@B#B$B!C@C#C$C!D@D#D$D!1@1#1$1!2@2#2$2!3@3#3$3!4@4#4$4!5@5#5$5!6@6#6$6!7@7#7$7!8@8#8$8!9@9#9$9!A@A#A$A!B@B#B$B!C@C#C$C!D@D#D$D****\0",214);
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
}