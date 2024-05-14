/*E*/
/*Fichier : $Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $   Release : $Revision: 1.15 $        Date : $Date: 2020/12/22 07:08:33 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
------------------------------------------------------
* MODULE DROL_PMVA * FICHIER drol_pmva_util.c
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
* JMG	21/06/05	: Creation
* PNI	24/09/07 : correction SAPIENS 1.5
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : variables "version", traces "PMVA"; IDEM PRECEDENTE  1.6
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : utilisation fonctions Pmva; types args. XZAO440,XZAO443  1.7
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : Gestion conversions types de tailles differentes (=)  1.8
* JPL	14/01/11 : Migration architecture HP ia64 (DEM 971) : retour valeur manquant (pmva_mise_en_page)  1.9
* VR	25/01/12 : Ajout Regulation (DEM/1016)
* VR	17/08/12 : Ajout tdp PMVA (DEM/1014 PMA)
* PNI	20/01/15 : modification dela pol d'affichage PMV PMVA 1.11 DEM1106
* JPL	04/01/16 : Types de donnees tracees : erreur en Debug  1.12
* JPL	09/02/18 : Migration Linux 64 bits (DEM 1274) : Suppression de declarations inutiles  1.14
* PNI	22/12/20 : Ajout des entr�es lors des recherches PMVA en sens inconnu 1.15
* LCL	04/10/23 	: Adaptations CNA DEM-483 : lecture site principal en base
------------------------------------------------------*/

/* fichiers inclus */
#include "drol_pmva_util.h"

#include "xzao440.h"
#include "xzao840.h"
#include "xzao843.h"
#include "xzat050.h"
#include "xzat150.h"


/* definitions de constantes */
#define DISTANCE_INFINIE	1000000
#define NB_LIGNES_PMVA		5

#define CLASSE_PERTURBATION	"perturbation"
#define ATT_PERTURBATION_TYPE	"type"
#define ATT_PMV_PERTURBATION	"perturbation"
#define NbMaxEnr_o15		6
#define NbMaxEnr_t050		6
#define CLASSE_LOCALISATION        "localisation"

#define C_PMV_TROUVE		0
#define C_PB_PMV		1
#define C_PLUS_DE_PMV		2

/* definitions de types locaux */
typedef struct {
	XDY_Octet	DispoTexte;
	T_STRING	L1;		/* texte de la premiere ligne */
	T_STRING	L2;		/* texte de la deuxieme ligne */
	T_STRING	L3;		/* texte de la troisieme ligne */
	T_STRING	L4;		/* texte de la troisieme ligne */
	T_STRING	L5;		/* texte de la troisieme ligne */
	T_STRING	A1;		/* texte du premier alternat */
	T_STRING	A2;		/* texte du deuxieme alternat */
	T_STRING	A3;		/* texte du troisieme alternat */
	T_STRING	A4;		/* texte du troisieme alternat */
	T_STRING	A5;		/* texte du troisieme alternat */
	XDY_Octet	Flash;
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
	XDY_Booleen     PresenceBifurcation;
	T_STRING        DirectionPMV;
	XDY_Booleen     PMVA_SAGA;
} PMVAT_Utilisation;

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
} PMVAT_xzao15;				/* structure d'appel de XZAO15 */

typedef struct {
	XDY_Eqt		NumPMV;
	XDY_Octet	dispo_texte;
	XDY_Ligne_PMV	l1;
	XDY_Ligne_PMV	l2;
	XDY_Ligne_PMV	l3;
	XDY_Ligne_PMV	l4;
	XDY_Ligne_PMV	l5;
	XDY_Ligne_PMV	a1;
	XDY_Ligne_PMV	a2;
	XDY_Ligne_PMV	a3;
	XDY_Ligne_PMV	a4;
	XDY_Ligne_PMV	a5;
	XDY_Booleen	c1;
	XDY_Booleen	c2;
	XDY_Booleen	c3;
	XDY_Booleen	c4;
	XDY_Booleen	c5;
	XDY_Octet	flash;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
} PMVAT_xzat050;				/* structure d'appel de XZAT05 */


/*definition de variables globales*/
static	T_Liste_Zone		*pg_zones=NULL;
static  T_Liste_Zone		*pg_zone_courante=NULL;
static  T_Liste_Zone		*pg_zone_fin_courante=NULL;
static	T_PERT_PMVA		pm_pert;
T_NOEUD		*vm_noeud;
/* definition de macro locales */


/* declaration de variables locales */
static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : drol_pmva_util" ;

PMVAT_Utilisation	*vg_Utilisation;
PMVAT_xzao15		vg_appel_o15[NbMaxEnr_o15];
PMVAT_xzao15		vg_appel_o15_saga[NbMaxEnr_o15];
PMVAT_xzat050		vg_appel_t050[NbMaxEnr_t050];
PMVAT_xzat050		vg_appel_t050_saga[NbMaxEnr_t050];
int 			vg_max_num_pmva=0;
T_PMVA			vg_pmva[C_NB_MAX_PMVA];
T_PMVA			vg_pmva_saga[C_NB_MAX_PMVA];
int			vg_priorite_retenue;

/* declaration de fonctions internes */
static int etat_pmva();
static int etat_pmva_saga();
static XDY_Booleen creer_pmva(char *, XDY_Booleen);

static int ajouter_pmva(XDY_Eqt ,
			XDY_District ,
			XDY_Autoroute ,
			XDY_PR ,
			int ,
			XDY_Sens ,
			char *,
			int,
			XDY_District ,
			int ,
			XDY_Horodate ,
			char *,
			int *);

static int ajouter_pmva_saga(XDY_Eqt ,
			XDY_District ,
			XDY_Autoroute ,
			XDY_PR ,
			int ,
			XDY_Sens ,
			char *,
			int,
			XDY_District ,
			int ,
			XDY_Horodate ,
			char *,
			int *);

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   verification de la longueur des lignes PMV
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern XDY_Booleen	pmva_ligne_trop_longue(char *Texte, int NbCarLigne)

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
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : pmva_ligne_trop_longue" ;
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);
	
	XZST_03EcritureTrace(XZSTC_DEBUG1, "%s Texte = %s, NbCarLigne = %d", version,Texte, NbCarLigne);

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
*   mise en page d'un message PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern char *pmva_mise_en_page(char *Texte, int NbCarLigne)

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
	int		separateur[6];	/* position des separateurs d'elements */
	int		separateur_ini[6];	/* position des separateurs d'elements */
	int		code_retour=0;
	int		nb_cr=0;
	char        message[50];
	XZIAT_destinataire      Destinataire;
        int		vl_position_tdp=0;
	char	vl_texte_init[50];
	int		vl_nb_lignes=0;
	char		*vl_elt_tdp;
	char		*vl_tronque;
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : pmva_mise_en_page" ;

	XZST_03EcritureTrace(XZSTC_DEBUG1, "pmva_mise_en_page Texte = %s, NbCarLigne = %d", Texte, NbCarLigne);

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
			Texte[separateur[i]-1] = ' ';
			separateur[i] = separateur[i-1];
		}
		else
		{
	XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: texte n = %s",Texte);
			Texte[separateur[i]-1] = '\n';
	XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: texte n = %s",Texte);
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

	XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: texte n = %s",Texte);

	/*si j'ai un element temps de parcours*/
	if (vl_elt_tdp!=NULL) {
	  XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: tdp = %s",vl_elt_tdp);
	  XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: nb lignes= %d",vl_nb_lignes);

	  /*suivant le nombre de lignes avant l'element temps de parcours*/
	  switch (vl_nb_lignes) {
	    case -1:
	    case 0 : sprintf(Texte,"\n%s\n",vl_elt_tdp+1);
		     nb_cr=NB_LIGNES_PMVA;
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
				XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: txt for %s",Texte);
				separateur_ini[i] = separateur_ini[i-1];
			}
			else 
				break;
		     }
		     XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: i=%d",separateur_ini[i]);
		     vl_tronque=strncpy(vl_texte_init,Texte, (size_t) separateur_ini[i]-2);
		     vl_tronque[separateur_ini[i]-1]='\0';
		     XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page: tini  %s",vl_tronque);

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
		     nb_cr=NB_LIGNES_PMVA;
		     break;
	  }
	}

	XZST_03EcritureTrace(XZSTC_WARNING, "pmva_mise_en_page final : texte %s",Texte);
	/*A
	**  on ajoute les retour chariot
	*/
	while (nb_cr < NB_LIGNES_PMVA)
	{
		strcat(Texte, "\n");
		nb_cr++;
	}

       /*A
       **  si on a plus de 3 lignes, on previent l'admistrateur systeme
       **  et on tronque pour pouvoir comparer la proposition a l'utilisation courante
       */
       if (nb_cr > NB_LIGNES_PMVA)
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
	       while (nb_cr < NB_LIGNES_PMVA)
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

extern void texte_pmva_de_base(char *Ligne)

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
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : texte_pmva_de_base" ;

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

static int etat_pmva(Horodate, NumPMV)
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
	char	l1[30]="";
	char	l2[30]="";
	char	l3[30]="";
	char	l4[30]="";
	char	l5[30]="";
	char	a1[30]="";
	char	a2[30]="";
	char	a3[30]="";
	char	a4[30]="";
	char	a5[30]="";
	XDY_Booleen	c1;
	XDY_Booleen	c2;
	XDY_Booleen	c3;
	XDY_Booleen	c4;
	XDY_Booleen	c5;
	XDY_Booleen	flash;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
/*	T_IE_SLOT	slot;*/
	int		i=0, cr=XDC_NOK;
	int		j=0,k=0;
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : etat_pmva" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ETAT_PMVA %s",version);

		/*B on regarde si on a deja fait cette requete */
	while (vg_appel_t050[i].NumPMV != 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on est dans la boucle, pour i = %d", i);

		if (vg_appel_t050[i].NumPMV == NumPMV)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on a trouve pour i = %d", i);

			cr = XDC_OK;
			dispo_texte = vg_appel_t050[i].dispo_texte;
			strcpy(l1, vg_appel_t050[i].l1);
			strcpy(l2, vg_appel_t050[i].l2);
			strcpy(l3, vg_appel_t050[i].l3);
			strcpy(l4, vg_appel_t050[i].l4);
			strcpy(l5, vg_appel_t050[i].l5);
			strcpy(a1, vg_appel_t050[i].a1);
			strcpy(a2, vg_appel_t050[i].a2);
			strcpy(a3, vg_appel_t050[i].a3);
			strcpy(a4, vg_appel_t050[i].a4);
			strcpy(a5, vg_appel_t050[i].a5);
			c1 = vg_appel_t050[i].c1;
			c2 = vg_appel_t050[i].c2;
			c3 = vg_appel_t050[i].c3;
			c4 = vg_appel_t050[i].c4;
			c5 = vg_appel_t050[i].c5;
			flash = vg_appel_t050[i].flash;
			numero_evt_U = vg_appel_t050[i].numero_evt_U;
			cle_evt_U = vg_appel_t050[i].cle_evt_U ;
			priorite_U = vg_appel_t050[i].priorite_U;
			distance_U = vg_appel_t050[i].distance_U;
			break;
		}
		i++;
	}
		/*B si ce n'est pas le cas, on appelle XZAT050 */
	if (cr != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on n'a pas trouve");

		cr = XZAT050_Utilisation_PMVA(	Horodate, NumPMV,
					&dispo_texte,
					l1, l2, l3, l4,l5,a1, a2, a3, a4,a5,&c1, &c2, &c3, &c4,&c5,&flash,
					&numero_evt_U, &cle_evt_U, &priorite_U, &distance_U);
		/*B et on enregistre le resultat */
		if ( (cr == XDC_OK) && (i < NbMaxEnr_t050) )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "PMVA on enregistre le resultat de la requete %s/%s/%s/%s/%s",l1, l2, l3, l4,l5);

			vg_appel_t050[i].NumPMV = NumPMV;
			vg_appel_t050[i].dispo_texte = dispo_texte;
			strcpy(vg_appel_t050[i].l1, l1);
			strcpy(vg_appel_t050[i].l2, l2);
			strcpy(vg_appel_t050[i].l3, l3);
			strcpy(vg_appel_t050[i].l4, l4);
			strcpy(vg_appel_t050[i].l5, l5);
			strcpy(vg_appel_t050[i].a1, a1);
			strcpy(vg_appel_t050[i].a2, a2);
			strcpy(vg_appel_t050[i].a3, a3);
			strcpy(vg_appel_t050[i].a3, a4);
			strcpy(vg_appel_t050[i].a3, a5);
			vg_appel_t050[i].c1 = c1;
			vg_appel_t050[i].c2 = c2;
			vg_appel_t050[i].c3 = c3;
			vg_appel_t050[i].c4 = c4;
			vg_appel_t050[i].c5 = c5;
			vg_appel_t050[i].flash = flash;
			vg_appel_t050[i].numero_evt_U = numero_evt_U;
			vg_appel_t050[i].cle_evt_U = cle_evt_U ;
			vg_appel_t050[i].priorite_U = priorite_U;
			if ((numero_evt_U == 0) && (priorite_U == 0))
				vg_appel_t050[i].priorite_U = priorite_U = 254;
			XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmva par evt %d prio %d",vg_appel_t050[i].numero_evt_U,vg_appel_t050[i].priorite_U );
		}
	}

	/*A
	**  traitement de la reponse
	*/
	if (cr == XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s avant maj Utilisation %s/%s/%s/%s/%s", version,l1, l2, l3, l4,l5);
		vg_Utilisation->DispoTexte = dispo_texte;
		strcpy(vg_Utilisation->L1, l1);
		strcpy(vg_Utilisation->L2, l2);
		strcpy(vg_Utilisation->L3, l3);
		strcpy(vg_Utilisation->L4, l4);
		strcpy(vg_Utilisation->L5, l5);
		strcpy(vg_Utilisation->A1, a1);
		strcpy(vg_Utilisation->A2, a2);
		strcpy(vg_Utilisation->A3, a3);
		strcpy(vg_Utilisation->A4, a4);
		strcpy(vg_Utilisation->A5, a5);
		vg_Utilisation->Flash = flash;
		vg_Utilisation->NumeroEvtU = numero_evt_U;
		vg_Utilisation->CleEvtU = cle_evt_U;
		vg_Utilisation->PrioriteU = priorite_U;
		vg_Utilisation->DistanceU = distance_U;
		if ((numero_evt_U == 0) && ((priorite_U == 0)||(priorite_U == 254))) {
			vg_Utilisation->PrioriteU = 254;
			vg_Utilisation->DistanceU = 500000;
		}
		XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmva par evt vg_Utilisation->NumeroEvtU=%d prio vg_Utilisation->PrioriteU=%d vg_Utilisation->DistanceU=%d",vg_Utilisation->NumeroEvtU,vg_Utilisation->PrioriteU ,vg_Utilisation->DistanceU);
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s maj OK",version);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"%s Etat PMVA %d inconnu a la date %lf - on le considere indispo", version, NumPMV, Horodate);
		vg_Utilisation->DispoTexte = XDC_EQT_MAJEUR;
		strcpy(vg_Utilisation->L1, "");
		strcpy(vg_Utilisation->L2, "");
		strcpy(vg_Utilisation->L3, "");
		strcpy(vg_Utilisation->L4, "");
		strcpy(vg_Utilisation->L5, "");
		strcpy(vg_Utilisation->A1, "");
		strcpy(vg_Utilisation->A2, "");
		strcpy(vg_Utilisation->A3, "");
		strcpy(vg_Utilisation->A4, "");
		strcpy(vg_Utilisation->A5, "");
		vg_Utilisation->Flash = FALSE;
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

static XDY_Booleen creer_pmva(char *Perturbation, XDY_Booleen va_SAGA)

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
*	cree le pmva s'il n'existe pas.
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
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : creer_pmva" ;
	T_PMVA vl_pmva;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : %s, pmva = %d", version,vg_Utilisation->NumPMV);

	if ( vg_Utilisation->NumPMV > vg_max_num_pmva)
		vg_max_num_pmva=(int)vg_Utilisation->NumPMV;
		
	strcpy(pmv_nom, "pmva");
	sprintf(pmv_str, "%d", vg_Utilisation->NumPMV);
	strcat(pmv_nom, pmv_str);

	if (va_SAGA)
		  vl_pmva = vg_pmva_saga[vg_Utilisation->NumPMV] ;
	else
	vl_pmva = vg_pmva[vg_Utilisation->NumPMV];
	strcpy(vl_pmva.Nom,pmv_nom);

	/*si ce PMV n'est pas encore utilise*/
	if (!vl_pmva.Utilise)
	{
		vl_pmva.Utilise=TRUE;
		vl_pmva.Identifiant = vg_Utilisation->NumPMV;
		vl_pmva.Autoroute = vg_Utilisation->AutoroutePMV;
		vl_pmva.PR = vg_Utilisation->PRPMV;
	        vl_pmva.Sens = vg_Utilisation->SensPMV;
		vl_pmva.Distance_C = vg_Utilisation->DistanceC;
		vl_pmva.Priorite_C = vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		vl_pmva.District = vg_Utilisation->DistrictPMV;
		vl_pmva.Realisable = TRUE;
		vl_pmva.PMVA_SAGA = va_SAGA;
		strcpy(vl_pmva.Nom_Pert,Perturbation);
		strcpy(vl_pmva.Direction,vg_Utilisation->DirectionPMV);
		vl_pmva.Perturbation = pm_pert;
		vl_pmva.Presence_Bifurcation = vg_Utilisation->PresenceBifurcation;
		vl_pmva.Dest_un_proche = 0;
		vl_pmva.Dest_un_loin= 0;
		vl_pmva.Dest_deux_proche = 0;
		vl_pmva.Dest_deux_loin= 0;
		if ((vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
						| XDC_EQT_CRITIQUE
						| XDC_EQT_DESACTIVE
						| XDC_EQT_HS
						| XDC_EQT_INHIBE))== 0 )
			vl_pmva.Dispo_Texte = TRUE;
		else
			vl_pmva.Dispo_Texte = FALSE;

		if (vg_Utilisation->NumeroEvtU != 0)
		{
			texte_pmva_de_base(vg_Utilisation->L1);
			texte_pmva_de_base(vg_Utilisation->L2);
			texte_pmva_de_base(vg_Utilisation->L3);
			texte_pmva_de_base(vg_Utilisation->L4);
			texte_pmva_de_base(vg_Utilisation->L5);
			texte_pmva_de_base(vg_Utilisation->A1);
			texte_pmva_de_base(vg_Utilisation->A2);
			texte_pmva_de_base(vg_Utilisation->A3);
			texte_pmva_de_base(vg_Utilisation->A4);
			texte_pmva_de_base(vg_Utilisation->A5);

			if (*(vg_Utilisation->L1)!='\0')
				strcpy(vl_pmva.Ligne1,vg_Utilisation->L1);
			else
				vl_pmva.Ligne1[0]='\0';
			if (*(vg_Utilisation->L2)!='\0')
				strcpy(vl_pmva.Ligne2,vg_Utilisation->L2);
			else
				vl_pmva.Ligne2[0]='\0';
			if (*(vg_Utilisation->L3)!='\0')
				strcpy(vl_pmva.Ligne3,vg_Utilisation->L3);
			else
				vl_pmva.Ligne3[0]='\0';
			if (*(vg_Utilisation->L4)!='\0')
				strcpy(vl_pmva.Ligne4,vg_Utilisation->L4);
			else
				vl_pmva.Ligne4[0]='\0';
			if (*(vg_Utilisation->L5)!='\0')
				strcpy(vl_pmva.Ligne5,vg_Utilisation->L5);
			else
				vl_pmva.Ligne5[0]='\0';
			if (*(vg_Utilisation->A1)!='\0')
				strcpy(vl_pmva.Alternat1,vg_Utilisation->A1);
			else
				vl_pmva.Alternat1[0]='\0';
			if (*(vg_Utilisation->A2)!='\0')
				strcpy(vl_pmva.Alternat2,vg_Utilisation->A2);
			else
				vl_pmva.Alternat2[0]='\0';
			if (*(vg_Utilisation->A3)!='\0')
				strcpy(vl_pmva.Alternat3,vg_Utilisation->A3);
			else
				vl_pmva.Alternat3[0]='\0';
			if (*(vg_Utilisation->A4)!='\0')
				strcpy(vl_pmva.Alternat4,vg_Utilisation->A4);
			else
				vl_pmva.Alternat4[0]='\0';
			if (*(vg_Utilisation->A5)!='\0')
				strcpy(vl_pmva.Alternat5,vg_Utilisation->A5);
			else
				vl_pmva.Alternat5[0]='\0';

			vl_pmva.Numero_Evt_U=vg_Utilisation->NumeroEvtU;
			vl_pmva.Cle_Evt_U=vg_Utilisation->CleEvtU;
			vl_pmva.Priorite_U=vg_Utilisation->PrioriteU;
			vl_pmva.Distance_U=vg_Utilisation->DistanceU;
			vl_pmva.Flash = vg_Utilisation->Flash;
		}
		else
		{
			vl_pmva.Numero_Evt_U=vg_Utilisation->NumeroEvtU;
			vl_pmva.Cle_Evt_U=vg_Utilisation->CleEvtU;
			vl_pmva.Priorite_U=vg_Utilisation->PrioriteU;
			vl_pmva.Distance_U=vg_Utilisation->DistanceU;
		}
		
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE(%d)", version, vg_Utilisation->NumPMV);
	if (va_SAGA)
		  vg_pmva_saga[vg_Utilisation->NumPMV] = vl_pmva;
	  else
		    vg_pmva[vg_Utilisation->NumPMV] = vl_pmva;
	 return (TRUE);
	}

	if (va_SAGA)
		  vg_pmva_saga[vg_Utilisation->NumPMV] = vl_pmva;
	  else
		    vg_pmva[vg_Utilisation->NumPMV] = vl_pmva;

	/*A
	**  sinon cela depend de la priorite de l'evt_c
	**  si l'ancien est moins prioritaire, on l'ecrase
	*/
	priorite_old = vl_pmva.Priorite_C;
	
	if (priorite_old > vg_Utilisation->PrioriteC) 
	{
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : ancien (%s %d) est moins prioritaire (%s %d), on l'ecrase",vg_pmva[vg_Utilisation->NumPMV].Nom_Pert,priorite_old,Perturbation,vg_Utilisation->PrioriteC);
		vl_pmva.Priorite_C=vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		vl_pmva.Presence_Bifurcation = vg_Utilisation->PresenceBifurcation;
		strcpy(vl_pmva.Nom_Pert,Perturbation);
		vl_pmva.Perturbation=pm_pert;
		if (va_SAGA)
		  vg_pmva_saga[vg_Utilisation->NumPMV] = vl_pmva;
		else
		  vg_pmva[vg_Utilisation->NumPMV] = vl_pmva;

		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE",version);
		return(TRUE);
	}

	/*B
	**  si les deux ont la meme priorite, la politique est ambigue :
	**  on previent l'administrateur systeme (XZIA06)
	*/
	if ((priorite_old == vg_Utilisation->PrioriteC) && (vg_Utilisation->NumeroEvtU !=0))
	{
		/* on va lire le nom de la perturbation qui utilise deja le PMV */
		/* on construit le texte du message */
		strcpy(perturbation_old,vg_pmva[vg_Utilisation->NumPMV].Nom_Pert);
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
*    recherche tous les PMVA en amont 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	les_PMVA_amont( XDY_Autoroute Autoroute,
	XDY_PR		PR,
	XDY_PR		PR_tete,
	XDY_Sens	Sens,
	XDY_Horodate	Horodate,
	XDY_District	District,
	int		Priorite,
	char		*Perturbation,
	T_PERT_PMVA	va_pert
	)

/*
*
* ARGUMENTS EN SORTIE :
*   TRUE si on a trouve au moins un PMVA, FALSE sinon
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
	int		vl_sens;
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : les_PMVA_amont" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_DEBUG1,
		"%s Autoroute = %d, PR = %d, PRTete = %d, Sens = %d, Horodate = %lf, District = %d, Priorite = %d, Perturbation = %s",
		 version,Autoroute, PR, PR_tete, Sens, Horodate, District, Priorite, Perturbation);

	/*A
	**  tant que le nieme pmv en amont existe et est a moins de Dmax,
	**  on demande le suivant.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PMVAT_Utilisation *) malloc(sizeof(PMVAT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;
	vg_Utilisation->PresenceBifurcation = FALSE;
/*        if  ( ((XDY_Sens) Sens) > 10)
		Sens=Sens-10;*/

	if (( ((XDY_Sens) Sens) != XDC_SENS_INCONNU) && (((XDY_Sens) Sens) < 10))
	{

		if ((Sens==XDC_SENS_SORTIE_NORD) ||
		    (Sens==XDC_SENS_ENTREE_NORD) ||
		    (Sens==XDC_SENS_NORD))
		     vl_sens=XDC_SENS_NORD;
		else
		     vl_sens=XDC_SENS_SUD;
                /*recherche les PMVA en amont*/
                XZAO440_Rechercher_tous_les_PMVA_amont((XDY_Autoroute) Autoroute,
                                                /* (XDY_Sens) vl_sens,*/
                                                (XDY_Sens) Sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                va_pert.Type_Zone,
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
 
	}
	else
	{
                /*recherche les PMVA en amont*/
                XZAO440_Rechercher_tous_les_PMVA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                va_pert.Type_Zone,
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
                XZAO440_Rechercher_tous_les_PMVA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_ENTREE_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                va_pert.Type_Zone,
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
 
                /*recherche les PMVA en amont*/
                XZAO440_Rechercher_tous_les_PMVA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                va_pert.Type_Zone,
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
               XZAO440_Rechercher_tous_les_PMVA_amont((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_ENTREE_SUD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_District) District,
                                                (XDY_Entier) Priorite,
                                                (XDY_Horodate) Horodate,
                                                (char *) Perturbation,
                                                va_pert.Type_Zone,
                                                (XDY_FonctionInt)ajouter_pmva,
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
*    recherche les PMV situes entre les deux PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	les_PMVA_ds_perturbation( XDY_Autoroute Autoroute,
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
	T_PERT_PMVA	va_pert)


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
*   TRUE si on a trouve au moins un pmva, FALSE sinon
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
	XDY_Mot		AutorouteInversee=1;
	XDY_Octet	n=1,i=0;
	XDY_Entier	flag=FALSE;
	int		code_retour=0;
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : les_PMVA_ds_perturbation" ;
	T_REAL8		vl_pr_min,vl_pr_max;
	int             pmv_ok=C_PMV_TROUVE;
	T_REAL8		vl_sens;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"PRTete = %d, Autoroute = %d, PR = %d, Sens = %d, Horodate = %lf, District = %d, Priorite = %d, Perturbation = %s",
		 PR_tete, Autoroute, PR, Sens, Horodate, District, Priorite, Perturbation);

	vl_pr_min = PR_tete <= PR ? PR_tete : PR;
	vl_pr_max = PR_tete > PR ? PR_tete : PR;
	/*A
	**  tant que le nieme pmv en amont existe et est dans la perturbation,
	**  on demande le suivant.
	*/
	pm_pert=va_pert;
	vg_Utilisation = (PMVAT_Utilisation *) malloc(sizeof(PMVAT_Utilisation));
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
                XZAO443_Rechercher_les_PMVA_ds_pert((XDY_Autoroute) Autoroute,
                                                /*(XDY_Sens) vl_sens,*/
                                                (XDY_Sens) Sens,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR)DISTANCE_INFINIE,
                                                (XDY_PR)DISTANCE_INFINIE,
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
                                                (XDY_FonctionInt)ajouter_pmva,
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
                XZAO443_Rechercher_les_PMVA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_PR)DISTANCE_INFINIE,
                                                (XDY_PR)DISTANCE_INFINIE,
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
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
               XZAO443_Rechercher_les_PMVA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_ENTREE_SUD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_PR)DISTANCE_INFINIE,
                                                (XDY_PR)DISTANCE_INFINIE,
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
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
 

	
                /*recherche les 1� PMV en amont en NORD*/
                XZAO443_Rechercher_les_PMVA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_NORD,
                                                (XDY_PR) PR,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR)DISTANCE_INFINIE,
                                                (XDY_PR)DISTANCE_INFINIE,
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
                                                (XDY_FonctionInt)ajouter_pmva,
                                                &flag
                                               );
               XZAO443_Rechercher_les_PMVA_ds_pert((XDY_Autoroute) Autoroute,
                                                (XDY_Sens) XDC_SENS_ENTREE_NORD,
                                                (XDY_PR) PR_tete,
                                                (XDY_PR) PR,
                                                (XDY_PR)DISTANCE_INFINIE,
                                                (XDY_PR)DISTANCE_INFINIE,
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
                                                (XDY_FonctionInt)ajouter_pmva,
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

extern int pmva_init_vg_requetes()

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
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : pmva_init_vg_requetes" ;

   pmva_init_appels_base();

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int pmva_init_appels_base()

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

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : pmva_init_appels_base");
  for (vl_i=0; vl_i < NbMaxEnr_o15 ; vl_i++) {
    vg_appel_o15[vl_i].NumPMV=0;
  }
  for (vl_i=0; vl_i < NbMaxEnr_o15 ; vl_i++) {
    vg_appel_o15_saga[vl_i].NumPMV=0;
  }

  for (vl_i=0; vl_i < NbMaxEnr_t050; vl_i++) {
    vg_appel_t050[vl_i].NumPMV=0;
  }
  for (vl_i=0; vl_i < NbMaxEnr_t050; vl_i++) {
    vg_appel_t050_saga[vl_i].NumPMV=0;
  }
  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : pmva_init_appels_base");

  for (vl_i=0; vl_i < C_NB_MAX_PMVA; vl_i++) {
    vg_pmva[vl_i].Utilise=FALSE;
  }
  for (vl_i=0; vl_i < C_NB_MAX_PMVA; vl_i++) {
    vg_pmva_saga[vl_i].Utilise=FALSE;
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
* pmva_libere_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/

extern int pmva_libere_ressources()
{
  XZST_03EcritureTrace(XZSTC_FONCTION,"pmva_libere_ressources: OUT");
  return (XDC_OK);
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
* ajouter_pmva()
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_pmva(XDY_Eqt va_numero_pmv,
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

  XZST_03EcritureTrace(XZSTC_WARNING,
	"ajouter_pmva: NumPMV = %d, DistrictPMV = %d, DistancePMV = %d, District = %d, Bifurc=%d", 
	va_numero_pmv, va_district_pmv, va_distance_pmv, va_district,va_presence_bifurc);

  vg_Utilisation->DistanceC = (float)va_distance_pmv;
  if (va_presence_bifurc==1)
    vg_Utilisation->PresenceBifurcation = TRUE;
				
  /*B
  ** on ne retient que les pmv du district dont la partie texte est disponible
  */
  if    ( (etat_pmva(va_horodate, va_numero_pmv) == XDC_OK) )
  {
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
    if ( (vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
					| XDC_EQT_CRITIQUE
					| XDC_EQT_DESACTIVE
					| XDC_EQT_HS
					| XDC_EQT_INHIBE))== 0 )
    {
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI2");
	vg_Utilisation->NumPMV = va_numero_pmv;
	vg_Utilisation->DistrictPMV = va_district_pmv;
	vg_Utilisation->AutoroutePMV = va_autoroute_pmv;
	vg_Utilisation->PRPMV = va_pr_pmv;
	vg_Utilisation->SensPMV = va_sens_pmv;
	vg_Utilisation->PrioriteC = va_priorite;	
	strcpy(vg_Utilisation->DirectionPMV,va_direction);

	vl_cr = creer_pmva(va_perturbation, XDC_FAUX);
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


extern void le_PMVA_null()
{
  return;
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche l'etat d'un PMV.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int etat_pmva_saga(Horodate, NumPMV)
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
	char	l1[30]="";
	char	l2[30]="";
	char	l3[30]="";
	char	l4[30]="";
	char	l5[30]="";
	char	a1[30]="";
	char	a2[30]="";
	char	a3[30]="";
	char	a4[30]="";
	char	a5[30]="";
	XDY_Booleen	c1;
	XDY_Booleen	c2;
	XDY_Booleen	c3;
	XDY_Booleen	c4;
	XDY_Booleen	c5;
	XDY_Booleen	flash;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
	XDY_Booleen     cligno;
	XDY_Mot lumino, barriere, scenario;
	XDY_Octet       numpicto;
	char picto[10];
	char bandeau[10];
	XDY_Octet       type_pmv;

/*	T_IE_SLOT	slot;*/
	int		i=0, cr=XDC_NOK;
	int		j=0,k=0;
	static char *version = "$Id: drol_pmva_util.c,v 1.15 2020/12/22 07:08:33 gesconf Exp $ : etat_pmva" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : ETAT_PMVA %s",version);

		/*B on regarde si on a deja fait cette requete */
	/*
	while (vg_appel_t050_saga[i].NumPMV != 0)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "on est dans la boucle, pour i = %d (%d)", i, vg_appel_t050_saga[i].NumPMV);

		if (vg_appel_t050_saga[i].NumPMV == NumPMV)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "on a trouve pour i = %d", i);

			cr = XDC_OK;
			dispo_texte = vg_appel_t050_saga[i].dispo_texte;
			strcpy(l1, vg_appel_t050_saga[i].l1);
			strcpy(l2, vg_appel_t050_saga[i].l2);
			strcpy(l3, vg_appel_t050_saga[i].l3);
			strcpy(l4, vg_appel_t050_saga[i].l4);
			strcpy(l5, vg_appel_t050_saga[i].l5);
			strcpy(a1, vg_appel_t050_saga[i].a1);
			strcpy(a2, vg_appel_t050_saga[i].a2);
			strcpy(a3, vg_appel_t050_saga[i].a3);
			strcpy(a4, vg_appel_t050_saga[i].a4);
			strcpy(a5, vg_appel_t050_saga[i].a5);
			c1 = vg_appel_t050_saga[i].c1;
			c2 = vg_appel_t050_saga[i].c2;
			c3 = vg_appel_t050_saga[i].c3;
			c4 = vg_appel_t050_saga[i].c4;
			c5 = vg_appel_t050_saga[i].c5;
			flash = vg_appel_t050_saga[i].flash;
			numero_evt_U = vg_appel_t050_saga[i].numero_evt_U;
			cle_evt_U = vg_appel_t050_saga[i].cle_evt_U ;
			priorite_U = vg_appel_t050_saga[i].priorite_U;
			distance_U = vg_appel_t050_saga[i].distance_U;
			break;
		}
		i++;
	}
	*/
		/*B si ce n'est pas le cas, on appelle XZAT050 */
	if (cr != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on n'a pas trouve");

		cr = XZAT150_Utilisation_Eqt_SAGA(	Horodate, NumPMV,
					&dispo_texte,
					l1, l2, l3, l4,l5,a1, a2, a3, a4,a5,&cligno, picto, &lumino, bandeau,
					&barriere, &scenario,
					&numero_evt_U, &cle_evt_U, &priorite_U, &distance_U, &type_pmv);
		c1 = cligno;
		c3 = cligno;
		c2 = cligno;
		c4 = cligno;
		c5 = cligno;
		flash=0;

		/*B et on enregistre le resultat */
		if ( (cr == XDC_OK) && (i < NbMaxEnr_t050) )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "PMVA on enregistre le resultat de la requete %s/%s/%s/%s/%s",l1, l2, l3, l4,l5);

			vg_appel_t050_saga[i].NumPMV = NumPMV;
			vg_appel_t050_saga[i].dispo_texte = dispo_texte;
			strcpy(vg_appel_t050_saga[i].l1, l1);
			strcpy(vg_appel_t050_saga[i].l2, l2);
			strcpy(vg_appel_t050_saga[i].l3, l3);
			strcpy(vg_appel_t050_saga[i].l4, l4);
			strcpy(vg_appel_t050_saga[i].l5, l5);
			strcpy(vg_appel_t050_saga[i].a1, a1);
			strcpy(vg_appel_t050_saga[i].a2, a2);
			strcpy(vg_appel_t050_saga[i].a3, a3);
			strcpy(vg_appel_t050_saga[i].a3, a4);
			strcpy(vg_appel_t050_saga[i].a3, a5);
			vg_appel_t050_saga[i].c1 = c1;
			vg_appel_t050_saga[i].c2 = c2;
			vg_appel_t050_saga[i].c3 = c3;
			vg_appel_t050_saga[i].c4 = c4;
			vg_appel_t050_saga[i].c5 = c5;
			vg_appel_t050_saga[i].flash = flash;
			vg_appel_t050_saga[i].numero_evt_U = numero_evt_U;
			vg_appel_t050_saga[i].cle_evt_U = cle_evt_U ;
			vg_appel_t050_saga[i].priorite_U = priorite_U;
			if ((numero_evt_U == 0) && (priorite_U == 0))
				vg_appel_t050_saga[i].priorite_U = priorite_U = 254;
			XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmva par evt %d prio %d",vg_appel_t050_saga[i].numero_evt_U,vg_appel_t050_saga[i].priorite_U );
		}
	}

	/*A
	**  traitement de la reponse
	*/
	if (cr == XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s avant maj Utilisation %s/%s/%s/%s/%s", version,l1, l2, l3, l4,l5);
		vg_Utilisation->DispoTexte = dispo_texte;
		drol_trim(l1);
		strcpy(vg_Utilisation->L1, l1);
		drol_trim(l2);
		strcpy(vg_Utilisation->L2, l2);
		drol_trim(l3);
		strcpy(vg_Utilisation->L3, l3);
		drol_trim(l4);
		strcpy(vg_Utilisation->L4, l4);
		drol_trim(l5);
		strcpy(vg_Utilisation->L5, l5);
		drol_trim(a1);
		strcpy(vg_Utilisation->A1, a1);
		drol_trim(a2);
		strcpy(vg_Utilisation->A2, a2);
		drol_trim(a3);
		strcpy(vg_Utilisation->A3, a3);
		drol_trim(a4);
		strcpy(vg_Utilisation->A4, a4);
		drol_trim(a5);
		strcpy(vg_Utilisation->A5, a5);
		vg_Utilisation->Flash = flash;
		vg_Utilisation->NumeroEvtU = numero_evt_U;
		vg_Utilisation->CleEvtU = cle_evt_U;
		vg_Utilisation->PrioriteU = priorite_U;
		vg_Utilisation->DistanceU = distance_U;
		if ((numero_evt_U == 0) && ((priorite_U == 0)||(priorite_U == 254))) {
			vg_Utilisation->PrioriteU = 254;
			vg_Utilisation->DistanceU = 500000;
		}
		XZST_03EcritureTrace(XZSTC_INFO, "Utilisation pmva par evt vg_Utilisation->NumeroEvtU=%d prio vg_Utilisation->PrioriteU=%d vg_Utilisation->DistanceU=%d",vg_Utilisation->NumeroEvtU,vg_Utilisation->PrioriteU ,vg_Utilisation->DistanceU);
		XZST_03EcritureTrace(XZSTC_MESSAGE, "%s maj OK",version);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"%s Etat PMVA %d inconnu a la date %lf - on le considere indispo", version, NumPMV, Horodate);
		vg_Utilisation->DispoTexte = XDC_EQT_MAJEUR;
		strcpy(vg_Utilisation->L1, "");
		strcpy(vg_Utilisation->L2, "");
		strcpy(vg_Utilisation->L3, "");
		strcpy(vg_Utilisation->L4, "");
		strcpy(vg_Utilisation->L5, "");
		strcpy(vg_Utilisation->A1, "");
		strcpy(vg_Utilisation->A2, "");
		strcpy(vg_Utilisation->A3, "");
		strcpy(vg_Utilisation->A4, "");
		strcpy(vg_Utilisation->A5, "");
		vg_Utilisation->Flash = FALSE;
	}
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
	return(XDC_OK);
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
* ajouter_pmva()
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_pmva_saga(XDY_Eqt va_numero_pmv,
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

  XZST_03EcritureTrace(XZSTC_WARNING,
	"ajouter_pmva_saga: NumPMV = %d, DistrictPMV = %d, DistancePMV = %d, District = %d, Bifurc=%d", 
	va_numero_pmv, va_district_pmv, va_distance_pmv, va_district,va_presence_bifurc);

  vg_Utilisation->DistanceC = (float)va_distance_pmv;
  if (va_presence_bifurc==1)
    vg_Utilisation->PresenceBifurcation = TRUE;
				
  /*B
  ** on ne retient que les pmv du district dont la partie texte est disponible
  */

  if    ( (etat_pmva_saga(va_horodate, va_numero_pmv) == XDC_OK) )
  {
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI");
    if ( (vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
					| XDC_EQT_CRITIQUE
					| XDC_EQT_DESACTIVE
					| XDC_EQT_HS
					| XDC_EQT_INHIBE))== 0 )
    {
  XZST_03EcritureTrace(XZSTC_FONCTION,"ICI2");
	vg_Utilisation->NumPMV = va_numero_pmv;
	vg_Utilisation->DistrictPMV = va_district_pmv;
	vg_Utilisation->AutoroutePMV = va_autoroute_pmv;
	vg_Utilisation->PRPMV = va_pr_pmv;
	vg_Utilisation->SensPMV = va_sens_pmv;
	vg_Utilisation->PrioriteC = va_priorite;	
	strcpy(vg_Utilisation->DirectionPMV,va_direction);

	vl_cr = creer_pmva(va_perturbation, XDC_VRAI);
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




