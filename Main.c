#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void sub_coringa(char card[],char back,char front){
	if((back>='1'&&back<='8')||(back>='A'&&back<='C')){
		card[0]=back+1;
	}else if(back==9){
		card[0]='A';
	}else if((front>='2'&&front<='9')||(front>='B'&&front<='D')){
		card[0]=front-1;
	}else if(front=='A'){
		card[0]='9';
	}
}

int point_card(char card){
	if(card=='*'){
		return 20;
	}else if(card>='1' && card<='9'){
		return card-'0';
	}else{
		return card-'A'+10;
	}
}

int dist(char comp[],char card){
	int last;
	last=strlen(comp)-2;
	if(card==comp[last]+1||(comp[last]=='9'&&card=='A')){
		return 1;
	}else if(comp[last]=='*'&&(card==comp[last-2]+2||(comp[last-2]=='8'&&card=='A')||(comp[last-2]=='9'&&card=='B'))){
		return 1;
	}else if(comp[last]=='*'&&comp[last-2]=='*'&&(card==comp[last-3]+4||(comp[last-4]=='7'&&card=='A')||(comp[last-4]=='8'&&card=='B')||(comp[last-4]=='9'&&card=='D'))){
		return 1;
	}else if(card==comp[0]-1||(comp[0]=='A'&&card=='9')){
		return -1;
	}else if(comp[0]=='*'&&(card==comp[2]-2||(comp[0]=='A'&&card=='8')||(comp[2]=='B'&&card=='9'))){
		return -1;
	}else if(comp[0]=='*'&&comp[2]=='*'&&(card==comp[4]-3||(comp[4]=='A'&&card=='7')||(comp[4]=='B'&&card=='8')||(comp[4]=='D'&&card=='9'))){
		return -1;
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

void print_game(char hand[],char tri[][12],char seq[][30]){
	int i;
	printf("\n>Mao: ");
	print_list(hand);
	
	printf("\n>Trincas\n");
	for(i=0;tri[i][0]!='\0';i++){
		print_list(tri[i]);
	}
	printf(">Sequencias\n");
	for(i=0;seq[i][0]!='\0';i++){
		print_list(seq[i]);
	}
}

void ord(char hand[]){
	char naipe[4]="!@#$";
	char num[13]="DBCA987654321";
	char card[4];
	int i,j,pos;
	for(i=0;i<4;i++){
		for(j=0;j<13;j++){
			card[0]=num[j];
			card[1]=naipe[i];
			pos=find_card(hand,card);
			if(pos!=-1){
				remove_pos(hand,pos);
				add_in_pos(hand,card,0);
				pos=find_card(hand+2,card);
				if(pos!=-1){
					remove_pos(hand+2,pos);
					add_in_pos(hand,card,0);
				}
			}
		}
	}
}

void game(int mode,int num_jogs, char Root_deck[]){
	char hand[num_jogs][214];
	char tri[36][12];
	char seq[36][30];
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
		if(hand[jog_atual][0]=='\0'){
			printf("\n\n\n#Mao Vazia#\n");
			printf("\nJogador %d venceu\n\n",jog_atual+1);
			return;
		}
		ord(hand[jog_atual]);
		printf("\n\n\n\n\n>Turno do jogador %d\n",jog_atual+1);
		print_game(hand[jog_atual],tri,seq);
		printf("\n\n                     O que fazer:\n");

		printf("(0)Comprar carta           |   (1)Terminar turno\n");
		printf("(2)Criar uma sequencia     |   (3)Criar uma trinca\n");
		printf("(4)Cortar uma sequencia    |   (5)Cortar uma quadra\n");
		printf("(6)Dividir uma sequencia   |   (7)Adiconar em trinca\n");
		printf("(8)Adicionar em sequencia  |   (9)Substituir um coringa\n");
		
		fgets(ask,3,stdin);
		if(ask[0]=='0'){
			if(points==0||num_jogs==1){
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
					printf("\nVoce ja fez jogadas esse turno\n");
					printf("Para comprar cartas suas jogadas serao canceladas\n");
					printf("Deseja continuar?\n");
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
						ask[0]='~';
					}
				}
			}
			if(deck[0]=='\0'){
				points=1000;
				printf("\n\n>Nao ha mais cartas para comprar.\n");
				printf("Fim de jogo:\n");
				for(i=0;i<num_jogs;i++){
					for(j=0;hand[i][j]!='\0';j+=2){
						counter+=point_card(hand[i][j]);
					}
					printf("\nJogador %d: %d pontos\n",i+1,counter);
					if(counter<points){
						points=counter;
						jog_atual=i;
						mode=1;
					}else if(counter==points){
						mode=0;
					}
				}
				if(mode==1){
					printf("\nVitoria do jogador %d.\n",jog_atual+1);
				}else{
					printf("\nEmpate.\n");
				}
				return;
			}
		}else if(ask[0]=='1'&&(jogou[jog_atual]!=0||points>=30)){
			jogou[jog_atual]=1;
			temp_seq=0;
			temp_tri=0;
			points=0;
			jog_atual++;
			jog_atual%=num_jogs;
		}else if(ask[0]=='1'&&jogou[jog_atual]==0&&points<30){
			printf("\n\n\n\n\n\n#######################################################################\n");
			printf("#Sao necessarios pelo menos 30 pontos para finalizar a primeira jogada#\n");
			printf("#######################################################################\n");
			printf("Voce possui %d pontos\n",points);
		}else if(ask[0]=='2'){
			counter=0;
			while(1){
				printf("\n\n\n\n\n>Sequencia:");
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
						printf("\n\n\n\n\n>Sequencia:");
						print_list(seq[num_seq]);
						printf("\n>Sua mao:");
						print_list(hand[jog_atual]);
						printf("\nDigite '0' para cancelar.\n");
						printf("Digite a carta para adicionar: ");
						fgets(card,4,stdin);
						if(card[0]=='0'){
							i=0;
						}else if(find_card(hand[jog_atual],card)!=-1){
							j=dist(seq[num_seq],card[0]);
							if(counter==0||j==-1){
								add_in_pos(seq[num_seq],card,0);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								counter++;
								i=0;
							}else if(j==1){
								add_in_pos(seq[num_seq],card,2*counter);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								counter++;
								i=0;
							}else if(card[0]=='*'){
								ask[0]='~';
								while(ask[0]=='~'){
									printf("\n\n\n\n\nPosicionar coringa no começo ou no fim?\n");
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
								counter++;
								i=0;
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
						printf("\n\n>Sequencia:\n");
						print_list(seq[num_seq]);
						printf("\nQue carta remover: ");
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
				printf("\n\n\n\n\n>Trinca:");
				print_list(tri[num_tri]);
				printf("\n>Sua mao: ");
				print_list(hand[jog_atual]);
				printf("\n                 O que fazer:\n");
				printf("(1)Adicionar carta    |  (2)Remover carta\n");
				printf("(3)Finalizar jogada   |  (4)Cancelar jogada\n");
				fgets(ask,3,stdin);
				if(ask[0]=='1'&&counter<4){
					i=1;
					while(i){
						printf("\n\n\n\n\n>Trinca:");
						print_list(tri[num_tri]);
						printf("\n>Sua mao:");
						print_list(hand[jog_atual]);
						printf("\nDigite '0' para cancelar.\n");
						printf("Digite a carta para adicionar: ");
						fgets(card,4,stdin);
						if(card[0]=='0'){
							i=0;
						}else if(counter==0&&card[0]=='*'){
							printf("\n\n\n###Nao Inicie Com Coringa###\n");
						}else if(find_card(hand[jog_atual],card)!=-1&&((card[0]=='*'&&card[1]=='*')||(find_card(tri[num_tri],card)==-1&&card[0]==tri[num_tri][strlen(tri[num_tri])-2]))){
							i=0;
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}
					if(card[0]!='0'){
						add_in_pos(tri[num_tri],card,0);
						remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
						counter++;
					}
				}else if(ask[0]=='2'&&counter>0){
					i=1;
					while(i){
						printf("\n\n\n\n\n>Trinca:\n");
						print_list(tri[num_tri]);
						printf("\nQue carta remover: ");
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
		}else if(ask[0]=='4'&&num_seq>0){
			i=1;
			while(i){
				printf("\n\n\n\n\nSua mao:\n");
				print_list(hand[jog_atual]);
				printf("\n>Sequencias:\n");
				for(i=0;i<num_seq;i++){
					if(strlen(seq[i])>=8){
						printf("|%d|-> ",i);
						print_list(seq[i]);
					}
				}
				i=1;
				printf("\n#Digite um numero negativo para cancelar#\n");
				printf("Que sequencia cortar: \n");
				scanf("%d",&j);
				while(getchar()!='\n'){}
				if(j<0){
					i=0;
				}else if(j<num_seq&&strlen(seq[j])>=8){
					printf("\n");
					printf("\n\n\n\n\nSequencia:\n");
					print_list(seq[j]);
					printf("\nOnde remover:\n");
					printf("(1)Inicio   (0)Fim\n");
					fgets(ask,3,stdin);
					if(ask[0]=='1'){
						points-=point_card(seq[j][0]);
						add_in_pos(hand[jog_atual],seq[j],0);
						remove_pos(seq[j],0);
						i=0;
					}else if(ask[0]=='0'){
						points-=point_card(seq[j][strlen(seq[j])-2]);
						add_in_pos(hand[jog_atual],seq[j]+strlen(seq[j])-2,0);
						remove_pos(seq[j],strlen(seq[j])-2);
						i=0;
					}else{
						printf("###Comando Invalido###\n");
					}
				}else{
					printf("###Sequencia Invalida###\n\n");
				}
			}
		}else if(ask[0]=='5'&&num_tri>0){
			i=1;
			while(i){
				printf("\n\n\n\n\nSua mao:\n");
				print_list(hand[jog_atual]);
				printf("\n>Quadras:\n");
				for(i=0;i<num_tri;i++){
					if(strlen(tri[i])==8){
						printf("|%d|-> ",i);
						print_list(tri[i]);
					}
				}
				i=1;
				printf("\n#Digite um numero negativo para cancelar#\n");
				printf("Que quadra cortar: \n");
				scanf("%d",&j);
				while(getchar()!='\n'){}
				if(j<0){
					i=0;
				}else if(j<num_tri&&strlen(tri[j])==8){
					while(i){
						printf("\n\n\n\n\nQuadra:\n");
						print_list(tri[j]);
						printf("\nQue carta remover: ");
						fgets(card,3,stdin);
						pos=find_card(tri[j],card);
						if(pos!=-1){
							points-=point_card(card[0]);
							add_in_pos(hand[jog_atual],card,0);
							remove_pos(tri[j],pos);
							i=0;
						}else{
							printf("###Carta Invalida###\n");
						}
					}
				}
			}
		}else if(ask[0]=='6'&&num_seq>0){
			i=1;
			while(i){
				printf("\n\n\n\n\n>Sequencias:\n");
				for(i=0;i<num_seq;i++){
					if(strlen(seq[i])>=12){
						printf("|%d|-> ",i);
						print_list(seq[i]);
					}
				}
				i=1;
				printf("\n>Sua mao:\n");
				print_list(hand[jog_atual]);
				printf("\n#Digite um numero negativo para cancelar#\n");
				printf("Que sequencia dividir: ");
				scanf("%d",&j);
				while(getchar()!='\n'){}
				if(j<0){
					i=0;
				}else if(j<num_seq&&strlen(seq[j])>=12){
					while(i){	
						printf("\n\n\n\n\n>Sequencia\n");
						print_list(seq[j]);
						printf("\n>Sua mao:\n");
						print_list(hand[jog_atual]);
						printf("\nEm que carta vc vai partir a sequencia: \n");
						fgets(card,3,stdin);
						pos=find_card(seq[j],card);
						if(card[0]=='*'&&card[1]=='*'&&pos>2&&pos<strlen(seq[j])-2){
							if(find_card(tri[j]+pos+2,"**")!=-1){
								ask[0]='~';
								while(ask[0]=='~'){
									printf("\n##Dois coringas encontrados##\n");
									printf("Usar o primeiro ou segundo?\n");
									printf("(0)Primeiro   (1)Segundo\n");
									if(ask[0]=='1'){
										pos+=find_card(tri[j]+pos+2,"**")+2;
									}else if(ask[0]!='0'){
										ask[0]='~';
										printf("###Comando Invalido###\n");
									}
								}
							}
							card[1]=seq[j][1];
							sub_coringa(card,seq[j][pos-2],seq[j][pos+2]);
						}
						if(pos>2&&pos<strlen(seq[j])-2){
							for(;pos>=0;pos-=2){
								add_in_pos(seq[num_seq],seq[j]+pos,0);
								remove_pos(seq[j],pos);
							}
							num_seq++;
							i=0;
						}else if((pos==2||pos==strlen(seq[j])-2)&&(find_card(hand[jog_atual],card)!=-1||find_card(hand[jog_atual],"**")!=-1)){
							ask[0]='~';
							if(find_card(hand[jog_atual],card)!=-1&&find_card(hand[jog_atual],"**")!=-1){
								while(ask[0]=='~'){
									printf("\nUsar um coringa?\n");
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
				}else{
					printf("###Sequencia Invalida###\n");
				}

			}
		}else if(ask[0]=='7'&&num_tri>0){
			i=1;
			while(i){
				printf("\n\n\n\n\nSua mao:\n");
				print_list(hand[jog_atual]);
				printf("\n>Trincas:\n");
				for(i=0;i<num_tri;i++){
					if(strlen(tri[i])==6){
						printf("|%d|-> ",i);
						print_list(tri[i]);
					}
				}
				i=1;
				printf("\n#Digite um numero negativo para cancelar#\n");
				printf("Em qual trinca adicionar: ");
				scanf("%d",&pos);
				while(getchar()!='\n'){}
				if(pos<0){
					i=0;
				}else if(pos<num_tri&&strlen(tri[pos])==6){
					i=1;
					while(i){
						printf("Que carta adicionar: ");
						fgets(card,4,stdin);
						j=find_card(hand[jog_atual],card);
						if(j!=-1){
							if(card[0]=='*'||(find_card(tri[pos],card)==-1&&card[0]==tri[pos][strlen(tri[pos])-2])){
								add_in_pos(tri[pos],card,0);
								remove_pos(hand[jog_atual],j);
								i=0;
							}else{
								printf("\n###Carta Invalida###\n");
							}
						}else{
							printf("\n###Carta Invalida###\n");
						}
					}
				}else{
					printf("\n\n\n###Trinca Invalida###\n");
				}
			}
		}else if(ask[0]=='8'&&num_seq>0){
			i=1;
			while(i){
				printf("\n\n\n\n\nSua mao:\n");
				print_list(hand[jog_atual]);
				printf(">Sequencias:\n");
				for(i=0;i<num_seq;i++){
					if(strlen(seq[i])<26){
						printf("|%d|-> ",i);
						print_list(seq[i]);
					}
				}
				i=1;
				printf("\n#Digite um numero negativo para cancelar#\n");
				printf("Em qual sequencia adicionar: ");
				scanf("%d",&pos);
				while(getchar()!='\n'){}
				if(pos<0){
					i=0;
				}else if(pos<num_seq&&strlen(seq[pos])<26){
					while(i){
						printf(">Sequencia:");
						print_list(seq[pos]);
						printf(">Sua mao:");
						print_list(hand[jog_atual]);
						printf("\nDigite '0' para cancelar.\n");
						printf("Digite a carta para adicionar: ");
						fgets(card,4,stdin);
						if(card[0]=='0'){
							i=0;
						}else if(find_card(hand[jog_atual],card)!=-1){
							j=dist(seq[pos],card[0]);
							if(cardcounter==0||j==-1){
								add_in_pos(seq[pos],card,0);
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								counter++;
								i=0;
							}else if(j==1){
								add_in_pos(seq[pos],card,2*counter);
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
										add_in_pos(seq[pos],card,0);
									}else if(ask[0]=='1'){
										add_in_pos(seq[pos],card,2*counter);
									}else{
										printf("###Comando Invalido###\n");
									}
								}
								remove_pos(hand[jog_atual],find_card(hand[jog_atual],card));
								i=0;
							}else{
								printf("\n\n\n###Carta Invalida###\n");
							}
						}else{
							printf("\n\n\n###Carta Invalida###\n");
						}
					}		
				}else{
					printf("###Sequencia Invalida###\n");
				}
			}
		}else if(ask[0]=='9'&&(num_seq>0||num_tri>0)){
			if(num_seq>0&&num_tri>0){
				i=1;
				while(i){
					printf("\n\n\n\n\nSua mao:\n");
					print_list(hand[jog_atual]);
					printf("\n>Trincas:\n");
					for(i=0;i<num_tri;i++){
						if(find_card(tri[i],"**")!=-1){
							print_list(tri[i]);
						}
					}
					printf("\n>Sequencias:\n");
					for(i=0;i<num_seq;i++){
						if(find_card(seq[i],"**")!=-1){
							print_list(tri[i]);
						}
					}
					printf("\nOnde Substituir:\n");
					printf("(0)Trinca   (1)Sequencia\n");
					fgets(ask,3,stdin);
					if(ask[0]!='0'&&ask[0]!='1'){
						printf("###Comando Invalido###\n");
					}else{
						i=0;
					}
				}
			}
			if(ask[0]=='0'||num_tri>0){
				i=1;
				while(i){
					printf("\n\n\n\n\nSua mao:\n");
					print_list(hand[jog_atual]);
					printf(">Trincas:\n");
					for(i=0;i<num_tri;i++){
						if(find_card(tri[i],"**")!=-1){
							printf("|%d|-> ",i);
							print_list(tri[i]);
						}
					}
					i=1;
					printf("\n#Digite um numero negativo para cancelar#\n");
					printf("Em qual trinca adicionar: ");
					scanf("%d",&j);
					while(getchar()!='\n'){}
					if(j<0){
						i=0;
					}else if(j<num_tri&&find_card(tri[j],"**")!=-1){
						pos=find_card(tri[j],"**");
						if(find_card(tri[j]+pos+2,"**")!=-1){
							ask[0]='~';
							while(ask[0]=='~'){
								printf("\n##Dois coringas encontrados##\n");
								printf("Usar o primeiro ou segundo?\n");
								printf("(0)Primeiro   (1)Segundo\n");
								if(ask[0]=='1'){
									pos+=find_card(tri[j]+pos+2,"**")+2;
								}else if(ask[0]!='0'){
									ask[0]='~';
									printf("###Comando Invalido###\n");
								}
							}
						}
						sub_coringa(card,tri[j][pos-2],tri[j][pos+2]);
						if((find_card(hand[jog_atual],card))!=-1){
							remove_pos(tri[j],pos);
							add_in_pos(tri[j],card,pos);
							pos=find_card(hand[jog_atual],card);
							remove_pos(hand[jog_atual],pos);
							add_in_pos(seq[num_seq],"**",pos);
						}else{
							printf("###Trinca Invalida###\n");
						}
					}else{
						printf("###Trinca Invalida###\n");
					}
				}
			}else if(ask[0]=='1'||num_seq>0){
				i=1;
				while(i){
					printf("\n\n\n\n\nSua mao:\n");
					print_list(hand[jog_atual]);
					printf(">Sequencias:\n");
					for(i=0;i<num_seq;i++){
						if(find_card(seq[i],"**")!=-1){
							printf("|%d|-> ",i);
							print_list(seq[i]);
						}
					}
				}
				i=1;
				printf("\n#Digite um numero negativo para cancelar#\n");
				printf("Em qual sequencia adicionar: ");
				scanf("%d",&j);
				while(getchar()!='\n'){}
				if(j<0){
					i=0;
				}else if(j<num_seq&&find_card(seq[j],"**")!=-1){
					pos=find_card(seq[j],"**");
					if(find_card(tri[j]+pos+2,"**")!=-1){
						ask[0]='~';
						while(ask[0]=='~'){
							printf("\n##Dois coringas encontrados##\n");
							printf("Usar o primeiro ou segundo?\n");
							printf("(0)Primeiro   (1)Segundo\n");
							if(ask[0]=='1'){
								pos+=find_card(tri[j]+pos+2,"**")+2;
							}else if(ask[0]!='0'){
								ask[0]='~';
								printf("###Comando Invalido###\n");
							}
						}
						sub_coringa(card,seq[j][pos-2],seq[j][pos+2]);
						if((find_card(hand[jog_atual],card))!=-1){
							remove_pos(seq[j],pos);
							add_in_pos(seq[j],card,pos);
							pos=find_card(hand[jog_atual],card);
							remove_pos(hand[jog_atual],pos);
							add_in_pos(seq[num_seq],"**",pos);
						}else{
							printf("###Sequencia Invalida###\n");
						}
					}
				}else{
					printf("###Sequencia Invalida###\n");
				}
			}else{
				printf("###Comando Invalido###");
			}
		}else{
			printf("\n\n\n\n\n###Comando Invalido###\n");
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