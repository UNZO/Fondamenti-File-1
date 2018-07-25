#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define numCorsi	6	//	NUMERO	DI	CORSI
#define numEsami	10	//	NUMERO	DI	ESAMI

typedef struct { int CodCorso; char AnnoCorso[10]; int Crediti; } TipoCorso;
typedef struct { int Matricola; int CodCorso; int Voto; } TipoEsame;

TipoCorso	Corso[numCorsi] = {
{ 100,	"ANNO_1",	12 },
{ 101,	"ANNO_1",	13 },
{ 102,	"ANNO_1",	12 },
{ 103,	"ANNO_2",	12 },
{ 104,	"ANNO_2",	13 },
{ 105,	"ANNO_3",	12 } };

TipoEsame	Esami[numEsami] = {
{ 13,	100,	10 },
{ 13,	104,	21 },
{ 13,	102,	31 },
{ 13,	105,	10 },
{ 55,	104,	1 },
{ 23,	102,	4 },
{ 55,	102,	7 },
{ 23,	101,	9 },
{ 55,	105,	6 },
{ 23,	104,	4 } };

char NFCorso[] = "Corso.dat";
char NFesami[] = "Esami.dat";

void CreaFileCorso(void);

void CreaFileEsami(void);

int ContaEsami(char AnnoCorso[], int Matricola);

void StessoCorso(char AnnoCorso[], int Matricola1, int Matricola2);

TipoEsame *VettoreDinamico(char AnnoCorso[], int *DimVD);

void StampVD(int indirizzoVD, int DimVD);

void CambiaCrediti(char AnnoCorso[]);

void InserisciCorso(TipoCorso NewCorso);

//void VisualizzaCorsi(void);

void main()
{
	CreaFileCorso();
	CreaFileEsami();
	
	int x;
	int DimVD = 0;
	int indirizzoVD;

	x=ContaEsami("ANNO_1", 13);

	printf("il numero di esami restituiti e : %d", x);

	StessoCorso("ANNO_1", 13, 55);

	indirizzoVD=VettoreDinamico("ANNO_2", &DimVD);

	StampVD(indirizzoVD,DimVD);

	CambiaCrediti("ANNO_1");

	TipoCorso NewCorso = {999,"ANNO_2",999};

	InserisciCorso(NewCorso);

	//VisualizzaCorsi();

	getchar();
	getchar();
}

int ContaEsami(char AnnoCorso[], int Matricola)
{
	FILE *FC, *FE;
	
	int num= 0;
	
	TipoCorso Corso;
	TipoEsame Esame;

	FC=fopen(NFCorso, "rb");
	FE=fopen(NFesami, "rb");

	while (fread(&Corso, sizeof(TipoCorso), 1, FC) > 0)//fino a che non l'ho letto tutto
	{
		if (strcmp(Corso.AnnoCorso, AnnoCorso) != 0)
		{
			//controllo ora in esami per controllare se la matricaol ha sostenuto un esame di quel corso
			while (fread(&Esame, sizeof(TipoEsame), 1, FE) > 0)//controllo tutti esami
			{
				if ((Esame.CodCorso == Corso.CodCorso) && (Esame.Matricola == Matricola) && (Esame.Voto < 25))
				{
					num++;
				}
			}
			fseek(FE, 0, SEEK_SET);
		}
	}
	
		fclose(FC);
		fclose(FE);

		return num;
	//dato un anno di corso (AnnoCorso) ed una Matricola, restituisce il numero di esami sostenuti dalla
	//Matricola relativi a corsi con anno differente dall’AnnoCorso dato e con voto inferiore a 25.
}

void StessoCorso(char AnnoCorso[], int Matricola1, int Matricola2)
{
	// stampi a video i corsi del file “Corso.dat” dell’anno AnnoCorso tali che entrambe le matricole
	//Matricola1 e Matricola2 hanno sostenuto l’esame del corso

	int m1bool = 0;
	int m2bool = 0;

	FILE *FC, *FE;

	FC = fopen(NFCorso, "rb");
	FE = fopen(NFesami, "rb");

	TipoCorso Corso;
	TipoEsame Esame;

	while (fread(&Corso, sizeof(TipoCorso), 1, FC))
	{
		if (strcmp(Corso.AnnoCorso, AnnoCorso) == 0)
		{
			while (fread(&Esame, sizeof(TipoEsame), 1, FE))
			{
				if ((Esame.Matricola == Matricola1)&&(Esame.CodCorso==Corso.CodCorso))
				{
					m1bool = 1;
				}
			}
			fseek(FE,0,SEEK_SET);
			while (fread(&Esame, sizeof(TipoEsame), 1, FE))
			{
				if ((Esame.Matricola == Matricola2) && (Esame.CodCorso == Corso.CodCorso))
				{
					m2bool = 1;
				}
			}
			fseek(FE, 0, SEEK_SET);
			if ((m1bool == 1) && (m2bool == 1))
			{
				//printf && preparo per nuovo corso
				printf("\n %d  %s  %d", Corso.CodCorso, Corso.AnnoCorso, Corso.Crediti);
				m2bool = 0;
				m2bool = 0;
			}
		}
	}

	fclose(FC);
	fclose(FE);

}

TipoEsame *VettoreDinamico(char AnnoCorso[], int *DimVD)
{
	FILE *FC, *FE;

	FC = fopen(NFCorso, "rb");
	FE = fopen(NFesami, "rb");

	TipoCorso Corso;
	TipoEsame Esame;

	TipoEsame *VD=NULL;

	int len = 0;

	while (fread(&Corso, sizeof(TipoCorso), 1, FC) >0 )
	{
		if (strcmp(Corso.AnnoCorso, AnnoCorso) == 0)//se trovo l'anno giusto guardo in tutti gli esami
		{
			while (fread(&Esame, sizeof(TipoEsame), 1, FE) > 0)
			{
				if (Esame.CodCorso == Corso.CodCorso)
				{
					(*DimVD)++;
				}
			}
			fseek(FE, 0, SEEK_SET);
		}
	}//fine calcolo VD ora alloco
	
	fseek(FC, 0, SEEK_SET);//mi riporo in posizione iniziale anche perr i corsi

	VD = (TipoEsame*)malloc((*DimVD) * sizeof(TipoEsame));//ho allocato correttamente il vettore VD e ora ci scrivo dentro e poi lo ritorno
	
	if ((*DimVD) > 0)
	{
		while (fread(&Corso, sizeof(TipoCorso), 1, FC) > 0)
		{
			if (strcmp(Corso.AnnoCorso, AnnoCorso) == 0)//se trovo l'anno giusto guardo in tutti gli esami
			{
				while (fread(&Esame, sizeof(TipoEsame), 1, FE) > 0)
				{
					if (Esame.CodCorso == Corso.CodCorso)//se trovo un esame del corso giusto
					{
						VD[len++] = Esame;
						//VD[len] = Esame;
						//len++;
					}
				}
				fseek(FE, 0, SEEK_SET);
			}
		}
	}
	fclose(FC); fclose(FE);
	return VD;
}

void StampVD(int indirizzoVD, int DimVD)
{
	int i;
	TipoEsame *temp;
	temp = indirizzoVD;
	
	for (i = 0;i < DimVD;i++)
	{
		printf("\n\n%d  %d  %d", temp[i].CodCorso,temp[i].Matricola,temp[i].Voto);
	}
}

void CambiaCrediti(char AnnoCorso[])
{
	//modifichi il file “Corso.dat” incrementando di uno i
	//crediti dell’AnnoCorso specificato, ad eccezione dei corsi con maggior numero di crediti per quell’anno.
	//Ad esempio CambiaCrediti("ANNO_1") incrementa per i corsi 100 e 102. Stampare a video il nuovo file.
	int maxCred = 0;
	int alreadyChanged = 0;

	FILE *FC, *FE;

	TipoCorso Corso;
	TipoEsame Esame;

	FC = fopen(NFCorso, "rb+");
	FE = fopen(NFesami, "rb");

	//calcolo subito quale è il numero di crediti massino min corsi

	while (fread(&Corso, sizeof(TipoCorso), 1, FC) >0 )
	{
		if ((Corso.Crediti>maxCred)&&(strcmp(Corso.AnnoCorso,AnnoCorso)==0))
		{
			maxCred = Corso.Crediti;
		}
	}
	fseek(FC, 0, SEEK_SET);

	//ora so quali corsi eventialmente escludere

	while (fread(&Corso, sizeof(TipoCorso), 1, FC) > 0)
	{
		if ((strcmp(Corso.AnnoCorso, AnnoCorso) == 0) && (Corso.Crediti != maxCred))
		{
			while (fread(&Esame, sizeof(TipoEsame), 1, FE) > 0)
			{
				if (Esame.CodCorso == Corso.CodCorso)
				{
					if (!alreadyChanged)
					{
						//ora cambio i crediti
						fseek(FC, -(int)sizeof(TipoCorso), SEEK_CUR);//mando indietro di 1 iun modo tale da riscrivere con il credito incrementato
						Corso.Crediti++;//incremento il credito di 1
						fwrite(&Corso, sizeof(TipoCorso), 1, FC);//scrivo l'esame in questione
						fseek(FC, 0, SEEK_CUR);//non so a cosa serva
						alreadyChanged = 1;
					}
				}
			}
			fseek(FE, 0, SEEK_SET);
		}
		alreadyChanged = 0;
	}
	//ora stampo il nuovo file cosi generato
	fseek(FC, 0, SEEK_SET);
	while (fread(&Corso, sizeof(TipoCorso), 1, FC) > 0)
	{
		printf("\n -- %d  %s  %d", Corso.CodCorso, Corso.AnnoCorso, Corso.Crediti);
	}
}

void InserisciCorso(TipoCorso NewCorso)
{
	FILE *FC ,*FT;

	FC = fopen(NFCorso, "rb");
	FT = fopen("temp.dat", "wb");
	
	TipoCorso Corso;
	TipoCorso temp;

	while ((fread(&Corso, sizeof(TipoCorso), 1, FC) > 0)&&(strcmp(Corso.AnnoCorso,NewCorso.AnnoCorso)<0))
	{
			//copio semplicemente senza aggiungere
			fwrite(&Corso, sizeof(TipoCorso), 1, FT);
	}
	fwrite(&NewCorso, sizeof(TipoCorso), 1, FT);

	if (!feof(FC))//se non sono ancora arrivato a EOF continuo a copiare
	{
		do
		{
			//copio e basta fino a EOF
			fwrite(&Corso, sizeof(TipoCorso), 1, FT);
		} while (fread(&Corso, sizeof(TipoCorso), 1, FC) > 0);
	}
	fclose(FT);
	FT = fopen("temp.dat", "rb");

	fseek(FT, 0, SEEK_SET);
	while (fread(&temp, sizeof(TipoCorso), 1, FT)	>	0)
		printf("\n	%d	- %s		%d", temp.CodCorso, temp.AnnoCorso, temp.Crediti);

	fclose(FT);
	fclose(FC);
	

}

//void VisualizzaCorsi(void) {
//	FILE	*FC;
//	TipoCorso	RC;
//	FC = fopen(NFCorso, "rb");
//	while (fread(&RC, sizeof(TipoCorso), 1, FC)	>	0)
//		printf("\n	%d	- %s		%d", RC.CodCorso, RC.AnnoCorso, RC.Crediti);
//	fclose(FC);
//}

void CreaFileCorso(void) {
	FILE*	FA;
	FA = fopen(NFCorso, "wb");
	fwrite(Corso, sizeof(TipoCorso), numCorsi, FA);
	fclose(FA);
}

void CreaFileEsami(void) {
	FILE*	FA;
	FA = fopen(NFesami, "wb");
	fwrite(Esami, sizeof(TipoEsame), numEsami, FA);
	fclose(FA);
}
