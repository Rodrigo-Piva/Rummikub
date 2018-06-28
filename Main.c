#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



int point_card(char card){
	if(card=='*'){
		return 20;
	}else if(card>='1' && card<='9'){
		return card-'0';
	}else{
		return card-'A'+10;
	}
}

int dist(char comp,char card){
	if(card==comp+1||(comp=='9'&&card=='A')){
		return 1;
	}else if(card==comp+2||(comp=='8'&&card=='A')||(comp=='9'&&comp=='B')){
		return 2;
	}else if(card==comp-1||(comp=='A'&&card=='9')){
		return -1;
	}else if(card==comp-2||(comp=='A'&&card=='8')||(comp=='B'&&comp=='9')){
		return -2;
	}else{
		return 0;
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

void print_game(char hand[],char tri[][10],char seq[][30]){
	int i;
	printf("Mesa:\n");
	printf(">Trincas\n");
	for(i=0;tri[i][0]!='\0';i++){
		print_list(tri[i]);
	}
	printf(">Sequencias\n");
	for(i=0;seq[i][0]!='\0';i++){
		print_list(seq[i]);
	}
	printf("Sua mao:\n");
	print_list(hand);
}

void game(int mode,int num_jogs, char Root_deck[]){
	char hand[num_jogs][214];
	char tri[32][10];
	char seq[32][30];
	char deck[214],ask[4],card[8];
	int jogou[num_jogs];
	int i,j,counter,jog_atual,num_tri,num_seq,points,pos,temp_tri,temp_seq;
	strncpy(deck,Root_deck,214);
	for(i=0;i<28;i+=2){
		for(j=0;j<num_jogs;j++){
			if(mode==1){
				pos=0;
			}else{
				pos=(rand()%strlen(deck));
				pos/=2;
				pos*=2;
			}
			hand[j][i]=deck[pos];
			hand[j][i+1]=deck[pos+1];
			hand[j][i+2]='\0';
			remove_pos(deck,pos);
		}
	}
	for(i=0;i<16;i++){
		tri[i][0]='\0';
		seq[i][0]='\0';
	}
	for(j=0;j<num_jogs;j++){
		jogou[j]=0;
	}
	jog_atual=0;
	num_seq=0;
	num_tri=0;
	points=0;
	while(1){
		print_game(hand[jog_atual],tri,seq);
		printf("\n>Turno do jogador %d\n\n",jog_atual+1);
		printf("                     O que fazer:\n");

		printf("(0)Comprar carta           |   (1)Terminar turno\n");
		printf("(2)Criar uma sequencia     |   (3)Criar uma trinca\n");
		printf("(4)Cortar uma sequencia    |   (5)Cortar uma quadra\n");
		printf("(6)Dividir uma sequencia   |   (7)Adiconar em trinca\n");
		printf("(8)Adicionar em sequencia  |   (9)Substituir um coringa\n");
		
		fgets(ask,3,stdin);
		if(ask[0]=='0'){
			if(points==0){
				if(mode){
					pos=0;
				}else{
					pos=(rand()%strlen(deck));
					pos/=2;
					pos*=2;
				}
				add_in_pos(hand[jog_atual],deck+pos,0);
				remove_pos(deck,pos);
				temp_seq=0;
				temp_tri=0;
				jog_atual++;
				jog_atual%=num_jogs;
			}else{
				ask[0]='~';
				while(ask[0]=='~'){
					printf("voce ja fez jogadas esse turno\n");
					printf("Para comprar cartas suas jogadas serao canceladas\n");
					printf("Deseja continua?\n");
					printf("(1)Sim   (0)Nao\n");
					fgets(ask,3,stdin);
					if(ask[0]=='1'){
						while(temp_seq>=0){
							while(seq[num_seq-1][0]!='\0'){
								add_in_pos(hand[jog_atual],seq[num_seq-1],0);
								remove_pos(seq[num_seq-1],0);
							}
							temp_seq--;
							num_seq--;
						}
						while(temp_tri>=0){
							while(tri[num_tri-1][0]!='\0'){
								add_in_pos(hand[jog_atual],tri[num_tri-1],0);
								remove_pos(tri[num_tri-1],0);
							}
							temp_tri--;
							num_tri--;
						}
						points=0;
						jog_atual++;
						jog_atual%=num_jogs;
					}else if(ask[0]!='0'){
						printf("###Comando Invalido###\n");
					}
				}
			}
		}else if(ask[0]=='1'&&(jogou[i]!=0||points>=30)){
			jogou[i]=1;
			temp_seq=0;
			temp_tri=0;
			points=0;
			jog_atual++;
			jog_atual%=num_jogs;
		}else if(ask[0]=='1'&&jogou[i]==0&&points<30){
			printf("\n\n\n\n\n\n#######################################################################\n");
			printf("#Sao necessarios pelo menos 30 pontos para finalizar a primeira jogada#\n");
			printf("#######################################################################\n");
			printf("Voce possui %d pontos\n\n",points);
		}else if(ask[0]=='2'){
			counter=0;
			while(1){
				printf(">Sequencia:");
				print_list(seq[num_seq]);
				printf(">Sua mao: ");
				print_list(hand[jog_atual]);
				printf("\n                 O que fazer:\n");
				printf("(1)Adicionar carta    |  (2)Remover carta\n");
				printf("(3)Finalizar jogada   |  (4)Cancelar jogada\n");
				fgets(ask,3,stdin);
				if(ask[0]=='1'&&counter<13){
					i=1;
					while(i){
						printf(">Sequencia:");
						print_list(seq[num_seq]);
						printf(">Sua mao:");
						print_list(hand[jog_atual]);
						printf("\nDigite '0' para cancelar.\n");
						printf("Digite a carta para adicionar: ");
						fgets(card,4,stdin);
						if(card[0]=='0'){
							i=0;
						}else if(find_card(hand[jog_atual],card)!=-1){
							if(counter==0||dist(seq[num_seq][0],card[0])==-1||(dist(seq[num_seq][2],card[0])==-2&&seq[num_seq][0]=='*')){
								add_in_pos(seq[num_seq],card,0);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								counter++;
								i=0;
							}else if(dist(seq[num_seq][2*counter-2],card[0])==1||(dist(seq[num_seq][2*counter-4],card[0])==2&&seq[num_seq][2*counter-2]=='*')){
								add_in_pos(seq[num_seq],card,2*counter);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								counter++;
								i=0;
							}else if(card[0]=='*'){
								ask[0]='~';
								while(ask[0]=='~'){
									printf("Posicionar coringa no começo ou no fim?\n");
									printf("(0)Inicio   (1)Fim\n");
									fgets(ask,3,stdin);
									if(ask[0]=='0'){
										add_in_pos(seq[num_seq],card,0);
									}else if(ask[0]=='1'){
										add_in_pos(seq[num_seq],card,2*counter);
									}else{
										printf("###Comando Invalido###\n");
									}
								}
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
							}else{
								printf("\n\n\n###Carta Invalida###\n");
							}
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
				}else if(ask[0]=='2'&&counter>0){
					i=1;
					while(i){
						printf(">Sequencia:\n");
						print_list(seq[num_seq]);
						printf("Que carta remover: ");
						fgets(card,4,stdin);
						j=find_card(seq[num_seq],card);
						if(j!=-1){
							remove_pos(seq[num_seq],j);
							add_in_pos(hand[jog_atual],card,0);
							i=0;
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
					counter--;
				}else if(ask[0]=='3'&&counter>=3){
					for(i=0;i<counter;i++){
						strncpy(card,seq[num_seq]+2*i,2);
						points+=point_card(card[0]);
					}
					printf("%d\n",points);
					temp_seq++;
					num_seq++;
					break;
				}else if(ask[0]=='4'){
					for(i=0;i<counter;i++){
						add_in_pos(hand[jog_atual],seq[num_seq],0);
						remove_pos(seq[num_seq],0);
					}
					break;
				}else{
					printf("###Comando Invalido###\n");
				}

			}
		}else if(ask[0]=='3'){
			counter=0;
			while(1){
				printf(">Trinca:");
				print_list(tri[num_tri]);
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
						print_list(tri[num_tri]);
						printf(">Sua mao:");
						print_list(hand[jog_atual]);
						printf("\nDigite '0' para cancelar.\n");
						printf("Digite a carta para adicionar: ");
						fgets(card,4,stdin);
						if(card[0]=='0'){
							i=0;
						}else if(find_card(hand[jog_atual],card)!=-1){
							for(j=0;j<2*counter;j+=2){
								if(card[1]==tri[num_tri][j+1]||card[0]!=tri[num_tri][j]){
									break;
								}
							}
							if(card[0]=='*'){
								i=0;
							}else if(j==2*counter){
								i=0;
							}else{
								printf("\n\n\n###Carta Invalida###\n");	
							}
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
					i=1;
					if(counter==0){
						add_in_pos(tri[num_tri],card,0);
						i=0;
					}
					while(i){
						print_list(tri[num_tri]);
						printf("Posicionar %c%c no:\n",card[0],card[1]);
						printf("(0)Inicio   (1)Fim\n");
						fgets(ask,3,stdin);
						if(ask[0]=='0'){
							add_in_pos(tri[num_tri],card,0);
							i=0;
						}else if(ask[0]=='1'){
							add_in_pos(tri[num_tri],card,2*counter);
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
						print_list(tri[num_tri]);
						printf("Que carta remover: ");
						fgets(card,4,stdin);
						j=find_card(tri[num_tri],card);
						if(j!=-1){
							remove_pos(tri[num_tri],j);
							add_in_pos(hand[jog_atual],card,0);
							i=0;
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
					counter--;
				}else if(ask[0]=='3'&&counter>=3){
					for(i=0;i<counter;i++){
						strncpy(card,tri[num_tri]+2*i,2);
						points+=point_card(card[0]);
					}
					printf("%d\n",points);
					temp_tri++;
					num_tri++;
					break;
				}else if(ask[0]=='4'){
					for(i=0;i<counter;i++){
						add_in_pos(hand[jog_atual],tri[num_tri],0);
						remove_pos(tri[num_tri],0);
					}
					break;
				}else{
					printf("###Comando Invalido###\n");
				}

			}
		}else if(ask[0]=='4'){

		}else if(ask[0]=='5'){

		}else if(ask[0]=='6'){
			i=1;
			while(i){
				printf(">Sequencias:\n");
				for(i=0;i<num_seq;i++){
					if(strlen(seq[i])>=12){
						printf("|%d|-> ",i);
						print_list(seq[i]);
					}
				}
				i=1;
				printf(">Sua mao:\n");
				print_list(hand[jog_atual]);
				printf("#Digite -1 para cancelar#\n");
				printf("Que sequencia dividir: ");
				scanf("%d",&j);
				while(getchar()!='\n'){}
				if(j>=0&&strlen(seq[j])>=12){
					while(i){	
						printf(">Sequencia\n");
						print_list(seq[j]);
						printf(">Sua mao:\n");
						print_list(hand[jog_atual]);
						printf("Em que carta vc vai partir a sequencia: \n");
						fgets(card,3,stdin);
						pos=find_card(seq[j],card);
						if(pos>=2&&pos<=strlen(seq[j])-2&&(find_card(hand[jog_atual],card)!=-1||find_card(hand[jog_atual],"**")!=-1)){
							ask[0]='~';
							if(find_card(hand[jog_atual],card)!=-1&&find_card(hand[jog_atual],"**")!=-1){
								while(ask[0]=='~'){
									printf("Usar um coringa?\n");
									printf("(1)Sim   (0)Nao\n");
									fgets(ask,3,stdin);
									if(ask[0]!='1'&&ask[0]!='0'){
										printf("###Comando Invalido###\n");
										ask[0]='~';
									}
								}
							}
							if((ask[0]=='~'&&find_card(hand[jog_atual],"**")!=-1)||ask[0]=='1'){
								add_in_pos(seq[num_seq],"**",0);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],"**"));
								points+=point_card(card[0]);
							}else if((ask[0]=='~'&&find_card(hand[jog_atual],card)!=-1)||ask[0]=='0'){
								add_in_pos(seq[num_seq],card,0);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								points+=point_card(card[0]);
							}
							for(pos-=2;pos>=0;pos-=2){
								add_in_pos(seq[num_seq],seq[j]+pos,0);
								remove_pos(seq[j],pos);
							}
							num_seq++;
							i=0;
						}else{
							printf("###Carta Invalida###\n");
						}
					}
				}else if(j==-1){
					i=0;
				}else{
					printf("###Sequencia Invalida###\n");
				}

			}
		}else if(ask[0]=='7'){

		}else if(ask[0]=='8'){

		}else if(ask[0]=='9'){
			
		}else{
			printf("###Comando Invalido###\n");
		}
	}
}

int main(){
	int mode,num_jogs,i;
	char ask[4],deck[214];
	strncpy(deck,"1!1@1#1$2!2@2#2$3!3@3#3$4!4@4#4$5!5@5#5$6!6@6#6$7!7@7#7$8!8@8#8$9!9@9#9$A!A@A#A$B!B@B#B$C!C@C#C$D!D@D#D$1!1@1#1$2!2@2#2$3!3@3#3$4!4@4#4$5!5@5#5$6!6@6#6$7!7@7#7$8!8@8#8$9!9@9#9$A!A@A#A$B!B@B#B$C!C@C#C$D!D@D#D$****\0",214);
	srand(time(NULL));
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