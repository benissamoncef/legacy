/* Fichier : aces_evt.c   Release : 1.1   Date : 05/15/97	*
*************************************************
* STERIA * Projet MIGRAZUR			*
*************************************************
* MODULE  : BASEDD				*
* FICHIER : aces_evt.c				*
*************************************************
* DESCRIPTION :
* module externe de la tache client evenement
* pour le PC STRADA
* Il contient les fonctions d'armements des timers
* necessaires + les call-backs associees
*************************************************
* HISTORIQUE :
*
* PENOT	05 Mai 1997 : Creation 			V1.1
* Niepceron  30 Oct 1998 : PHA devient PHR cf "STRADA-NET V1 Interface pour situation v1.3" v1.3
* Niepeceron 02 Mar 1999 : Modif intergest. dem/1724 v1.4 1.5
* Niepeceron 30 Oct 2001 : Modif intergest. pour strada V2 1.6
* Niepeceron 03 Mar 2005 : Modif bouchon accordeon 1.7 DEM428
* JMG   02/03/04 : modif aces_evt_decode_readfile (SAGA) 1.8
* FL    02/03/04 : modif aces_evt_en_base et ajout de aces_evt_cree_fmc (SAGA) 1.8
* JMG   05/09/05 : plus d appel a XZIT08 (reveil en double) 1.9
* JMG 	05/09/05 : de nouveau les appels a XZIT08 1.10
* JMG	19/09/05 : decodage specifique pour les PMV DDE 83 1.11
* JPL	02/11/09 : Sectorisation 1.12
* JPL	02/11/09 : Appel XZIT06 avec parametres commande et action TFM (nulle) 1.13
* JPL	17/11/10 : Suppression warnings mineurs pour compilation sur architecture HP ia64 (DEM 961)  1.14
* JMG	09/03/17 : REGIONALISATION - DEM1220 1.15
* LCL	27/09/23 : Adaptations CNA DEM-483	
************************************************/


#include "aces_evt.h"


/* d�finition des variables globales � l'application */

liste_att 	pm_att[NBR_ELT_TABLE];  /* tableau de listes utilisees pour stocker les codes attributs */
lib_oblig	vm_oblig; 		/* libelles des attributs obligatoires */

/* definition des variables locales au module */

static char *	version = "@(#)aces_evt.c	1.1 05/15/97 : commentaire" ;
static int	vm_frequence = 0; 

/* + les stations*/

/* declaration de fonctions internes */
int     	aces_evt_hash(char *);
int	 	aces_evt_localisation(loc_rds,XDY_Octet *,XDY_PR *,XDY_Sens *,XDY_Octet *,XDY_PR *);
int     	aces_evt_init();
void    	aces_evt_ftp();
loc_rds 	aces_evt_decode_readfile_loc(char *);
void    	aces_evt_decode_readfile(char *);
void    	aces_evt_decode();
void    	aces_evt_freemem();
int	 	aces_evt_decode_readfile_date(char *,XZSMT_Horodate *,char *, int,XDY_Horodate *);
int 		aces_evt_en_base    (char *, char *, XDY_Mot, att_obl , XDY_Octet, XDY_PR , XDY_Sens, XDY_Octet, XDY_PR, att_nonobl , char *);

/* definition de fonctions externes */
XDY_Entier   vg_NbAutoroutes;
XZAOT_Liste_Auto     *vg_Autoroutes;

char		pm_localisation[TAILLE*4];

lib_local	vm_libel;

static int aces_Autoroutes();


/*A
*******************************************
* Fonction qui permet d'ellaborer le
* hashage d'une string
*******************************************
* Arguments : pointeur sur chaine de caracteres
*******************************************/
int aces_evt_hash(char * pa_str)
{
int vl_i,vl_r;

	vl_r = 0;
	
	for(vl_i=0;vl_i<3;vl_i++)
	{
		vl_r = (vl_r*NBR_PREMIER+((int)(pa_str[vl_i]))*vl_i) % NBR_ELT_TABLE;
	}
	/* XZST_03EcritureTrace(   XZSTC_INFO, "IN : hash : (%s) %d",pa_str,vl_r); */
	return vl_r;
}

/*A
*******************************************
* Fonction qui permet d'ellaborer le
* decodage de la localisation
*******************************************
* Arguments : pointeur sur la localisation loc_rds
*******************************************/
int aces_evt_localisation(loc_rds pa_loc,
				XDY_Octet *pa_autoroute,
				XDY_PR	  *pa_pr,
				XDY_Octet *pa_sens,
				XDY_Octet *pa_autoroutetete,
				XDY_PR	  *pa_prtete) 


{
char	tl_str[TAILLE];
char 	tl_string[TAILLE];
char	tl_table[TAILLE];
char	tl_type_loc[TAILLE];
char 	pl_ligne[256];

int	vl_code,vl_indice;


	if (vm_libel.l01_b == TRUE) {
		if (! strncasecmp(pa_loc.l01,"F",1)) 
			sprintf (tl_table," %s |",vm_libel.l01_string[0]);
		else 
			sprintf (tl_table," %s |",vm_libel.l01_string[1]);
	} 
	else
		sprintf (tl_table," ");
		
	if (vm_libel.l02_b == TRUE) {   
		if      (! strncasecmp(pa_loc.l02,"A1",2))   vl_indice = 0;
		else if (! strncasecmp(pa_loc.l02,"A2",2))   vl_indice = 1;
		else if (! strncasecmp(pa_loc.l02,"A3",2))   vl_indice = 2;
		else if (! strncasecmp(pa_loc.l02,"A4",2))   vl_indice = 3;
		else if (! strncasecmp(pa_loc.l02,"A5",2))   vl_indice = 4;
		else if (! strncasecmp(pa_loc.l02,"A6",2))   vl_indice = 5;
		else if (! strncasecmp(pa_loc.l02,"A7",2))   vl_indice = 6;
		else if (! strncasecmp(pa_loc.l02,"A8",2))   vl_indice = 7;
		else if (! strncasecmp(pa_loc.l02,"A9",2))   vl_indice = 8;
		else if (! strncasecmp(pa_loc.l02,"A10",3))  vl_indice = 9;
		else if (! strncasecmp(pa_loc.l02,"A11",3))  vl_indice = 10;
		else if (! strncasecmp(pa_loc.l02,"A12",3))  vl_indice = 11;
		else if (! strncasecmp(pa_loc.l02,"L1",2))   vl_indice = 12;
		else if (! strncasecmp(pa_loc.l02,"L2",2))   vl_indice = 13;
		else if (! strncasecmp(pa_loc.l02,"L3",2))   vl_indice = 14;
		else if (! strncasecmp(pa_loc.l02,"L4",2))   vl_indice = 15;
		else if (! strncasecmp(pa_loc.l02,"L5",2))   vl_indice = 16;
		else if (! strncasecmp(pa_loc.l02,"L6",2))   vl_indice = 17;
		else if (! strncasecmp(pa_loc.l02,"P1",2))   vl_indice = 18;
		else if (! strncasecmp(pa_loc.l02,"P2",2))   vl_indice = 19;
		else if (! strncasecmp(pa_loc.l02,"P3",2))   vl_indice = 20;
		else if (! strncasecmp(pa_loc.l02,"LOC",3))  vl_indice = 21;
		else vl_indice = 22;
			
		sprintf(tl_type_loc,"%s |",vm_libel.l02_string[vl_indice]);
	}
	else sprintf(tl_type_loc," ");
		

	

	strcpy(pm_localisation,tl_table);
	strcat(pm_localisation,tl_type_loc);
	
	if ( XZAE153_Localisation_PR( pa_loc.l01,
					pa_loc.l02,	
					pa_loc.l03,	
					pa_loc.l04,	
					pa_loc.l05,	
					pa_loc.l06,	
					pa_loc.l07,	
					pa_loc.l08,	
					pa_loc.l09,	
					pa_loc.l10,	
					pa_loc.l11,	
					pa_loc.l12,	
					pl_ligne,
					pa_autoroute,
					pa_pr,
					pa_sens,
					pa_autoroutetete,
					pa_prtete) == XDC_NOK)
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_localisation : Localisation impossible a calculer");
		return XDC_NOK;	
	}	
	strcat(pm_localisation,pl_ligne);
	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_localisation : %s", pm_localisation);
	return XDC_OK;
}

/*A
*******************************************
* Fonction d'initialisation pour la tache
* serveur de donnees trafic
* Elle lit les fichiers de parametrage
* et arme les deux timers :
* - le timer de programmation qui envoie 
* un datagroup
* - le timer de purge
* - charger le tableau de listes chainees
* contenant l'intitule relatif au code 
* evenement Strada
*******************************************
* Arguments : Aucun
*******************************************/

int aces_evt_init()
{
XZSMT_CleProg		vl_cle;
XZSCT_Datagroup 	vl_Datagroup ;
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
XZSCT_NomMachine 	pl_NomMachine	= "\0";
T_BOOL 			vl_etat;
FILE 			*pl_fp;
char			pl_param[CO_MAX_CARACTERE];
char			pl_ligne[CO_MAX_CARACTERE*5];
char 			vl_str1[TAILLE] ,vl_str2[TAILLE*5],vl_str3[TAILLE] ,vl_str4[TAILLE]   ,vl_str5[TAILLE] ,vl_str6[TAILLE]   ,vl_str7[TAILLE] ,vl_str8[TAILLE];
char 			vl_str9[TAILLE] ,vl_str10[TAILLE] ,vl_str11[TAILLE],vl_str12[TAILLE*5],vl_str13[TAILLE],vl_str14[TAILLE]  ,vl_str15[TAILLE],vl_str16[TAILLE];
char 			vl_str17[TAILLE],vl_str18[TAILLE] ,vl_str19[TAILLE],vl_str20[TAILLE]  ,vl_str21[TAILLE],vl_str22[TAILLE*5],vl_str23[TAILLE];
int			vl_val;
int			vl_frequence = 0; 
int			vl_validate;
int			vl_type;
int 			vl_hash;
int			vl_i;
int			vl_b;
liste_att		pl_att;

	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_init" );
 	
  	/* Recuperation du nom de la machine locale */
	XZSC_07NomMachine(pl_NomMachine) ;
  	
  	/* Lecture de la frequence d'exportation des donnees vers Strada */
  	sprintf(vl_str1,"%s/%s",XDC_PATH_STRADA,CO_FIC_FREQUENCE);
	if ( (pl_fp = fopen(vl_str1, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_init : erreur de lecture du fichier %s !", CO_FIC_FREQUENCE);
		return;
	}	

	/* On lit les lignes du fichier une par une */
	while ( fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # ou ne contenant rien */
		if ( pl_ligne[0] == '#' || pl_ligne[0] == '\n' ) 
			continue;

		/* Pour les autres lignes on recherche la frequence de notre tache */
		if(sscanf(pl_ligne,"%[^;];%d", pl_param, &vl_val) == 2) {
			if ( strcmp( pl_param, "taces.x") == NULL ) {
				vl_frequence = vl_val;
				continue;			
			}
		}
		else {
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_init : Le fichier %s n'a pas le bon format !", CO_FIC_FREQUENCE);
		}
	}	
	
	if (! vl_frequence)	/* dans le cas ou le fichier frequence.txt n'a pas de valeur on l'initialise avec 6 minutes */ 
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_init : La frequence du client Strada donnees n'a pu etre lue dans le fichier %s !", CO_FIC_FREQUENCE);
		vl_frequence = CO_DELAI_FREQUENCE;
	}

  	fclose(pl_fp);
  	
	XZST_03EcritureTrace(   XZSTC_WARNING, "IN :  aces_evt_init : La frequence du timer evenements Strada est %d", vl_frequence);

	/*A
  	 * Lecture du fichier lib_obligatoires.txt et mise en memoire des libelles 
  	 */
  	sprintf(vl_str1,"%s/%s",XDC_PATH_STRADA,CO_FIC_OBLI);
	if ( (pl_fp = fopen(vl_str1, "r")) == NULL ) {
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN :  aces_evt_init : Erreur de lecture du fichier %s !", CO_FIC_OBLI);
		return;
	}	


	/* On lit les lignes du fichier une par une */
	while (fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # */
		if (  pl_ligne[0] == '#' || pl_ligne[0] == '\n' || pl_ligne[0] == ' ')  {
			continue;
		}
		
		/* Pour les autres lignes on cree la liste chainee */

		if(sscanf(pl_ligne,"%[^,],%d,%[^\n]\n",vl_str1, &vl_validate ,vl_str2)==3) {
		
 XZST_03EcritureTrace(   XZSTC_INFO, "IN : %s : %d : %s",vl_str1, vl_validate,vl_str2); 
			if (!strcasecmp(vl_str1,"SNM")) {
				strcpy(vm_oblig.snm,vl_str2); vm_oblig.snm_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"ERF")) {
				strcpy(vm_oblig.erf,vl_str2); vm_oblig.erf_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"DOB")) {
				strcpy(vm_oblig.dob,vl_str2); vm_oblig.dob_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"PHR")) {
				strcpy(vm_oblig.pha,vl_str2); vm_oblig.pha_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"CLV")) {
				strcpy(vm_oblig.clv,vl_str2); vm_oblig.clv_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"INP")) {
				strcpy(vm_oblig.inp,vl_str2); vm_oblig.inp_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"STA")) {
				strcpy(vm_oblig.sta,vl_str2); vm_oblig.sta_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"LTV")) {
				strcpy(vm_oblig.ltv,vl_str2); vm_oblig.ltv_b = vl_validate;}
                        if (!strcasecmp(vl_str1,"VNM")) {
				strcpy(vm_oblig.vnm,vl_str2); vm_oblig.vnm_b = vl_validate;}
		}
	}
	fclose(pl_fp);
  	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_init : Lecture du fichier %s terminee",CO_FIC_OBLI);

	/*A
  	 * Lecture du fichier att_evt_strada.txt et initialisation de la liste chainee contenant les codes equipement 
  	 */
  	sprintf(vl_str1,"%s/%s",XDC_PATH_STRADA,CO_FIC_ATTSTR);
	if ( (pl_fp = fopen(vl_str1, "r")) == NULL )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN :  aces_evt_init : Erreur de lecture du fichier %s !", CO_FIC_ATTSTR);
		return;
	}	

	for(vl_i=0 ; vl_i < NBR_ELT_TABLE ; vl_i++)
		pm_att[vl_i] = NULL;			/* initialisation du tableau contenant les 
							   listes chainees relatives aux attributs */

	/* On lit les lignes du fichier une par une */
	while (fgets(pl_ligne, CO_MAX_CARACTERE, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # */
		if (  pl_ligne[0] == '#' || pl_ligne[0] == '\n' || pl_ligne[0] == ' ') {
			continue;
		}
		
		/* Pour les autres lignes on cree la liste chainee */
		if(sscanf(pl_ligne,"%[^,],%d,%d,%[^\n]\n",vl_str1, &vl_validate , &vl_type,vl_str2)==4) {
			vl_hash = aces_evt_hash(vl_str1);
			XZST_03EcritureTrace(   XZSTC_INFO, "%s,%d,%d,%s",vl_str1, vl_validate , vl_type,vl_str2);

			pl_att             = (liste_att) malloc (sizeof(struct code_att));
			pl_att -> suivant  = pm_att[vl_hash];
			pl_att -> name     = strdup(vl_str1);
			pl_att -> validate = vl_validate;
			pl_att -> type 	= vl_type;
			pl_att -> string   = strdup(vl_str2);		/* le nouvel attribut est place en debut de chaine */

			pm_att[vl_hash] = pl_att;
		}
		else  {
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_init : Probleme de lecture des attributs Strada a decoder dans le fichier, %s", pl_fp); 
		}
	}	
	
  	fclose(pl_fp);
  	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_init : Lecture du fichier %s terminee",CO_FIC_ATTSTR);
 	
 	
 	/*A
  	 * Lecture du fichier libelle_loc.txt et chargement des informations 
  	 */
  	sprintf(vl_str1,"%s/%s",XDC_PATH_STRADA,C_FILE_LIB_LOC);
	if ( (pl_fp = fopen(vl_str1, "r")) == NULL )
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "IN :  aces_evt_init : Erreur de lecture du fichier %s !", C_FILE_LIB_LOC);
		return;
	}	


	/* On lit les lignes du fichier une par une */
	while (fgets(pl_ligne, CO_MAX_CARACTERE*5, pl_fp) != NULL) {
		/* On ignore les lignes commencant par un # */
		if (  pl_ligne[0] == '/' || pl_ligne[0] == '\n' || pl_ligne[0] == ' '|| pl_ligne[0] == '#') {
			continue;
		}
		
/*		XZST_03EcritureTrace(   XZSTC_INFO, "IN : %s",pl_ligne); */
		

		if(sscanf(pl_ligne,"L01#%d#%[^#]#%[^;];\n",&vl_b,vl_str1 ,vl_str2)==3) {
			vm_libel.l01_b = vl_b;
			vm_libel.l01_string[0] = strdup(vl_str1);
			vm_libel.l01_string[1] = strdup(vl_str2);
		}
		else if(sscanf(pl_ligne,"L02#%d#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^;];\n",
				&vl_b,
				vl_str1 ,vl_str2,vl_str3 ,vl_str4,vl_str5 ,vl_str6,vl_str7 ,vl_str8,vl_str9 ,vl_str10,
				vl_str11 ,vl_str12,vl_str13 ,vl_str14,vl_str15 ,vl_str16,vl_str17 ,vl_str18,vl_str19 ,vl_str20,
				vl_str21 ,vl_str22,vl_str23 ) == 24) {

			vm_libel.l02_b = vl_b;
			vm_libel.l02_string[0]  = strdup(vl_str1+3) ;vm_libel.l02_string[1]  = strdup(vl_str2+3) ;vm_libel.l02_string[2]  = strdup(vl_str3+3);
			vm_libel.l02_string[3]  = strdup(vl_str4+3) ;vm_libel.l02_string[4]  = strdup(vl_str5+3) ;vm_libel.l02_string[5]  = strdup(vl_str6+3);
			vm_libel.l02_string[6]  = strdup(vl_str7+3) ;vm_libel.l02_string[7]  = strdup(vl_str8+3) ;vm_libel.l02_string[8]  = strdup(vl_str9+3);
			vm_libel.l02_string[9]  = strdup(vl_str10+4);vm_libel.l02_string[10] = strdup(vl_str11+4);vm_libel.l02_string[11] = strdup(vl_str12+4);
			vm_libel.l02_string[12] = strdup(vl_str13+3);vm_libel.l02_string[13] = strdup(vl_str14+3);vm_libel.l02_string[14] = strdup(vl_str15+3);
			vm_libel.l02_string[15] = strdup(vl_str16+3);vm_libel.l02_string[16] = strdup(vl_str17+3);vm_libel.l02_string[17] = strdup(vl_str18+3);
			vm_libel.l02_string[18] = strdup(vl_str19+3);vm_libel.l02_string[19] = strdup(vl_str20+3);vm_libel.l02_string[20] = strdup(vl_str21+3);
			vm_libel.l02_string[21] = strdup(vl_str22+3);vm_libel.l02_string[22] = strdup(vl_str23);
			
		}
		
		else if(sscanf(pl_ligne,"L03#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l03_b = vl_b;
			vm_libel.l03_string = strdup(vl_str1);
		}
		
		else if(sscanf(pl_ligne,"L04#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l04_b = vl_b;
			vm_libel.l04_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L05#%d#%[^#]#%[^#]#%[^#]#%[^;];\n",&vl_b, vl_str1, vl_str2, vl_str3, vl_str4)==5) {
			vm_libel.l05_b = vl_b;
			vm_libel.l05_string[0] = strdup(vl_str1);
			vm_libel.l05_string[1] = strdup(vl_str2);
			vm_libel.l05_string[2] = strdup(vl_str3);
			vm_libel.l05_string[3] = strdup(vl_str4);
		}
		
		else if(sscanf(pl_ligne,"L06#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l06_b = vl_b;
			vm_libel.l06_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L07#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l07_b = vl_b;
			vm_libel.l07_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L08#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l08_b = vl_b;
			vm_libel.l08_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L09#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l09_b = vl_b;
			vm_libel.l09_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L10#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l10_b = vl_b;
			vm_libel.l10_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L11#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l11_b = vl_b;
			vm_libel.l11_string = strdup(vl_str1);
		}
		else if(sscanf(pl_ligne,"L12#%d#%[^;];\n",&vl_b, vl_str1)==2) {
			vm_libel.l12_b = vl_b;
			vm_libel.l12_string = strdup(vl_str1);
		}
		
		else  {
			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_init  : Probleme de lecture des attributs Strada a decoder dans le fichier %s", pl_fp);
		}
	}	
	
  	fclose(pl_fp);
  	
	XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_init : Lecture du fichier %s terminee",C_FILE_LIB_LOC);
  	
  	vm_frequence=vl_frequence;
  	
	/*A initialisation des timers */
	 XZSM_00InitServices(); 
	
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
	/*A Abonnement au datagroup pour que le timer nous envoie les messages */
	if (! XDG_JoindreDG(XDG_AS_TIMER_CLT_EVT) ) {	
			/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING, 
				"IN : aces_evt_init : Abonnement sur %s non effectuee. \n " ,vl_Datagroup);
			return;
	}
		
        vl_HorodateSec+=vm_frequence;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						0,
						XDG_AS_TIMER_CLT_EVT,
						XDM_AS_TIMER_CLT_EVT,
						"cle_timer_client_evt",
						XZSMC_SEC_VRAI) !=  XDC_OK ) {
		/* Ecriture Trace */
        	XZST_03EcritureTrace( XZSTC_WARNING, "IN : aces_evt_init : Programmation du timer de donnees evenement non declenchee. \n " );
        	return ;
	} 

       /*A
	* Initialisation de la callback du timer de programmation 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_CLT_EVT),aces_evt_decode,(T_CB_ARG)NULL) == NULL)  {	
                XZST_03EcritureTrace( XZSTC_FATAL,"IN : aces_evt_init : %s Callback sur XDM_AS_TIMER_CLT_EVT non cree.",version );
		return ;
	}

        if (XZAO316_Liste_Sens_Autoroute("CFG", aces_Autoroutes) != XDC_OK)
                XZST_03EcritureTrace(XZSTC_WARNING, ": Echec lors de l'init des autoroutes");
	/* Ecriture Trace */
	return XDC_OK ;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   mise a jour du slot "inversee" des autoroutes
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int aces_Autoroutes(Autoroutes, Nb_Autoroutes)
XZAOT_Liste_Auto        *Autoroutes;
XDY_Entier              Nb_Autoroutes;

/*
* ARGUMENTS EN ENTREE :
*       XZAOT_Liste_Auto        *Autoroutes;
*       XDY_Entier              Nb_Autoroutes;
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR :
*   XDC_OK / XDC_NOK
*
* CONDITION D'UTILISATION
*   fonction passee en argument a XZAO316
*
* FONCTION
*   - copie le tableau passe en argument dans une variable globale
*   - recherche l'instance RTie correspondant � chaque element
*   du tableau et met a jour le slot "inversee" de l'instance
*   en fonction du champ "inversee" de l'element correspondant
------------------------------------------------------*/
{
        static char     *version = "$Id: aces_evt.c,v 1.15 2017/03/09 15:04:28 pc2dpdy Exp $ : aces_Autoroutes";
        int             vl_i,i;
        T_STRING        NomIE;

        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s, Nb_Autoroutes = %d", version, Nb_Autoroutes);

        /*A on alloue la memoire pour la variable globale */
        vg_NbAutoroutes = Nb_Autoroutes;
        vg_Autoroutes = (XZAOT_Liste_Auto *)malloc(Nb_Autoroutes*sizeof(XZAOT_Liste_Auto));

        /*A on parcourt le tableau */
        for (i=0; i < Nb_Autoroutes; i++)
        {
                /*B on copie l'element dans la variable globale */
                vg_Autoroutes[i].numero = Autoroutes[i].numero;
                strcpy(vg_Autoroutes[i].nom, Autoroutes[i].nom);
  		vl_i= strlen(vg_Autoroutes[i].nom)-1;
  		while (vl_i > 0 && vg_Autoroutes[i].nom[vl_i]==' ' ) {
  			vl_i--;
  		}
  		vg_Autoroutes[i].nom[vl_i+1]='\0';
                vg_Autoroutes[i].inversee = Autoroutes[i].inversee;

        }

        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne XDC_OK", version);
        return(XDC_OK);
}

/*A
*******************************************
* Fonction qui recupere les donnees trafic
* sur le PC Strada et les place dans le 
* repertoire client trafic
*******************************************
* Arguments : Aucun
*******************************************/

void aces_evt_ftp()
{
FILE 			* pl_fD=NULL;
char 			tl_buffer[TAILLE+1];

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN : aces_evt_ftp ");

	/* execution d'un script de recuperation des fichiers */
	/*system(C_SCRIPT_SIT); */
	if ( (pl_fD = popen(C_SCRIPT_SIT,"r")) != NULL) {
		while(fgets(tl_buffer,TAILLE,pl_fD) != NULL) {
		}
	}
	
	pclose(pl_fD);
	
	/* ecriture trace */
	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : aces_evt_ftp ");
	

}


/*A
********************************************
* fonction  de aces_evt_decode_readfile 
* permettant de lire et de convertir une date 
* Strada en une horodate
*******************************************
* Arguments : le nom du fichier
*******************************************/
int aces_evt_decode_readfile_date(char * pa_string ,XZSMT_Horodate * pa_strdate, char * pa_file,int va_boolean,XDY_Horodate *pa_horodate)
{
double			vl_horodate;
XZSMT_Horodate		pl_string;
int			vl_annee;
int			vl_mois;
int			vl_jour;
int			vl_heure;
int			vl_minute;
int			vl_seconde = 0;
char			vl_signe;
int			vl_gmt;

	
	if ( strlen(pa_string) != 15 )	/* test du nombre de caract�res de la date fournie */
		return FALSE;

	if (sscanf(pa_string,"%4d%2d%2d%2d%2d%c%2d", 	&vl_annee,
      								&vl_mois,
      								&vl_jour,
      								&vl_heure,
      								&vl_minute,
      								&vl_signe,
      								&vl_gmt     ) != 7 )
      	{
      		/* la date n'a pas le bon format, il ne sera pas pris en compte */
      		if (va_boolean == TRUE) 
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_decode_readfile_date : la date du fichier %s n'a pas le bon format",pa_file );
      		
		return FALSE;
      	}

	/* on met a jour l'annee : valable de 1995 jusqu'en 2094 */
      	/*if (vl_annee < 95)			
      		vl_annee = vl_annee + 2000;
      	else	vl_annee = vl_annee + 1900;*/
      			
      	sprintf(pl_string,"%2d/%2d/%4d %2d:%2d:%2d", 	vl_jour,
      							vl_mois,
      							vl_annee,
      							vl_heure,
      							vl_minute,
      							vl_seconde);
      									
      	/* conversion de la date en secondes pour pouvoir appliquer le decelage GMT */
      	XZSM_13ConversionHorodateSec(pl_string,&vl_horodate);
      		 
      	/* l'horodate est donnee pour GMT = +1 et il faut pouvoir la convertir lorsque celle-ci
      		  est differente */
      	if (vl_signe == 'N')
      		 vl_gmt = - vl_gmt;
      	else {
      		if (vl_signe != 'P')
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : acts_traf_decode_readfile_date : le format GMT n'est pas correct (%s)",pa_string);
      	}
      			 	
      	vl_horodate = vl_horodate + (vl_gmt - 1) * 3600 ; 	/* 3600 nbre de seconde par heure */
      		
    	/* conversion de la date de secondes en horodate */
	XZSM_11ConversionHorodate(vl_horodate,pa_strdate);
	*pa_horodate=vl_horodate;
	
	return TRUE;
}

/*A
********************************************
* fonction  de aces_evt_decode permettant de 
* lire un fichier de donees evenement Strada
*******************************************
* Arguments : le nom du fichier
*******************************************/
void aces_evt_decode_readfile (char *pa_file)
{
char			* pl_buffer;	
char 			  pl_message[TAILLE*10]="";
char 			* pl_message_buffer;
FILE 			* pl_fD;
char 			* pl_string;
char			* pl_free;

char 			tl_buffer[TAILLE];
char 			tl_buftok[TAILLE];
char			pl_donnee[TAILLE];
char			tl_message_bd[TAILLE_BD];

int			vl_att_int;			/* attribut entier de la localisation RDS Strada 	*/
double			vl_att_float;			/* attribut flottant de la localisation RDS Strada 	*/

loc_rds			vl_localisation;
att_obl			vl_attribut; 
att_nonobl              vl_attribut_nonobl={0.0,XDC_FAUX,1,XDC_FAUX,0,0,0,0,0,0,0,"",3,0,0,0,1,99,"","","","",0,-1};  /*SAGA*/
liste_att		pl_att;
liste_att		pl_att_pha;

XZSMT_Horodate		pl_strdate;
XDY_Horodate		vl_horodate;

int			vl_i;
int			vl_loop;
int			vl_counter;
int			vl_hash;
int 			vl_memory;
int			vl_boolean;
int			vl_eof;

char			tl_emetteur[TAILLE];
char			vl_code_dob[4];
char			vl_heures[3];
char			vl_minutes[3];
char			tl_att_char[100];
char			vl_nom_att[4];
int			tl_att_int;
double			tl_att_float;
XDY_Mot         	vl_typeFMC;
XDY_Octet		vl_autoroute,vl_autoroutetete;
XDY_PR			vl_PR,vl_PRtete;
XDY_Sens		vl_sens;
int			vl_long=0,vl_long_cumul=0;
char			vl_ligne1[256],vl_ligne2[256],vl_ligne3[256],vl_ligne4[256],vl_nom[256];

	/* decodage de la premiere ligne emetteur */
	tl_emetteur[0] = '\0';
		
	
  	if ((pl_fD = fopen (pa_file,"r")) != NULL) {

/* XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : fichier %s",pa_file);*/
      	
    		if (fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL) {   /* leture entete du fichier ligne 1 */
   			/*
   			 * On verifie que la localisation a le bon format
   			 */
			pl_buffer = tl_buffer;


			if (sscanf(pl_buffer,"MSE#%[^#]#",tl_emetteur)!=1) {      			
      				/* l'attribut MSE du fichier n'a pas le bon format, il ne sera pas pris en compte */
      				XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : L'attribut MSE du fichier %s n'a pas le bon format ou n'existe pas\n%s",pa_file,pl_buffer );
				return;
      			}
      			      		
/* XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : emetteur %s\n", tl_emetteur); */
		}

		/*A
		 * lecture des attributs obligatoires avec initialisation de vl_attribut
		 */

		vl_attribut.snm_b = FALSE;
		vl_attribut.erf_b = FALSE;
		vl_attribut.dob_b = FALSE;
		vl_attribut.pha_b = FALSE;
		vl_attribut.clv_b = FALSE;
		vl_attribut.inp_b = FALSE;
		vl_attribut.sta_b = FALSE;
		vl_attribut.ltv_b = FALSE;
		vl_attribut.vnm_b = FALSE;

		for (vl_loop=0;vl_loop<9;vl_loop++) {
    			if (fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL)    /* leture entete des lignes 2 a 10 fichier */
		
    			pl_string = strtok (tl_buffer,"#");
    			
    			if (!strcasecmp(pl_string,"SNM")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 49)
				{
					/* le fichier a un message SNM trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message SNM trop long ",pa_file );
					return;	
				}
				strcpy(vl_attribut.snm,pl_string);
    				vl_attribut.snm_b = TRUE;
    				continue;
    			}
    			
    			if (!strcasecmp(pl_string,"ERF")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 49){
					/* le fichier a un message ERF trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message ERF trop long ",pa_file );
					return;	
				}
				strcpy(vl_attribut.erf,pl_string);
    				vl_attribut.erf_b = TRUE;
    				continue;
    			}

    			if (!strcasecmp(pl_string,"DOB")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 25){
					/* le fichier a un message DOB trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message DOB trop long ",pa_file );
					return;	
				}
				
				vl_hash = aces_evt_hash(pl_string);
				pl_att = pm_att[vl_hash];
				while(strcmp(pl_att -> name, pl_string)!=0 && pl_att!=NULL)
					if (pl_att == NULL)
						continue;
					else pl_att = pl_att -> suivant;
    				strcpy(vl_attribut.dob,pl_att -> string);
    				vl_attribut.dob_b = TRUE;
    				strncpy(vl_code_dob,pl_string,3);
    				vl_code_dob[3]='\0';
    				vl_typeFMC=(XDY_Mot)pl_att -> type;
				continue;
    			}

    			if (!strcasecmp(pl_string,"PHR")) {
    				pl_string = strtok (NULL,"#");
				if (strlen(pl_string) > 9) {
					/* le fichier a un message PHR trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message PHR trop long ",pa_file );
					return;	
				}
    				strcpy((char *) vl_attribut.pha,(char *)pl_string);
    				vl_attribut.pha_b = TRUE;
    				continue;
    			}

    			if (!strcasecmp(pl_string,"CLV")) {
    				pl_string = strtok (NULL,"#");

				if (sscanf(pl_string,"%d",&vl_i)!=1) {
					/* le fichier a un message CLV non conforme au format Strada / DATEX, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message CLV non conforme au format Strada / DATEX ",pa_file );
					return;	
				}
    				vl_attribut.clv   = vl_i;
    				vl_attribut.clv_b = TRUE;
				continue;
    			}

    			if (!strcasecmp(pl_string,"INP"))
			{
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 29)
				{
					/* le fichier a un messageINP trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message INP trop long ",pa_file );
					return;	
				}
				strcpy(vl_attribut.inp,pl_string);
    				vl_attribut.inp_b = TRUE;
    				continue;
    			}


    			if (!strcasecmp(pl_string,"STA")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 29)
				{
					/* le fichier a un message STA trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message STA trop long ",pa_file );
					return;	
				}
				strcpy(vl_attribut.sta,pl_string);
    				vl_attribut.sta_b = TRUE;
    				continue;
    			}

    			if (!strcasecmp(pl_string,"LTV")) {
    				pl_string = strtok (NULL,"#");
    				if (sscanf(pl_string,"%d",&vl_i)!=1)
				{
					/* le fichier a un message LTV non conforme au format Strada / DATEX, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message LTV non conforme au format Strada / DATEX ",pa_file );
					return;	
				}
    				vl_attribut.ltv = atoi(pl_string);
    				vl_attribut.ltv_b = TRUE;
				continue;
    			}
    			
    			if (!strcasecmp(pl_string,"VNM")) {
    				pl_string = strtok (NULL,"#");
    				if (sscanf(pl_string,"%d",&vl_i)!=1)
				{
					/* le fichier a un message VNM non conforme au format Strada / DATEX, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message VNM non conforme au format Strada / DATEX ",pa_file );
					return;	
				}
    				vl_attribut.vnm = atoi(pl_string);
    				vl_attribut.vnm_b = TRUE;
				continue;
    			}

			/* le fichier n'a pas tous les attributs obligatoires, il ne sera pas pris en compte */
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s n'a pas tous les attributs obligatoires",pa_file );
			return;
    		}
    		
    		/* test que tous les attributs obligatoires sont presents */
    		if (	vl_attribut.snm_b == FALSE || vl_attribut.erf_b == FALSE || vl_attribut.dob_b == FALSE ||
			vl_attribut.pha_b == FALSE || vl_attribut.clv_b == FALSE || vl_attribut.inp_b == FALSE ||
			vl_attribut.sta_b == FALSE || vl_attribut.ltv_b == FALSE || vl_attribut.vnm_b == FALSE	)
		{
			/* le fichier n'a pas tous les attributs obligatoires, il ne sera pas pris en compte */
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s n'a pas tous les attributs obligatoires",pa_file );
			return;
		}

    		
		XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : Les attributs obligatoires du fichier %s sont presents",pa_file);


		/* traitement du message. Ecriture des attributs obligatoires du message */
		/* message SNM */

		if (vm_oblig.snm_b == TRUE)	{
			strcpy(pl_message,vm_oblig.snm);
			strcat(pl_message," = ");
			strcat(pl_message,vl_attribut.snm);
			strcat(pl_message,"|"); 
		}

		/* message ERF */

		if (vm_oblig.erf_b == TRUE)	{
			strcpy(pl_message,vm_oblig.erf);
			strcat(pl_message," = ");
			strcat(pl_message,vl_attribut.erf);
			strcat(pl_message,"|"); 
		}

		/* message DOB */
		
		if (vm_oblig.dob_b == TRUE)	{
			strcat(pl_message,vm_oblig.dob);
			strcat(pl_message," = ");
			strcat(pl_message,vl_attribut.dob);
			strcat(pl_message,"|");
		}

		/* message PHR */
		
		if (vm_oblig.pha_b == TRUE)	{
			strcat(pl_message,vm_oblig.pha);
			strcat(pl_message," = ");
			strcat(pl_message,vl_attribut.pha);
			strcat(pl_message,"|");
		}

		/* message CLV */
		if (vm_oblig.clv_b == TRUE) {
			strcat(pl_message,vm_oblig.clv);
			strcat(pl_message,(char *) vl_attribut.clv);
			strcat(pl_message,"|");
		}
		
		/* message INP */
		if (aces_evt_decode_readfile_date (vl_attribut.inp,&pl_strdate,pa_file,TRUE,&vl_horodate) == FALSE) {
			/* le fichier a un message INP au mauvais format, il ne sera pas pris en compte */
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L01 trop long : %s ", pa_file, vl_attribut.sta);
			return;	
		}
		if (vm_oblig.inp_b == TRUE) {
			strcat(pl_message,vm_oblig.inp);
			strcat(pl_message," = ");
			strcat(pl_message,pl_strdate);
			strcat(pl_message,"|");
		}

		/* message STA */
		if (aces_evt_decode_readfile_date (vl_attribut.sta,&pl_strdate,pa_file,TRUE,&vl_horodate) == FALSE) {
			/* le fichier a un message STA au mauvais format, il ne sera pas pris en compte */
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L01 trop long : %s ", pa_file, vl_attribut.sta);
			return;	
		}
		if (vm_oblig.sta_b == TRUE) {
			strcat(pl_message,vm_oblig.sta);
			strcat(pl_message," = ");
			strcat(pl_message,pl_strdate);
			strcat(pl_message,"|");
		}

		/* message LTV */
		sprintf(pl_donnee, " = %d",vl_attribut.ltv);
		if (vm_oblig.ltv_b == TRUE) {
			strcat(pl_message,vm_oblig.ltv);
			strcat(pl_message,pl_donnee);
			strcat(pl_message,"|");
		}


		/* message VNM */
		sprintf(pl_donnee, " = %d",vl_attribut.vnm);
		if (vm_oblig.vnm_b == TRUE) {
			strcat(pl_message,vm_oblig.vnm);
			strcat(pl_message,pl_donnee);
			strcat(pl_message,"|");
		}

/* XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : message attributs obligatoires :\n %s",pl_message);*/

		/*A
		 * lecture des localisations obligatoires 
		 */

		vl_localisation.l01_b = FALSE;
		vl_localisation.l02_b = FALSE;
		vl_localisation.l03_b = FALSE;
		vl_localisation.l04_b = FALSE;
		vl_localisation.l05_b = FALSE;
		vl_localisation.l06_b = FALSE;
		vl_localisation.l07_b = FALSE;
		vl_localisation.l08_b = FALSE;
		vl_localisation.l09_b = FALSE;
		vl_localisation.l10_b = FALSE;
		vl_localisation.l11_b = FALSE;
		vl_localisation.l12_b = FALSE;

                  /* Ajout SAGA */
                strcpy(vl_localisation.l01,"");
                strcpy(vl_localisation.l02,"");
                vl_localisation.l03 = 0;
                vl_localisation.l04 = 0;
                strcpy(vl_localisation.l05,"");
                strcpy(vl_localisation.l06,"");
                strcpy(vl_localisation.l07,"");
                vl_localisation.l08 = 0;
                vl_localisation.l09 = 0;
                strcpy(vl_localisation.l10,"");
                vl_localisation.l11 = 0;
                vl_localisation.l12 = 0;

		for (vl_loop=0;vl_loop<4;vl_loop++) {
    			if (fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL)    /* leture entete des lignes 2 a 10 fichier */
				
    			pl_string = strtok (tl_buffer,"#");
  			
    			if (!strcasecmp(pl_string,"L01")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 4) {
					/* le fichier a un message SNM trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L01 trop long ",pa_file );
					return;	
				}
				strcpy(vl_localisation.l01,pl_string);
    				vl_localisation.l01_b = TRUE;
    				continue;
    			}
    			
    			if (!strcasecmp(pl_string,"L02")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 6) {
					/* le fichier a un message SNM trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L02 trop long ",pa_file );
					return;	
				}
				strcpy(vl_localisation.l02,pl_string);
    				vl_localisation.l02_b = TRUE;
    				continue;
    			}
    			if (!strcasecmp(pl_string,"L03")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 2) {
					/* le fichier a un message SNM trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L03 trop long ",pa_file );
					return;	
				}
				vl_localisation.l03 = atoi(pl_string);
    				vl_localisation.l03_b = TRUE;
    				continue;
    			}
    			if (!strcasecmp(pl_string,"L04")) {
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 6) {
					/* le fichier a un message SNM trop long, il ne sera pas pris en compte */
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L04 trop long ",pa_file );
					return;	
				}
				vl_localisation.l04 = atoi(pl_string);
    				vl_localisation.l04_b = TRUE;
    				continue;
    			}
    		}
    			
		/* test que tous les attributs obligatoires de localisation sont presents */
    		if (	vl_localisation.l01_b == FALSE || vl_localisation.l02_b == FALSE || vl_localisation.l03_b == FALSE || vl_localisation.l04_b == FALSE )
		{
			/* le fichier n'a pas tous les attributs obligatoires, il ne sera pas pris en compte */
      			XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s n'a pas tous les attributs localisation obligatoires",pa_file );
			return;
		}

    		
		XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : Les attributs localisation obligatoires du fichier %s sont presents",pa_file);
    			
    		/********************************************************/
    		/* recuperation des localisations non obligatoires	*/
    		
    		vl_eof = FALSE;
    		
    		if (fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL)	{
    			strcpy(tl_buftok,tl_buffer);
    			pl_string = strtok (tl_buftok,"#");
    			
    			if (!strcasecmp(pl_string,"L05")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 2) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L05 trop long ",pa_file );
					return;	
				}
				strcpy(vl_localisation.l05,pl_string);
				vl_localisation.l05_b = TRUE;
    				if(fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL) {
    					strcpy(tl_buftok,tl_buffer);
    					pl_string = strtok (tl_buftok,"#");
    				}
    				else {
    					vl_eof = TRUE;
    					pl_string = NULL;
    				}
    			}
    		
    			if (!strcasecmp(pl_string,"L06")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 75) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L06 trop long ",pa_file );
					return;	
				}
				strcpy(vl_localisation.l06,pl_string);
				vl_localisation.l06_b = TRUE;
    				if(fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL) {
    					strcpy(tl_buftok,tl_buffer);
    					pl_string = strtok (tl_buftok,"#");
    				}
    				else {
    					vl_eof = TRUE;
    					pl_string = NULL;
    				}
    			}
    			if (!strcasecmp(pl_string,"L07")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 40) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L07 trop long ",pa_file );
					return;	
				}
				strcpy(vl_localisation.l07,pl_string);
				vl_localisation.l07_b = TRUE;
    				if(fgets(tl_buffer,sizeof(tl_buffer),pl_fD)!= NULL) {
    					strcpy(tl_buftok,tl_buffer);
    					pl_string = strtok (tl_buftok,"#");
    				}
    				else {
    					vl_eof = TRUE;
    					pl_string = NULL;
    				}
    			}
    			
    			
    			if (!strcasecmp(pl_string,"L08")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 5) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L08 trop long ",pa_file );
					return;	
				}
				vl_localisation.l08 = atoi(pl_string);
				vl_localisation.l08_b = TRUE;
    				if(fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL) {
    					strcpy(tl_buftok,tl_buffer);
    					pl_string = strtok (tl_buftok,"#");
    				}
    				else {
    					vl_eof = TRUE;
    					pl_string = NULL;
    				}
    			}
    			
    			if (!strcasecmp(pl_string,"L09")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 6) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L09 trop long ",pa_file );
					return;	
				}
				vl_localisation.l09 = atoi(pl_string);
				vl_localisation.l09_b = TRUE;
    				if(fgets(tl_buffer,sizeof(tl_buffer),pl_fD) != NULL) {
    					strcpy(tl_buftok,tl_buffer);
    					pl_string = strtok (tl_buftok,"#");
    				}
    				else {
    					vl_eof = TRUE;
    					pl_string = NULL;
    				}
    			}
    			
    			if (!strcasecmp(pl_string,"L10")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 40) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L10 trop long ",pa_file );
					return;	
				}
				strcpy(vl_localisation.l10,pl_string);
				vl_localisation.l10_b = TRUE;
    				fgets(tl_buffer,sizeof(tl_buffer),pl_fD);
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    			}
    			
    			if (!strcasecmp(pl_string,"L11")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 8 ) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L11 trop long ",pa_file );
					return;	
				}
 				vl_localisation.l11 = atoi(pl_string);
				vl_localisation.l11_b = TRUE;
   				fgets(tl_buffer,sizeof(tl_buffer),pl_fD);
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    			}
    			
    			if (!strcasecmp(pl_string,"L12")) {
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    				pl_string = strtok (NULL,"#");
    				if (strlen(pl_string) > 8 || (sscanf(pl_string,"%d\0",&vl_localisation.l12)!=1)) {
      					XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s a un message L12 trop long ",pa_file );
					return;	
				}
 				vl_localisation.l12 = atoi(pl_string);
				vl_localisation.l12_b = TRUE;
    				fgets(tl_buffer,sizeof(tl_buffer),pl_fD);
    				strcpy(tl_buftok,tl_buffer);
    				pl_string = strtok (tl_buftok,"#");
    			}
    		}    			
    		
    		XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : attributs de localisation \n\tL01 %s   \tL02 %s  \tL03 %d    \tL04 %d  \tL05 %s \tL06 %s  \tL07 %s \n\tL08 %d  \tL09 %d \tL10 %s  \tL11 %d  \tL12 %d ",vl_localisation.l01,
    										vl_localisation.l02,
    										vl_localisation.l03,
    										vl_localisation.l04,
    										vl_localisation.l05,
    										vl_localisation.l06,
    										vl_localisation.l07,
    										vl_localisation.l08,
    										vl_localisation.l09,
    										vl_localisation.l10,
    										vl_localisation.l11,
    										vl_localisation.l12);
    		
    		aces_evt_localisation (vl_localisation,&vl_autoroute,&vl_PR,&vl_sens,&vl_autoroutetete,&vl_PRtete);
    		
		strcat(pl_message,pm_localisation);
		strcat(pl_message,"|");
		
		pm_localisation[0]='\0';
		
    		
    		/************************************************************/	
    		/* insertion dans le message des attributs non obligatoires */
    		vl_attribut_nonobl.len=0;
		vl_attribut_nonobl.que=0;
		if (tl_buffer[0]!='\0' && vl_eof==FALSE )  { 
		  do {
			
	    		pl_string = strtok (tl_buffer,"#");
	   		XZST_03EcritureTrace(   XZSTC_INFO, "IN :  --%s-",pl_string) ;
	    		if (pl_string[0] == NULL || pl_string[0] == '\n' || pl_string[0] == '\0')
	    			continue;
			vl_hash = aces_evt_hash(pl_string);
			pl_att = pm_att[vl_hash];
			while(strcmp(pl_att -> name, pl_string)!=0 && pl_att!=NULL)
				if (pl_att == NULL)
					continue;
				else pl_att = pl_att -> suivant;
				
			if (pl_att -> validate == TRUE) {
							
				strcat(pl_message,pl_att -> string);
				strcat(pl_message," = ");

				pl_string = strtok (NULL,"#");
	   			XZST_03EcritureTrace(   XZSTC_INFO, "IN :  ->%s.",pl_string) ;
				if (aces_evt_decode_readfile_date (pl_string,&pl_strdate,pa_file,FALSE,&vl_horodate) == TRUE) {
					strcat(pl_message,pl_strdate);
					strcat(pl_message,"|");
				}
				else {
					strcat(pl_message,pl_string);
					strcat(pl_message,"|");
				}
				
				if (!strcmp( pl_att -> name, "STO" ))
				{
					vl_attribut_nonobl.sto=vl_horodate;
				}
				else if(!strcmp( pl_att -> name, "END" ))
				{
					if (!strcmp(pl_string,"Y"))
						vl_attribut_nonobl.end=XDC_VRAI;
					else
						vl_attribut_nonobl.end=XDC_FAUX;
				}
				else if(!strcmp( pl_att -> name, "QIN" ))
				{
					vl_attribut_nonobl.qin=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "CAN" ))
				{
					if (!strcmp(pl_string,"Y"))
						vl_attribut_nonobl.can=XDC_VRAI;
					else
						vl_attribut_nonobl.can=XDC_FAUX;
				
				}
				else if(!strcmp( pl_att -> name, "NVE" ))
				{
					vl_attribut_nonobl.nve=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "NHL" ))
				{
					vl_attribut_nonobl.nhl=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "NHV" ))
				{
					vl_attribut_nonobl.nhv=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "CAV" ))
				{
					vl_attribut_nonobl.cav=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "NBU" ))
				{
					vl_attribut_nonobl.nbu=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "INJ" ))
				{
					vl_attribut_nonobl.inj=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "DEA" ))
				{
					vl_attribut_nonobl.dea=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "LOA" ))
				{
					strncpy(vl_attribut_nonobl.loa,pl_string,3);
					vl_attribut_nonobl.loa[3]='\0';
				}
				else if(!strcmp( pl_att -> name, "SEV" ))
				{
					vl_attribut_nonobl.sev=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "QUE" ))
				{
					vl_attribut_nonobl.que=(int)(atof(pl_string)*1000);
				}
				else if(!strcmp( pl_att -> name, "LEN" ))
				{
					vl_attribut_nonobl.len=(int)(atof(pl_string)*1000);
				}
				else if(!strcmp( pl_att -> name, "VIS" ))
				{
					vl_attribut_nonobl.vis=atoi(pl_string);
					XZST_03EcritureTrace(   XZSTC_INFO, "VIS:%d.",vl_attribut_nonobl.vis);
				}
				else if(!strcmp( pl_att -> name, "MOT" ))
				{
					vl_attribut_nonobl.mot=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "NLN" ))
				{
					vl_attribut_nonobl.nln=atoi(pl_string);
				}
				else if(!strcmp( pl_att -> name, "LNK" ))
				{
					strncpy(vl_attribut_nonobl.lnk,pl_string,35);
					vl_attribut_nonobl.lnk[35]='\0';
				}
				else if(!strcmp( pl_att -> name, "LNP" ))
                                {
                                        vl_attribut_nonobl.lnp=atoi(pl_string);
                                }
				else if(!strcmp( pl_att -> name, "TET" ))
				{
					strncpy(vl_attribut_nonobl.tet,pl_string,3);
					vl_attribut_nonobl.tet[3]='\0';
				}
				else if(!strcmp( pl_att -> name, "DIS" ))
				{
					strncpy(vl_attribut_nonobl.dis,pl_string,350);
					vl_attribut_nonobl.dis[350]='\0';
				}
				else if(!strcmp( pl_att -> name, "SUR" ))
				{
					strncpy(vl_attribut_nonobl.sur,pl_string,350);
					vl_attribut_nonobl.sur[350]='\0';
				}
				else if(!strcmp( pl_att -> name, "DUV" ))
				{
					strncpy(vl_heures,pl_string,2);
					vl_heures[2]='\0';
					strncpy(vl_minutes,pl_string+2,2);
					vl_minutes[2]='\0';
					vl_attribut_nonobl.duv=atoi(vl_heures)*60+atoi(vl_minutes);
				}
				else if(!strcmp( pl_att -> name, "DUR" ))
				{ 
					if(!strcmp( pl_string,"D1"))
						vl_attribut_nonobl.duv=30;
					else if(!strcmp( pl_string,"D2"))
						vl_attribut_nonobl.duv=60;
					else if(!strcmp( pl_string,"D3"))
						vl_attribut_nonobl.duv=120;
					else if(!strcmp( pl_string,"D4"))
						vl_attribut_nonobl.duv=180;
					else if(!strcmp( pl_string,"D5"))
						vl_attribut_nonobl.duv=240;
					else if(!strcmp( pl_string,"D6"))
						vl_attribut_nonobl.duv=361;
					else if(!strcmp( pl_string,"L1"))
						vl_attribut_nonobl.duv=480;
					else if(!strcmp( pl_string,"L2"))
						vl_attribut_nonobl.duv=720;
					else if(!strcmp( pl_string,"L3"))
						vl_attribut_nonobl.duv=720;
					else if(!strcmp( pl_string,"L4"))
						vl_attribut_nonobl.duv=5040;
					else if(!strcmp( pl_string,"L5"))
						vl_attribut_nonobl.duv=15120;
					else if(!strcmp( pl_string,"L6"))
						vl_attribut_nonobl.duv=21600;
					else if(!strcmp( pl_string,"L7"))
						vl_attribut_nonobl.duv=32000;
				}
			}
		  }
		  while ((fgets(tl_buffer,TAILLE,pl_fD)) != NULL) ;  
		}

      	fclose (pl_fD);  
      	
      	
      	
      	aces_evt_en_base(tl_emetteur,vl_code_dob,vl_typeFMC,vl_attribut,vl_autoroute,vl_PR,vl_sens,vl_autoroutetete,vl_PRtete,vl_attribut_nonobl,pl_message);
	if (!strcmp(vl_code_dob,"EQU"))	
	{
		if (strcmp(tl_emetteur,"frd83") == 0) {
	    			pl_string= strtok (vl_attribut_nonobl.dis,",");
	    	/*		pl_string= strtok (NULL,",");
	    			pl_string= strtok (NULL,",");*/
	    			strncpy(vl_ligne1,pl_string,256);
	    			vl_ligne1[255]='\0';
	    			pl_string= strtok (NULL,",");
	    			strncpy(vl_ligne2,pl_string,256);
	    			vl_ligne2[255]='\0';
	    			pl_string= strtok (NULL,",");
	    			strncpy(vl_ligne3,pl_string,256);
	    			vl_ligne3[255]='\0';
	    	/*		pl_string= strtok (NULL,",");
	    			strncpy(vl_ligne4,pl_string,256);
	    			vl_ligne4[255]='\0';*/
		}
		else {
	    			pl_string= strtok (vl_attribut_nonobl.dis,"/");
	    			pl_string= strtok (NULL,"/");
	    			pl_string= strtok (NULL,"/");
	    			strncpy(vl_ligne1,pl_string,256);
	    			vl_ligne1[255]='\0';
	    			pl_string= strtok (NULL,"/");
	    			strncpy(vl_ligne2,pl_string,256);
	    			vl_ligne2[255]='\0';
	    			pl_string= strtok (NULL,"/");
	    			strncpy(vl_ligne3,pl_string,256);
	    			vl_ligne3[255]='\0';
	    			pl_string= strtok (NULL,"/");
	    			strncpy(vl_ligne4,pl_string,256);
	    			vl_ligne4[255]='\0';
	    	}		
	    			/* mettre A8 a la place de 1 !!!!!!!!!!!!*/
	    			sprintf(vl_nom,"%s-%d-%d",vg_Autoroutes[vl_autoroute-1].nom,vl_PR,vl_sens);
	    			if ( vl_attribut_nonobl.end==XDC_VRAI )
	    			{
	    			if (XZIS24_Raf_PMVDATEX(vl_nom,"","","","") != XDC_OK)
	    				XZST_03EcritureTrace(   XZSTC_WARNING, " : Probleme a l'envoi de l'affichage PMV");
	    			}
	    			else
	    			{
	    			if (XZIS24_Raf_PMVDATEX(vl_nom,vl_ligne1,vl_ligne2,vl_ligne3,vl_ligne4) != XDC_OK)
	    				XZST_03EcritureTrace(   XZSTC_WARNING, " : Probleme a l'envoi de l'affichage PMV");
	    			}

	}

/* XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : informations fournies a la base de donnees du fichier : %sType          : %sVersion       : %dHorodate      : %sMessage       : \n%sRef Evenement : %s\n",
				        pa_file,
				        vl_attribut.dob,
				        vl_attribut.vnm,
				        pl_strdate,
				 	pl_message,
				 	vl_attribut.snm);  */
    	
      		XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode_readfile : le fichier %s a ete lu et a ete sauve",pa_file );      		

      	
      		return ;
    	}
    	
    	/* Le fichier n'a pu etre ouvert */
    	XZST_03EcritureTrace(   XZSTC_WARNING, "IN : aces_evt_decode_readfile : le fichier %s n'a pu etre lu",pa_file );
  	return ;

}

/*A
*******************************************
* Fonction call back qui reagit au datagroup
* deginit dans le timer de purge
* Elle decode les fichiers donnees trafic
* et les transmet au synoptique
*******************************************
* Arguments : Aucun
*******************************************/

void aces_evt_decode()
{
FILE 			* pl_fD=NULL;
char 			tl_buffer[TAILLE+1];
int 			vl_JourSemaine;
double 			vl_HorodateSec; 
char 			tl_str[TAILLE];

    	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN : aces_evt_decode" );
 
	/* rapatriement des fichiers a decoder */
	aces_evt_ftp();

	/* lecture des fichiers donnees trafic Strada */
	sprintf(tl_str, "ls %sS*.sit %sfr*.sit 2>&-",C_PATH,C_PATH);

  XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode : %s",tl_str); 


	if ( (pl_fD = popen(tl_str,"r")) != NULL) {
		while(fgets(tl_buffer,TAILLE,pl_fD) != NULL) {
			/* lecture du fichier Strada */
			if(sscanf(tl_buffer,"%s\n",tl_str)==1)
				aces_evt_decode_readfile(tl_str);

		}
		/*pclose(pl_fD);*/
	}
	
	pclose(pl_fD);
  XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode : apres close"); 
	/* C_PATH contient le repertoire de travail du SAE ou sont transferes les fichiers trafic */
	sprintf(tl_str, "rm %sS* %sfr*",C_PATH,C_PATH); 
	
	/* effacer tous les fichiers trafic apres traitement */
	system(tl_str);	
  XZST_03EcritureTrace(   XZSTC_INFO, "IN : aces_evt_decode : apres rm"); 
	
	/*A Lecture horodate courante systeme */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);
   
        vl_HorodateSec+=vm_frequence;
	/* armement du timer de programmation */
	if (XZSM_01DemanderProgrammation(	vl_HorodateSec,
						0,
						XDG_AS_TIMER_CLT_EVT,
						XDM_AS_TIMER_CLT_EVT,
						"cle_timer_client_evt",
						XZSMC_SEC_VRAI) !=  XDC_OK ) {
		/* Ecriture Trace */
        	XZST_03EcritureTrace( XZSTC_WARNING, "IN : aces_evt_init : Programmation du timer de donnees evenement non declenchee. \n " );
        	return ;
	} 
    	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : aces_evt_decode" );
  return ;
}


/*A
*******************************************
* Fonction liberant la memoire dynamique
* allouee pour la liste chainee contenant
* le code des equipements
*******************************************
* Arguments : Aucun
*******************************************/
void aces_evt_freemem()
{
liste_att	pl_att1 ;
liste_att	pl_att2 ;
int 		vl_i;

	for (vl_i=0;vl_i<NBR_ELT_TABLE;vl_i++)
		for (pl_att1 = pm_att[vl_i];pl_att1 != NULL; pl_att1 = pl_att2)
		{
			pl_att2 =  pl_att1 -> suivant;
			free (pl_att1 -> string);
			free (pl_att1 -> name);
			free (pl_att1);
		}
}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU :
*       aces_evt_cree_fmc : determine, pour un evenement qui provient de la DDE 83,
*       s'il faut creer une FMC en fonction du type d'�venement et de sa position
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int aces_evt_cree_fmc (XDY_Mot          va_typeFMC,
                       XDY_Octet        va_autoroute,
                       XDY_PR           va_PR,
                       XDY_Sens         va_sens,
                       XDY_Octet        va_autoroute_tete,
                       XDY_PR           va_PR_tete,
                       XDY_Sens         va_sens_tete,
                       att_nonobl       va_attribut_nonobl)
{
        int evt_type_1 = 0;
        int evt_type_2 = 0;
        int evt_type_3 = 0;

        XZST_03EcritureTrace(XZSTC_FONCTION, "IN : aces_evt_cree_fmc" );
        XZST_03EcritureTrace(XZSTC_WARNING, "aces_evt_cree_fmc : typeFMC %d, autoroute %d, PR %d, sens %d, autoroute_tete %d PR_tete %d sens_tete", va_typeFMC,va_autoroute,va_PR,va_sens,va_autoroute_tete,va_PR_tete,va_sens_tete);


/* evt_type_1 designe les evenements de type    */
        /* - Travaux (hors basculement)                 */
        /* - Vehicule arrete sur BAU                    */
        /* - Vehicule en panne sur BAU                  */
        /* - Obstacle sur BAU                           */
        if ((va_typeFMC==XZAEC_FMC_Travaux) ||
            ((va_typeFMC==XZAEC_FMC_Obstacle) && (va_attribut_nonobl.nln==0)) ||
            ((va_typeFMC==XZAEC_FMC_VehArrete) && (va_attribut_nonobl.nln==0)) ||
            ((va_typeFMC==XZAEC_FMC_VehPanne) && (va_attribut_nonobl.nln==0)))
            evt_type_1 = 1;

       /* evt_type_2 designe les evenements de type   */
        /*  - d�lestage ou d�viation,                  */
        /*  - basculement,                             */
        /*  - coupure de chauss�e.                     */
        else
        if ((va_typeFMC==XZAEC_FMC_Basculement) ||
            (va_typeFMC==XZAEC_FMC_Delestage) ||
            (va_typeFMC==XZAEC_FMC_Deviation) ||
            (va_attribut_nonobl.lnp == 0))
           evt_type_2 = 1;
        /* evt_type_3 designe les evenements de type   */
        /*  - accident,                                */
        /*  - bouchon > 5km sans cause,                */
        /*  - bouchon > 3km ayant une cause,           */
        else
        if ((va_typeFMC==XZAEC_FMC_Accident) ||
            ((va_typeFMC==XZAEC_FMC_QueueBouchon) && (va_attribut_nonobl.len > 5000)) ||
            ((va_typeFMC==XZAEC_FMC_QueueBouchon) && (va_attribut_nonobl.len > 3000) && (va_attribut_nonobl.lnk != '\0')))
            evt_type_3 = 1;

        /* Dans le cas A57 (num�ro = 8), sens 1, PR 6.350 � 4.4 */
        /* ou          A50 (num�ro = 3), sens 1, PR 67.700 � 69 */
        /* Creation d'une FMC pour tout type d'evenement        */
        /* sauf type 1                                          */
        if (((va_autoroute      == 8) && (va_sens      == 1) && (va_PR      <= 6350)  && (va_PR      >= 4400))  ||
            ((va_autoroute_tete == 8) && (va_sens_tete == 1) && (va_PR_tete <= 6350)  && (va_PR_tete >= 4400))  ||
            ((va_autoroute      == 3) && (va_sens      == 1) && (va_PR      >= 67700) && (va_PR      <= 69000)) ||
            ((va_autoroute_tete == 3) && (va_sens_tete == 1) && (va_PR_tete >= 67700) && (va_PR_tete <= 69000)))
        {
           if (evt_type_1)
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_RIEN");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_RIEN);
           }
           else
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_FMC");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_FMC);
           }
        }

        /* Dans le cas A57 sens 1 PR 4.4 a 0                     */
        /* ou          A57 sens 2 PR 0 � 12                      */
        /* ou          A50 sens 1 PR 69 � 72.780                 */
        /* ou          A50 sens 2 PR 72.780 � 67.700             */
        /* Creation d'une FMC pour les evenements de type 2 ou 3 */
        if (((va_autoroute      == 8) && (va_sens      == 1) && (va_PR      >= 0)     && (va_PR      <= 4400))  ||
            ((va_autoroute_tete == 8) && (va_sens_tete == 1) && (va_PR_tete >= 0)     && (va_PR_tete <= 4400))  ||
            ((va_autoroute      == 8) && (va_sens      == 2) && (va_PR      >= 0)     && (va_PR      <= 12000)) ||
            ((va_autoroute_tete == 8) && (va_sens_tete == 2) && (va_PR_tete >= 0)     && (va_PR_tete <= 12000)) ||
            ((va_autoroute      == 3) && (va_sens      == 1) && (va_PR      >= 69000) && (va_PR      <= 72780)) ||
            ((va_autoroute_tete == 3) && (va_sens_tete == 1) && (va_PR_tete >= 69000) && (va_PR_tete <= 72780)) ||
            ((va_autoroute      == 3) && (va_sens      == 2) && (va_PR      >= 67700) && (va_PR      <= 72780)) ||
            ((va_autoroute_tete == 3) && (va_sens_tete == 2) && (va_PR_tete >= 67700) && (va_PR_tete <= 72780)))
        {
           if (evt_type_2 || evt_type_3)
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_FMC");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_FMC);
           }
           else
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_RIEN");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_RIEN);
           }
        }
        /* Dans le cas A570 (num�ro = 11), sens 2, PR 12 � 0 */
        /* Creation d'une FMC pour les evenements de type 2  */
        if (((va_autoroute      == 11) && (va_sens      == 2) && (va_PR      >= 0)     && (va_PR      <= 7160))  ||
            ((va_autoroute_tete == 11) && (va_sens_tete == 2) && (va_PR_tete >= 0)     && (va_PR_tete <= 7160)))
        {
           if (evt_type_2)
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_FMC");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_FMC);
           }
           else
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_RIEN");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_RIEN);
           }
        }

        /* Dans le cas A570 (num�ro = 11), sens 1, PR 12 � 0 */
        /* Creation d'une FMC pour les evenements de type 2  */
        if (((va_autoroute      == 11) && (va_sens      == 1) && (va_PR      >= 0)     && (va_PR      <= 7160))  ||
            ((va_autoroute_tete == 11) && (va_sens_tete == 1) && (va_PR_tete >= 0)     && (va_PR_tete <= 7160)))
        {
           if (evt_type_2 || evt_type_3)
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_FMC");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_FMC);
           }
           else
           {
              XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_RIEN");
              XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
              return (XDC_DATEX_RIEN);
           }
	}

        /* pour le reste */
        XZST_03EcritureTrace(XZSTC_INFO, "aces_evt_cree_fmc : XDC_DATEX_FMC");
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : aces_evt_cree_fmc" );
        return (XDC_DATEX_RIEN);

}


/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	aaces_evt_en_base : recoit le message RTWks XDM_AA_alerte, recupere les informations
*	les met en forme Sybase et appelle la SP XZAE141.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int aces_evt_en_base 	(char		*pa_emetteur, 
			char		*pa_code_dob, 
			XDY_Mot		va_typeFMC, 
			att_obl		va_attribut, 
			XDY_Octet	va_autoroute,
			XDY_PR		va_PR,
			XDY_Sens	va_sens,
			XDY_Octet	va_autoroutetete,
			XDY_PR		va_PRtete,
			att_nonobl	va_attribut_nonobl, 
			char		*pa_synthese)

/*
* ARGUMENTS EN ENTREE :
*
*
* ARGUMENTS EN SORTIE :
*
*   aucun
*
* CODE RETOUR : 
*  aucun
*
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Creation de l'evt en base 
* 	
------------------------------------------------------*/
{
	static char *version = "aces_evt_en_base" ;
	
	/* Variables locales pour messages RTWks */
	
	T_STR 		vl_reference;
	T_STR		vl_type;
	T_STR		vl_date;
	T_STR		vl_ligne;
	T_INT4		vl_version;
	
	/* Variables locales pour L Open Client SYBASE */
	
	CS_CHAR *rpc_name = "XZAE141";	/*Nom de la SP a appele*/
	CS_CHAR *rpc_name149 = "XZAE149";	/*Nom de la SP a appele*/
	
	CS_CONTEXT      *pl_context   = XZAGV_Context;
	CS_CONNECTION   *pl_connection= XZAGV_Connexion; 	/* Variable Globale de connexion 
								** initialise par xzag_connexion
								** appele dans acli_dir.
								*/							
	
	CS_COMMAND	*vl_cmd;
	
	CS_RETCODE      vl_retcode;
	CS_INT          vl_res_type;
	CS_SMALLINT     vl_msg_id;
	XDY_Entier      vl_status=0;
	
	XDY_Horodate	vl_horofin=0.0;
	XDY_Horodate	vl_horoinp=0.0;
	XDY_Horodate	vl_horosta=0.0;
	XDY_Octet	vl_duree;
	XDY_Booleen	vl_confirme;
	XDY_Booleen	vl_matiere;
	XDY_Booleen	vl_personnalite;
	XDY_Booleen	vl_bloquant=XDC_FAUX;
	XDY_Octet	vl_perturbation;
	char            vl_texte[256];
	char		vl_texte1[256];
	char		vl_texte2[101];
        CS_DATETIME     vl_Horodatefin;	
        CS_DATETIME     vl_Horodatefinprevue;	
        CS_DATETIME     vl_Horodatedebut;	
        CS_DATETIME     vl_Horodateinp;	
	XZSMT_Horodate	vl_Horodate_Strada;
	XDY_Octet	vl_pl,vl_vl,vl_type_bouchon;
	XDY_Entier	vl_longueur=0;
	XDY_Entier	vl_long_synt=0,vl_pt=0;
	XDY_Octet	vl_morceau=0;

	XDY_Act		vl_actionTFM;

	int             vl_TraiterDATEXNormal = 1; /* Ajout SAGA : indique si l'evenement doit etre trait� de fa�on normale */
                                                   /* comme tout evenement DATEX                                            */
        int             vl_CreerFMC           = 0; /* Ajout SAGA : indique si une FMC doit etre creee automatiquement       */
	
	vl_actionTFM.numero = 0;
	vl_actionTFM.cle = 0;

XZST_03EcritureTrace(XZSTC_INFO,"	aces_evt_en_base: emetteur:%s,pa_code_dob:%s; typeFMC:%d",pa_emetteur,pa_code_dob,va_typeFMC);
	/*A Conversion des horodates INP et STA */
	if (aces_evt_decode_readfile_date(va_attribut.inp ,&vl_Horodate_Strada, (char *)"Mise en base",TRUE,&vl_horoinp) == FALSE)
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: Probleme lors de la conversion de date");
                return (XDC_NOK);
	}
	vl_horoinp=vl_horoinp+1.0;

	if (aces_evt_decode_readfile_date(va_attribut.sta ,&vl_Horodate_Strada, (char *)"Mise en base",TRUE,&vl_horosta) == FALSE)
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: Probleme lors de la conversion de date");
                return (XDC_NOK);
	}
	vl_horosta=vl_horosta+1.0;
	
	
	if ( va_attribut_nonobl.sto != 0 )
	{
		if ( va_attribut_nonobl.end == XDC_VRAI)
			vl_horofin=va_attribut_nonobl.sto;
	}
	else
	{
		if ( va_attribut_nonobl.end == XDC_VRAI)
			vl_horofin=vl_horoinp;
	}
	
	vl_duree=(XDY_Octet) va_attribut_nonobl.duv/60;
/*	if ( va_attribut_nonobl.duv % 60 != 0)
		vl_duree++;*/
		
	if ( (va_attribut_nonobl.qin >=5) || (va_attribut_nonobl.qin=0) )
		vl_confirme=XDC_FAUX;
	else
		vl_confirme=XDC_VRAI;
		
		
	if (!strcmp(va_attribut_nonobl.loa,"DAN"))
		vl_matiere=XDC_VRAI;
	else
		vl_matiere=XDC_FAUX;
		
			
	if (!strcmp(va_attribut_nonobl.loa,"VIP"))
		vl_personnalite=XDC_VRAI;
	else
		vl_personnalite=XDC_FAUX;
	if (va_attribut_nonobl.nln==0)
		vl_bloquant=XDC_VRAI;
	else
		vl_bloquant=XDC_FAUX;
		

	if (!strcmp(pa_code_dob,"WIN"))
		vl_perturbation=XZAEC_MET_VentFort;
	else if (!strcmp(pa_code_dob,"FOS"))
		vl_perturbation=XZAEC_MET_Brouillard;
	else if (!strcmp(pa_code_dob,"SNO"))
		vl_perturbation=XZAEC_MET_NeigeChaus;
	else if (!strcmp(pa_code_dob,"PRE"))
		vl_perturbation=XZAEC_MET_Pluie;
	else if (!strcmp(pa_code_dob,"WDA"))
		vl_perturbation=XZAEC_MET_Autre;
	else
		vl_perturbation=XZAEC_MET_Autre;
		
	/*A calcul des pl et vl */
	
	vl_pl=va_attribut_nonobl.nhl+va_attribut_nonobl.nhv;
	if ( va_attribut_nonobl.nve>= vl_pl)
		vl_vl=va_attribut_nonobl.nve-vl_pl-va_attribut_nonobl.nbu-va_attribut_nonobl.cav;
	else
		vl_vl=0;
	
	vl_type_bouchon=XZAEC_BOU_Ralenti;	
	switch (va_attribut_nonobl.sev)
	{
		case 1 :
			vl_type_bouchon=XZAEC_BOU_Bloque;
			break;
		case 2 :
			vl_type_bouchon=XZAEC_BOU_Bouche;
			break;
		case 3 :
			vl_type_bouchon=XZAEC_BOU_Ralenti;
			break;
		case 4 :
			vl_type_bouchon=XZAEC_BOU_Accordeon;
			break;
		case 5 :
			vl_type_bouchon=XZAEC_BOU_Dense;
			break;
		default :
			vl_type_bouchon=XZAEC_BOU_Ralenti;
			break;
	}
	
	if (va_attribut_nonobl.que > va_attribut_nonobl.len)
		vl_longueur=va_attribut_nonobl.que;
	else
		vl_longueur=va_attribut_nonobl.len;

	if (!strcmp(pa_code_dob,"EQU"))	
	{
	strncpy(vl_texte1,va_attribut_nonobl.dis,250);
	vl_texte1[250]='\0';
	if (strlen(va_attribut_nonobl.dis) > 250)
		strncpy(vl_texte2,va_attribut_nonobl.dis,100);
	else
		vl_texte2[0]='\0';
	vl_texte2[100]='\0';
	}
	else
	{
	if (va_attribut_nonobl.sur[0]=='\n')
		va_attribut_nonobl.sur[0]='\0';
	XZST_03EcritureTrace(XZSTC_INFO,"sur:%s.",va_attribut_nonobl.sur);
	strncpy(vl_texte1,va_attribut_nonobl.sur,250);
	vl_texte1[250]='\0';
	if (strlen(va_attribut_nonobl.sur) > 250)
		strncpy(vl_texte2,&va_attribut_nonobl.sur[250],100);
	else
		vl_texte2[0]='\0';
	vl_texte2[100]='\0';
	}

	/*A
	** Open Client
	*/
       /*A
        ** Conversion des dates en entree du format Unix au format Sybase
        */

        if ((vl_retcode = asql_date_Ux2Sybase(vl_horofin,&vl_Horodatefin))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
                return (XDC_NOK);
        }
        if ((vl_retcode = asql_date_Ux2Sybase(va_attribut_nonobl.sto,&vl_Horodatefinprevue))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
                return (XDC_NOK);
        }
        if ((vl_retcode = asql_date_Ux2Sybase(vl_horosta,&vl_Horodatedebut))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
                return (XDC_NOK);
        }
        if ((vl_retcode = asql_date_Ux2Sybase(vl_horoinp,&vl_Horodateinp))!= XDC_OK)
        {

                XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_date_Ux2Sybase(va_Horodate,&va_Horodate_in) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
                return (XDC_NOK);
        }

	 /* Cas particulier des messages SAGA */
        if (strcmp(pa_emetteur,"frd83") == 0)
        {
           int vl_cree_fmc = aces_evt_cree_fmc(va_typeFMC,va_autoroute,va_PR,va_sens,
                                               va_autoroutetete,va_PRtete,va_sens,va_attribut_nonobl);
           XDY_Entier vl_evt_fmc = 0;
           XDY_Octet  vl_cle_fmc = 0;
           int        vl_cle_fmc_temp = 0;

	   XZST_03EcritureTrace(XZSTC_WARNING,"vl_cree_fmc = %d",vl_cree_fmc);

           /* Dans le cas dun evenement d'origine ESCOTA,     */
/* l'evenement est cree dans ADA_SIT si necessaire */
           /* pour faire le lien avec la FMC ESCOTA           */
           if (strncmp(va_attribut.snm,"ESCOTA/",7) == 0)
           {
              /* Si l'evenement possede une reference ESCOTA    */
              /* la reference est de la forme ESCOTA/numEvt/cle */
              if(sscanf(&va_attribut.snm[7],"%d/%d",&vl_evt_fmc,&vl_cle_fmc_temp) == 2)
              {
                    vl_cle_fmc = vl_cle_fmc_temp;
                    XZST_03EcritureTrace(XZSTC_INFO,"Il s'agit d'une reference ESCOTA : %d, %d",vl_evt_fmc,vl_cle_fmc);
		    if (XZAE553_Creer_Evenement_DATEX(pa_emetteur,va_attribut.snm,vl_evt_fmc,vl_cle_fmc) != XDC_OK)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : XZAE553_Creer_Evenement_DATEX a echoue");
                       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base Sortie");
                       return (XDC_NOK);
       		}
           }
        }
	 if (strncmp(va_attribut_nonobl.lnk,"ESCOTA/",7) == 0)
           {
              /* Si l'evenement possede un lien vers une reference ESCOTA    */
              /* la reference est de la forme ESCOTA/numEvt/cle */
              if(sscanf(&va_attribut_nonobl.lnk[7],"%d/%d",&vl_evt_fmc,&vl_cle_fmc_temp) == 2)
              {
                   vl_cle_fmc = vl_cle_fmc_temp;
                    XZST_03EcritureTrace(XZSTC_INFO,"Il s'agit d'un lien vers une reference ESCOTA : %d, %d",vl_evt_fmc,vl_cle_fmc);
                    /* On cree un evenement correspondant dans ADA_SIT pour faire le lien avec la FMC */
                    if (XZAE553_Creer_Evenement_DATEX(pa_emetteur,va_attribut_nonobl.lnk,vl_evt_fmc,vl_cle_fmc) != XDC_OK)
                    {
                       XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : XZAE553_Creer_Evenement_DATEX a echoue");
                       XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base Sortie");
                       return (XDC_NOK);
                    }
              }
           }
           if (strcmp(pa_code_dob,"INF") == 0)
           {
              char vl_message[500];
              char vl_msg1[256];
              char vl_msg2[256];
              vl_TraiterDATEXNormal = 1;
              vl_CreerFMC     = 0;
              /* dans le cas d'un message INF de la DDE83                   */
              /* Si ce message n'est lie a aucune FMC,                      */
              /* enregistrer le message TFM en base dans TFM_DTX (XZAI500)  */
              /* puis raffraichir la liste sur le poste TFM                 */
              /* Le cas ou le message est lie a une FMC est traite par      */
              /* XZAE141.                                                   */
              strcpy(vl_message,"La DDE du Var nous informe :\n\n");
              strcat(vl_message,va_attribut_nonobl.sur);
              /* le message est decoupe en 2 */
              strncpy(vl_msg1,vl_message,250);
              vl_msg1[250]='\0';
              if (strlen(vl_message) > 250)
              {
                 strncpy(vl_msg2,vl_message+250,250);
                 vl_msg2[250]='\0';
              }
              else
              {
                 vl_msg2[0]='\0';
              }
              if (XZAI500_Enregistre_Message_TFM (pa_emetteur,va_attribut.snm,va_attribut_nonobl.lnk,vl_horosta,vl_msg1,vl_msg2) == XDC_OK)
              {
                 if (XZIT06_Modif_Animateur_TFM(XDC_AJOUT, vl_actionTFM) != XDC_OK)
                 {
                    XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : XZIT06_Modif_Animateur_TFM a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base Sortie");
                    return (XDC_NOK);
                 }
              }
           }
           else if (strcmp(pa_code_dob,"EQU") == 0)
           {
              /* dans le cas d'un message EQU de la DDE83 il faut conserver le traitement normal     */
              vl_TraiterDATEXNormal = 1;
              vl_CreerFMC     = 0;
           }
           else if (strcmp(pa_code_dob,"OPA") == 0)
           {
              vl_TraiterDATEXNormal = 0;
              vl_CreerFMC     = 0;
              if (strcmp(va_attribut.pha,"CLE") == 0)
              {
                 /* dans le cas d'un message action (DOB=OPA et PHR=CLE) de la DDE83                 */
                 /* l'action est memorise avec la FMC si la FMC existe dans MIGRAZUR (XZAC80)        */
                 int vl_numeroAction;
                 /* la champ SUR est eventuellement tron,que a 250 caracteres */
                 va_attribut_nonobl.sur[250]='\0';
                 if (XZAC80_Ajouter_Action_DATEX(pa_emetteur,va_attribut.snm,va_attribut_nonobl.lnk,
                                                 vl_horosta,vl_horofin,va_attribut_nonobl.sur,&vl_numeroAction) != XDC_OK)
                 {
                    XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : XZAC80_Ajouter_Action_DATEX a echoue");
                    XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base Sortie");
                    return (XDC_NOK);
                 }
              }
           }
	   else
           {
              /* Recherche si une FMC existe pour cet evenement */
              if (XZAE550_Chercher_FMC_DATEX (pa_emetteur,va_attribut.snm,&vl_evt_fmc,&vl_cle_fmc) != XDC_OK)
              {
                 XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : XZAE550_Chercher_FMC_DATEX a echoue");
                 XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base Sortie");
                 return (XDC_NOK);
              }
              if ((vl_evt_fmc != 0) && (vl_cle_fmc!= 0))
              {
		 XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : ici");
                 /* Si une FMC existe, on la met a jour */
                 vl_TraiterDATEXNormal = 1;
                 vl_CreerFMC           = 0;
              }
              else if (vl_cree_fmc == XDC_DATEX_FMC)
              {
		 XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : ici2");
                 /* dans le cas d'un evennement  de la DDE83 avec  aces_evt_cree_fmc=XDC_DATEX_FMC   */
                 /* on cree directement la FMC (XZAE552)                                             */
                 vl_TraiterDATEXNormal = 1;
                 vl_CreerFMC     = 1;
              }
              else
              {
		 XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : ici3");
                 /* L'evennement n'est pas traite */
                 vl_TraiterDATEXNormal = 0;
                 vl_CreerFMC     = 0;
              }
           }
        }
        else
        {
           /* Traitement normal */
           vl_TraiterDATEXNormal = 1;
           vl_CreerFMC     = 0;
        }
        /* Fin du cas particulier des messages SAGA */


	XZST_03EcritureTrace(XZSTC_WARNING, "vl_TraiterDATEXNormal = %d, vl_CreerFMC = %d",vl_TraiterDATEXNormal,vl_CreerFMC);

	if (vl_TraiterDATEXNormal)
        {
	/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"aaces_evt_en_base : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aaces_evt_en_base Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return ; 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	pa_emetteur[10]='\0';
	XZST_03EcritureTrace(XZSTC_INFO,"emetteur:%s.",pa_emetteur);
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)pa_emetteur,"@va_emetteur_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(emetteur) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	
	XZST_03EcritureTrace(XZSTC_INFO,"ref:%s.",va_attribut.snm);
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)va_attribut.snm,"@va_reference_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(reference) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&va_attribut.clv,"@va_version_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	XZST_03EcritureTrace(XZSTC_INFO,"dob:%s.",pa_code_dob);
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)pa_code_dob,"@va_dob_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(emetteur) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	
 	XZST_03EcritureTrace(XZSTC_INFO,"lnk:%s.",va_attribut_nonobl.lnk);
 	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)va_attribut_nonobl.lnk,"@va_lnk_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(emetteur) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
	   	return ;
	}	
        if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodateinp,"@va_date_in"))!= CS_SUCCEED)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(date) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)va_attribut.dob,"@va_type_in"))!= CS_SUCCEED)      
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(type) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&va_typeFMC,"@va_type_FMC_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_smallint_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodatedebut,"@va_debut_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_date_input(Horodate) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }
        if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodatefin,"@va_fin_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_date_input(Horodate) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }
        if ((vl_retcode = asql_ctparam_date_input(vl_cmd,(CS_DATETIME *)&vl_Horodatefinprevue,"@va_fin_prevue_in"))!= CS_SUCCEED)

        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzae22: asql_ctparam_date_input(Horodate) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAE22_Decrire_Accident_PA retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_duree,"@va_duree_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_confirme,"@va_confirme_signale_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(confirme_signale) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.can,"@va_fausse_alerte_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
 	XZST_03EcritureTrace(XZSTC_INFO,"autoroute p:%d.",va_autoroute);
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_autoroutetete,"@va_autoroute_p_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&va_PRtete,"@va_PR_p_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_sens,"@va_sens_p_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_autoroute,"@va_autoroute_s_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&va_PR,"@va_PR_s_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_sens,"@va_sens_s_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.mot,"@va_niveau_trafic_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.sev,"@va_gravite_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_vl,"@va_vl_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_vl_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_pl,"@va_pl_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_pl_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.cav,"@va_caravanes_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_caravanes_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.nbu,"@va_cars_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.inj,"@va_blesses_graves_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&va_attribut_nonobl.dea,"@va_morts_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_personnalite,"@va_personnalites_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(personnalites) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }
        if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_matiere,"@va_matieres_dangereuses_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(matieres_dangereuses) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_type_bouchon,"@va_type_bouchon_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_int_input(vl_cmd,(CS_INT *)&vl_longueur,"@va_longueur_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_longueur_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_perturbation,"@va_type_perturbation_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_perturbation_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
           XZST_03EcritureTrace(XZSTC_INFO, "visi %d", va_attribut_nonobl.vis);
	if ((vl_retcode = asql_ctparam_smallint_input(vl_cmd,(CS_SMALLINT *)&va_attribut_nonobl.vis,"@va_visibilite_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_version_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
        if ((vl_retcode = asql_ctparam_bit_input(vl_cmd,(CS_BIT *)&vl_bloquant,"@va_bloquant_in"))!= CS_SUCCEED)
        {
           XZST_03EcritureTrace(XZSTC_WARNING,"xzao84: asql_ctparam_bit_input(bloquant) a echoue");
           XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : XZAO84_Ajouter_Echangeur retourne %d", vl_retcode);
           asql_clean_cmd(pl_connection, vl_cmd);
           return (XDC_NOK);
        }


        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_texte1,"@va_texte1_in"))!= CS_SUCCEED)      
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(ligne) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
        if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_texte2,"@va_texte2_in"))!= CS_SUCCEED)      
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(type) a echoue");
                XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
                asql_clean_cmd(pl_connection, vl_cmd);

                return ;
        }
	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return ;
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd,&vl_status );
						XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : XZAA01SP retourne %d",vl_status);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
		
                                	return;
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
						return;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return;
				}
		}
	XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base: POK_EVT_STRADA");
	XZIT12_Evt_Strada();
	asql_clean_cmd(pl_connection, vl_cmd);	
	XZST_03EcritureTrace(XZSTC_INFO,"fin0:%s",pa_synthese);
	}	

        /* Creation automatique des FMC pour les evenements SAGA */
        if (vl_CreerFMC)
        {
           /* Creation de la FMC (XZAE552)                                                        */
           /* et raffraichissement la liste des FMC au CI et dans le district concern� (DC ou DA) */
           XDY_Octet vl_site;
           int       vl_numFMC = 0;
           XZITT_Destinataire      vl_Destinataire;
           XDY_Evt                 vl_Evt;
           /* Le site est DA pour l'autoroute A50 ou DC pour les autres autoroutes */
           if (va_autoroute == 3)
           {
              vl_site = XDC_DP;
              strcpy(vl_Destinataire.site,XDC_NOM_SITE_DP);
           }
           else
           {
              vl_site = XDC_VC;
              strcpy(vl_Destinataire.site,XDC_NOM_SITE_VC);
           }

           strncpy(vl_texte,pa_synthese,250);
           vl_texte[250]='\0';

           if (XZAE552_Creation_FMC(pa_emetteur,va_attribut.snm,CLE_SAGA,vl_site,vl_texte,&vl_numFMC) != XDC_OK)
           {
              XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aaces_evt_en_base Sortie");
              return (XDC_NOK);
           }

           vl_Evt.numero = vl_numFMC;
           vl_Evt.cle    = CLE_SAGA;
           if (XZIT08_Modif_Liste_FMC(vl_Evt,vl_Destinataire,"Nouvelle FMC") != XDC_OK)
           {
              XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aaces_evt_en_base Sortie");
              return (XDC_NOK);
           }
           strcpy(vl_Destinataire.site,XDC_NOM_SITE_CI);
           if (XZIT08_Modif_Liste_FMC(vl_Evt,vl_Destinataire,"Nouvelle FMC") != XDC_OK)
           {
              XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aaces_evt_en_base Sortie");
              return (XDC_NOK);
           }
        }
	
	if ((!strcmp(pa_code_dob,"INF")) || (!strcmp(pa_code_dob,"EQU")))
	{
		XZST_03EcritureTrace(XZSTC_INFO,"OUT %s: Pas de memorisation de la synthese",version);
		return  (XDC_NOK); 		
        }
        
	/*A Mise en base de la synthese */
	
	vl_morceau=0;
	vl_long_synt=strlen(pa_synthese);
	
	vl_pt=0;
	
	while (vl_pt <= vl_long_synt)
	{
	vl_morceau++;
	XZST_03EcritureTrace(XZSTC_INFO,"morceau:%d,pr:%d",vl_morceau,vl_pt);
	strncpy(vl_texte,pa_synthese,250);
	vl_texte[250]='\0';
	XZST_03EcritureTrace(XZSTC_INFO,"texte:%s.",vl_texte);
		/*A
	** Initialisation de la commande				
	*/

	if ((vl_retcode = asql_cmd_alloc(pl_connection, &vl_cmd)) != CS_SUCCEED)
	{
	 	XZST_03EcritureTrace(XZSTC_FATAL,"aaces_evt_en_base : ct_cmd_alloc() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aaces_evt_en_base Sortie");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return  (XDC_NOK); 		
	}
	
	/*A
	** Envoie de la commande RPC pour notre stored procedure.
	*/
	if ((vl_retcode = ct_command(vl_cmd, CS_RPC_CMD, rpc_name149, CS_NULLTERM, CS_NO_RECOMPILE)) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_command() a echoue avec le code %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
		asql_clean_cmd(pl_connection, vl_cmd);
		return  (XDC_NOK); 		
	}
	/*A
	** Preparation de l envoie des parametres a la SP
	*/
	pa_emetteur[10]='\0';
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)pa_emetteur,"@va_emetteur_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(emetteur) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
		return  (XDC_NOK); 		
	}	
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)va_attribut.snm,"@va_reference_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_input(reference) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
		return  (XDC_NOK); 		
	}	
	if ((vl_retcode = asql_ctparam_tinyint_input(vl_cmd,(CS_TINYINT *)&vl_morceau,"@va_morceau_in"))!= CS_SUCCEED)
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : \nasql_ctparam_int_input(va_morceau_in) a echoue avec %d",vl_retcode);
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");	
		asql_clean_cmd(pl_connection, vl_cmd);
		return (XDC_NOK);
	}
	if ((vl_retcode = asql_ctparam_char_input(vl_cmd,(CS_CHAR *)vl_texte,"@va_texte_in"))!= CS_SUCCEED)	
	{	
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_ct_param_char_inputtexte) a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
	  	asql_clean_cmd(pl_connection, vl_cmd);
		
		return (XDC_NOK);
	}	


	
	/*A
	** Envoie de la commande au Sql server
	*/
	if (ct_send(vl_cmd) != CS_SUCCEED)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : ct_send() a echoue");
		XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base sort ");		
		asql_clean_cmd(pl_connection, vl_cmd);
		return (XDC_NOK);
	}
	/*A
	** Traitement du resultat
	*/

	while ((vl_retcode = ct_results(vl_cmd, &vl_res_type)) == CS_SUCCEED)
	{
		switch ((int)vl_res_type)
		{
			case CS_ROW_RESULT:
			case CS_PARAM_RESULT:
			case CS_STATUS_RESULT:
				/*B
				** Impression de l entete en fonction du type de resultat.
				*/
				switch ((int)vl_res_type)
				{
					case  CS_ROW_RESULT:
						
						vl_retcode = asql_fetch_data(vl_cmd);
						XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base: ROW RESULTS");
						break;

					case  CS_PARAM_RESULT:
						vl_retcode = asql_fetch_data (vl_cmd );
						XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : PARAMETER RESULTS\n");
						break;

					case  CS_STATUS_RESULT:
						vl_retcode = asql_fetch (vl_cmd,&vl_status );
						XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : XZAA01SP retourne %d",vl_status);
						break;
				}
				
				if (vl_retcode != CS_SUCCEED)
                                {
                                        XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base : asql_fetch_data a echoue");
                                        XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
                                        asql_clean_cmd(pl_connection, vl_cmd);
		
                                	return (XDC_NOK);
                                }

	
				break;


				case CS_MSG_RESULT:
					/*
					**
					*/
					vl_retcode = ct_res_info(vl_cmd, CS_MSGTYPE,(CS_VOID *)&vl_msg_id, CS_UNUSED, NULL);
					if (vl_retcode != CS_SUCCEED)
					{
						XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_res_info(msgtype) a echoue");
						XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
						/*return vl_retcode; OM*/
                                		asql_clean_cmd(pl_connection, vl_cmd);
						return (XDC_NOK);;
					}
					
					XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : ct_result retourne CS_MSG_RESULT l id du message est : %d.\n", vl_msg_id);

					break;

				case CS_CMD_SUCCEED:
					/*
					** Pas de colonnes retournees.
					*/
					XZST_03EcritureTrace(XZSTC_INFO,"aces_evt_en_base : CS_CMD_SUCCEED \n");
					
					break;

				case CS_CMD_DONE:

					/*
					** Commande effectuee : positionnement des parametres de retour.
					*/
					XZST_03EcritureTrace(XZSTC_WARNING,"aces_evt_en_base : CS_CMD_DONE \n");
					break;

				case CS_CMD_FAIL:
					/*
					** Le serveur SQL a detecte une erreur en executant notre commande : Sortie.
					*/
					XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_results retourne CS_CMD_FAIL.");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return  (XDC_NOK); 		
					break;

				default:
					/*
					** Il s est passe quelque chose d inattendu.
					*/
					printf("general = default \n");
					XZST_03EcritureTrace(XZSTC_FATAL,"aces_evt_en_base: ct_results retourne un resultat de type inattendu");
					XZST_03EcritureTrace(XZSTC_INTERFACE, "OUT : aces_evt_en_base : sortie");
					asql_clean_cmd(pl_connection, vl_cmd);
					return  (XDC_NOK); 		
				}
		}
	asql_clean_cmd(pl_connection, vl_cmd);	

	vl_pt+=250;
	pa_synthese+=250;
	
	}
	
	XZST_03EcritureTrace(XZSTC_INFO,"OUT %s",version);
        return;
}

