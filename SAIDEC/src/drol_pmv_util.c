/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
------------------------------------------------------
* MODULE DROL_PMV * FICHIER drol_pmv_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf drol_pmv_util.h 
*
*   NB : toute modification de l'interface des fonctions
*	de recherche des PMV doit etre repercutee sur
*	l'editeur de connaissances (modele pmv_eqpt.aw)
*
------------------------------------------------------
* HISTORIQUE :
*
* IT	08 Sep 1994	: Creation
* JMG	02/04/04	: gestion ligne vide sur recup utilisation courante 1.2
* JMG	17/06/04	: trace appel xzat05 + maj lignes PMV a vide dans init_appel_base 1.3
* PCL   08/03/05	: bug "=> AIX/NICE V" au lieu de "=> AIX/NICE \n VEHICULE ARRETE" REPERE1  1.4
* PNI   24/01/06 	: Correction priorit� PMV 1.5 1.6
* PNI   17/07/07	: Recherche PMV dans les 2 sens pour FMC basculement travaux actif DEM658 v1.8
* PNI	06/08/07	: Mise � jour DistanceU en cas de PMV inutilis� pour prise en compte des distances pour ne pas piloter en SAPIENS DEM655 v1.9
* PCL	13/08/08	: bug util_pmv_amont_sortie si la distance entre le PMV et la sortie est plus petite que la distance entre le pmv et l'evt    
			  ce n'est pas pour autant que la sortie existe (cas ou cette distance = 0) REPERE1 FI 125 127 137 DEM   v1.10
* JPL	14/01/11	: Migration architecture HP ia64 (DEM 971) : Variables "version"; IDEM PRECEDENTE  1.11
* JPL	17/01/11	: Migration HP ia64 (DEM 971) : types parametres vs. profil fonctions (XZAO419,XZAO424 a 426)  1.12
* JPL	17/01/11	: Migration architecture HP ia64 (DEM 971) : retour valeur manquant pour (pmva_mise_en_page)  1.13
* JPL	17/01/11	: Migration HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=,-)  1.14
* JMG	24/05/12	 : linux 1.15
* JPL	12/07/12	: Utilisation des noms de sortie "usager" et indicateurs de bifurcations (DEM 1033)  1.16
* PNI	12/02/13	: Integration PMV 501 modif util_pmv_amont_fmc v1.17 FI 1821
* PNI	12/05/15	: suppression de l'inversion des sortie pour sens 0 pour util_pmv_amont_sortie FI 3301 1.18
* JMG	23/11/16	: SAGA 1.19
* JMG	20/06/17	: convergence 1.20
* PNI	20/12/217	: correctif init de vg_appel_t05[vl_i].NumPMV=0 1.21
* JPL	09/02/18	: Migration Linux 64 bits (DEM 1274) : Suppression de declarations inutiles  1.22
* JMG	12/08/18	: ajout type_pmv . suppression SAGA DEM1306 1.23
* PNI	28/01/21	; Correctif pmv_tabuler pour ligne > nb_car SAE-230 1.23
* LCL	04/10/23 	: Adaptations CNA DEM-483 : lecture site principal en base
* JPL	07/11/23	: Correction sens de recherche 1er PMV amont pour un Basculement pour travaux (SAE-541)  1.24
* JPL	08/11/23	: Basculement pour travaux: ignorer regle bouchon mobile en sens inverse (SAE-541)  1.25
* GGY	29/11/23	: Augmentation de vg_type_pmv (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include "drol_pmv_util.h"

#include "xzao17.h"
#include "xzat150.h"
#include "xzao419.h"
#include "xzao424.h"
#include "xzao425.h"
#include "xzao426.h"
#include "xzao791.h"
#include "xzao819.h"
#include "xzao824.h"
#include "xzao825.h"
#include "xzao826.h"


/* definitions de constantes */
#define CM_VERSION_SOURCE	"drol_pmv_util.c 1.25"

#define DISTANCE_INFINIE	1000000
#define NB_LIGNES_PMV		3

#define CLASSE_PERTURBATION	"perturbation"
#define ATT_PERTURBATION_TYPE	"type"
#define ATT_PMV_PERTURBATION	"perturbation"
#define NbMaxEnr_o15		6
#define NbMaxEnr_t05		6
#define CLASSE_LOCALISATION        "localisation"

#define C_PMV_TROUVE		0
#define C_PB_PMV		1
#define C_PLUS_DE_PMV		2

/* definitions de types locaux */
typedef struct {
	XDY_Octet	DispoTexte;
	XDY_Octet	DispoPicto;
	T_STRING	L1;		/* texte de la premiere ligne */
	T_STRING	L2;		/* texte de la deuxieme ligne */
	T_STRING	L3;		/* texte de la troisieme ligne */
	T_STRING	A1;		/* texte du premier alternat */
	T_STRING	A2;		/* texte du deuxieme alternat */
	T_STRING	A3;		/* texte du troisieme alternat */
	T_STRING	Picto;
	T_STRING	Bandeau;
	XDY_Entier	NumeroEvtU;
	XDY_Octet	CleEvtU;
	XDY_Octet	PrioriteU;
	XDY_Entier	DistanceU;
	XDY_Octet	PrioriteC;
	T_REAL8		DistanceC;
	XDY_Eqt		NumPMV;	
	XDY_Autoroute	AutoroutePMV;	
	XDY_PR		PRPMV;	
	XDY_Sens	SensPMV;	
	XDY_District	DistrictPMV;	
	XDY_Booleen	PresenceBifurcation;
	T_STRING	DirectionPMV;
	XDY_Booleen	PMV_SAGA;
	XDY_Octet	Type_SAGA;
	XDY_Octet	Type;
} PMVT_Utilisation;

typedef struct {
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Mot		Numero;
	XDY_Eqt		NumPMV;
	XDY_District	DistrictPMV;
	XDY_Autoroute	AutoroutePMV;
	XDY_PR		PRPMV;
	XDY_Sens	SensPMV;
	XDY_Entier	DistancePMV;
} PMVT_xzao15;				/* structure d'appel de XZAO15 */

typedef struct {
	XDY_Eqt		NumPMV;
	XDY_Octet	dispo_texte;
	XDY_Octet	dispo_picto;
	XDY_Octet	type_pmv;
	char	l1[20];
	char	l2[20];
	char	l3[20];
	char	a1[20];
	char	a2[20];
	char	a3[20];
	XDY_Booleen	c1;
	XDY_Booleen	c2;
	XDY_Booleen	c3;
	XDY_Booleen	flash;
	T_STRING	picto;
	T_STRING	bandeau;
	XDY_Octet	numpicto;
	XDY_Booleen	c;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
} PMVT_xzat05;				/* structure d'appel de XZAT05 */


/*definition de variables globales*/
static	T_Liste_Influ		*pg_zones=NULL;
static  T_Liste_Influ		*pg_zone_courante=NULL;
static  T_Liste_Influ		*pg_zone_fin_courante=NULL;
static	T_PERT			*pm_pert=NULL;

/* definition de macro locales */


/* declaration de variables locales */
PMVT_Utilisation	*vg_Utilisation;
PMVT_xzao15		vg_appel_o15[NbMaxEnr_o15];
PMVT_xzao15		vg_appel_o15_saga[NbMaxEnr_o15];
PMVT_xzat05		vg_appel_t05[NbMaxEnr_t05];
PMVT_xzat05		vg_appel_t05_saga[NbMaxEnr_t05];
int 			vg_max_num_pmv=0;
T_PMV			vg_pmv[C_NB_MAX_PMV];
T_PMV			vg_pmv_saga[C_NB_MAX_PMV];
int			vg_priorite_retenue;
XZAOT_TypePMV    vg_type_pmv[25];

/* declaration de fonctions internes */
static int etat_pmv();
static int etat_pmv_saga();
static XDY_Booleen creer_pmv( char *, XDY_Booleen);

static int ajouter_pmv(XDY_Eqt ,
			XDY_District ,
			XDY_Autoroute ,
			XDY_PR ,
			int ,
			XDY_Sens ,
			char *,
			int ,
			XDY_District ,
			int ,
			XDY_Horodate ,
			char *,
			int	*);

static int ajouter_pmv_saga(XDY_Eqt ,
			XDY_District ,
			XDY_Autoroute ,
			XDY_PR ,
			int ,
			XDY_Sens ,
			char *,
			int ,
			XDY_District ,
			int ,
			XDY_Horodate ,
			char *,
			int	*);


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   verification de la longueur des lignes PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen	pmv_ligne_trop_longue(char *Texte, int NbCarLigne)

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
	static char *version = CM_VERSION_SOURCE ": pmv_ligne_trop_longue" ;

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
*   formattage d'une ligne PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void pmv_tabuler_ligne(char *va_str_in, int va_nbcar, char *va_str_out)

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
if ( vl_len>0)
{
  for (vl_indice=0;vl_indice< vl_len;vl_indice++) {
    va_str_out[vl_indice]=' ';
  }
  va_str_out[vl_len]='\0';
  strncat(va_str_out,va_str_in,va_nbcar);
 }
 else
 {
 va_str_out[va_nbcar]='\0';
 strncpy(va_str_out,va_str_in,va_nbcar);
 }
  XZST_03EcritureTrace(XZSTC_WARNING, "pmv_tabuler : LEN=%d, OUT = %s",
				vl_len,va_str_out);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   mise en page d'un message PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern char *pmv_mise_en_page(char *Texte, int NbCarLigne)

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
*    l'element suivant le '\\' tiennent sur une meme ligne PMV,
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
	static char *version = CM_VERSION_SOURCE ": pmv_mise_en_page" ;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page Texte = %s, NbCarLigne = %d", Texte, NbCarLigne);

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
	XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: texte n = %s",Texte);
			Texte[separateur[i]-1] = '\n';
	XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: texte n = %s",Texte);
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

	XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: texte n = %s",Texte);

	/*si j'ai un element temps de parcours*/
	if (vl_elt_tdp!=NULL) {
	  XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: tdp = %s",vl_elt_tdp);
	  XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: nb lignes= %d",vl_nb_lignes);

	  /*suivant le nombre de lignes avant l'element temps de parcours*/
	  switch (vl_nb_lignes) {
	    case -1:
	    case 0 : sprintf(Texte,"\n%s\n",vl_elt_tdp+1);
		     nb_cr=NB_LIGNES_PMV;
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
				XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: txt for %s",Texte);
				separateur_ini[i] = separateur_ini[i-1];
			}
			else 
				break;
		     }
		     XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: i=%d",separateur_ini[i]);
		     vl_tronque=strncpy(vl_texte_init,Texte, (size_t) separateur_ini[i]-2);
		     vl_tronque[separateur_ini[i]-1]='\0';
		     XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: tini  %s",vl_tronque);

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
		     nb_cr=NB_LIGNES_PMV;
		     break;
	  }
	}

	XZST_03EcritureTrace(XZSTC_DEBUG1, "pmv_mise_en_page: texte %s",Texte);
	/*A
	**  on ajoute les retour chariot
	*/
	while (nb_cr < NB_LIGNES_PMV)
	{
		strcat(Texte, "\n");
		nb_cr++;
	}

       /*A
       **  si on a plus de 3 lignes, on previent l'admistrateur systeme
       **  et on tronque pour pouvoir comparer la proposition a l'utilisation courante
       */
       if (nb_cr > NB_LIGNES_PMV)
       {
               /* on construit le texte du message */
/*               sprintf(message,
                       "POLITIQUE PMV - la fmc %d %d engendre un texte de plus de trois lignes : %s",
                       vg_IE_NumEvt.numero, vg_IE_NumEvt.cle, Texte);
*/ 
               /* on appelle XZIA_06 */
               strcpy(Destinataire.machine, XDC_MACHINE_GECONF);
               XZST_03EcritureTrace(XZSTC_WARNING, "%s : %s",version,message);
               XZIA_06AffMsgSyst(Destinataire, message);

	       /* on tronque le texte PMV */
	       nb_cr = 0;
	       i = 0;
	       while (nb_cr < NB_LIGNES_PMV)
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
*    ignore la mise en page faite par les PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void texte_pmv_de_base(char *Ligne)

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
	static char *version = CM_VERSION_SOURCE ": texte_pmv_de_base" ;

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
*    recherche l'etat d'un PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int etat_pmv(Horodate, NumPMV)
XDY_Horodate		Horodate;
XDY_Eqt			NumPMV;
/*
* ARGUMENTS EN ENTREE :
*   XDY_Horodate	Horodate;
*   XDY_Eqt		NumPMV;
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
	XDY_Octet	dispo_picto;
	XDY_Octet	type_pmv;
	char	l1[30];
	char	l2[30];
	char	l3[30];
	char	l4[30];
	char	l5[30];
	char	a1[30];
	char	a2[30];
	char	a3[30];
	char	a4[30];
	char	a5[30];
	XDY_Booleen	c1;
	XDY_Booleen	c2;
	XDY_Booleen	c3;
	XDY_Booleen	flash;
	char	picto[30];
	char	bandeau[30];
	XDY_Booleen	cligno;
	XDY_Mot	lumino, barriere, scenario;
	XDY_Octet	numpicto;
	XDY_Booleen	c;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
/*	T_IE_SLOT	slot;*/
	int		i=0, cr=XDC_NOK;
	int		j=0,k=0;
	static char *version = CM_VERSION_SOURCE ": etat_pmv" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

		/*B on regarde si on a deja fait cette requete */
	while (vg_appel_t05[i].NumPMV != 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on est dans la boucle, pour i = %d", i);

		if (vg_appel_t05[i].NumPMV == NumPMV)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on a trouve pour i = %d", i);

			cr = XDC_OK;
			dispo_texte = vg_appel_t05[i].dispo_texte;
			dispo_picto = vg_appel_t05[i].dispo_picto;
			type_pmv = vg_appel_t05[i].type_pmv;
			strcpy(l1, vg_appel_t05[i].l1);
			strcpy(l2, vg_appel_t05[i].l2);
			strcpy(l3, vg_appel_t05[i].l3);
			strcpy(a1, vg_appel_t05[i].a1);
			strcpy(a2, vg_appel_t05[i].a2);
			strcpy(a3, vg_appel_t05[i].a3);
			c1 = vg_appel_t05[i].c1;
			c2 = vg_appel_t05[i].c2;
			c3 = vg_appel_t05[i].c3;
			flash = vg_appel_t05[i].flash;
			strcpy(picto, vg_appel_t05[i].picto);
			strcpy(bandeau, vg_appel_t05[i].bandeau);
			numpicto = vg_appel_t05[i].numpicto;
			c = vg_appel_t05[i].c;
			numero_evt_U = vg_appel_t05[i].numero_evt_U;
			cle_evt_U = vg_appel_t05[i].cle_evt_U ;
			priorite_U = vg_appel_t05[i].priorite_U;
			distance_U = vg_appel_t05[i].distance_U;
			break;
		}
		i++;
	}
		/*B si ce n'est pas le cas, on appelle XZAT05 */
	if (cr != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on n'a pas trouve");

		  cr = XZAT05_Utilisation_PMV(	Horodate, NumPMV,
					&dispo_texte, &dispo_picto, &type_pmv,
					l1, l2, l3, a1, a2, a3, &c1, &c2, &c3, &flash,
					picto, bandeau, &numpicto, &c,
					&numero_evt_U, &cle_evt_U, &priorite_U, &distance_U);
		/*B et on enregistre le resultat */
		if ( (cr == XDC_OK) && (i < NbMaxEnr_t05) )
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on enregistre le resultat de la requete");

			vg_appel_t05[i].NumPMV = NumPMV;
			vg_appel_t05[i].dispo_texte = dispo_texte;
			vg_appel_t05[i].dispo_picto = dispo_picto;
			vg_appel_t05[i].type_pmv = type_pmv;
			strcpy(vg_appel_t05[i].l1, l1);
			strcpy(vg_appel_t05[i].l2, l2);
			strcpy(vg_appel_t05[i].l3, l3);
			strcpy(vg_appel_t05[i].a1, a1);
			strcpy(vg_appel_t05[i].a2, a2);
			strcpy(vg_appel_t05[i].a3, a3);
			vg_appel_t05[i].c1 = c1;
			vg_appel_t05[i].c2 = c2;
			vg_appel_t05[i].c3 = c3;
			vg_appel_t05[i].flash = flash;
			strcpy(vg_appel_t05[i].picto, picto);
			strcpy(vg_appel_t05[i].bandeau, bandeau);
			vg_appel_t05[i].numpicto = numpicto;
			vg_appel_t05[i].c = c;
			vg_appel_t05[i].numero_evt_U = numero_evt_U;
			vg_appel_t05[i].cle_evt_U = cle_evt_U ;
			vg_appel_t05[i].priorite_U = priorite_U;
			vg_appel_t05[i].distance_U = distance_U;
			if ((numero_evt_U == 0) && ((priorite_U == 0)||(priorite_U == 254))) {
				vg_appel_t05[i].priorite_U = priorite_U = 254;
				vg_appel_t05[i].distance_U = distance_U = 500000;
			}
			XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmv par evt %d prio %d dist %d",vg_appel_t05[i].numero_evt_U,vg_appel_t05[i].priorite_U ,vg_appel_t05[i].distance_U);
			/*A on supprime les espaces de fin de ligne pourle picto*/
			j= (int) strlen(picto);
			for (k=j-1;k>=0;k--) {
			if ( picto[k] == ' ' ) 
				picto[k] = '\0';
			else break;
			}
			strcpy(vg_appel_t05[i].picto, picto);
		}
	}

	/*A
	**  traitement de la reponse
	*/
	if (cr == XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s avant maj Utilisation : ligne1=%s / ligne2=%s / ligne3=%s ", version,l1,l2,l3);
		vg_Utilisation->DispoTexte = dispo_texte;
		vg_Utilisation->DispoPicto = dispo_picto;
		vg_Utilisation->Type = type_pmv;
		strcpy(vg_Utilisation->L1, l1);
		strcpy(vg_Utilisation->L2, l2);
		strcpy(vg_Utilisation->L3, l3);
		strcpy(vg_Utilisation->A1, a1);
		strcpy(vg_Utilisation->A2, a2);
		strcpy(vg_Utilisation->A3, a3);
		strcpy(vg_Utilisation->Picto, picto);
		strcpy(vg_Utilisation->Bandeau, bandeau);
		vg_Utilisation->NumeroEvtU = numero_evt_U;
		vg_Utilisation->CleEvtU = cle_evt_U;
		vg_Utilisation->PrioriteU = priorite_U;
		vg_Utilisation->DistanceU = distance_U;
		if ((numero_evt_U == 0) && (priorite_U == 0)) {
			vg_Utilisation->PrioriteU = 254;
			vg_Utilisation->DistanceU = 500000;
		}
		XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmv par evt vg_Utilisation->NumeroEvtU=%d prio vg_Utilisation->PrioriteU=%d vg_Utilisation->DistanceU=%d",vg_Utilisation->NumeroEvtU,vg_Utilisation->PrioriteU ,vg_Utilisation->DistanceU);
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s maj OK",version);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"%s Etat PMV %d inconnu a la date %lf - on le considere indispo", version, NumPMV, Horodate);
		vg_Utilisation->DispoTexte = XDC_EQT_MAJEUR;
		vg_Utilisation->DispoPicto = XDC_EQT_MAJEUR;
		strcpy(vg_Utilisation->L1, "");
		strcpy(vg_Utilisation->L2, "");
		strcpy(vg_Utilisation->L3, "");
		strcpy(vg_Utilisation->A1, "");
		strcpy(vg_Utilisation->A2, "");
		strcpy(vg_Utilisation->A3, "");
		strcpy(vg_Utilisation->Picto, XDC_PICTO_PASPICTO);
		strcpy(vg_Utilisation->Bandeau, "");
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    cree un PMV dans le RTie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static XDY_Booleen creer_pmv(char *Perturbation, XDY_Booleen va_SAGA)

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
*	cree le pmv s'il n'existe pas.
*	met a jour les attributs.
*
------------------------------------------------------*/
{
	/*T_IE_CLASS	pmv_classe;*/
	char	pmv_str[30];
	char	pmv_nom[30];
	int		priorite_old;
	char	nom_objet[30];
	char	perturbation_old[100];
	/*T_IE_SLOT	slot;*/
	XZIAT_destinataire      Destinataire;
	char	message[1000];
	static char *version = CM_VERSION_SOURCE ": creer_pmv" ;
	T_PMV vl_pmv;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : %s, pmv = %d, saga=%d", version,vg_Utilisation->NumPMV,va_SAGA);

	if ( vg_Utilisation->NumPMV > vg_max_num_pmv)
		vg_max_num_pmv=(int)vg_Utilisation->NumPMV;
		
	strcpy(pmv_nom, "pmv");
	sprintf(pmv_str, "%d", vg_Utilisation->NumPMV);
	strcat(pmv_nom, pmv_str);

	if (va_SAGA)
	  vl_pmv = vg_pmv_saga[vg_Utilisation->NumPMV];
	else
	  vl_pmv = vg_pmv[vg_Utilisation->NumPMV];

	strcpy(vl_pmv.Nom,pmv_nom);

	/*si ce PMV n'est pas encore utilise*/
	if (!vl_pmv.Utilise)
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "creer_pmv : %s PMV n'est pas encore utilise (%d)", version, vg_Utilisation->NumPMV);

		vl_pmv.Utilise=TRUE;
		vl_pmv.Identifiant = vg_Utilisation->NumPMV;
		vl_pmv.Autoroute = vg_Utilisation->AutoroutePMV;
		vl_pmv.PR = vg_Utilisation->PRPMV;
	        vl_pmv.Sens = vg_Utilisation->SensPMV;
		vl_pmv.Distance_C = vg_Utilisation->DistanceC;
		vl_pmv.Priorite_C = vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		vl_pmv.District = vg_Utilisation->DistrictPMV;
		vl_pmv.Realisable = TRUE;
		vl_pmv.Type = vg_Utilisation->Type;
		vl_pmv.Presence_Bifurcation = vg_Utilisation->PresenceBifurcation;
		strcpy(vl_pmv.Direction , vg_Utilisation->DirectionPMV);
		strcpy(vl_pmv.Bandeau, "");
		strcpy(vl_pmv.Nom_Pert,Perturbation);
		vl_pmv.Perturbation = pm_pert;
		vl_pmv.Destination1 = 0;
		vl_pmv.Destination2 = 0;
		vl_pmv.Type_SAGA=vg_Utilisation->Type_SAGA;
		vl_pmv.PMV_SAGA=vg_Utilisation->PMV_SAGA;
		if ((vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
						| XDC_EQT_CRITIQUE
						| XDC_EQT_DESACTIVE
						| XDC_EQT_HS
						| XDC_EQT_INHIBE))== 0 )
			vl_pmv.Dispo_Texte = TRUE;
		else
			vl_pmv.Dispo_Texte = FALSE;

		if ((vg_Utilisation->DispoPicto & ( XDC_EQT_MAJEUR
						| XDC_EQT_CRITIQUE
						| XDC_EQT_DESACTIVE
						| XDC_EQT_HS
						| XDC_EQT_INHIBE))== 0 )
			vl_pmv.Dispo_Picto = TRUE;
		else
			vl_pmv.Dispo_Picto = FALSE;

		/* PNI 24/01/07 : Memorisation de l'utilisation */
		vl_pmv.Numero_Evt_U=vg_Utilisation->NumeroEvtU;
		vl_pmv.Cle_Evt_U=vg_Utilisation->CleEvtU;
		vl_pmv.Priorite_U=vg_Utilisation->PrioriteU;
		vl_pmv.Distance_U=vg_Utilisation->DistanceU;

		if (vg_Utilisation->NumeroEvtU != 0)
		{
			texte_pmv_de_base(vg_Utilisation->L1);
			texte_pmv_de_base(vg_Utilisation->L2);
			texte_pmv_de_base(vg_Utilisation->L3);
			texte_pmv_de_base(vg_Utilisation->A1);
			texte_pmv_de_base(vg_Utilisation->A2);
			texte_pmv_de_base(vg_Utilisation->A3);
			texte_pmv_de_base(vg_Utilisation->Bandeau);
			if (*(vg_Utilisation->L1)!='\0')
				strcpy(vl_pmv.Ligne1,vg_Utilisation->L1);
 			else /*1.2*/
				strcpy(vl_pmv.Ligne1,"");
			if (*(vg_Utilisation->L2)!='\0')
				strcpy(vl_pmv.Ligne2,vg_Utilisation->L2);
 			else /*1.2*/
				strcpy(vl_pmv.Ligne2,"");
			if (*(vg_Utilisation->L3)!='\0')
				strcpy(vl_pmv.Ligne3,vg_Utilisation->L3);
 			else /*1.2*/
				strcpy(vl_pmv.Ligne3,"");
			if (*(vg_Utilisation->A1)!='\0')
				strcpy(vl_pmv.Alternat1,vg_Utilisation->A1);
 			else /*1.2*/
				strcpy(vl_pmv.Alternat1,"");
			if (*(vg_Utilisation->A2)!='\0')
				strcpy(vl_pmv.Alternat2,vg_Utilisation->A2);
 			else /*1.2*/
				strcpy(vl_pmv.Alternat2,"");
			if (*(vg_Utilisation->A3)!='\0')
				strcpy(vl_pmv.Alternat3,vg_Utilisation->A3);
 			else /*1.2*/
				strcpy(vl_pmv.Alternat3,"");
			if (*(vg_Utilisation->Bandeau)!='\0')
				strcpy(vl_pmv.Bandeau,vg_Utilisation->Bandeau);
			else
				strcpy(vl_pmv.Bandeau,"");
			if (*(vg_Utilisation->Picto)!='\0')
				strcpy(vl_pmv.Picto,vg_Utilisation->Picto);
 			else /*1.2*/
				strcpy(vl_pmv.Picto,"");
			vl_pmv.Numero_Evt_U=vg_Utilisation->NumeroEvtU;
			vl_pmv.Cle_Evt_U=vg_Utilisation->CleEvtU;
			vl_pmv.Priorite_U=vg_Utilisation->PrioriteU;
			vl_pmv.Distance_U=vg_Utilisation->DistanceU;
		}
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne (%d)", version, vg_Utilisation->NumPMV);
	/*	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE(%d)", version, vg_Utilisation->NumPMV);
		return(TRUE);*/
	}

	if (va_SAGA)
	  vg_pmv_saga[vg_Utilisation->NumPMV] = vl_pmv;
	else
	  vg_pmv[vg_Utilisation->NumPMV] = vl_pmv;
	/*A
	**  sinon cela depend de la priorite de l'evt_c
	**  si l'ancien est moins prioritaire, on l'ecrase
	*/
	priorite_old = vl_pmv.Priorite_C;
	if (priorite_old > vg_Utilisation->PrioriteC) 
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ancien est moins prioritaire, on l'ecrase");
		vl_pmv.Priorite_C=vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		strcpy(vl_pmv.Nom_Pert,Perturbation);
		vl_pmv.Perturbation=pm_pert;
		XZST_03EcritureTrace(XZSTC_FONCTION, "clignotement retenu %d (utilise = %d)",pm_pert->Clignotement,
						vl_pmv.Utilise);
		vl_pmv.Destination1 = 0;
		vl_pmv.Destination2 = 0;
		if (va_SAGA)
		  vg_pmv_saga[vg_Utilisation->NumPMV] = vl_pmv;
		else
		  vg_pmv[vg_Utilisation->NumPMV] = vl_pmv;
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE",version);
		return(TRUE);
	}

	/*B
	**  si les deux ont la meme priorite, la politique est ambigue :
	**  on previent l'administrateur systeme (XZIA06)
	*/
	if ((priorite_old == vg_Utilisation->PrioriteC) && (vg_Utilisation->NumeroEvtU !=0) &&
	   (strlen(perturbation_old)>0))
	{
		/* on va lire le nom de la perturbation qui utilise deja le PMV */
		/* on construit le texte du message */
		if (va_SAGA)
		strcpy(perturbation_old,vg_pmv_saga[vg_Utilisation->NumPMV].Nom_Pert);
		else
		strcpy(perturbation_old,vg_pmv[vg_Utilisation->NumPMV].Nom_Pert);
		sprintf(message,
			"POLITIQUE PMV AMBIGUE : la fmc %d %d engendre les perturbations \"%s\" et \"%s\", toutes deux de priorite %d, et qui cherchent toutes deux a utiliser le PMV du PR %d",
			vg_Utilisation->NumeroEvtU,vg_Utilisation->CleEvtU, perturbation_old, Perturbation,
			vg_Utilisation->PrioriteC, vg_Utilisation->PRPMV);

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
*    recherche tous les PMV en amont et eloignes d'au plus DMax (metres) 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	tous_les_PMV_amont( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)

/*
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve au moins un PMV, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis le RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PMV, FALSE sinon
*
------------------------------------------------------*/
{
	XDY_Octet	n=1;
	XDY_Entier	flag=FALSE;
	int		code_retour=0;
	int		vl_sens,vl_sens_i;
	int	vl_DMax,vl_DMax_bifurc;
	static char *version = CM_VERSION_SOURCE ": tous_les_PMV_amont" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	// XZST_03EcritureTrace(XZSTC_DEBUG1,
	//	"%s Autoroute = %lf, PR = %lf, PRTete = %lf, Sens = %lf, Horodate = %lf, District = %lf, Priorite = %lf, Perturbation = %s, DMax = %lf, DMax_bifurc = %lf",
	//	 version,Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, Perturbation, DMax,DMax_bifurc);

	/*A
	**  tant que le nieme pmv en amont existe et est a moins de Dmax,
	**  on demande le suivant.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation = FALSE;
	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;
	if ( ((XDY_Sens) Sens) != XDC_SENS_INCONNU)
	{
	   if ( (Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD) ||
		    (Sens==XDC_SENS_SORTIE_SUD) ||
		    (Sens==XDC_SENS_ENTREE_SUD) ||
		    (Sens==XDC_SENS_SUD))
		{
		if ((Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;
                /*recherche les 1� PMV en amont*/
                XZAO424_Rechercher_tous_les_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                XZAO824_Rechercher_tous_les_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );
		}
		else 
		{ 	/* Traitement PMV dans les 2 sens */
			vl_sens=XDC_SENS_SUD-10;
		 	if ((vl_sens==XDC_SENS_SORTIE_NORD) ||
		    	(vl_sens==XDC_SENS_ENTREE_NORD) ||
		    	(vl_sens==XDC_SENS_NORD))
				{
		     		vl_sens=XDC_SENS_NORD;
		     		vl_sens_i=XDC_SENS_SUD;
				}
			else
		     		{
				vl_sens=XDC_SENS_SUD;
		     		vl_sens_i=XDC_SENS_NORD;
				}
			
                	XZAO424_Rechercher_tous_les_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                	XZAO824_Rechercher_tous_les_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );
                	XZAO424_Rechercher_tous_les_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens_i,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                	XZAO824_Rechercher_tous_les_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens_i,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );
			

		}
	}
	else
	{
                /*recherche les 1� PMV en amont en sens NORD*/
                XZAO424_Rechercher_tous_les_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                XZAO824_Rechercher_tous_les_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );

                /*recherche les 1� PMV en amont en sens SUD*/
                XZAO424_Rechercher_tous_les_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_PR)vl_DMax,
                                                (XDY_PR)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                XZAO824_Rechercher_tous_les_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_PR)vl_DMax,
                                                (XDY_PR)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );
	}
	free(vg_Utilisation);

	/*A
	**  on renvoie le flag au RTie
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne %d", version, flag);
	return;
}


 


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche le nieme PMV en amont et eloigne d'au plus DMax (metres) 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	le_premier_PMV_amont( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)

/*
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve un pmv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne le nombre de candidats.	
*
------------------------------------------------------*/
{

	XDY_Entier	cr=FALSE;
	int		pmv_ok=C_PB_PMV;
	XDY_Mot		n;
	int		code_retour=0;
	PMVT_Utilisation vl_le_premier;
	PMVT_Utilisation vl_le_second;
	XDY_Booleen		vl_chercher_en_sens_inverse = XDC_VRAI;
	int		vl_sens, vl_sens_i;
	int	vl_DMax,vl_DMax_bifurc;
	static char *version = CM_VERSION_SOURCE ": le_premier_PMV_amont" ;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"Autoroute = %d, PR = %d, PRTete = %d, Sens = %d, Horodate = %lf, District = %d, Priorite = %d, Perturbation = %s, DMax = %d, aut amont = %d, aut aval=%d, DMax_bifurc = %d",
		 Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, Perturbation, DMax,vl_aut_bifamont,vl_aut_bifaval,DMax_bifurc);

	/*A
	**  on cherche le nieme pmv disponible en amont.
	*/
	pm_pert=va_pert;
	vl_le_second.NumPMV=0;
	vl_le_premier.NumPMV=0;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation=FALSE;

	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;

	/*******************************************/
	/*******************************************/
	if (((XDY_Sens) Sens) != XDC_SENS_INCONNU)
	{


		if ( (Sens==XDC_SENS_SORTIE_NORD) ||
		     (Sens==XDC_SENS_ENTREE_NORD) ||
		     (Sens==XDC_SENS_NORD) ||
		     (Sens==XDC_SENS_SORTIE_SUD) ||
		     (Sens==XDC_SENS_ENTREE_SUD) ||
		     (Sens==XDC_SENS_SUD))
		{
			if ((Sens==XDC_SENS_SORTIE_NORD) ||
			    (Sens==XDC_SENS_ENTREE_NORD) ||
			    (Sens==XDC_SENS_NORD))
				vl_sens=XDC_SENS_NORD;
			else
				vl_sens=XDC_SENS_SUD;

			/*recherche le 1er PMV en amont*/
			XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv,
						&cr
           				       );
			XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv_saga,
						&cr);
		}
		else
		{ 	/* Traitement PMV dans les 2 sens */
			vl_sens = Sens - 10;
		 	if ((vl_sens==XDC_SENS_SORTIE_NORD) ||
		    	   (vl_sens==XDC_SENS_ENTREE_NORD) ||
		    	   (vl_sens==XDC_SENS_NORD))
			{
		     		vl_sens=XDC_SENS_NORD;
		     		vl_sens_i=XDC_SENS_SUD;
			}
			else
		     	{
				vl_sens=XDC_SENS_SUD;
		     		vl_sens_i=XDC_SENS_NORD;
			}

			/* Ne pas chercher un PMV en sens inverse pour certains cas ( regles ) */
			/* Cas de l'affichage du bouchon mobile pour un Basculement pour Travaux */
			if ( (va_pert->Condition->Fonction.Comp.Valeur == XZAEC_FMC_BasculTravaux)  &&
			     strcasestr (va_pert->Ligne1.Texte, "BOUCHON") != NULL  &&
			     strcasestr (va_pert->Libelle, "mobile") != NULL )
			{
				vl_chercher_en_sens_inverse = XDC_FAUX;
			}
			
			XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv,
						&cr
           				       );
			XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv_saga,
						&cr);

			if (vl_chercher_en_sens_inverse == XDC_VRAI)
			{
				XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens_i,
						(XDY_PR) PR_tete,
						(XDY_PR) PR,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_PR) vl_pr_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv,
						&cr
           				       );
				XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens_i,
						(XDY_PR) PR_tete,
						(XDY_PR) PR,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_PR) vl_pr_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv_saga,
						&cr
           				       );
			}
		}
	}
	else
	{
                /*recherche les 1� PMV en amont en NORD*/
                XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr
                                               );


                /*recherche les 1� PMV en amont en SUD*/
                XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr
                                               );

	}
	free((void *)vg_Utilisation);
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne %d", version, cr);
	return;

}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche le PMV en amont de l'aire en amont du PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	le_PMV_amont_aire( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)


/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PR_tete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve un pmv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PMV, FALSE sinon.
*
------------------------------------------------------*/
{
	XDY_Entier	cr=FALSE;
	int		pmv_ok=C_PMV_TROUVE;
	XDY_Octet      	NumAire;
	T_STRING       	NomAire;
	T_STRING	AbrevAire;
	XDY_Autoroute  	AutorouteAire;
	XDY_PR         	PRAire;
	XDY_Sens       	SensAire;
	XDY_Entier     	DistanceAire;
	XDY_Mot     	n=1;
	int		code_retour=0;
	static char *version = CM_VERSION_SOURCE ": le_PMV_amont_aire" ;
	int		vl_DMax,vl_DMax_bifurc;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"++ %s Autoroute = %lf, PR = %lf, PRTete = %lf, Sens = %lf, Horodate = %lf, District = %lf, Priorite = %lf, Perturbation = %s, DMax = %lf, DMax_bifurc = %lf",
		 version,Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, Perturbation, DMax,DMax_bifurc);

	/*A
	**  on cherche la premiere aire en amont
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	if (XZAO16_Aire_Amont(	(XDY_Autoroute) Autoroute, (XDY_PR) PR, (XDY_Sens) Sens, 1,
				&NumAire, NomAire, AbrevAire,
				&AutorouteAire, &PRAire, &SensAire, &DistanceAire) == XDC_OK) 
		XZST_03EcritureTrace(XZSTC_DEBUG1,
			"%s NumAire = %d, AutorouteAire = %d, PRAire = %d, SensAire = %d",
			 version,NumAire, AutorouteAire, PRAire, SensAire);	

	/*A
	**  on cherche le premier pmv disponible en amont de l'aire precedente.
	*/
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation=FALSE;
	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;
	if ( NumAire != 0 )
	{
                /*recherche les 1� PMV en amont*/
                XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) AutorouteAire,
                                                (XDY_Sens) SensAire,
                                                (XDY_PR) PRAire,
                                                (XDY_PR) PRAire,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						DistanceAire,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) AutorouteAire,
                                                (XDY_Sens) SensAire,
                                                (XDY_PR) PRAire,
                                                (XDY_PR) PRAire,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						DistanceAire,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr
                                               );
	}
	free((void *)vg_Utilisation);

	/*A
	**  on renvoie le compte-rendu au RTie
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne %d", version, cr);
	return;

}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche le PMV en amont de la sortie en amont du PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	le_PMV_amont_sortie( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)


/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PR_tete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve un pmv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PMV, FALSE sinon.
*
------------------------------------------------------*/
{
	XDY_Entier	cr=FALSE;
	int		pmv_ok=C_PMV_TROUVE;
	XDY_Octet      	NumSortie;
	XDY_Nom       	NomSortie;
	XDY_Abrev	AbrevSortie;
	XDY_Sortie     	SortieUsager;
	XDY_Octet      	Bifurcation;
	XDY_Autoroute  	AutorouteSortie;
	XDY_PR         	PRSortie;
	XDY_Sens       	SensSortie;
	XDY_Entier     	DistanceSortie;
	XDY_Mot		n=1;
	int		code_retour=0;
	static char *version = CM_VERSION_SOURCE ": le_PMV_amont_sortie" ;
	int	vl_DMax,vl_DMax_bifurc;

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"__ %s Autoroute = %lf, PR = %lf, PRTete = %lf, Sens = %lf, Horodate = %lf, District = %lf, Priorite = %lf, DMax = %lf, DMax_bifurcx = %lf",
		 version,Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, DMax, DMax_bifurc);

	/*A
	**  on cherche la premiere sortie en amont
	*/
	if (XZAO17_Sortie_Amont((XDY_Autoroute) Autoroute, (XDY_PR) PR, (XDY_Sens) Sens, 1,
				&NumSortie, NomSortie, AbrevSortie, SortieUsager, &Bifurcation,
				&AutorouteSortie, &PRSortie, &SensSortie, &DistanceSortie) == XDC_OK) 
		XZST_03EcritureTrace(XZSTC_DEBUG1,
			"%s Sortie %d = %s %s, AutorouteSortie = %d, PRSortie = %d, SensSortie = %d",
			 version, NumSortie, SortieUsager, (Bifurcation) ? "(BIF)" : "", AutorouteSortie, PRSortie, SensSortie);

	/*A
	**  on cherche le premier pmv en amont de la sortie precedente.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation=FALSE;
	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;
	if ( NumSortie != 0 )
	{
                /*recherche les 1� PMV en amont*/
                XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) AutorouteSortie,
                                                (XDY_Sens) SensSortie,
                                                (XDY_PR) PRSortie,
                                                (XDY_PR) PRSortie,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) AutorouteSortie,
                                                (XDY_Sens) SensSortie,
                                                (XDY_PR) PRSortie,
                                                (XDY_PR) PRSortie,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr
                                               );
	}
	free(vg_Utilisation);

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s  retourne %d", version, cr);
	return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche les PMV situes entre les deux PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	les_PMV_ds_perturbation( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)


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
*   TRUE si on a trouve au moins un pmv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PMV, FALSE sinon.
*
------------------------------------------------------*/
{
	T_REAL8		tmp;
	XDY_Mot		AutorouteInversee;
	XDY_Octet	n=1,i=0;
	XDY_Entier	flag=FALSE;
	int		code_retour=0;
	static char *version = CM_VERSION_SOURCE ": les_PMV_ds_perturbation" ;
	T_REAL8		vl_pr_min,vl_pr_max;
	int             pmv_ok=C_PMV_TROUVE;
	T_REAL8		vl_sens;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	vl_pr_min = PR_tete <= PR ? PR_tete : PR;
	vl_pr_max = PR_tete > PR ? PR_tete : PR;
	/*A
	**  tant que le nieme pmv en amont existe et est dans la perturbation,
	**  on demande le suivant.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	if (((XDY_Sens) Sens) != XDC_SENS_INCONNU)
	{

		if ((Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;
                /*recherche les 1� PMV en amont*/
                XZAO425_Rechercher_les_PMV_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                XZAO825_Rechercher_les_PMV_SAGA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) vl_sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );


	}
	else	/*A cas du sens inconnu */
	{
		/*B on regarde si l'autoroute est inversee */
		/*
		while ((i < vg_IE_NbAutoroutes)
			&& (vg_IE_Autoroutes[i].numero != (XDY_Autoroute) Autoroute))	i++;
		if (i != vg_IE_NbAutoroutes)
			AutorouteInversee = vg_IE_Autoroutes[i].inversee;
		else
			XZST_03EcritureTrace(XZSTC_WARNING,
			"%s : l'autoroute %d ne fait pas partie des autoroutes connues",
			version, (XDY_Autoroute) Autoroute);
		*/
		/*B  on considere que le Pr de tete est le plus grand des deux
		**  (sauf si l'autoroute est inversee)
		*/

		/*PATCH POUR A57*/
		if (Autoroute==8) AutorouteInversee=-1;

		if (PR_tete*AutorouteInversee < PR*AutorouteInversee)
		{
			tmp = PR_tete;
			PR_tete = PR;
			PR = tmp;
		}

                /*recherche les 1� PMV en amont en SUD*/
                XZAO425_Rechercher_les_PMV_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                XZAO825_Rechercher_les_PMV_SAGA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );


	
                /*recherche les 1� PMV en amont en NORD*/
                XZAO425_Rechercher_les_PMV_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &flag
                                               );
                XZAO825_Rechercher_les_PMV_SAGA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_Entier)DISTANCE_INFINIE,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &flag
                                               );
	}
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

extern int pmv_init_vg_requetes()

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
*   	Mise a jour des variables globales vg_appel_o15 et vg_appel_t05
*
------------------------------------------------------*/
{
	static char *version = CM_VERSION_SOURCE ": pmv_init_vg_requetes" ;

   pmv_init_appels_base();

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int pmv_init_appels_base()

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
*       Mise a jour des variables globales vg_appel_o15 et vg_appel_t05
*
------------------------------------------------------*/
{ 
  int vl_i;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : pmv_init_appels_base");
  for (vl_i=0; vl_i < NbMaxEnr_o15 ; vl_i++) {
    vg_appel_o15[vl_i].NumPMV=0;
  }

  for (vl_i=0; vl_i < NbMaxEnr_t05; vl_i++) {
    vg_appel_t05_saga[vl_i].NumPMV=0;
    vg_appel_t05[vl_i].NumPMV=0;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : pmv_init_appels_base");

  for (vl_i=0; vl_i < C_NB_MAX_PMV; vl_i++) {
    vg_pmv[vl_i].Utilise=FALSE;
    vg_pmv[vl_i].Perturbation=NULL;
    strcpy(vg_pmv[vl_i].Ligne1,"");
    strcpy(vg_pmv[vl_i].Ligne2,"");
    strcpy(vg_pmv[vl_i].Ligne3,"");
    strcpy(vg_pmv[vl_i].Picto,"");
    vg_pmv[vl_i].PMV_SAGA=XDC_FAUX;
  }
  for (vl_i=0; vl_i < C_NB_MAX_PMV; vl_i++) {
    vg_pmv_saga[vl_i].Utilise=FALSE;
    vg_pmv_saga[vl_i].Perturbation=NULL;
    strcpy(vg_pmv_saga[vl_i].Ligne1,"");
    strcpy(vg_pmv_saga[vl_i].Ligne2,"");
    strcpy(vg_pmv_saga[vl_i].Ligne3,"");
    strcpy(vg_pmv_saga[vl_i].Picto,"");
    vg_pmv_saga[vl_i].PMV_SAGA=XDC_VRAI;
  }
  vg_priorite_retenue = 0;

  return(XDC_OK);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche les 2 PMV eligibles en amont (temps de parcours)
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	les_2_PMV_eligibles_en_amont( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)


/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PR_Tete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;
*	T_REAL8		PR_ZDP;
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve au moins un PMV, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis le RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PMV, FALSE sinon
*
------------------------------------------------------*/
{
	XDY_Octet	n=1;
	T_BOOL		flag=FALSE;
	int		pmv_ok=C_PMV_TROUVE;
	int		code_retour=0;
	static char *version = CM_VERSION_SOURCE ": les_2_PMV_eligibles_en_amont" ;
	T_REAL8		PR_ZDP;
	T_REAL8		vl_pr;
	T_REAL8		vl_zdp_sens;
	T_REAL8		vl_fmc_operateur;
	T_REAL8		vl_first_pmv=TRUE;
	int		vl_DMax,vl_DMax_bifurc;
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);
	XZST_03EcritureTrace(XZSTC_WARNING, "FONCTION INHIBEE");
return;

/******************************
	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"%s Autoroute=%lf, PR=%lf, PRTete=%lf, Sens=%lf, Horo=%lf, District=%lf, Prio=%lf, Perturbation=%s, DMax=%lf, DMax_bifurc=%lf",
		 version,Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, Perturbation, DMax, DMax_bifurc);

	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;
	*si je traite une fmc operateur*
	if (vl_fmc_operateur==1) {
	  *PR et sens sont ceux de la zdp*
	  vl_pr = PR_ZDP;
	  Sens = vl_zdp_sens;
	}
	else {
	  * on recherche les PMV en amont de l'evenement et de la zdp*
	  if (Sens==XDC_SENS_SUD)
	    vl_pr = (PR < PR_ZDP) ? PR : PR_ZDP ; 
          else
	    vl_pr = (PR > PR_ZDP) ? PR : PR_ZDP ;
	}

	*A
	**  on demande les 2 PMV en amont .
	*
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation=FALSE;
	free(vg_Utilisation);
	
	*A
	**  on renvoie le compte-rendu au RTie
	*
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne %d", version, flag);
	return;
******************************/
}

/******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* pmv_libere_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int pmv_libere_ressources()
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"pmv_libere_ressources: OUT");
  return (XDC_OK);
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche le nieme PMV en amont et eloigne d'au plus DMax (metres) 
* SUR LA CHAUSSEE OPPOSEE
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	le_PMV_sur_la_chaussee_opposee( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)

/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Numero;
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PR_tete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve un pmv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne le nombre de candidats.	
*
------------------------------------------------------*/
{

	XDY_Entier	cr=FALSE;
	int		pmv_ok=C_PB_PMV;
	XDY_Mot		n;
	int		code_retour=0;
	PMVT_Utilisation vl_le_premier;
	PMVT_Utilisation vl_le_second;
	XDY_Booleen		vl_trouve_un=FALSE;
	XDY_Sens		vl_sens;
	int	vl_DMax,vl_DMax_bifurc;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"Autoroute = %d, PR = %d, PRTete = %d, Sens = %d, Horodate = %lf, District = %d, Priorite = %d, Perturbation = %s, DMax = %d, aut amont = %d, aut aval=%d, DMax_bifurc = %d",
		 Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, Perturbation, DMax,vl_aut_bifamont,vl_aut_bifaval,DMax_bifurc);

	/*uniquement en sens section courante*/
	if ( (Sens!=XDC_SENS_NORD) &&
	     (Sens!=XDC_SENS_SUD)
           )
	   return;

	/*A
	**  on cherche le nieme pmv disponible en amont.
	*/
	pm_pert=va_pert;
	vl_le_second.NumPMV=0;
	vl_le_premier.NumPMV=0;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation=FALSE;
	vl_trouve_un=FALSE;

	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;

	/*******************************************/
	/*******************************************/
	if (Sens == XDC_SENS_NORD)
	{

		/*recherche les 1� PMV en amont sur la chaussee opposee*/
		XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) XDC_SENS_SUD,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv,
						&cr
           				       );
		XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) XDC_SENS_SUD,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)vl_DMax, 
						(XDY_Entier)vl_DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_Entier) 0,
						(XDY_FonctionInt)ajouter_pmv_saga,
						&cr);
	}
	else
	{
                /*recherche les 1� PMV en amont  sur la chaussee opposee*/
                XZAO419_Rechercher_le_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO819_Rechercher_le_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)vl_DMax,
                                                (XDY_Entier)vl_DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
						(XDY_Entier) 0,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr);
	}
	free((void *)vg_Utilisation);
	
}



/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO419
* utilisee pour creer un PMV
*
******************************************************
* SEQUENCE D'APPEL :
* ajouter_pmv)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_pmv(XDY_Eqt va_numero_pmv,
			XDY_District va_district_pmv,
			XDY_Autoroute va_autoroute_pmv,
			XDY_PR va_pr_pmv,
			int va_distance_pmv,
			XDY_Sens va_sens_pmv,
			char *va_direction,
			int va_presence_bifurc,
			XDY_District va_district,
			int va_priorite,
			XDY_Horodate va_horodate,
			char *va_perturbation,
			int *pa_resultat)
{
  T_BOOL vl_cr=FALSE;
  XDY_Octet vl_type;

  XZST_03EcritureTrace(XZSTC_FONCTION,
	"ajouter_pmv: NumPMV = %d, DistrictPMV = %d, DistancePMV = %d District=%d DispoTexte=%d", 
	va_numero_pmv, va_district_pmv, va_distance_pmv,va_district,vg_Utilisation->DispoTexte);

  vg_Utilisation->DistanceC = (float)va_distance_pmv;
  vg_Utilisation->PresenceBifurcation = va_presence_bifurc;
				
  /*B
  ** on ne retient que les pmv du district dont la partie texte est disponible
  */
  if    ( (etat_pmv(va_horodate, va_numero_pmv) == XDC_OK) )
  {

    if ( (vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
					| XDC_EQT_CRITIQUE
					| XDC_EQT_DESACTIVE
					| XDC_EQT_HS
					| XDC_EQT_INHIBE))== 0 )
    {
	vg_Utilisation->NumPMV = va_numero_pmv;
	vg_Utilisation->DistrictPMV = va_district_pmv;
	vg_Utilisation->AutoroutePMV = va_autoroute_pmv;
	vg_Utilisation->PRPMV = va_pr_pmv;
	vg_Utilisation->SensPMV = va_sens_pmv;
	vg_Utilisation->PrioriteC = va_priorite;	
	  vg_Utilisation->PMV_SAGA = XDC_FAUX;
	  vg_Utilisation->Type_SAGA = 0;
	strcpy(vg_Utilisation->DirectionPMV,va_direction);

	vl_cr = creer_pmv(va_perturbation, XDC_FAUX);
    }
    /*B
    **  on se place en file d'attente si le PMV est indispo et pas site CI
    */
    else if (va_district_pmv != XDC_CI)
	/* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
	/* Sur COFIROUTE, le site principal peut piloter les PMV */
    {
	/*XZAP21_Placer_Evt_File_Attente(	vg_IE_NumPA, va_numero_pmv, XDC_EQT_PMV,
			va_priorite, va_distance_pmv);*/
      vl_cr=0; /*temporaire a virer JMG*/
    }
  }
  *pa_resultat=vl_cr;
  return XDC_OK;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche le deuxieme PMV en amont et eloigne d'au plus DMax (metres) 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	le_second_PMV_amont( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char	*Perturbation,
	int		DMax,
	int		DMax_bifurc,
	XDY_PR		vl_pr_bifamont,
	XDY_PR		vl_pr_bifaval,
	XDY_PR		vl_pr_bifamont_tete,
	XDY_PR		vl_pr_bifaval_tete,
	XDY_Autoroute	vl_aut_bifamont,
	XDY_Autoroute	vl_aut_bifaval,
	XDY_Autoroute	vl_aut_bifamont_tete,
	XDY_Autoroute	vl_aut_bifaval_tete,
	XDY_PR		vl_pr_debut_bifamont,
	XDY_PR		vl_pr_debut_bifaval,
	XDY_PR		vl_pr_debut_bifamont_tete,
	XDY_PR		vl_pr_debut_bifaval_tete,
	T_PERT		*va_pert)


/*
* ARGUMENTS EN ENTREE :
*	T_REAL8		Numero;
*	T_REAL8		Autoroute;
*	T_REAL8		PR;
*	T_REAL8		PR_tete;
*	T_REAL8		Sens;
*	T_REAL8		Horodate;
*	T_REAL8		District;
*	T_REAL8		Priorite;
*	T_STRING	Perturbation;
*	T_REAL8		DMax;
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve un pmv, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PMV par PMV candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne le nombre de candidats.	
*
------------------------------------------------------*/
{
	XDY_Entier	cr=FALSE;
	int		pmv_ok=C_PB_PMV;
	int		code_retour=0;
	static char *version = CM_VERSION_SOURCE ": le_second_PMV_amont" ;
	PMVT_Utilisation vl_le_premier;
	PMVT_Utilisation vl_le_second;
	T_BOOL		vl_trouve_un=FALSE;
	T_REAL8		vl_sens;
	int		vl_DMax,vl_DMax_bifurc;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	/*A
	**  on cherche le nieme pmv disponible en amont.
	*/
	pm_pert=va_pert;
	vl_le_second.NumPMV=0;
	vl_le_premier.NumPMV=0;
	vg_Utilisation = (PMVT_Utilisation *) malloc(sizeof(PMVT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation=FALSE;
	vl_trouve_un=FALSE;

	vl_DMax = ( DMax==0 ) ? DISTANCE_INFINIE : DMax;
	vl_DMax_bifurc = ( DMax_bifurc==0 ) ? DISTANCE_INFINIE : DMax_bifurc;

	/*******************************************/
	/*******************************************/
	if (((XDY_Sens) Sens) != XDC_SENS_INCONNU)
	{

		if ((Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;

		/*recherche les 1� PMV en amont*/
		XZAO426_Rechercher_les_seconds_PMV_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)DMax, 
						(XDY_Entier)DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_FonctionInt)ajouter_pmv,
						&cr
           				       );
		XZAO826_Rechercher_les_seconds_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
						(XDY_Sens) vl_sens,
						(XDY_PR) PR,
						(XDY_PR) PR_tete,
						(XDY_Entier)DMax, 
						(XDY_Entier)DMax_bifurc,
						(XDY_PR) vl_pr_bifamont,
						(XDY_PR) vl_pr_bifamont_tete,
						(XDY_Autoroute) vl_aut_bifamont,
						(XDY_Autoroute) vl_aut_bifamont_tete,
						(XDY_PR) vl_pr_debut_bifamont,
						(XDY_PR) vl_pr_debut_bifamont_tete,
						(XDY_District) District,
						(XDY_Entier) Priorite,
						(XDY_Horodate) Horodate,
						(char *) Perturbation,
						(XDY_FonctionInt)ajouter_pmv_saga,
						&cr);
	}
	else
	{
                /*recherche les 1� PMV en amont en NORD*/
                XZAO426_Rechercher_les_seconds_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)DMax,
                                                (XDY_Entier)DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO826_Rechercher_les_seconds_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_Entier)DMax,
                                                (XDY_Entier)DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr);


                /*recherche les 1� PMV en amont en SUD*/
                XZAO426_Rechercher_les_seconds_PMV_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)DMax,
                                                (XDY_Entier)DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv,
                                                &cr
                                               );
                XZAO826_Rechercher_les_seconds_PMV_SAGA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_Entier)DMax,
                                                (XDY_Entier)DMax_bifurc,
                                                (XDY_PR) vl_pr_bifamont,
                                                (XDY_PR) vl_pr_bifamont_tete,
                                                (XDY_Autoroute) vl_aut_bifamont,
                                                (XDY_Autoroute) vl_aut_bifamont_tete,
                                                (XDY_PR) vl_pr_debut_bifamont,
                                                (XDY_PR) vl_pr_debut_bifamont_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                (XDY_FonctionInt)ajouter_pmv_saga,
                                                &cr);

	}
	free((void *)vg_Utilisation);
	
	/*A
	**  on renvoie le compte-rendu 
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne %d", version, cr);
	return;

}

extern void le_PMV_null()
{
  return;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  teste si il y a une sortie entre le PMV et le debut de la FMC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/


extern XDY_Booleen util_pmv_amont_sortie(int 	Dist_PMV,
					int 	Sens_PMV,
					int	Dist_sortie_amont,
					int	Dist_sortie_aval,
					int	Sens_fmc,
					int	Zone_Urbaine)

/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   T_BOOL      sortie entre PMV et FMC
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du Rtie
*
* FONCTION
*   renvoie TRUE si il y a une sortie entre le PMV et la FMC.
*
------------------------------------------------------*/
{
	static char *version = CM_VERSION_SOURCE ": util_pmv_amont_sortie" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, " in : util_pmv_amont_sortie : Distance_C %d, DistanceSortieAmont %d, DistanceSortieAval %d",
			Dist_PMV, Dist_sortie_amont,Dist_sortie_aval);
	if ((Zone_Urbaine ==0) && (Dist_PMV<15000)) /*pas de message APRES SORTIE si PMV a moins de 15 km et hors urbain*/
		return (FALSE);

	
	/*A
	**  si le sens de la FMC est inconnu et si le PMV est en sens sud, le PR de tete devient le PR de queue
	*/
/*	if ( (XDY_Sens) Sens_fmc == XDC_SENS_INCONNU && (XDY_Sens) Sens_PMV == XDC_SENS_SUD)
		Dist_sortie_amont = Dist_sortie_aval;
*/
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
	if ( Dist_PMV > Dist_sortie_amont) {
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
*  teste si un PMV est en amont d'une FMC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen util_pmv_amont_fmc(int	PR_PMV,
					int	Sens_PMV,
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
*   renvoie TRUE si le PMV est en amont de la FMC, FALSE sinon.
*
------------------------------------------------------*/
{
XZST_03EcritureTrace(XZSTC_FONCTION, "util_pmv_amont_fmc %d,%d,%d,%d,%d,%d",
			PR_PMV,
			Sens_PMV,
			PR_Queue,
			PR_Tete,
			Sens_fmc,
			Autoroute_inversee);
	  /*A
	  **  si le sens de la FMC est inconnu et si le PMV est en sens sud, le PR de tete devient le PR de queue
	  */
	  if ( (XDY_Sens) Sens_fmc == XDC_SENS_INCONNU && (XDY_Sens) Sens_PMV == XDC_SENS_SUD)
		  PR_Queue = PR_Tete;

	/*A
	** le sens sud est le sens des PR croissant (sauf si autoroute inversee)
	*/
	if (( (((XDY_Sens) Sens_PMV == XDC_SENS_SUD) && (PR_PMV*Autoroute_inversee < PR_Queue*Autoroute_inversee))
		|| (((XDY_Sens) Sens_PMV == XDC_SENS_NORD) && (PR_PMV*Autoroute_inversee > PR_Queue*Autoroute_inversee))) &&
	(PR_PMV!=1900 || PR_Queue < 12600)) {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : util_pmv_amont_fmc retourne TRUE");
		return(TRUE);
	}
	else {
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : util_pmv_amont_fmc retourne FALSE");
		return(FALSE);
	}
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  convertit un booleen en chaine de caracteres
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern char *util_bool_to_str(XDY_Booleen Booleen, char *StrTrue, char *StrFalse)

/*
* ARGUMENTS EN ENTREE :
*   T_BOOL      booleen;
*   T_STRING    chaine associee a TRUE
*   T_STRING    chaine associee a FALSE
*
* ARGUMENTS EN SORTIE :
*   T_STRING    valeur associee au booleen
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION
*   un simple test
*
------------------------------------------------------*/
{
	XZST_03EcritureTrace(XZSTC_INTERFACE, "util_bool_to_str: bool %d, str true %s, str false %s",
					Booleen,
					StrTrue,
					StrFalse);

	if (Booleen) {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : util_bool_to_str retourne %s", StrTrue);
	  return (StrTrue);
        }
	else {
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : util_bool_to_str retourne %s", StrFalse);
	  return (StrFalse);
        }
}


/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO419
* utilisee pour creer un PMV
*
******************************************************
* SEQUENCE D'APPEL :
* ajouter_pmv)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_pmv_saga(XDY_Eqt va_numero_pmv,
			XDY_District va_district_pmv,
			XDY_Autoroute va_autoroute_pmv,
			XDY_PR va_pr_pmv,
			int va_distance_pmv,
			XDY_Sens va_sens_pmv,
			char *va_direction,
			int va_presence_bifurc,
			XDY_District va_district,
			int va_priorite,
			XDY_Horodate va_horodate,
			char *va_perturbation,
			int *pa_resultat)
{
  T_BOOL vl_cr=FALSE;
  XDY_Octet vl_type;

  XZST_03EcritureTrace(XZSTC_FONCTION,
	"ajouter_pmv_saga: NumPMV = %d, DistrictPMV = %d, DistancePMV = %d District=%d DispoTexte=%d", 
	va_numero_pmv, va_district_pmv, va_distance_pmv,va_district,vg_Utilisation->DispoTexte);

  vg_Utilisation->DistanceC = (float)va_distance_pmv;
  vg_Utilisation->PresenceBifurcation = va_presence_bifurc;
				
  /*B
  ** on ne retient que les pmv du district dont la partie texte est disponible
  */
  if    ( (etat_pmv_saga(va_horodate, va_numero_pmv) == XDC_OK) )
  {

    XZST_03EcritureTrace(XZSTC_FONCTION,"avant DispoTexte %d", vg_Utilisation->DispoTexte);
    if (vg_Utilisation->DispoTexte==32)
      vg_Utilisation->DispoTexte=0;
    if ( (vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
					| XDC_EQT_CRITIQUE
					| XDC_EQT_DESACTIVE
					| XDC_EQT_HS
					| XDC_EQT_INHIBE))== 0 )
    {
	vg_Utilisation->NumPMV = va_numero_pmv;
	vg_Utilisation->DistrictPMV = va_district_pmv;
	vg_Utilisation->AutoroutePMV = va_autoroute_pmv;
	vg_Utilisation->PRPMV = va_pr_pmv;
	vg_Utilisation->SensPMV = va_sens_pmv;
	vg_Utilisation->PrioriteC = va_priorite;	
	  vg_Utilisation->PMV_SAGA = XDC_VRAI;

	  /*recupere le type SAGA*/
	 /* XZAO791_Type_SAGA(vg_Utilisation->NumPMV, &vl_type);
	  vg_Utilisation->Type_SAGA = vl_type;*/
	strcpy(vg_Utilisation->DirectionPMV,va_direction);

	vl_cr = creer_pmv(va_perturbation, XDC_VRAI);
    }
    /*B
    **  on se place en file d'attente si le PMV est indispo et pas site CI
    */
    else if (va_district_pmv != XDC_CI)
	/* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
	/* Sur COFIROUTE, le site principal peut piloter les PMV */
    {
	/*XZAP21_Placer_Evt_File_Attente(	vg_IE_NumPA, va_numero_pmv, XDC_EQT_PMV,
			va_priorite, va_distance_pmv);*/
      vl_cr=0; /*temporaire a virer JMG*/
    }
  }
  *pa_resultat=vl_cr;
  return XDC_OK;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche l'etat d'un PMV SAGA.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int etat_pmv_saga(Horodate, NumPMV)
XDY_Horodate		Horodate;
XDY_Eqt			NumPMV;
/*
* ARGUMENTS EN ENTREE :
*   XDY_Horodate	Horodate;
*   XDY_Eqt		NumPMV;
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
	XDY_Octet	dispo_picto;
	XDY_Octet	type_pmv;
	char	l1[30];
	char	l2[30];
	char	l3[30];
	char	l4[30];
	char	l5[30];
	char	a1[30];
	char	a2[30];
	char	a3[30];
	char	a4[30];
	char	a5[30];
	XDY_Booleen	c1;
	XDY_Booleen	c2;
	XDY_Booleen	c3;
	XDY_Booleen	flash;
	char	picto[30];
	char	bandeau[30];
	XDY_Booleen	cligno;
	XDY_Mot	lumino, barriere, scenario;
	XDY_Octet	numpicto;
	XDY_Booleen	c;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
/*	T_IE_SLOT	slot;*/
	int		i=0, cr=XDC_NOK;
	int		j=0,k=0;
	static char *version = CM_VERSION_SOURCE ": etat_pmv_saga" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

		/*B on regarde si on a deja fait cette requete */
	while (vg_appel_t05_saga[i].NumPMV != 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on est dans la boucle, pour i = %d", i);

		if (vg_appel_t05_saga[i].NumPMV == NumPMV)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on a trouve pour i = %d", i);

			cr = XDC_OK;
			dispo_texte = vg_appel_t05_saga[i].dispo_texte;
			dispo_picto = vg_appel_t05_saga[i].dispo_picto;
			type_pmv = vg_appel_t05_saga[i].type_pmv;
			strcpy(l1, vg_appel_t05_saga[i].l1);
			strcpy(l2, vg_appel_t05_saga[i].l2);
			strcpy(l3, vg_appel_t05_saga[i].l3);
			strcpy(a1, vg_appel_t05_saga[i].a1);
			strcpy(a2, vg_appel_t05_saga[i].a2);
			strcpy(a3, vg_appel_t05_saga[i].a3);
			c1 = vg_appel_t05_saga[i].c1;
			c2 = vg_appel_t05_saga[i].c2;
			c3 = vg_appel_t05_saga[i].c3;
			flash = vg_appel_t05_saga[i].flash;
			strcpy(picto, vg_appel_t05_saga[i].picto);
			strcpy(bandeau, vg_appel_t05_saga[i].bandeau);
			numpicto = vg_appel_t05_saga[i].numpicto;
			c = vg_appel_t05_saga[i].c;
			numero_evt_U = vg_appel_t05_saga[i].numero_evt_U;
			cle_evt_U = vg_appel_t05_saga[i].cle_evt_U ;
			priorite_U = vg_appel_t05_saga[i].priorite_U;
			distance_U = vg_appel_t05_saga[i].distance_U;
			break;
		}
		i++;
	}
		/*B si ce n'est pas le cas, on appelle XZAT05 */
	if (cr != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on n'a pas trouve");

		  cr = XZAT150_Utilisation_Eqt_SAGA(Horodate, NumPMV,
		  				&dispo_texte, l1, l2, l3, l4, l5,
						 a1, a2, a3, a4, a5,
						 &cligno, picto, &lumino, bandeau,
						 &barriere, &scenario,
						  &numero_evt_U, &cle_evt_U, &priorite_U, &distance_U, &type_pmv);
			c1 = cligno;
			c2 = cligno;
			c3 = cligno;
			flash = 0;
			numpicto = 0;
			c = cligno;
			dispo_picto = dispo_texte;
		XZST_03EcritureTrace(XZSTC_FONCTION,"dispo_texte %d // distance_U %d // type_pmv %d",
					dispo_texte, distance_U, type_pmv);
		/*B et on enregistre le resultat */
		if ( (cr == XDC_OK) && (i < NbMaxEnr_t05) )
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on enregistre le resultat de la requete");

			vg_appel_t05_saga[i].NumPMV = NumPMV;
			vg_appel_t05_saga[i].dispo_texte = dispo_texte;
			vg_appel_t05_saga[i].dispo_picto = dispo_picto;
			vg_appel_t05_saga[i].type_pmv = type_pmv;
			strcpy(vg_appel_t05_saga[i].l1, l1);
			strcpy(vg_appel_t05_saga[i].l2, l2);
			strcpy(vg_appel_t05_saga[i].l3, l3);
			strcpy(vg_appel_t05_saga[i].a1, a1);
			strcpy(vg_appel_t05_saga[i].a2, a2);
			strcpy(vg_appel_t05_saga[i].a3, a3);
			vg_appel_t05_saga[i].c1 = c1;
			vg_appel_t05_saga[i].c2 = c2;
			vg_appel_t05_saga[i].c3 = c3;
			vg_appel_t05_saga[i].flash = flash;
			strcpy(vg_appel_t05_saga[i].picto, picto);
			strcpy(vg_appel_t05_saga[i].bandeau, bandeau);
			vg_appel_t05_saga[i].numpicto = numpicto;
			vg_appel_t05_saga[i].c = c;
			vg_appel_t05_saga[i].numero_evt_U = numero_evt_U;
			vg_appel_t05_saga[i].cle_evt_U = cle_evt_U ;
			vg_appel_t05_saga[i].priorite_U = priorite_U;
			vg_appel_t05_saga[i].distance_U = distance_U;
			if ((numero_evt_U == 0) && ((priorite_U == 0)||(priorite_U == 254))) {
				vg_appel_t05_saga[i].priorite_U = priorite_U = 254;
				vg_appel_t05_saga[i].distance_U = distance_U = 500000;
			}
			XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmv par evt %d prio %d dist %d",vg_appel_t05_saga[i].numero_evt_U,vg_appel_t05_saga[i].priorite_U ,vg_appel_t05_saga[i].distance_U);
			XZST_03EcritureTrace(XZSTC_INFO, "L1 %s, L2 %s, L3 %s",
						vg_appel_t05_saga[i].l1,
						vg_appel_t05_saga[i].l2,
						vg_appel_t05_saga[i].l3);
			/*A on supprime les espaces de fin de ligne pourle picto*/
			j= (int) strlen(picto);
			for (k=j-1;k>=0;k--) {
			if ( picto[k] == ' ' ) 
				picto[k] = '\0';
			else break;
			}
			strcpy(vg_appel_t05_saga[i].picto, picto);
		}
	}

	/*A
	**  traitement de la reponse
	*/
	if (cr == XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s avant maj Utilisation : ligne1=%s / ligne2=%s / ligne3=%s ", version,l1,l2,l3);
		vg_Utilisation->DispoTexte = dispo_texte;
		vg_Utilisation->DispoPicto = dispo_picto;
		strcpy(vg_Utilisation->L1, l1);
		strcpy(vg_Utilisation->L2, l2);
		strcpy(vg_Utilisation->L3, l3);
		strcpy(vg_Utilisation->A1, a1);
		strcpy(vg_Utilisation->A2, a2);
		strcpy(vg_Utilisation->A3, a3);
		strcpy(vg_Utilisation->Picto, picto);
		strcpy(vg_Utilisation->Bandeau, bandeau);
		vg_Utilisation->NumeroEvtU = numero_evt_U;
		vg_Utilisation->Type_SAGA =type_pmv;
		vg_Utilisation->CleEvtU = cle_evt_U;
		vg_Utilisation->PrioriteU = priorite_U;
		vg_Utilisation->DistanceU = distance_U;
		if ((numero_evt_U == 0) && (priorite_U == 0)) {
			vg_Utilisation->PrioriteU = 254;
			vg_Utilisation->DistanceU = 500000;
		}
		XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmv par evt vg_Utilisation->NumeroEvtU=%d prio vg_Utilisation->PrioriteU=%d vg_Utilisation->DistanceU=%d",vg_Utilisation->NumeroEvtU,vg_Utilisation->PrioriteU ,vg_Utilisation->DistanceU);
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s maj OK",version);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"%s Etat PMV %d inconnu a la date %lf - on le considere indispo", version, NumPMV, Horodate);
		vg_Utilisation->DispoTexte = XDC_EQT_MAJEUR;
		vg_Utilisation->DispoPicto = XDC_EQT_MAJEUR;
		strcpy(vg_Utilisation->L1, "");
		vg_Utilisation->Type_SAGA =type_pmv;
		strcpy(vg_Utilisation->L2, "");
		strcpy(vg_Utilisation->L3, "");
		strcpy(vg_Utilisation->A1, "");
		strcpy(vg_Utilisation->A2, "");
		strcpy(vg_Utilisation->A3, "");
		strcpy(vg_Utilisation->Picto, XDC_PICTO_PASPICTO);
		strcpy(vg_Utilisation->Bandeau, "");
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
}

