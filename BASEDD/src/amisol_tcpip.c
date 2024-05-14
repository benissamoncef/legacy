/*E*/
/************************************************
* MODULE  : BASEDD                             
* FICHIER : amisol_tcpic.c                          
*************************************************
* DESCRIPTION : fichier contenant les fonctions
* de gestion du transfert tcpip et 
* de formatage des donnees a envoyer
*
* et des parametres echange + langage LCR
*
*************************************************
* HISTORIQUE :
* ET : creation 06/2002
* JPL	17/11/10 : Suppression warnings; GESTION DES TAILLES VARIABLES (compil. HP ia64) (DEM 961)  1.2
* SFO   06/10/14 : Adaptation pour compilation sous linux
************************************************/
#include <xzsem.h>
#include "amisol_tcpip.h"


/****************************/
/* definitions de constantes */
/*****************************/

/* --------- Definition Constantes Transfert misol --------------- */
/* -- Format attendu pour la ligne d'entete: */
/* #p=B,dt=jj/mm/aa,hr=hh:mm:ss,sq=1 */
/* -------------------------------------------------------------------*/


#define C_SYMBOL_P		"#p=B"
#define C_SYMBOL_DT		"dt="
#define C_SYMBOL_HR		"hr="
#define C_SYMBOL_SQ		"sq=1"


/* -- Format attendu pour un point de mesure:
* #pm=MMS69.A1 \n
* QT,063,1 \n
* TT,02,1 \n
* VT,120,1 \n
* 
* 1 est ici l'indicateur de validite
* 1 : donnees brutes
* 2 : donnees reconstituees manuellement
* 3 : donnees validees
* -------------------------------------------------------------------*/

#define C_SYMBOL_PM 		"#pm="
#define C_SYMBOL_QT		"QT"
#define C_SYMBOL_TT             "TT"
#define C_SYMBOL_VT             "VT"

/* --- Autres constantes du format d'echange --*/


#define C_SYMBOL_END_LINE       "\n"
#define C_SYMBOL_SEPARATOR	","
#define C_SYMBOL_VALIDITE	"1"

/* --  Constantes identification, acknowledgement -- */
#define C_LOGIN         	"ID FESCOTA FESCOTA\r\n"
#define C_SYMBOL_OK 		"ACQ 1"
#define C_SYMBOL_TC_MES		"TC MES\r\n"
#define C_SYMBOL_FIN		"FIN\r\n"

/* -- constantes max string --- */

#define C_MAX_SIZE_ENTETE	35
#define C_MAX_SIZE_PM		13
#define C_MAX_SIZE_QT		9
#define C_MAX_SIZE_TT		8
#define C_MAX_SIZE_VT           9

/* -- format attendu --*/

#define C_FORMAT_QT		3
#define C_FORMAT_TT             2
#define C_FORMAT_VT             3

/* -- constantes max val ---*/
#define C_MAX_VAL_QT 	999
#define C_MAX_VAL_TT  	99
#define C_MAX_VAL_VT  	999

/* -- Definition Constantes Connection TCPIP --*/

#define C_DEST_IP	"10.202.100.109"
/*
#define C_DEST_IP	"194.5.181.215"
*/
#define C_DEST_PORT	10015

/* -- Constantes Max Buffer pour reception message misol --*/
#define C_REC_MAX_SIZE		60	

/********************************************/
/* declaration de variables locales au module*/
/*********************************************/
static char *version = "@(#)amisol_tcpip.c 1.0 06/2002 : creation";

/*-- table contient les points de mesure (station,qt,tt,vt)*/ 
/*init to NULL so first time realloc = malloc*/

char **pm_table_mesure=NULL; 

/*-- compteur lie a la table des mesure */
int ind_mesure = 0;

/*-- string d'entete */
char vm_entete[C_MAX_SIZE_ENTETE];

/* descripteur et struct socket declaree dans socket.h*/
int vm_sockfd = 0;
struct sockaddr_in sm_dest_addr;




/*A
**************************************************
* Construit la ligne d'entete a transferer
* Format : #p=B,dt=dd/mm/yy,hr=hh:mm:ss,sq=1
*************************************************
*/
void amisol_tcpip_construct_entete()
{
	char vl_dt[20];
	char vl_hr[20];
	char vl_date[40];

	char vl_c_annee[5];
	char vl_c_jour[3];
	char vl_c_mois[3];
	char vl_c_heure[3];
	char vl_c_minute[3];
	char vl_c_seconde[3];

	int vl_jour 	= 0;
	int vl_mois 	= 0;
	int vl_annee 	= 0;
	int vl_heure 	= 0;
	int vl_minute 	= 0;
	int vl_seconde 	= 0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_construct_entete : IN");
	
	/* recuperation date et heure */
	strcpy(vl_date,amisol_tcpip_get_heure());


	if(sscanf( vl_date, "%02d/%02d/%4d %02d:%02d:%02d",
                                         &vl_jour,
                                         &vl_mois,
                                         &vl_annee,
                                         &vl_heure,
                                         &vl_minute,
                                         &vl_seconde)!=6)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_get_heure : le format de la date est incorrecte !" );
		return;
	}

	
	/* conversion des elements en string */
	/*itoa(vl_annee,vl_c_annee);
	itoa(vl_mois,vl_c_mois);
	itoa(vl_jour,vl_c_jour);
	itoa(vl_heure,vl_c_heure);
	itoa(vl_minute,vl_c_minute);
	itoa(vl_seconde,vl_c_seconde);*/

         sprintf(vl_c_annee,"%d",vl_annee);
         sprintf(vl_c_mois,"%d",vl_mois);
         sprintf(vl_c_jour,"%d",vl_jour);
         sprintf(vl_c_heure,"%d",vl_heure);
         sprintf(vl_c_minute,"%d",vl_minute);
         sprintf(vl_c_seconde,"%d",vl_seconde);

	/* tous sur 2 digits */
/*	fill_array_to_size(vl_c_mois, 2);
	fill_array_to_size(vl_c_jour, 2);
	fill_array_to_size(vl_c_heure, 2);
	fill_array_to_size(vl_c_minute, 2);
	fill_array_to_size(vl_c_seconde, 2);
*/
	
	/* get only the last 2 chars of the year */
	vl_c_annee[0] = vl_c_annee[2];
	vl_c_annee[1] = vl_c_annee[3];
	vl_c_annee[2] = '\0';

	/* construction date */
	strcpy(vl_dt ,vl_c_jour);
	strcat(vl_dt,"/");
	strcat(vl_dt,vl_c_mois);
	strcat(vl_dt,"/");
	strcat(vl_dt,vl_c_annee);

	/* construction heure */
	strcpy(vl_hr ,vl_c_heure);
	strcat(vl_hr,":");
	strcat(vl_hr,vl_c_minute);
	strcat(vl_hr,":");
	strcat(vl_hr,vl_c_seconde);

	/* construction entete */
	strcpy(vm_entete , C_SYMBOL_P);
	strcat(vm_entete,C_SYMBOL_SEPARATOR);
	strcat(vm_entete,C_SYMBOL_DT);
	strcat(vm_entete,vl_dt);
	strcat(vm_entete,C_SYMBOL_SEPARATOR);
	strcat(vm_entete,C_SYMBOL_HR);
	strcat(vm_entete,vl_hr);
	strcat(vm_entete,C_SYMBOL_SEPARATOR);
	strcat(vm_entete,C_SYMBOL_SQ);
	strcat(vm_entete,C_SYMBOL_END_LINE);

	XZST_03EcritureTrace(XZSTC_INFO,"amisol_tcpip_construct_entete :  entete = %s ",vm_entete);
	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_construct_entete : OUT");

	
	return;
}


/*A
***************************************************
* MODULE amisol_tcpip 
**************************************************
* DESCRIPTION DE LA FONCTION :
* Construit tableau des mesures avant transfert tcp
*************************************************
* SEQUENCE D'APPEL :
*
* Arguments
* va_nom_station : point de mesure
* va_val_QT : debit tous vehicules
* va_val_TT : taux tous vehicules
* va_val_VT : vitesses moyenne tous vehicules
*
*
* Code Retour
* XD_OK
* XD_NOK
*
* Mecanismes
* Cette procedure est appelee pour chaque ligne retournee
* par la procedure dans la fonction XZAE175_Get_6min_Sequence.
* Elle met a jour le tableau pm_table_mesure avec la ligne de la proc retournee.
* Le compteur ind_mesure est mis a jour et sera reutilise
* lors de l'envoi de ces donnees.
**********************************************/
int amisol_tcpip_construct_table_mesure(char *va_nom_station,int va_val_QT,int va_val_TT, int va_val_VT)
{


//	char vl_ligne_pm[C_MAX_SIZE_PM];
//	char vl_ligne_QT[C_MAX_SIZE_QT];
//	char vl_ligne_TT[C_MAX_SIZE_TT];
//	char vl_ligne_VT[C_MAX_SIZE_VT];
	char vl_ligne_pm[255];
	char vl_ligne_QT[255];
	char vl_ligne_TT[255];
	char vl_ligne_VT[255];

	/* pour forcer au format attendu */
	char vl_temp_QT[C_FORMAT_QT + 2];
	char vl_temp_TT[C_FORMAT_TT + 2];
	char vl_temp_VT[C_FORMAT_VT + 2];
	
	char vl_temp2_VT[C_FORMAT_VT + 2];
	char vl_temp2_TT[C_FORMAT_TT + 2];
	char vl_temp2_QT[C_FORMAT_QT + 2];

	/* pour conversion int en char */
	char s_QT[C_FORMAT_QT + 2];
	char s_TT[C_FORMAT_TT + 2];
	char s_VT[C_FORMAT_VT + 2];
	
	/* length des strings */
	int lenVT = 0;
	int lenTT = 0;
	int lenQT = 0;

	/* pour parcours string */
	int i = 0;
	int diff = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_construct_table_mesure : IN - va__nom_station=%s va_val_QT=%d va_val_TT=%d va_val_VT=%d", va_nom_station, va_val_QT, va_val_TT, va_val_VT);

	/* check des valeurs max */
	if (va_val_QT > C_MAX_VAL_QT)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"amisol_tcpip_construct_table_mesure : la valeur de QT %d est > au max autorise de %d",va_val_QT,C_MAX_VAL_QT);
/*		return(XDC_NOK);
PFE 1 */
va_val_QT = 0;

	}
	if (va_val_TT > C_MAX_VAL_TT)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"amisol_tcpip_construct_table_mesure : la valeur de TT %d est > au max autorise de %d",va_val_TT,C_MAX_VAL_TT);
		return(XDC_NOK);
	}
	if (va_val_VT > C_MAX_VAL_VT)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"amisol_tcpip_construct_table_mesure : la valeur de VT %d est > au max autorise de %d",va_val_VT,C_MAX_VAL_VT);
		return(XDC_NOK);
	}

	/* conversion int to array */
	/*itoa(va_val_QT,s_QT);*/
	sprintf(s_QT,"%d",va_val_QT);
	strcpy(vl_temp_QT,s_QT);

/*	itoa(va_val_TT,s_TT);*/
	sprintf(s_TT,"%d",va_val_TT);
	strcpy(vl_temp_TT, s_TT);

/*	itoa(va_val_VT,s_VT);*/
	sprintf(s_VT,"%d",va_val_VT);
	strcpy(vl_temp_VT,s_VT);
 

	/* Get length */
	lenVT = strlen(vl_temp_VT);
	lenTT = strlen(vl_temp_TT);
	lenQT = strlen(vl_temp_QT);

	
	/* pour QT */
	 if (lenQT < C_FORMAT_QT)
	 {

		 diff = C_FORMAT_QT - lenQT;
		 for (i=0;i<diff;i++)
		 {
			 vl_temp2_QT[i]='0';
		 }
		 vl_temp2_QT[i]='\0';
		strcat(vl_temp2_QT,vl_temp_QT);
	 }
	 else
	 {
	 	strcpy(vl_temp2_QT,vl_temp_QT);
	 }

	/* pour TT */
	if (lenTT < C_FORMAT_TT)
	{

		diff = C_FORMAT_TT - lenTT;
		for (i=0;i<diff;i++)
		{
			vl_temp2_TT[i]='0';
		}
		vl_temp2_TT[i]='\0';
		strcat(vl_temp2_TT,vl_temp_TT);

	}
	else
	{
		 strcpy(vl_temp2_TT,vl_temp_TT);
	}
	
	/* pour VT */
	lenVT = strlen(vl_temp_VT);
	if ( lenVT  < C_FORMAT_VT)
	{
		
		diff = C_FORMAT_VT - lenVT;
		for (i=0;i<diff;i++)
		{
			vl_temp2_VT[i]='0';
		}
		vl_temp2_VT[i]='\0';
		strcat(vl_temp2_VT,vl_temp_VT);
	}
	else
	{
		strcpy(vl_temp2_VT,vl_temp_VT);
	}
	
	strcpy(vl_ligne_pm , C_SYMBOL_PM);
	strcat(vl_ligne_pm,va_nom_station);
	strcat(vl_ligne_pm,C_SYMBOL_END_LINE);
	vl_ligne_pm[C_MAX_SIZE_PM] = '\0';

XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_temp_QT = [%s]",vl_temp_QT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_temp_TT = [%s]",vl_temp_TT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_temp_VT = [%s]",vl_temp_VT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_temp2_QT = [%s]",vl_temp2_QT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_temp2_TT = [%s]",vl_temp2_TT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_temp2_VT = [%s]",vl_temp2_VT);

XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_pm = [%s]",vl_ligne_pm);
	amisol_tcpip_format_mesure(vl_temp2_VT, vl_ligne_VT, C_SYMBOL_VT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_VT = [%s]",vl_ligne_VT);
/*	strcpy(s_VT,vl_ligne_VT);*/
	amisol_tcpip_format_mesure(vl_temp2_QT, vl_ligne_QT, C_SYMBOL_QT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_QT = [%s]",vl_ligne_QT);
	amisol_tcpip_format_mesure(vl_temp2_TT, vl_ligne_TT, C_SYMBOL_TT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_TT = [%s]",vl_ligne_TT);

XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_pm = [%s]",vl_ligne_pm);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_QT = [%s]",vl_ligne_QT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_TT = [%s]",vl_ligne_TT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : vl_ligne_VT = [%s]",vl_ligne_VT);

	if ((pm_table_mesure = (char **) realloc(pm_table_mesure,(ind_mesure+4) * sizeof(char *))) != NULL)
	{
		
		/* allocation taille pour le PME */
		if ((pm_table_mesure[ind_mesure] = (char *) malloc(strlen(vl_ligne_pm)+1)) != NULL)
		{
			 strcpy(pm_table_mesure[ind_mesure],vl_ligne_pm);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : PME pm_table_mesure[%d]=[%s]",ind_mesure,pm_table_mesure[ind_mesure]);
			 ind_mesure++;
		}
		else
		{
			 XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_construct_table_mesure : le malloc a echoue !" );
			 return XDC_NOK;

		}
		/* allocation taille pour le QT */
		 if ((pm_table_mesure[ind_mesure] = (char *) malloc(strlen(vl_ligne_QT)+1)) != NULL)
		{
			strcpy(pm_table_mesure[ind_mesure],vl_ligne_QT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : QT  pm_table_mesure[%d]=[%s]",ind_mesure,pm_table_mesure[ind_mesure]);
			ind_mesure++;
		}
		 else
		{
		  	XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_construct_table_mesure : le malloc a echoue !" );                  
	   		return XDC_NOK;

		}
		/* allocation taille pour le TT */
		if ((pm_table_mesure[ind_mesure] = (char *) malloc(strlen(vl_ligne_TT)+1)) != NULL)
		{
			strcpy(pm_table_mesure[ind_mesure],vl_ligne_TT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : TT  pm_table_mesure[%d]=[%s]",ind_mesure,pm_table_mesure[ind_mesure]);
			ind_mesure++;
		}
		else
		{
		  	XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_construct_table_mesure : le malloc a echoue !" );
		  	return XDC_NOK;
		}
		/* allocation taille pour le VT */
		if ((pm_table_mesure[ind_mesure] = (char *) malloc(strlen(vl_ligne_VT)+1)) != NULL)
		{
			strcpy(pm_table_mesure[ind_mesure],vl_ligne_VT);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_tcpip_construct_table_mesure : VT  pm_table_mesure[%d]=[%s]",ind_mesure,pm_table_mesure[ind_mesure]);
			ind_mesure++;
		}
		else
	 	{
	   		XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_construct_table_mesure : le malloc a echoue !" );
		     	return XDC_NOK;
      		}
	}
	else
	{
		 XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_construct_table_mesure : le realloc a echoue !" );
		 return XDC_NOK;
	}
XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_construct_table_mesure : OUT - ind_mesure = %d",ind_mesure);
	return (XDC_OK);
}


/*A
************************************************
* liberation des ressources
* reinitialisation du compteur
***********************************************
*/
int amisol_free_ressources()
{
	int m=0;
	XZST_03EcritureTrace(XZSTC_INFO,"amisol_free_ressources: IN avec ind_mesure vaut %d", ind_mesure);
	for (m = 0; m < ind_mesure; m ++)
	{
		free(pm_table_mesure[m]);
#ifdef _HPUX_SOURCE
		free(pm_table_mesure);
#endif
	}
	/* re_init for next run */
	pm_table_mesure = NULL;
	ind_mesure = 0;
	return(XDC_OK);
}

/*A
************************************************
* construit une ligne de mesure QT, TT ou VT
**********************************************
*/
//void amisol_tcpip_format_mesure(char s_source[], char s_dest[], char s_symbol[])
void amisol_tcpip_format_mesure(char *s_source, char *s_dest, char *s_symbol)
{
	
//	s_dest[0] = '\0';
	strcpy(s_dest , s_symbol);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	strcat(s_dest,C_SYMBOL_SEPARATOR);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	strcat(s_dest,s_source);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	strcat(s_dest,C_SYMBOL_SEPARATOR);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	strcat(s_dest,C_SYMBOL_VALIDITE);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	strcat(s_dest,C_SYMBOL_END_LINE);
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	s_dest[strlen(s_dest)] = '\0';
XZST_03EcritureTrace(XZSTC_DEBUG1,"amisol_free_ressources: sdest=[%s] strlen(sdest)=%d", s_dest, strlen(s_dest));
	
	return;

}

/*A
************************************************
* complemente par des 0 a gauche string s, 
* pour obtenir une taille de length_end
**********************************************
*/
void fill_array_to_size(char s[], int length_end)
{

	char s_temp2[C_MAX_SIZE_ENTETE];
	int i = 0;
	int length_beg = 0;
	int diff = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION," fill_array_to_size: IN");

	length_beg = strlen(s);

	if (length_beg > length_end)
	{ exit;}

	diff = length_end - length_beg;
	
	for(i=0;i<diff;++i)
	{
		s_temp2[i]= '0';
	}
	s_temp2[i]='\0';
	strcat(s_temp2,s);
	
	s[0] = '\0';
	strcpy(s,s_temp2);

	return;

}

/*A
*************************************************
* fonction qui retourne la date courante
************************************************
*/
char * amisol_tcpip_get_heure()
{
static XZSMT_Horodate  vl_HorodateZero = "";
XZSMT_Horodate  vl_Horodate     = "";
double          vl_HorodateSec  = 0.0;
int             vl_JourSemaine  = 0;

	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME, &vl_JourSemaine, &vl_HorodateSec);
	XZSM_11ConversionHorodate(vl_HorodateSec, &vl_HorodateZero);
	return(vl_HorodateZero);
}

/*A
******************************************************
* fonction qui convertit un integer en char
******************************************************
*/
void itoa(int n, char s[])
{
	int i, sign;

	if ((sign = n) < 0)
	{
		n = -n;
	}
	i=0;
	do
	{
		s[i++] = n % 10 + '0';
	} while((n /=10) > 0);
	if (sign < 0)
	{
		s[i++] = '-';
	}
	s[i] = '\0';
	reverse(s);

	return;
}


/*A
******************************************************
* fonction appelee par itoa qui inverse une string
*****************************************************
*/
void reverse(char s[])
{
	int c, i, j;
	for(i=0,j=strlen(s)-1;i<j; i++,j--)
	{
		c=s[i];
		s[i] = s[j];
		s[j]=c;
	}
	return;
}
/*A
**************************************************
* fonction qui initialise la socket de connection 
*************************************************
*/
int amisol_tcpip_connexion_init()
{

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_connexion_init: IN");
	
	/* recupere fichier descripteur et test*/ 
	if ((vm_sockfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur creation socket amisol_tcpip_connexion_init");
		return(XDC_NOK);

	}

	/* definition de la socket */
	sm_dest_addr.sin_family = AF_INET;
	sm_dest_addr.sin_port = htons(C_DEST_PORT);
	sm_dest_addr.sin_addr.s_addr = inet_addr(C_DEST_IP);
	bzero(&(sm_dest_addr.sin_zero), 8);

	/* connection serveur misol et test*/
	sem_wait(&vg_semaphore); 
	if (connect(vm_sockfd,(struct sockaddr *)&sm_dest_addr,sizeof(struct sockaddr)) == -1 )
	{
		sem_post(&vg_semaphore); 
		close(vm_sockfd);
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur connection fct  amisol_tcpip_connexion_init sur IP:%s PORT:%d",C_DEST_IP,C_DEST_PORT);
		return(XDC_NOK);

	}
	else
	{
		sem_post(&vg_semaphore); 
		XZST_03EcritureTrace(XZSTC_INFO, " connection fct  amisol_tcpip_connexion_init sur IP:%s PORT:%d OK !",C_DEST_IP,C_DEST_PORT);
		return(XDC_OK);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_connexion_init: OUT");
}

/*A
*******************************************************************
* fonction qui envoi un message vers le misol
******************************************************************
*/
int amisol_tcpip_send(char *vl_mess)
{
	int vl_bytes_sent;
	int vl_len;
	vl_bytes_sent = 0;
	vl_len=strlen(vl_mess);

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_send: IN");

	if (vl_len == 0)
	{
		 XZST_03EcritureTrace(XZSTC_WARNING, "amisol_tcpip_send: vl_mess vide !");
		 return(XDC_NOK);

	}

	sem_wait(&vg_semaphore); 
	if ((send(vm_sockfd,vl_mess,vl_len,0))!= vl_len)
	{
		sem_post(&vg_semaphore); 
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur fct amisol_tcpip_send: send retour en erreur pour envoi message %s ",vl_mess);
		return(XDC_NOK);

	}
	else
	{
		sem_post(&vg_semaphore); 
	 	XZST_03EcritureTrace(XZSTC_INFO, "fct amisol_tcpip_send envoi message %s  " ,vl_mess);
		return(XDC_OK);
	}
}


/*A
*****************************************************************
* fonction qui recoit un message du misol
***************************************************************
*/
int amisol_tcpip_receive(char *vl_buf)
{
	int vl_numbytes;
	int j=0;

	vl_numbytes = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_receive: IN");

	/* pour attente de reception du message */
	sem_wait(&vg_semaphore); 
	while (j<50)
	{
		vl_numbytes=recv(vm_sockfd,vl_buf,C_REC_MAX_SIZE,0);
		if (vl_numbytes != 0 && vl_numbytes != -1)
		/* message recu , message non vide */
		{
			j=50;
		}
		j++;
	}
	sem_post(&vg_semaphore); 

	if ((vl_numbytes == -1) || (vl_numbytes == 0))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "fct amisol_tcpip_receive pb reception message, message non recu ou message vide");
		return(XDC_NOK);
	}
	else
	{
		vl_buf[vl_numbytes]='\0';
		XZST_03EcritureTrace(XZSTC_INFO, "fct amisol_tcpip_receive  message %s " ,vl_buf);
		return(XDC_OK);
	}
}

/*A
*************************************************************
* fonction de fin de connexioin 
*************************************************************
*/
void amisol_tcpip_connexion_close()
{
	close(vm_sockfd);
	XZST_03EcritureTrace(XZSTC_FONCTION, "fct amisol_tcpip_connexion_close  OUT  ");
	return;
}

/*
***********************************************************
fonction de gestion header : id , tc_mes
**********************************************************
*/
int amisol_tcpip_manage_header_message()
{
	char *vl_id = C_LOGIN;
	char *vl_tc_mes = C_SYMBOL_TC_MES;
	char vl_buf_ACQ[C_REC_MAX_SIZE];
	char vl_buf2_ACQ[C_REC_MAX_SIZE];

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_manage_header_message: IN");

	/* send ID */
	if ((amisol_tcpip_send(vl_id)) != XDC_OK)
	{
		 XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : pb appel amisol_tcpip_send pour identification fournisseur %s", C_LOGIN);
		 return(XDC_NOK);
	}

	/* ID LOGIN sent with succes, get ACQ */
	if((amisol_tcpip_receive(vl_buf_ACQ)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : pb appel amisol_tcpip_receive pour reception ACQ #1");
		return(XDC_NOK);
	}
	else
	{
	
		XZST_03EcritureTrace( XZSTC_WARNING, "message recu depuis serveur : %s : %s : ",C_DEST_IP, vl_buf_ACQ);
		if((strcmp(vl_buf_ACQ,C_SYMBOL_OK)) != 0)
		{
			 XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : Erreur lors de l'identification; message retourne: %s ",vl_buf_ACQ);
			 return(XDC_NOK);

		}
		else
		{
			XZST_03EcritureTrace( XZSTC_INFO, "amisol_tcpip_manage_header_message : Identification aupres misol2 OK");
		}
	}

	/* send TC MES , indique que le fournisseur va envoyer des donnees */
	if ((amisol_tcpip_send(vl_tc_mes)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : pb appel amisol_tcpip_send pour envoi commande %s", C_SYMBOL_TC_MES);
		return(XDC_NOK);

	}

	/* TC MES command sent with succes get ACQ*/
	if((amisol_tcpip_receive(vl_buf2_ACQ)) != XDC_OK)
	{
		 XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : pb appel amisol_tcpip_receive pour reception ACQ #2 ");
		 return(XDC_NOK);
	}
	else
	{
		if((strcmp(vl_buf2_ACQ,C_SYMBOL_OK)) != 0)
		{
			  XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_header_message : Erreur lors de l envoi de la commande %s  message retourne: %s ",C_SYMBOL_TC_MES,vl_buf2_ACQ);
			  return(XDC_NOK);

		}
		else
		{
			XZST_03EcritureTrace( XZSTC_INFO, "amisol_tcpip_manage_header_message :  ACQ de TC_MES  OK");
		}
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "amisol_tcpip_manage_header_message :  OUT");
	return(XDC_OK);



}

/*A
***********************************************************************
* fonction qui envoi les mesures entetes + donnees 
**********************************************************************
*/
int amisol_tcpip_send_body_message()
{	
	int i;

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_send_body_message: IN");
	
	/* construction ligne entete en var globale */
	amisol_tcpip_construct_entete();
	
	/* envoi ligne entete */
	if ((amisol_tcpip_send(vm_entete)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_send_body_message : pb appel amisol_tcpip_send pour envoi commande %s", vm_entete);
		return(XDC_NOK);

	}
	else
	{
		XZST_03EcritureTrace( XZSTC_INFO, "amisol_tcpip_send_body_message : envoi entete  %s OK ", vm_entete);
	}
	

	/* envoi des donnees */
	for (i=0;i<ind_mesure;++i)
	{
		if ((amisol_tcpip_send(pm_table_mesure[i])) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_send_body_message : pb appel amisol_tcpip_send pour envoi commande %s", *pm_table_mesure[i]);
			return(XDC_NOK);

		}
		else
		{
			XZST_03EcritureTrace( XZSTC_INFO, "amisol_tcpip_send_body_message : envoi mesure  %s  ", pm_table_mesure[i]);
		}
	}
	
	/* test si toutes les donnees ont ete transferees */
	if(i == ind_mesure)
	{
		XZST_03EcritureTrace( XZSTC_INFO, "amisol_tcpip_send_body_message : toutes les donnees ont ete transferees");
	}

	
	/* free des ressources */
	 amisol_free_ressources();

	XZST_03EcritureTrace( XZSTC_FONCTION, "amisol_tcpip_send_body_message : OUT");
	return(XDC_OK);	
}

/*A
****************************************************************************
* fonction qui gere la fin de message vers le misol2
***************************************************************************
*/
int amisol_tcpip_manage_end_message()
{
	char vl_buf_ACQ[C_REC_MAX_SIZE];

	XZST_03EcritureTrace(XZSTC_FONCTION,"amisol_tcpip_manage_end_message: IN");

	/* send FIN for data transfert */
	if ((amisol_tcpip_send(C_SYMBOL_FIN)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_end_message : pb appel amisol_tcpip_send pour envoi commande %s", C_SYMBOL_FIN);
		return(XDC_NOK);

	}

	/* get ACQ for end of data transfert */
	if((amisol_tcpip_receive(vl_buf_ACQ)) != XDC_OK)
	{
		 XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_end_message : pb appel amisol_tcpip_receive pour reception ACQ #1");
		 return(XDC_NOK);
	}
	else
	{
		if(strcmp(vl_buf_ACQ,C_SYMBOL_OK) != 0)
		{
			XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_end_message : Erreur lors du transfert des donnees; message retourne: %s ",vl_buf_ACQ);
			return(XDC_NOK);

		}
		else
		{
			XZST_03EcritureTrace( XZSTC_INFO, "amisol_tcpip_manage_end_message : Traitement des donnees par  misol2 OK");
		}
	}

	/* fermeture session */
	if ((amisol_tcpip_send(C_SYMBOL_FIN)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "amisol_tcpip_manage_end_message : pb appel amisol_tcpip_send pour envoi commande %s", C_SYMBOL_FIN);
		return(XDC_NOK);
	}

	XZST_03EcritureTrace( XZSTC_FONCTION, "amisol_tcpip_manage_end_message : OUT ");
	return(XDC_OK);
}
