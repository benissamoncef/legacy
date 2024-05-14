/*E*/
/*Fichier : $Id: drol_prv_util.c,v 1.5 2018/02/09 14:41:46 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2018/02/09 14:41:46 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
------------------------------------------------------
* MODULE DROL_PRV * FICHIER drol_prv_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf drol_prv_util.h 
*
*   NB : toute modification de l'interface des fonctions
*	de recherche des PRV doit etre repercutee sur
*	l'editeur de connaissances (modele prv_eqpt.aw)
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	25/01/12 : Cr�ation (DEM/1016)
* JMG	01/009/15 : DEM131 1.2
* JPL	09/02/18 : Migration Linux 64 bits (DEM 1274) : Suppression de declarations inutiles  1.5
* LCL	05/10/23 	: Adaptations CNA DEM-483 : lecture site principal en base
------------------------------------------------------*/

/* fichiers inclus */
#include "drol_prv_util.h"

#include "xzao17.h"
#include "xzao419.h"
#include "xzao424.h"
#include "xzao425.h"
#include "xzao576.h"
#include "xzao577.h"
#include "xzat51.h"


/* definitions de constantes */
#define DISTANCE_INFINIE	1000000
#define NB_LIGNES_PRV		3

#define CLASSE_PERTURBATION	"perturbation"
#define ATT_PERTURBATION_TYPE	"type"
#define ATT_PRV_PERTURBATION	"perturbation"
#define NbMaxEnr_o15		6
#define NbMaxEnr_t51		6
#define CLASSE_LOCALISATION        "localisation"

#define C_PRV_TROUVE		0
#define C_PB_PRV		1
#define C_PLUS_DE_PRV		2

/* definitions de types locaux */
typedef struct {
	XDY_Octet	DispoTexte;
	T_STRING	Vitesse;
	T_STRING	Bandeau;
	XDY_Entier	NumeroEvtU;
	XDY_Octet	CleEvtU;
	XDY_Octet	PrioriteU;
	XDY_Entier	DistanceU;
	XDY_Octet	PrioriteC;
	T_REAL8		DistanceC;
	int		Amont;
	XDY_Eqt		NumPRV;	
	XDY_Autoroute	AutoroutePRV;	
	XDY_PR		PRPRV;	
	XDY_Sens	SensPRV;	
	XDY_District	DistrictPRV;	
	T_STRING	DirectionPRV;
} PRVT_Utilisation;

typedef struct {
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Mot		Numero;
	XDY_Eqt		NumPRV;
	XDY_District	DistrictPRV;
	XDY_Autoroute	AutoroutePRV;
	XDY_PR		PRPRV;
	XDY_Sens	SensPRV;
	XDY_Entier	DistancePRV;
} PRVT_xzao15;				/* structure d'appel de XZAO15 */

typedef struct {
	XDY_Eqt		NumPRV;
	XDY_Octet	dispo_texte;
	T_STRING	vitesse;
	T_STRING	bandeau;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
} PRVT_xzat51;				/* structure d'appel de XZAT05 */


/*definition de variables globales*/
static	T_PERT_PRV			*pm_pert=NULL;

/* definition de macro locales */


/* declaration de variables locales */
static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : drol_prv_util" ;

PRVT_Utilisation	*vg_Utilisation;
PRVT_xzao15		vg_appel_o15[NbMaxEnr_o15];
PRVT_xzat51		vg_appel_t51[NbMaxEnr_t51];
int 			vg_max_num_prv=0;
T_PRV			vg_prv[C_NB_MAX_PRV];
int				vg_priorite_retenue;
extern XDY_Mot	vg_SitePrincipal;

/* declaration de fonctions internes */
static int etat_prv();
static XDY_Booleen creer_prv();

static int ajouter_prv(XDY_Eqt va_numero_prv,
                        XDY_District va_district_prv,
                        XDY_Autoroute va_autoroute_prv,
                        XDY_PR va_pr_prv,
                        XDY_Sens va_sens_prv,
                        XDY_District va_district,
                        int va_priorite,
                        XDY_Horodate va_horodate,
                        char *va_perturbation,
			int	va_amont,
                        int *pa_resultat);


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   verification de la longueur des lignes PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen	prv_ligne_trop_longue(char *Texte, int NbCarLigne)

/*
* ARGUMENTS EN ENTREE :
*   T_STRING	Texte;
*   T_REAL8	NbCarLigne;
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   TRUE si la ligne est trop longue, FALSE sinon
*
* CONDITION D'UTILISATION
*   fonction appelee depuis le RTie
*
* FONCTION 
*    compte les caracteres entre deux \n
*
------------------------------------------------------*/
{
	int		i=0, n=0;
	int		code_retour=0;
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : prv_ligne_trop_longue" ;
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);
	
	XZST_03EcritureTrace(XZSTC_DEBUG1, "%s Texte = %s, NbCarLigne = %lf", version,Texte, NbCarLigne);

	/*A
	**  on compte les caracteres entre deux \n
	*/
	for (i=0; i<strlen(Texte); i++)
	{
		if (Texte[i] != '\n')
			n++;
		else
			n = 0;

		if (n > NbCarLigne)
		{
			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE",version);
			return(TRUE);
		}
	}

	/*A
	**  on renvoie le resultat au Rtie
	*/
	if (n <= NbCarLigne)
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne FALSE",version);
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE",version);
		return (TRUE);
	}
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   formattage d'une ligne PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void prv_tabuler_ligne(char *va_str_in, int va_nbcar, char *va_str_out)

/*
* ARGUMENTS EN ENTREE :
*   T_STRING	Texte;
*   T_REAL8	NbCarLigne;
*
* ARGUMENTS EN SORTIE :
*   T_STRING	Texte;
*
* CODE RETOUR : 
*   aucun.
*
* CONDITION D'UTILISATION
*   fonction appelee depuis le RTie
*
* FONCTION 
* centre le texte sur le bon nombre de caracteres 
*
------------------------------------------------------*/
{
  int vl_len;
  int vl_indice;
  vl_len=(va_nbcar - (int) strlen(va_str_in))/2;

  for (vl_indice=0;vl_indice< vl_len;vl_indice++) {
    va_str_out[vl_indice]=' ';
  }
  va_str_out[vl_len]='\0';
  strncat(va_str_out,va_str_in,va_nbcar);
  XZST_03EcritureTrace(XZSTC_WARNING, "prv_tabuler : LEN=%d, OUT = %s",
				vl_len,va_str_out);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   mise en page d'un message PRV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern char *prv_mise_en_page(char *Texte, int NbCarLigne)

/*
* ARGUMENTS EN ENTREE :
*   T_STRING	Texte;
*   T_REAL8	NbCarLigne;
*
* ARGUMENTS EN SORTIE :
*   T_STRING	Texte;
*
* CODE RETOUR : 
*   aucun.
*
* CONDITION D'UTILISATION
*   fonction appelee depuis le RTie
*
* FONCTION 
*    remplace le caractere de separation d'elements '\\'
*    - par un espace si l'element precedant le '\\' et
*    l'element suivant le '\\' tiennent sur une meme ligne PRV,
*    - par un retour chariot sinon.
*
------------------------------------------------------*/
{
	char	TexteTronque[50];
	int		i=0, j=0;
	int		separateur[4];	/* position des separateurs d'elements */
	int		separateur_ini[4];	/* position des separateurs d'elements */
	int		code_retour=0;
	int		nb_cr=0;
	char        message[50];
	XZIAT_destinataire      Destinataire;
        int		vl_position_tdp=0;
	char	vl_texte_init[50];
	int		vl_nb_lignes=0;
	char		*vl_elt_tdp;
	char		*vl_tronque;
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : prv_mise_en_page" ;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page Texte = %s, NbCarLigne = %d", Texte, NbCarLigne);

	/*A
	**  on repere les separateurs
	*/
	strcpy(vl_texte_init,Texte);
	separateur[0] = 0;
	separateur_ini[0] = 0;
	while(Texte[i] != '\0')
	{
		if (Texte[i] == '\n')
			nb_cr++;
		if (Texte[i] == '\\')
		{
			j++;
			separateur[j] = i+1;
			separateur_ini[j] = separateur[j];
		}
		else if (Texte[i] == '|')
			vl_position_tdp=i;
		i++;
	}

	/*recherche presence element temps de parcours*/
	vl_elt_tdp=strchr(Texte,'|');

	/*A
	**  on remplace les separateurs
	*/
		/*B ceux qui sont entre deux morceaux de texte */
	for(i = 1; i < j; i++)
	{
		if ((separateur[i+1] - separateur[i-1] - 1) <= NbCarLigne)
		{
/*REPERE1			Texte[separateur[i]-1] = ' ';*/
			Texte[separateur[i]-1] = '\n';
			separateur[i] = separateur[i-1];
		}
		else
		{
	XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: texte n = %s",Texte);
			Texte[separateur[i]-1] = '\n';
	XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: texte n = %s",Texte);
			nb_cr++;
		}
	}	
	/*B celui de la fin */
	if (j>0)
	{
		Texte[separateur[j]-1] = '\n';
		nb_cr++;
	}
	vl_nb_lignes=nb_cr-2;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: texte n = %s",Texte);

	/*si j'ai un element temps de parcours*/
	if (vl_elt_tdp!=NULL) {
	  XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: tdp = %s",vl_elt_tdp);
	  XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: nb lignes= %d",vl_nb_lignes);

	  /*suivant le nombre de lignes avant l'element temps de parcours*/
	  switch (vl_nb_lignes) {
	    case -1:
	    case 0 : sprintf(Texte,"\n%s\n",vl_elt_tdp+1);
		     nb_cr=NB_LIGNES_PRV;
		     break;

            case 1 : /*remplace le |*/
		     i=0;
		     while (i<strlen(Texte)) {
		       if (Texte[i]=='|') {
			 for (j=i; j<strlen(Texte) ;j++) 
			   Texte[j]=Texte[j+1];
			 Texte[j]='\0';
			 break;
                       }
		       else
			 i++;
                     }
		     break;

	    default: for(i = 1; i < j; i++)
		     {
			if ((separateur_ini[i+1] - separateur_ini[i-1] - 1) <= NbCarLigne)
			{
				Texte[separateur_ini[i]-1] = ' ';
				XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: txt for %s",Texte);
				separateur_ini[i] = separateur_ini[i-1];
			}
			else 
				break;
		     }
		     XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: i=%d",separateur_ini[i]);
		     vl_tronque=strncpy(vl_texte_init,Texte, (size_t) separateur_ini[i]-2);
		     vl_tronque[separateur_ini[i]-1]='\0';
		     XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: tini  %s",vl_tronque);

		     i=0;
		     while (vl_tronque[i]!='\0') {
		       /*1er element sur 2 lignes*/
		       if (vl_tronque[i]=='\n') {
			 i=-1;
			 /*on ne conserve que temps de parcours*/
			 sprintf(Texte,"\n%s",vl_elt_tdp+1);
			 break;
		       }
		       i++;
		     }
		     if (i!=-1)
		       sprintf(Texte,"%s\n%s",vl_tronque,vl_elt_tdp+1);
		     nb_cr=NB_LIGNES_PRV;
		     break;
	  }
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1, "prv_mise_en_page: texte %s",Texte);
	/*A
	**  on ajoute les retour chariot
	*/
	while (nb_cr < NB_LIGNES_PRV)
	{
		strcat(Texte, "\n");
		nb_cr++;
	}

       /*A
       **  si on a plus de 3 lignes, on previent l'admistrateur systeme
       **  et on tronque pour pouvoir comparer la proposition a l'utilisation courante
       */
       if (nb_cr > NB_LIGNES_PRV)
       {
               /* on construit le texte du message */
/*               sprintf(message,
                       "POLITIQUE PRV - la fmc %d %d engendre un texte de plus de trois lignes : %s",
                       vg_IE_NumEvt.numero, vg_IE_NumEvt.cle, Texte);
*/ 
               /* on appelle XZIA_06 */
               strcpy(Destinataire.machine, XDC_MACHINE_GECONF);
               XZST_03EcritureTrace(XZSTC_WARNING, "%s : %s",version,message);
               XZIA_06AffMsgSyst(Destinataire, message);

	       /* on tronque le texte PRV */
	       nb_cr = 0;
	       i = 0;
	       while (nb_cr < NB_LIGNES_PRV)
	       {
			TexteTronque[i] = Texte[i];
			if (Texte[i] == '\n')
				nb_cr++;
			i++;
	       }
	       TexteTronque[i] = '\0';
               XZST_03EcritureTrace(XZSTC_DEBUG1, "Le texte Tronque est : %s.", TexteTronque);
	       return(Texte);
       }
	
	/*A
	**  on renvoie au RTie le texte mis en page.
	*/
	XZST_03EcritureTrace(XZSTC_DEBUG1, "Texte = %s", Texte);
	return(Texte);

}





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    ignore la mise en page faite par les PRV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void texte_prv_de_base(char *Ligne)

/*
* ARGUMENTS EN ENTREE :
*	T_STR		Ligne;
*
* ARGUMENTS EN SORTIE :
*	T_STR		Ligne;
*
* CODE RETOUR : 
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   parcours de la chaine 
*
------------------------------------------------------*/
{
	int	i=0, j=0;
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : texte_prv_de_base" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s, Ligne = %s.",version, Ligne);

	/*A on supprime les espaces de debut de ligne */
	while ( Ligne[j] == ' ' )
		j++;
	for ( i=j; i<strlen(Ligne); i++)
		Ligne[i-j] = Ligne[i];
	Ligne[i-j] = '\0';

	/*A on supprime les espaces de fin de ligne */
        i = (int) strlen(Ligne);
	for (j=i-1;j>=0;j--) {
		if ( Ligne[j] == ' ' ) 
			Ligne[j] = '\0';
		else break;
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s, Ligne = %s.", version, Ligne);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche l'etat d'un PRV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int etat_prv(Horodate, NumPRV)
XDY_Horodate		Horodate;
XDY_Eqt			NumPRV;
/*
* ARGUMENTS EN ENTREE :
*   XDY_Horodate	Horodate;
*   XDY_Eqt		NumPRV;
*
* ARGUMENTS EN SORTIE :
*   aucun.
*
* CODE RETOUR : 
*   aucun.
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   appel de XZAT05
*
------------------------------------------------------*/
{
	XDY_Octet	dispo_texte;
	XDY_Octet	type_prv;
	char	vitesse[10];
	char	bandeau[12];
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
	XDY_Entier	action;
	XDY_Octet siteAction;
	int		i=0, cr=XDC_NOK;
	int		j=0,k=0;
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : etat_prv" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

		/*B on regarde si on a deja fait cette requete */
	while (vg_appel_t51[i].NumPRV != 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on est dans la boucle, pour i = %d", i);

		if (vg_appel_t51[i].NumPRV == NumPRV)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on a trouve pour i = %d", i);

			cr = XDC_OK;
			dispo_texte = vg_appel_t51[i].dispo_texte;
			strcpy(vitesse, vg_appel_t51[i].vitesse);
			strcpy(bandeau, vg_appel_t51[i].bandeau);
			numero_evt_U = vg_appel_t51[i].numero_evt_U;
			cle_evt_U = vg_appel_t51[i].cle_evt_U ;
			priorite_U = vg_appel_t51[i].priorite_U;
			distance_U = vg_appel_t51[i].distance_U;
			break;
		}
		i++;
	}
		/*B si ce n'est pas le cas, on appelle XZAT05 */
	if (cr != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on n'a pas trouve");

		cr = XZAT51_Utilisation_PRV(	Horodate, NumPRV,
					&dispo_texte, 
					vitesse, bandeau, 
					&numero_evt_U, &cle_evt_U, &priorite_U, &distance_U,&action, &siteAction);
		 XZST_03EcritureTrace(XZSTC_WARNING, "apres XZAT51_Utilisation_PRV");
		/*B et on enregistre le resultat */
		if ( (cr == XDC_OK) && (i < NbMaxEnr_t51) )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "on enregistre le resultat de la requete");

			vg_appel_t51[i].NumPRV = NumPRV;
			vg_appel_t51[i].dispo_texte = dispo_texte;
			strcpy(vg_appel_t51[i].vitesse, vitesse);
			strcpy(vg_appel_t51[i].bandeau, bandeau);
			vg_appel_t51[i].numero_evt_U = numero_evt_U;
			vg_appel_t51[i].cle_evt_U = cle_evt_U ;
			vg_appel_t51[i].priorite_U = priorite_U;
			vg_appel_t51[i].distance_U = distance_U;
			if ((numero_evt_U == 0) && ((priorite_U == 0)||(priorite_U == 254))) {
				vg_appel_t51[i].priorite_U = priorite_U = 254;
				vg_appel_t51[i].distance_U = distance_U = 500000;
			}
			XZST_03EcritureTrace(XZSTC_FONCTION, "Utilisation prv par evt %d prio %d dist %d",
						vg_appel_t51[i].numero_evt_U,
						vg_appel_t51[i].priorite_U ,
						vg_appel_t51[i].distance_U);
			XZST_03EcritureTrace(XZSTC_FONCTION, "vitesse %s bandeau %s",
						vg_appel_t51[i].vitesse,
						vg_appel_t51[i].bandeau);
			/*A on supprime les espaces de fin de ligne pourle picto*/
			j= (int) strlen(vitesse);
			for (k=j-1;k>=0;k--) {
			if ( vitesse[k] == ' ' ) 
				vitesse[k] = '\0';
			else break;
			}
			strcpy(vg_appel_t51[i].vitesse, vitesse);
		}
	}

	/*A
	**  traitement de la reponse
	*/
	if (cr == XDC_OK)
	{
		vg_Utilisation->DispoTexte = dispo_texte;
		strcpy(vg_Utilisation->Vitesse, vitesse);
		strcpy(vg_Utilisation->Bandeau, bandeau);
		vg_Utilisation->NumeroEvtU = numero_evt_U;
		vg_Utilisation->CleEvtU = cle_evt_U;
		vg_Utilisation->PrioriteU = priorite_U;
		vg_Utilisation->DistanceU = distance_U;
		if ((numero_evt_U == 0) && (priorite_U == 0)) {
			vg_Utilisation->PrioriteU = 254;
			vg_Utilisation->DistanceU = 500000;
		}
		XZST_03EcritureTrace(XZSTC_INFO, "Utilisation prv par evt vg_Utilisation->NumeroEvtU=%d prio vg_Utilisation->PrioriteU=%d vg_Utilisation->DistanceU=%d",vg_Utilisation->NumeroEvtU,vg_Utilisation->PrioriteU ,vg_Utilisation->DistanceU);
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s maj OK vitesse=%s, bandeau =%s" ,version,vg_Utilisation->Vitesse, vg_Utilisation->Bandeau);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"%s Etat PRV %d inconnu a la date %lf - on le considere indispo", version, NumPRV, Horodate);
		vg_Utilisation->DispoTexte = XDC_EQT_MAJEUR;
		strcpy(vg_Utilisation->Vitesse, "");
		strcpy(vg_Utilisation->Bandeau, "");
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    cree un PRV dans le RTie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen creer_prv(char *Perturbation)

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   TRUE / FALSE
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*	cree le prv s'il n'existe pas.
*	met a jour les attributs.
*
------------------------------------------------------*/
{
	/*T_IE_CLASS	prv_classe;*/
	char	prv_str[30];
	char	prv_nom[30];
	int		priorite_old;
	char	nom_objet[30];
	char	perturbation_old[100];
	/*T_IE_SLOT	slot;*/
	XZIAT_destinataire      Destinataire;
	char	message[1000];
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : creer_prv" ;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : %s, prv = %d", version,vg_Utilisation->NumPRV);

	if ( vg_Utilisation->NumPRV > vg_max_num_prv)
		vg_max_num_prv=(int)vg_Utilisation->NumPRV;
		
	strcpy(prv_nom, "prv");
	sprintf(prv_str, "%d", vg_Utilisation->NumPRV);
	strcat(prv_nom, prv_str);

	strcpy(vg_prv[vg_Utilisation->NumPRV].Nom,prv_nom);

	/*si ce PRV n'est pas encore utilise*/
	if (!vg_prv[vg_Utilisation->NumPRV].Utilise)
	{
		vg_prv[vg_Utilisation->NumPRV].Utilise=TRUE;
		vg_prv[vg_Utilisation->NumPRV].Identifiant = vg_Utilisation->NumPRV;
		vg_prv[vg_Utilisation->NumPRV].Autoroute = vg_Utilisation->AutoroutePRV;
		vg_prv[vg_Utilisation->NumPRV].PR = vg_Utilisation->PRPRV;
	        vg_prv[vg_Utilisation->NumPRV].Sens = vg_Utilisation->SensPRV;
		vg_prv[vg_Utilisation->NumPRV].Distance_C = vg_Utilisation->DistanceC;
		vg_prv[vg_Utilisation->NumPRV].Priorite_C = vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		vg_prv[vg_Utilisation->NumPRV].District = vg_Utilisation->DistrictPRV;
		vg_prv[vg_Utilisation->NumPRV].Amont = vg_Utilisation->Amont;
		vg_prv[vg_Utilisation->NumPRV].Realisable = TRUE;
		strcpy(vg_prv[vg_Utilisation->NumPRV].Bandeau, "");
		strcpy(vg_prv[vg_Utilisation->NumPRV].Vitesse, "");
		strcpy(vg_prv[vg_Utilisation->NumPRV].Nom_Pert,Perturbation);
		vg_prv[vg_Utilisation->NumPRV].Perturbation = pm_pert;

		if ((vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
						| XDC_EQT_CRITIQUE
						| XDC_EQT_DESACTIVE
						| XDC_EQT_HS
						| XDC_EQT_INHIBE))== 0 )
			vg_prv[vg_Utilisation->NumPRV].Dispo_Texte = TRUE;
		else
			vg_prv[vg_Utilisation->NumPRV].Dispo_Texte = FALSE;

		/* PNI 24/01/07 : Memorisation de l'utilisation */
		vg_prv[vg_Utilisation->NumPRV].Numero_Evt_U=vg_Utilisation->NumeroEvtU;
		vg_prv[vg_Utilisation->NumPRV].Cle_Evt_U=vg_Utilisation->CleEvtU;
		vg_prv[vg_Utilisation->NumPRV].Priorite_U=vg_Utilisation->PrioriteU;
		vg_prv[vg_Utilisation->NumPRV].Distance_U=vg_Utilisation->DistanceU;

		if (vg_Utilisation->NumeroEvtU != 0)
		{
			texte_prv_de_base(vg_Utilisation->Bandeau);
			texte_prv_de_base(vg_Utilisation->Vitesse);
			if (*(vg_Utilisation->Bandeau)!='\0')
				strcpy(vg_prv[vg_Utilisation->NumPRV].Bandeau,vg_Utilisation->Bandeau);
			else
				strcpy(vg_prv[vg_Utilisation->NumPRV].Bandeau,"");
			if (*(vg_Utilisation->Vitesse)!='\0')
				strcpy(vg_prv[vg_Utilisation->NumPRV].Vitesse,vg_Utilisation->Vitesse);
 			else /*1.2*/
				strcpy(vg_prv[vg_Utilisation->NumPRV].Vitesse,"");
			vg_prv[vg_Utilisation->NumPRV].Numero_Evt_U=vg_Utilisation->NumeroEvtU;
			vg_prv[vg_Utilisation->NumPRV].Cle_Evt_U=vg_Utilisation->CleEvtU;
			vg_prv[vg_Utilisation->NumPRV].Priorite_U=vg_Utilisation->PrioriteU;
			vg_prv[vg_Utilisation->NumPRV].Distance_U=vg_Utilisation->DistanceU;
		}
		
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE(%d)", version, vg_Utilisation->NumPRV);
		return(TRUE);
	}

	/*A
	**  sinon cela depend de la priorite de l'evt_c
	**  si l'ancien est moins prioritaire, on l'ecrase
	*/
	priorite_old = vg_prv[vg_Utilisation->NumPRV].Priorite_C;
	if (priorite_old > vg_Utilisation->PrioriteC) 
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s : ancien est moins prioritaire, on l'ecrase");
		vg_prv[vg_Utilisation->NumPRV].Priorite_C=vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		strcpy(vg_prv[vg_Utilisation->NumPRV].Nom_Pert,Perturbation);
		vg_prv[vg_Utilisation->NumPRV].Perturbation=pm_pert;
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE",version);
		return(TRUE);
	}

	/*B
	**  si les deux ont la meme priorite, la politique est ambigue :
	**  on previent l'administrateur systeme (XZIA06)
	*/
	if (priorite_old == vg_Utilisation->PrioriteC)
	{
		/* on va lire le nom de la perturbation qui utilise deja le PRV */
		/* on construit le texte du message */
		strcpy(perturbation_old,vg_prv[vg_Utilisation->NumPRV].Nom_Pert);
		sprintf(message,
			"POLITIQUE PRV AMBIGUE : la fmc %d %d engendre les perturbations \"%s\" et \"%s\", toutes deux de priorite %d, et qui cherchent toutes deux a utiliser le PRV du PR %d",
			vg_Utilisation->NumeroEvtU,vg_Utilisation->CleEvtU, perturbation_old, Perturbation,
			vg_Utilisation->PrioriteC, vg_Utilisation->PRPRV);

		/* on appelle XZIA_06 */
		strcpy(Destinataire.machine, XDC_MACHINE_GECONF);
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : %s",version,message);
		XZIA_06AffMsgSyst(Destinataire, message);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne FALSE",version);
	return(FALSE);	
}


 

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche les PRV situes entre les deux PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	les_PRV_ds_perturbation( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	T_PERT_PRV	*va_pert)


/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Longueur;
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PRTete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;	 (pour avoir une interface homogene)
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve au moins un prv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PRV par PRV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PRV, FALSE sinon.
*
------------------------------------------------------*/
{
	T_REAL8		tmp;
	XDY_Mot		AutorouteInversee;
	XDY_Octet	n=1,i=0;
	XDY_Entier	flag=FALSE;
	int		code_retour=0;
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : les_PRV_ds_perturbation" ;
	T_REAL8		vl_pr_min,vl_pr_max;
	int             prv_ok=C_PRV_TROUVE;
	T_REAL8		vl_sens;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"%s PRTete = %d, Autoroute = %d, PR = %d, Sens = %d, Horodate = %lf, District = %d, Priorite = %d, Perturbation = %s",
		 version, PR_tete, Autoroute, PR, Sens, Horodate, District, Priorite, Perturbation);

	vl_pr_min = PR_tete <= PR ? PR_tete : PR;
	vl_pr_max = PR_tete > PR ? PR_tete : PR;
	/*A
	**  tant que le nieme prv en amont existe et est dans la perturbation,
	**  on demande le suivant.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PRVT_Utilisation *) malloc(sizeof(PRVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;

		if ((Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;
                /*recherche les 1� PRV en amont*/
                XZAO576_PRV_chantier((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_prv,
                                                &flag
                                               );

	XZST_03EcritureTrace(XZSTC_FONCTION,"apres XZAO576_PRV_chantier");
	free(vg_Utilisation);

	/*A
	**  on renvoie le flag au RTie
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"OUT : %s  retourne %d", version, flag);
	return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*    recherche les PRV situes entre les deux PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	les_PRV_du_basculement( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	T_PERT_PRV	*va_pert)


/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Longueur;
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PRTete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;	 (pour avoir une interface homogene)
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve au moins un prv, FALSE sinon
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION
*	cree un objet dans la classe PRV par PRV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PRV, FALSE sinon.
*
------------------------------------------------------*/
{
	T_REAL8		tmp;
	XDY_Mot		AutorouteInversee;
	XDY_Octet	n=1,i=0;
	XDY_Entier	flag=FALSE;
	int		code_retour=0;
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : les_PRV_ds_perturbation" ;
	T_REAL8		vl_pr_min,vl_pr_max;
	int             prv_ok=C_PRV_TROUVE;
	T_REAL8		vl_sens;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"%s PRTete = %lf, Autoroute = %lf, PR = %lf, Sens = %lf, Horodate = %lf, District = %lf, Priorite = %lf, Perturbation = %s",
		 version, PR_tete, Autoroute, PR, Sens, Horodate, District, Priorite, Perturbation);

	vl_pr_min = PR_tete <= PR ? PR_tete : PR;
	vl_pr_max = PR_tete > PR ? PR_tete : PR;
	/*A
	**  tant que le nieme prv en amont existe et est dans la perturbation,
	**  on demande le suivant.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PRVT_Utilisation *) malloc(sizeof(PRVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;

		if ((Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;
                /*recherche les 1� PRV en amont*/
                XZAO577_PRV_basculement((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_prv,
                                                &flag
                                               );

	free(vg_Utilisation);

	/*A
	**  on renvoie le flag au RTie
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION,
		"OUT : %s  retourne %d", version, flag);
	return;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int prv_init_vg_requetes()

/*
* ARGUMENTS EN ENTREE :
*	aucun
*
* ARGUMENTS EN SORTIE :
*	aucun
*
* CODE RETOUR : 
*	XDC_OK 
*
* CONDITION D'UTILISATION
*       A appeler avant chaque calcul de plan d'actions
*
* FONCTION 
*   	Mise a jour des variables globales vg_appel_o15 et vg_appel_t51
*
------------------------------------------------------*/
{
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : prv_init_vg_requetes" ;

   prv_init_appels_base();

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int prv_init_appels_base()

/*
* ARGUMENTS EN ENTREE :
*       aucun
*
* ARGUMENTS EN SORTIE :
*       aucun
*
* CODE RETOUR :
*       XDC_OK
*
* CONDITION D'UTILISATION
*       A appeler avant chaque calcul de plan d'actions
*
* FONCTION
*       Mise a jour des variables globales vg_appel_o15 et vg_appel_t51
*
------------------------------------------------------*/
{ 
  int vl_i;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : prv_init_appels_base");
  for (vl_i=0; vl_i < NbMaxEnr_o15 ; vl_i++) {
    vg_appel_o15[vl_i].NumPRV=0;
  }

  for (vl_i=0; vl_i < NbMaxEnr_t51; vl_i++) {
    vg_appel_t51[vl_i].NumPRV=0;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : prv_init_appels_base");

  for (vl_i=0; vl_i < C_NB_MAX_PRV; vl_i++) {
    vg_prv[vl_i].Utilise=FALSE;
    vg_prv[vl_i].Perturbation=(T_PERT_PRV *)NULL;
    strcpy(vg_prv[vl_i].Bandeau,"");
    strcpy(vg_prv[vl_i].Vitesse,"");
  }
  vg_priorite_retenue = 0;

  return(XDC_OK);
}



/******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* prv_libere_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int prv_libere_ressources()
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"prv_libere_ressources: OUT");
  return (XDC_OK);
}



/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO419
* utilisee pour creer un PRV
*
******************************************************
* SEQUENCE D'APPEL :
* ajouter_prv)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_prv(XDY_Eqt va_numero_prv,
			XDY_District va_district_prv,
			XDY_Autoroute va_autoroute_prv,
			XDY_PR va_pr_prv,
			XDY_Sens va_sens_prv,
			XDY_District va_district,
			int va_priorite,
			XDY_Horodate va_horodate,
			char *va_perturbation,
			int va_amont,
			int *pa_resultat)
{
  T_BOOL vl_cr=FALSE;

  XZST_03EcritureTrace(XZSTC_FONCTION,
	"ajouter_prv: NumPRV = %d, DistrictPRV = %d, District=%d PRVamont=%d", 
	va_numero_prv, va_district_prv, va_district,va_amont);

  /*B
  ** on ne retient que les prv du district dont la partie texte est disponible
  */
  if    ( ((va_district_prv == va_district) || (va_district == vg_SitePrincipal)) && (etat_prv(va_horodate, va_numero_prv) == XDC_OK) )
  {

    if ( (vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
					| XDC_EQT_CRITIQUE
					| XDC_EQT_DESACTIVE
					| XDC_EQT_HS
					| XDC_EQT_INHIBE))== 0 )
    {
	vg_Utilisation->NumPRV = va_numero_prv;
	vg_Utilisation->DistrictPRV = va_district_prv;
	vg_Utilisation->AutoroutePRV = va_autoroute_prv;
	vg_Utilisation->PRPRV = va_pr_prv;
	vg_Utilisation->SensPRV = va_sens_prv;
	vg_Utilisation->PrioriteC = va_priorite;	
	vg_Utilisation->Amont  = va_amont;	

	vl_cr = creer_prv(va_perturbation);
    }
    /*B
    **  on se place en file d'attente si le PRV est indispo et pas site CI
    */
    else if (va_district_prv != XDC_CI)
	/* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
	/* Sur COFIROUTE, le site principal peut piloter les PMV */
    {
	/*XZAP21_Placer_Evt_File_Attente(	vg_IE_NumPA, va_numero_prv, XDC_EQT_PRV,
			va_priorite, va_distance_prv);*/
      vl_cr=0; /*temporaire a virer JMG*/
    }
  }
  *pa_resultat=vl_cr;
  return XDC_OK;
}





extern void le_PRV_null()
{
  return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  teste si il y a une sortie entre le PRV et le debut de la FMC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern XDY_Booleen util_prv_amont_sortie(int 	Dist_PRV,
					int 	Sens_PRV,
					int	Dist_sortie_amont,
					int	Dist_sortie_aval,
					int	Sens_fmc)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   T_BOOL      sortie entre PRV et FMC
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du Rtie
*
* FONCTION
*   renvoie TRUE si il y a une sortie entre le PRV et la FMC.
*
------------------------------------------------------*/
{
	static char *version = "@(#)drol_prv_util.c	1.14 02/21/11 : util_prv_amont_sortie" ;

	/*A
	**  si le sens de la FMC est inconnu et si le PRV est en sens sud, le PR de tete devient le PR de queue
	*/
	if ( (XDY_Sens) Sens_fmc == XDC_SENS_INCONNU && (XDY_Sens) Sens_PRV == XDC_SENS_SUD)
		Dist_sortie_amont = Dist_sortie_aval;

	/*A
	**  si Dist_sortie_amont est nulle c'est qu'il n'y a pas de sortie amont ( REPERE1 ) alors on retourne FALSE
	*/
	if ( Dist_sortie_amont == 0 ) {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne FALSE car pas de sortie amont (dist=0) ",version);
		return(FALSE);
		/*Dist_sortie_amont = Dist_sortie_aval;*/
	}
	
	/*A
	** on compare les distances
	*/
	if ( Dist_PRV > Dist_sortie_amont) {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE",version);
		return(TRUE);
	}
	else {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne FALSE",version);
		return(FALSE);
	}
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  teste si un PRV est en amont d'une FMC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen util_prv_amont_fmc(int	PR_PRV,
					int	Sens_PRV,
					int	PR_Queue,
					int	PR_Tete,
					int	Sens_fmc,
					int	Autoroute_inversee)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   T_BOOL      PMv amont FMC
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*  
*
* FONCTION
*   renvoie TRUE si le PRV est en amont de la FMC, FALSE sinon.
*
------------------------------------------------------*/
{
XZST_03EcritureTrace(XZSTC_FONCTION, "util_prv_amont_fmc %d,%d,%d,%d,%d,%d",
			PR_PRV,
			Sens_PRV,
			PR_Queue,
			PR_Tete,
			Sens_fmc,
			Autoroute_inversee);
	  /*A
	  **  si le sens de la FMC est inconnu et si le PRV est en sens sud, le PR de tete devient le PR de queue
	  */
	  if ( (XDY_Sens) Sens_fmc == XDC_SENS_INCONNU && (XDY_Sens) Sens_PRV == XDC_SENS_SUD)
		  PR_Queue = PR_Tete;

	/*A
	** le sens sud est le sens des PR croissant (sauf si autoroute inversee)
	*/
	if ( (((XDY_Sens) Sens_PRV == XDC_SENS_SUD) && (PR_PRV*Autoroute_inversee < PR_Queue*Autoroute_inversee))
		|| (((XDY_Sens) Sens_PRV == XDC_SENS_NORD) && (PR_PRV*Autoroute_inversee > PR_Queue*Autoroute_inversee))) {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : util_prv_amont_fmc retourne TRUE");
		return(TRUE);
	}
	else {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : util_prv_amont_fmc retourne FALSE");
		return(FALSE);
	}
}


XDY_Entier vitesseTextToVitesse(XDY_VitesseRegulTxt v_vitesseText)
{
        XDY_Entier vl_vitesse=0;

        if(!strncmp(v_vitesseText , "L50",3))
                vl_vitesse = 50;
        else if(!strncmp(v_vitesseText , "L70",3))
                vl_vitesse = 70;
        else if(!strncmp(v_vitesseText , "L90",3))
                vl_vitesse = 90;
        else if(!strncmp(v_vitesseText , "L110",4))
                vl_vitesse = 110;
        else if(!strncmp(v_vitesseText , "L130",4))
                vl_vitesse = 130;

        return vl_vitesse;
}

void vitesseToVitesseText(XDY_Entier va_vitesseIn, XDY_VitesseRegulTxt va_vitesseOut)
{
        XDY_VitesseRegulTxt vl_vitesseText="";

        if(va_vitesseIn == 50)
                strcpy(va_vitesseOut, "L50");
        else if(va_vitesseIn == 70)
                strcpy(va_vitesseOut , "L70");
        else if(va_vitesseIn == 90)
                strcpy(va_vitesseOut , "L90");
        else if(va_vitesseIn == 110)
                strcpy(va_vitesseOut , "L110");
        else if(va_vitesseIn == 130)
                strcpy(va_vitesseOut , "L130");
}

