#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/*
Realizzare un programma che mostri come viene rappresentato uno spazio di N bit con i relativi numeri 
in complemento a 2. Se ho 4 bit ho un range da [-8 a +7] ed indentificare i casi di underflow ed overflow per una somma;
Il numero che viene inserito deve essere visualizzabile in binario con N bit.
Gli N bit vengono acquisiti ogni volta da tastiera. Compreso il numero decimale da trasformare in binario.
Stampare tutta la rappresentazione.
*/

int numero_di_bit;
int indice = 0;
long contatore = 0;
int check(int);
int controllo(int);
void intervallo(int*,int*,int);
void codework(int, int,char*,int,char**);
void converto_from_decimal_to_bin(int,int,char*,int,int,char**);
void view_double_array(char**,int,int);
int controllo_range(int,int,int);
int range(int,int,int);
char* accedi_al_valore(char**,int,int,int,int);

int main(int argc, char **argv)
{
	char numero_binario[1024] = {0};
	int* valori = (int*)malloc(sizeof(int)*2);
	char** valori_binari = (char**)malloc(sizeof(char*)*1024);

	char* val_somma;
	int start = 0, finish = 0;
	printf("Inserisci il numero di bit (max 32 bit):");
	scanf("%d", &numero_di_bit);
	if(controllo(numero_di_bit)==0)
		return 0;
	intervallo(&start,&finish,numero_di_bit); //avrei potuto mettere int* ptr = &start e passare il puntatore;
/*int start = 0, finish = 0;
int *ptr = &start;
intervallo(ptr,&finish,numero_di_bit); //dopo se stampo start è modificato dentro la funzione *ptr = *ptr + 1, e start 1
printf("Valore di start dopo la funzione = %d \n", start);
*/
	char **array_di_caratteri = (char**)malloc(sizeof(char*)*finish +(-start));
	int ret,somma = 0;
	printf("Intervallo di numeri = [ %d , %d ] \n", start, finish);
	codework(start,finish,numero_binario,numero_di_bit,array_di_caratteri);
	view_double_array(array_di_caratteri,start,finish);
	printf("Inserisci un primo numero all'interno del range : ");
	scanf("%d", &*(valori + 0));
	if(range(*(valori + 0),start,finish)==1)
		return 0;
	printf("Inserisci un secondo numero all'interno del range : ");
	scanf("%d", &*(valori + 1));
	if(range(*(valori + 1),start,finish)==1)
		return 0;
	somma = *(valori + 0) + *(valori + 1);

	for(int i=0;i<2;i++)
	{
		*(valori_binari + i) = accedi_al_valore(array_di_caratteri,start,finish,*(valori+i),0);
		printf(" %d = %s \n", *(valori+i),*(valori_binari+i));

	}

	val_somma = accedi_al_valore(array_di_caratteri,start,finish,somma,1);
	printf("Somma = %s \n", val_somma);

	return 0;
}
int range(int num, int start, int finish)
{
	int ret;
	ret = controllo_range(num,start,finish);
	if(ret == -1)
	{
		printf("Numero non rientra nel range [*] - Aborted. \n");
		return 1;
	}
	return 2;
}
int controllo_range(int num, int start, int finish)
{
	if((num>finish)||(num<start))
		return -1;
	return 1;


}
int check(int val)
{
	if(val >32)
		return -1;
	if(val == 0)
		return 2;
	if(val < 0)
		return 3;
	return 4;
}
int controllo(int numero_di_bit)
{
	int ret = check(numero_di_bit);
	if(ret ==-1)
	{
		perror("Aborted - non oltre 32 bit [*] \n");
		return 0;
	}
	if(ret == 2)
	{
		perror("Aborted - 0 non è un valore corretto [*] \n");
		return 0;
	}
	if(ret == 3)
	{
		perror("Aborted - il numero dei bit non può essere un numero negativo \n");
		return 0;
	}
	return 1;
}
void intervallo(int* starti, int* finish, int numero_di_bit)
{
	*starti = -1;
	*finish = 1;

	for(int i=0;i<numero_di_bit-1;i++)
	{
		*starti *= 2;
		*finish *= 2;
	}
	*finish = *finish - 1;


	return;
}
void codework(int start, int finish, char* bin, int numero_di_bit, char**d)
{
	for(int i=0;i<finish+(-start)+1;i++) //start = -x , -start = - - x = x 
		converto_from_decimal_to_bin(i,numero_di_bit,bin,start,finish,d);
	contatore = 0;
	return;

}
void converto_from_decimal_to_bin(int num, int numero_di_bit,char *binary,int start,int finish, char**d)
{
	char *appoggio=malloc(sizeof(char)*1024);
	int num_zeri;
	while(num!=0)
	{
		if(num%2==0)
			strcat(appoggio,"0");
		if(num%2!=0)
			strcat(appoggio,"1");
		num = num / 2;

	}
	//printf(" %s \n", appoggio);
	for(int i=strlen(appoggio)-1,j=0;i>=0;i--,j++) *(binary + j) = *(appoggio + i);
	num_zeri = numero_di_bit - strlen(binary);

	//printf("  binary = %s  ", binary); //in binary ho i numeri giusti ma devo fare padding avanti 

	for(int i=0;i<strlen(binary);i++) *(appoggio + i) = *(binary + i);   //copy binary in appoggio
	//printf("  appoggio = %s   e num zeri = %d\n", appoggio,num_zeri);

	for(int i=0;i<num_zeri;i++) *(binary + i ) = '0';
	for(int i=num_zeri,j=0;i<strlen(appoggio)+num_zeri;i++,j++) *(binary + i) = *(appoggio + j);
	//printf(" \t %s \n ", binary); //numero giusto in binary

	d[indice] = (char*)malloc(sizeof(char)*1024);

	memcpy(d[indice],binary,1024);
	indice = indice + 1;

	memset(appoggio, 0, 1024); //azzera il valore ogni volta
	memset(binary,0,1024);

	return;
}
void view_double_array(char** d, int start, int finish)
{
	int num = 0;
	for(int i=0;i<finish+(-start)+1;i++)
	{
		if(i>finish)
		{
			num = start+contatore; //(se a 4 bit, -8 + 0  = -8, -8 + 1 = -7, -8 + 2 = -6, etc...)
			contatore = contatore + 1;
			printf("[%d] =  %s \n",num, *(d + i));
		}
		else
		{
			printf("[%d] =  %s \n",i, *(d + i));
		}
	}
	contatore = 0;
}
char* accedi_al_valore(char** d, int start, int finish, int num_da_accedere,int check)
{
	puts(" ");
	char* valore = (char*)malloc(sizeof(char)*1024);
	bool trovato = false;
	contatore = 0;
	int num = 0;
	int app=0,numero_effettivo=0;


	for(int i=0;i<finish+(-start)+1;i++)
	{
		if(i>finish)
		{
			num = start + contatore;

			if(num == num_da_accedere)
			{
				trovato = true;
				strncpy(valore,*(d+i),1024);
				if(check==1)
					if(trovato==true)
						printf("OF=0.\n");
			}

			contatore = contatore + 1;
		}
		else
		{
			if(i==num_da_accedere)
			{
				trovato = true;
				strncpy(valore ,*(d+i),1024);
				if(check==1)
					if(trovato==true)
						printf("OF=0.\n");

			}
		}
	}
	contatore = 0;
	if(trovato == false) // se il valore di una somma non è stato trovato c'è un OF
	{
		if(num_da_accedere>0)
		{
			app = num_da_accedere-finish;
			numero_effettivo = start + (app-1);
			printf("Overflow, OF = 1. \n");
		}
		if(num_da_accedere<0)
		{
			app = num_da_accedere + (-start);
			app= abs(app);
			numero_effettivo = finish - (app-1);
			printf("Underflow, OF = 1.\n");
		}
		printf("La somma è = %d \n", numero_effettivo);
		num_da_accedere = numero_effettivo;

		for(int i=0;i<finish+(-start)+1;i++)
		{
			if(i>finish)
			{
				num = start + contatore;

				if(num == num_da_accedere)
				{
					trovato = true;
					strncpy(valore,*(d+i),1024);
				}

				contatore = contatore + 1;
			}
			else
			{
				if(i==num_da_accedere)
				{
					trovato = true;
					strncpy(valore ,*(d+i),1024);
				}
			}
		}
	}
	return valore;
	free(valore);
}
