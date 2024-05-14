/*Fichier : $Id: drol_pic_util.c
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SAIDEC
------------------------------------------------------
* MODULE DROL_PIC * FICHIER drol_pic_util.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   cf drol_pic_util.h 
*
*   NB : toute modification de l'interface des fonctions
*	de recherche des PIC doit etre repercutee sur
*	l'editeur de connaissances (modele pic_eqpt.aw)
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	01/12/23	: Creation (DEM483)
------------------------------------------------------*/

/* fichiers inclus */
#include "drol_pic_util.h"

//#include "xzao17.h"
//#include "xzao419.h"
//#include "xzao424.h"
//#include "xzao425.h"
#include "xzao5761.h"
#include "xzao5771.h"
#include "xzat511.h"

/* definitions de constantes */
#define DISTANCE_INFINIE		1000000
#define NB_LIGNES_PIC			3

#define CLASSE_PERTURBATION		"perturbation"
#define ATT_PERTURBATION_TYPE	"type"
#define ATT_PIC_PERTURBATION	"perturbation"
#define NbMaxEnr_o15			6
#define NbMaxEnr_t511			6
#define CLASSE_LOCALISATION		"localisation"

#define C_PIC_TROUVE			0
#define C_PB_PIC				1
#define C_PLUS_DE_PIC			2

/* definitions de types locaux */
typedef struct {
	XDY_Octet		DispoTexte;
	T_STRING		Picto;
	T_STRING		Flash;
	T_STRING		Bandeau;
	XDY_Entier		NumeroEvtU;
	XDY_Octet		CleEvtU;
	XDY_Octet		PrioriteU;
	XDY_Entier		DistanceU;
	XDY_Octet		PrioriteC;
	T_REAL8			DistanceC;
	int				Amont;
	XDY_Eqt			NumPIC;	
	XDY_Autoroute	AutoroutePIC;	
	XDY_PR			PRPIC;	
	XDY_Sens		SensPIC;	
	XDY_District	DistrictPIC;	
	T_STRING		DirectionPIC;
} PICT_Utilisation;

typedef struct {
	XDY_Autoroute	Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Mot			Numero;
	XDY_Eqt			NumPIC;
	XDY_District	DistrictPIC;
	XDY_Autoroute	AutoroutePIC;
	XDY_PR			PRPIC;
	XDY_Sens		SensPIC;
	XDY_Entier		DistancePIC;
} PICT_xzao15;				/* structure d'appel de XZAO15 */

typedef struct {
	XDY_Eqt		NumPIC;
	XDY_Octet	dispo_texte;
	T_STRING	Bandeau;
	T_STRING	Flash;
	T_STRING	Picto;
	XDY_Entier	numero_evt_U;
	XDY_Octet	cle_evt_U;
	XDY_Octet	priorite_U;
	XDY_Entier	distance_U;
} PICT_xzat511;				/* structure d'appel de XZAT05 */


/*definition de variables globales*/
static	T_PERT_PIC			*pm_pert=NULL;

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "drol_pic_util.c	: drol_pic_util" ;

PICT_Utilisation	*vg_Utilisation;
PICT_xzao15			vg_appel_o15[NbMaxEnr_o15];
PICT_xzat511		vg_appel_t511[NbMaxEnr_t511];
int 				vg_max_num_pic=0;
T_PIC				vg_pic[C_NB_MAX_PIC];
int					vg_priorite_retenue;
extern XDY_Mot		vg_SitePrincipal;

/* declaration de fonctions internes */
static int etat_pic();
static XDY_Booleen creer_pic();

static int ajouter_pic(	XDY_Eqt 		va_numero_pic,
                        XDY_District 	va_district_pic,
                        XDY_Autoroute 	va_autoroute_pic,
                        XDY_PR 			va_pr_pic,
                        XDY_Sens 		va_sens_pic,
                        XDY_District 	va_district,
                        int				va_priorite,
                        XDY_Horodate 	va_horodate,
                        char 			*va_perturbation,
						int				va_amont,
                        int 			*pa_resultat);

/*------------------------------------------------------
* SERVICE RENDU : 
*   verification de la longueur des lignes PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern XDY_Booleen pic_ligne_trop_longue (char *Texte, int NbCarLigne)
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
	int			i=0, n=0;
	int			code_retour=0;
	static char *version = "drol_pic_util.c	: pic_ligne_trop_longue" ;
	XZST_03EcritureTrace (XZSTC_FONCTION, "IN : %s",version);
	XZST_03EcritureTrace (XZSTC_DEBUG1, "%s Texte = %s, NbCarLigne = %lf", version, Texte, NbCarLigne);

	/* on compte les caracteres entre deux \n */
	for (i=0; i<strlen (Texte); i++)
	{
		if (Texte [i] != '\n')
			n++;
		else
			n = 0;

		if (n > NbCarLigne)
		{
			XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne TRUE", version);
			return (TRUE);
		}
	}

	/* on renvoie le resultat au Rtie */
	if (n <= NbCarLigne)
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne FALSE", version);
		return (FALSE);
	}
	else
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne TRUE", version);
		return (TRUE);
	}
}

/*------------------------------------------------------
* SERVICE RENDU : 
*   formattage d'une ligne PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void pic_tabuler_ligne (char *va_str_in, int va_nbcar, char *va_str_out)
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

  vl_len = (va_nbcar - (int) strlen (va_str_in)) / 2;

  for (vl_indice = 0;vl_indice < vl_len; vl_indice++)
  {
    va_str_out [vl_indice]=' ';
  }
  va_str_out [vl_len]='\0';
  strncat (va_str_out,va_str_in,va_nbcar);
  XZST_03EcritureTrace (XZSTC_WARNING, "pic_tabuler : LEN=%d, OUT = %s", vl_len, va_str_out);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*   mise en page d'un message PIC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern char *pic_mise_en_page (char *Texte, int NbCarLigne)
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
*    l'element suivant le '\\' tiennent sur une meme ligne PIC,
*    - par un retour chariot sinon.
*
------------------------------------------------------*/
{
	char				TexteTronque[50];
	int					i=0, j=0;
	int					separateur[4];		/* position des separateurs d'elements */
	int					separateur_ini[4];	/* position des separateurs d'elements */
	int					code_retour=0;
	int					nb_cr=0;
	char				message[50];
	XZIAT_destinataire	Destinataire;
	int					vl_position_tdp=0;
	char				vl_texte_init[50];
	int					vl_nb_lignes=0;
	char				*vl_elt_tdp;
	char				*vl_tronque;
	static char 		*version = "drol_pic_util.c	: pic_mise_en_page" ;

	XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page Texte = %s, NbCarLigne = %d", Texte, NbCarLigne);

	/* on repere les separateurs */
	strcpy (vl_texte_init, Texte);
	separateur [0] = 0;
	separateur_ini [0] = 0;
	while(Texte [i] != '\0')
	{
		if (Texte [i] == '\n')
			nb_cr++;
		if (Texte [i] == '\\')
		{
			j++;
			separateur [j] = i+1;
			separateur_ini [j] = separateur [j];
		}
		else if ( Texte [i] == '|')
			vl_position_tdp = i;
		i++;
	}

	/*recherche presence element temps de parcours*/
	vl_elt_tdp = strchr (Texte, '|');

	/* on remplace les separateurs */
	/* ceux qui sont entre deux morceaux de texte */
	for(i = 1; i < j; i++)
	{
		if ((separateur [i+1] - separateur [i-1] - 1) <= NbCarLigne)
		{
/*REPERE1			Texte[separateur[i]-1] = ' ';*/
			Texte [separateur [i] - 1] = '\n';
			separateur [i] = separateur [i -1];
		}
		else
		{
			XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: texte n = %s", Texte);
			Texte [separateur [i] - 1] = '\n';
			XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: texte n = %s", Texte);
			nb_cr++;
		}
	}	
	/* celui de la fin */
	if (j > 0)
	{
		Texte [separateur [j] - 1] = '\n';
		nb_cr++;
	}
	vl_nb_lignes = nb_cr-2;

	XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: texte n = %s", Texte);
	/* si j'ai un element temps de parcours */
	if (vl_elt_tdp != NULL)
	{
		XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: tdp = %s", vl_elt_tdp);
		XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: nb lignes= %d", vl_nb_lignes);

		/* suivant le nombre de lignes avant l'element temps de parcours */
		switch (vl_nb_lignes)
		{
			case -1:
			case 0: 
				sprintf (Texte, "\n%s\n", vl_elt_tdp + 1);
				nb_cr = NB_LIGNES_PIC;
				break;

			case 1: /* remplace le | */
				i=0;
				while (i < strlen (Texte))
				{
					if (Texte[i]=='|')
					{
						for (j=i; j < strlen(Texte); j++)
							Texte [j] = Texte [j+1];
						Texte[j]='\0';
						break;
		    	    }
				  	else
						i++;
		    	}
				break;

			default :
				for(i = 1; i < j; i++)
				{
					if ((separateur_ini [i+1] - separateur_ini [i-1] - 1) <= NbCarLigne)
					{
						Texte[separateur_ini [i] - 1] = ' ';
						XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: txt for %s",Texte);
						separateur_ini [i] = separateur_ini[i-1];
					}
					else 
						break;
				}
				
				XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: i=%d", separateur_ini[i]);
				vl_tronque = strncpy(vl_texte_init,Texte, (size_t) separateur_ini[i]-2);
				vl_tronque [separateur_ini[i]-1]='\0';
				XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: tini  %s", vl_tronque);

				i = 0;
				while (vl_tronque[i] != '\0')
				{
					/* 1er element sur 2 lignes */
					if (vl_tronque [i] == '\n')
					{
						i=-1;
						/* on ne conserve que temps de parcours */
						sprintf (Texte, "\n%s", vl_elt_tdp+1);
						break;
					}
					i++;
				}
				if (i != -1)
					sprintf (Texte, "%s\n%s", vl_tronque, vl_elt_tdp + 1);
				nb_cr  =NB_LIGNES_PIC;
				break;
		}
	}

	XZST_03EcritureTrace (XZSTC_DEBUG1, "pic_mise_en_page: texte %s", Texte);
	/* on ajoute les retour chariot */
	while (nb_cr < NB_LIGNES_PIC)
	{
		strcat(Texte, "\n");
		nb_cr++;
	}

	/* si on a plus de 3 lignes, on previent l'admistrateur systeme
	**  et on tronque pour pouvoir comparer la proposition a l'utilisation courante */
	if (nb_cr > NB_LIGNES_PIC)
	{
		/* on construit le texte du message */
		/* sprintf(message,
		 "POLITIQUE PIC - la fmc %d %d engendre un texte de plus de trois lignes : %s",
		 vg_IE_NumEvt.numero, vg_IE_NumEvt.cle, Texte);
		*/ 
		/* on appelle XZIA_06 */
		strcpy (Destinataire.machine, XDC_MACHINE_GECONF);
		XZST_03EcritureTrace (XZSTC_WARNING, "%s : %s", version, message);
		XZIA_06AffMsgSyst (Destinataire, message);

		/* on tronque le texte PIC */
		nb_cr = 0;
		i = 0;
		while (nb_cr < NB_LIGNES_PIC)
		{
			TexteTronque [i] = Texte [i];
			if (Texte[i] == '\n')
				nb_cr++;
			i++;
		}
		TexteTronque [i] = '\0';
		XZST_03EcritureTrace (XZSTC_DEBUG1, "Le texte Tronque est : %s.", TexteTronque);
		return (Texte);
	}
	
	/* on renvoie au RTie le texte mis en page. */
	XZST_03EcritureTrace (XZSTC_DEBUG1, "Texte = %s", Texte);
	return (Texte);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*    ignore la mise en page faite par les PIC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void texte_pic_de_base (char *Ligne)
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
	static char *version = "drol_pic_util.c	 : texte_pic_de_base" ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s, Ligne = %s.", version, Ligne);

	/* on supprime les espaces de debut de ligne */
	while (Ligne [j] == ' ' )
		j++;
	for (i = j; i < strlen (Ligne); i++)
		Ligne [i-j] = Ligne [i];
	Ligne [i-j] = '\0';

	/* on supprime les espaces de fin de ligne */
	i = (int) strlen (Ligne);
	for (j = i - 1; j >= 0; j--)
	{
		if ( Ligne [j] == ' ' ) 
			Ligne [j] = '\0';
		else
			break;
	}
	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s, Ligne = %s.", version, Ligne);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche l'etat d'un PIC.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static int etat_pic(Horodate, NumPIC)
XDY_Horodate	Horodate;
XDY_Eqt			NumPIC;
/*
* ARGUMENTS EN ENTREE :
*   XDY_Horodate	Horodate;
*   XDY_Eqt		NumPIC;
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
	XDY_Octet		dispo_texte;
	XDY_Octet		type_pic;
	XDY_Bandeau_PIC	Bandeau;
	XDY_Booleen		Flash;	
	XDY_Picto_PIC	Picto;
	XDY_Entier		numero_evt_U;
	XDY_Octet		cle_evt_U;
	XDY_Octet		priorite_U;
	XDY_Entier		distance_U;
	XDY_Entier		action;
	XDY_Octet 		siteAction;
	int				i=0, cr=XDC_NOK;
	int				j=0,k=0;
	static char 	*version = "drol_pic_util.c : etat_pic" ;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN : %s", version);

	/* on regarde si on a deja fait cette requete */
	while (vg_appel_t511 [i].NumPIC != 0)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on est dans la boucle, pour i = %d", i);

		if (vg_appel_t511[i].NumPIC == NumPIC)
		{
			XZST_03EcritureTrace(XZSTC_DEBUG1, "on a trouve pour i = %d", i);
			cr = XDC_OK;
			dispo_texte = vg_appel_t511[i].dispo_texte;
			strcpy (Bandeau, vg_appel_t511[i].Bandeau);
			strcpy (Flash, vg_appel_t511[i].Flash);
			strcpy (Picto, vg_appel_t511[i].Picto);
			numero_evt_U = vg_appel_t511[i].numero_evt_U;
			cle_evt_U = vg_appel_t511[i].cle_evt_U ;
			priorite_U = vg_appel_t511[i].priorite_U;
			distance_U = vg_appel_t511[i].distance_U;
			break;
		}
		i++;
	}
	
	/* si ce n'est pas le cas, on appelle XZAT05 */
	if (cr != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1, "on n'a pas trouve");

		cr = XZAT511_Utilisation_PIC(	Horodate, NumPIC,
										&dispo_texte, 
										Bandeau, &Flash, Picto,
										&numero_evt_U, &cle_evt_U, &priorite_U, &distance_U,&action, &siteAction);
		XZST_03EcritureTrace(XZSTC_WARNING, "apres XZAT511_Utilisation_PIC");
		
		/* et on enregistre le resultat */
		if ( (cr == XDC_OK) && (i < NbMaxEnr_t511) )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "on enregistre le resultat de la requete");

			vg_appel_t511[i].NumPIC = NumPIC;
			vg_appel_t511[i].dispo_texte = dispo_texte;
			strcpy(vg_appel_t511[i].Bandeau, Bandeau);
			strcpy(vg_appel_t511[i].Flash, &Flash);
			strcpy(vg_appel_t511[i].Picto, Picto);
			vg_appel_t511[i].numero_evt_U = numero_evt_U;
			vg_appel_t511[i].cle_evt_U = cle_evt_U ;
			vg_appel_t511[i].priorite_U = priorite_U;
			vg_appel_t511[i].distance_U = distance_U;
			if ((numero_evt_U == 0) && ((priorite_U == 0)||(priorite_U == 254)))
			{
				vg_appel_t511[i].priorite_U = priorite_U = 254;
				vg_appel_t511[i].distance_U = distance_U = 500000;
			}
			XZST_03EcritureTrace(XZSTC_WARNING, "Utilisation pic par evt %d prio %d dist %d",
						vg_appel_t511[i].numero_evt_U,
						vg_appel_t511[i].priorite_U ,
						vg_appel_t511[i].distance_U);
			XZST_03EcritureTrace(XZSTC_WARNING, "Bandeau %s Flash %s Picto %s",
						vg_appel_t511[i].Bandeau,
						vg_appel_t511[i].Flash,
						vg_appel_t511[i].Picto);

			/* on supprime les espaces de fin de ligne pour le picto*/
			j= (int) strlen (Picto);
			for (k=j-1;k>=0;k--)
			{
				if ( Picto[k] == ' ' ) 
					Picto[k] = '\0';
				else
					break;
			}
			strcpy (vg_appel_t511[i].Picto, Picto);
		}
	}

	/* traitement de la reponse */
	if (cr == XDC_OK)
	{
		vg_Utilisation->DispoTexte = dispo_texte;
		strcpy (vg_Utilisation->Bandeau, Bandeau);
		strcpy (vg_Utilisation->Flash, &Flash);
		strcpy (vg_Utilisation->Picto, Picto);
		vg_Utilisation->NumeroEvtU = numero_evt_U;
		vg_Utilisation->CleEvtU = cle_evt_U;
		vg_Utilisation->PrioriteU = priorite_U;
		vg_Utilisation->DistanceU = distance_U;
		if ((numero_evt_U == 0) && (priorite_U == 0))
		{
			vg_Utilisation->PrioriteU = 254;
			vg_Utilisation->DistanceU = 500000;
		}
		XZST_03EcritureTrace (XZSTC_INFO, "Utilisation pic par evt vg_Utilisation->NumeroEvtU=%d prio vg_Utilisation->PrioriteU=%d vg_Utilisation->DistanceU=%d",vg_Utilisation->NumeroEvtU,vg_Utilisation->PrioriteU ,vg_Utilisation->DistanceU);
		XZST_03EcritureTrace (XZSTC_MESSAGE, "%s maj OK Bandeau=%s, Flash=%s, Picto=%s." ,version,vg_Utilisation->Bandeau, vg_Utilisation->Flash, vg_Utilisation->Picto);
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "%s Etat PIC %d inconnu a la date %lf - on le considere indispo", version, NumPIC, Horodate);
		vg_Utilisation->DispoTexte = XDC_EQT_MAJEUR;
		strcpy (vg_Utilisation->Bandeau, "");
		strcpy (vg_Utilisation->Flash, "");
		strcpy (vg_Utilisation->Picto, "");
	}
	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s", version);
	return (XDC_OK);
}

/*------------------------------------------------------
* SERVICE RENDU : 
*    cree un PIC dans le RTie.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
static XDY_Booleen creer_pic (char *Perturbation)
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
*	cree le pic s'il n'existe pas.
*	met a jour les attributs.
*
------------------------------------------------------*/
{
	/*T_IE_CLASS	pic_classe;*/
	char	pic_str [30];
	char	pic_nom [30];
	int		priorite_old;
	char	nom_objet [30];
	char	perturbation_old [100];
	/*T_IE_SLOT	slot;*/
	XZIAT_destinataire      Destinataire;
	char		message[ 1000];
	static char *version = "drol_pic_util.c	: creer_pic" ;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "IN : %s, pic = %d", version, vg_Utilisation->NumPIC);
	if ( vg_Utilisation->NumPIC > vg_max_num_pic)
		vg_max_num_pic=(int)vg_Utilisation->NumPIC;
		
	strcpy (pic_nom, "pic");
	sprintf (pic_str, "%d", vg_Utilisation->NumPIC);
	strcat (pic_nom, pic_str);

	strcpy(vg_pic[vg_Utilisation->NumPIC].Nom,pic_nom);

	/*si ce PIC n'est pas encore utilise*/
	if (!vg_pic[vg_Utilisation->NumPIC].Utilise)
	{
		vg_pic[vg_Utilisation->NumPIC].Utilise=TRUE;
		vg_pic[vg_Utilisation->NumPIC].Identifiant = vg_Utilisation->NumPIC;
		vg_pic[vg_Utilisation->NumPIC].Autoroute = vg_Utilisation->AutoroutePIC;
		vg_pic[vg_Utilisation->NumPIC].PR = vg_Utilisation->PRPIC;
		vg_pic[vg_Utilisation->NumPIC].Sens = vg_Utilisation->SensPIC;
		vg_pic[vg_Utilisation->NumPIC].Distance_C = vg_Utilisation->DistanceC;
		vg_pic[vg_Utilisation->NumPIC].Priorite_C = vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		vg_pic[vg_Utilisation->NumPIC].District = vg_Utilisation->DistrictPIC;
		vg_pic[vg_Utilisation->NumPIC].Amont = vg_Utilisation->Amont;
		vg_pic[vg_Utilisation->NumPIC].Realisable = TRUE;
		strcpy(vg_pic[vg_Utilisation->NumPIC].Bandeau, "");
		vg_pic[vg_Utilisation->NumPIC].Flash = 0;
		strcpy(vg_pic[vg_Utilisation->NumPIC].Picto, "");
		strcpy(vg_pic[vg_Utilisation->NumPIC].Nom_Pert,Perturbation);
		vg_pic[vg_Utilisation->NumPIC].Perturbation = pm_pert;

		if ((vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
						| XDC_EQT_CRITIQUE
						| XDC_EQT_DESACTIVE
						| XDC_EQT_HS
						| XDC_EQT_INHIBE))== 0 )
			vg_pic[vg_Utilisation->NumPIC].Dispo_Texte = TRUE;
		else
			vg_pic[vg_Utilisation->NumPIC].Dispo_Texte = FALSE;

		/* PNI 24/01/07 : Memorisation de l'utilisation */
		vg_pic[vg_Utilisation->NumPIC].Numero_Evt_U=vg_Utilisation->NumeroEvtU;
		vg_pic[vg_Utilisation->NumPIC].Cle_Evt_U=vg_Utilisation->CleEvtU;
		vg_pic[vg_Utilisation->NumPIC].Priorite_U=vg_Utilisation->PrioriteU;
		vg_pic[vg_Utilisation->NumPIC].Distance_U=vg_Utilisation->DistanceU;

		if (vg_Utilisation->NumeroEvtU != 0)
		{
			texte_pic_de_base(vg_Utilisation->Bandeau);
			texte_pic_de_base(vg_Utilisation->Picto);
			if (*(vg_Utilisation->Bandeau)!='\0')
				strcpy(vg_pic[vg_Utilisation->NumPIC].Bandeau, vg_Utilisation->Bandeau);
			else
				strcpy(vg_pic[vg_Utilisation->NumPIC].Bandeau, "");
			if (*(vg_Utilisation->Picto)!='\0')
				strcpy(vg_pic[vg_Utilisation->NumPIC].Picto, vg_Utilisation->Picto);
 			else
				strcpy(vg_pic[vg_Utilisation->NumPIC].Picto, "");
			vg_pic[vg_Utilisation->NumPIC].Flash = vg_Utilisation->Flash;
			vg_pic[vg_Utilisation->NumPIC].Numero_Evt_U = vg_Utilisation->NumeroEvtU;
			vg_pic[vg_Utilisation->NumPIC].Cle_Evt_U = vg_Utilisation->CleEvtU;
			vg_pic[vg_Utilisation->NumPIC].Priorite_U = vg_Utilisation->PrioriteU;
			vg_pic[vg_Utilisation->NumPIC].Distance_U = vg_Utilisation->DistanceU;
		}
		
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne TRUE(%d)", version, vg_Utilisation->NumPIC);
		return(TRUE);
	}

	/* sinon cela depend de la priorite de l'evt_c
	**  si l'ancien est moins prioritaire, on l'ecrase */
	priorite_old = vg_pic[vg_Utilisation->NumPIC].Priorite_C;
	if (priorite_old > vg_Utilisation->PrioriteC) 
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s : ancien est moins prioritaire, on l'ecrase");
		vg_pic [vg_Utilisation->NumPIC].Priorite_C = vg_Utilisation->PrioriteC;
		vg_priorite_retenue = vg_Utilisation->PrioriteC;
		strcpy (vg_pic[vg_Utilisation->NumPIC].Nom_Pert, Perturbation);
		vg_pic[vg_Utilisation->NumPIC].Perturbation = pm_pert;
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne TRUE", version);
		return (TRUE);
	}

	/* si les deux ont la meme priorite, la politique est ambigue :
	**  on previent l'administrateur systeme (XZIA06) */
	if (priorite_old == vg_Utilisation->PrioriteC)
	{
		/* on va lire le nom de la perturbation qui utilise deja le PIC */
		/* on construit le texte du message */
		strcpy(perturbation_old,vg_pic[vg_Utilisation->NumPIC].Nom_Pert);
		sprintf(message, "POLITIQUE PIC AMBIGUE : la fmc %d %d engendre les perturbations \"%s\" et \"%s\", toutes deux de priorite %d, et qui cherchent toutes deux a utiliser le PIC du PR %d",
			vg_Utilisation->NumeroEvtU,vg_Utilisation->CleEvtU, perturbation_old, Perturbation,
			vg_Utilisation->PrioriteC, vg_Utilisation->PRPIC);

		/* on appelle XZIA_06 */
		strcpy(Destinataire.machine, XDC_MACHINE_GECONF);
		XZST_03EcritureTrace(XZSTC_WARNING, "%s : %s",version,message);
		XZIA_06AffMsgSyst(Destinataire, message);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s retourne FALSE",version);
	return(FALSE);	
}

/*------------------------------------------------------
* SERVICE RENDU : 
*    recherche les PIC situes entre les deux PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void	les_PIC_ds_perturbation (	XDY_Autoroute Autoroute,
										XDY_PR			PR,
										XDY_PR			PR_tete,
										XDY_Sens		Sens,
										XDY_Horodate	Horodate,
										XDY_District	District,
										int				Priorite,
										char			*Perturbation,
										T_PERT_PIC		*va_pert)
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
*   TRUE si on a trouve au moins un pic, FALSE sinon
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION 
*	cree un objet dans la classe PIC par PIC candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PIC, FALSE sinon.
*
------------------------------------------------------*/
{
	T_REAL8		tmp;
	XDY_Mot		AutorouteInversee;
	XDY_Octet	n=1,i=0;
	XDY_Entier	flag=FALSE;
	int			code_retour=0;
	static char *version = "drol_pic_util.c	: les_PIC_ds_perturbation" ;
	T_REAL8		vl_pr_min,vl_pr_max;
	int			pic_ok=C_PIC_TROUVE;
	T_REAL8		vl_sens;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"%s PRTete = %d, Autoroute = %d, PR = %d, Sens = %d, Horodate = %lf, District = %d, Priorite = %d, Perturbation = %s",
		 version, PR_tete, Autoroute, PR, Sens, Horodate, District, Priorite, Perturbation);

	vl_pr_min = PR_tete <= PR ? PR_tete : PR;
	vl_pr_max = PR_tete > PR ? PR_tete : PR;
	/* tant que le nieme pic en amont existe et est dans la perturbation,
	**  on demande le suivant. */
	pm_pert=va_pert;
	vg_Utilisation = (PICT_Utilisation *) malloc(sizeof(PICT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;

	if ((Sens==XDC_SENS_SORTIE_NORD) ||
	    (Sens==XDC_SENS_ENTREE_NORD) ||
	    (Sens==XDC_SENS_NORD))
	     vl_sens=XDC_SENS_NORD;
	else
	     vl_sens=XDC_SENS_SUD;

	/*recherche les 1e PIC en amont*/
	XZAO5761_PIC_chantier(	(XDY_Autoroute) Autoroute,
							(XDY_Sens) vl_sens,
							(XDY_PR) PR,
							(XDY_PR) PR_tete,
							(XDY_District) District,
							(XDY_Entier) Priorite,
							(XDY_Horodate) Horodate,
							(char *) Perturbation,
							(XDY_FonctionInt)ajouter_pic,
							&flag);

	XZST_03EcritureTrace(XZSTC_FONCTION,"apres XZAO5761_PIC_chantier");
	free(vg_Utilisation);

	/* on renvoie le flag au RTie */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s  retourne %d", version, flag);
	return;
}

/*------------------------------------------------------
* SERVICE RENDU :
*    recherche les PIC situes entre les deux PR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void	les_PIC_du_basculement (	XDY_Autoroute	Autoroute,
										XDY_PR			PR,
										XDY_PR			PR_tete,
										XDY_Sens		Sens,
										XDY_Horodate	Horodate,
										XDY_District	District,
										int				Priorite,
										char			*Perturbation,
										T_PERT_PIC		*va_pert)
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
*   TRUE si on a trouve au moins un pic, FALSE sinon
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du RTie
*
* FONCTION
*	cree un objet dans la classe PIC par PIC candidat.
*	met a jour leur disponibilite et leur utilisation.
*	retourne TRUE si on a trouve au moins un PIC, FALSE sinon.
*
------------------------------------------------------*/
{
	T_REAL8		tmp;
	XDY_Mot		AutorouteInversee;
	XDY_Octet	n=1,i=0;
	XDY_Entier	flag=FALSE;
	int			code_retour=0;
	static char *version = "drol_pic_util.c : les_PIC_du_basculement" ;
	T_REAL8		vl_pr_min,vl_pr_max;
	int	pic_ok=C_PIC_TROUVE;
	T_REAL8		vl_sens;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s",version);

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"%s PRTete = %lf, Autoroute = %lf, PR = %lf, Sens = %lf, Horodate = %lf, District = %lf, Priorite = %lf, Perturbation = %s",
		 version, PR_tete, Autoroute, PR, Sens, Horodate, District, Priorite, Perturbation);

	vl_pr_min = PR_tete <= PR ? PR_tete : PR;
	vl_pr_max = PR_tete > PR ? PR_tete : PR;
	/* tant que le nieme pic en amont existe et est dans la perturbation,
	**  on demande le suivant. */
	pm_pert=va_pert;
	vg_Utilisation = (PICT_Utilisation *) malloc(sizeof(PICT_Utilisation));
	vg_Utilisation->DistanceC = 0.0;

	if ((Sens==XDC_SENS_SORTIE_NORD) ||
	    (Sens==XDC_SENS_ENTREE_NORD) ||
	    (Sens==XDC_SENS_NORD))
	     vl_sens=XDC_SENS_NORD;
	else
		vl_sens=XDC_SENS_SUD;
	
	/*recherche les 1e PIC en amont*/
	XZAO5771_PIC_basculement(	(XDY_Autoroute) Autoroute,
								(XDY_Sens) vl_sens,
								(XDY_PR) PR,
								(XDY_PR) PR_tete,
								(XDY_District) District,
								(XDY_Entier) Priorite,
								(XDY_Horodate) Horodate,
								(char *) Perturbation,
								(XDY_FonctionInt) ajouter_pic,
								&flag);
	free(vg_Utilisation);

	/* on renvoie le flag au RTie */
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s  retourne %d", version, flag);
	return;
}

/*------------------------------------------------------
* SERVICE RENDU : 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int pic_init_vg_requetes ()
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
*   	Mise a jour des variables globales vg_appel_o15 et vg_appel_t511
*
------------------------------------------------------*/
{
	static char *version = "drol_pic_util.c	1.14 : pic_init_vg_requetes" ;
	pic_init_appels_base ();
	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s", version);
	return (XDC_OK);
}

/*------------------------------------------------------
* SERVICE RENDU :
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern int pic_init_appels_base ()
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
*       Mise a jour des variables globales vg_appel_o15 et vg_appel_t511
*
------------------------------------------------------*/
{ 
	int vl_i;

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : pic_init_appels_base");
	for (vl_i=0; vl_i < NbMaxEnr_o15 ; vl_i++)
	{
		vg_appel_o15[vl_i].NumPIC = 0;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : pic_init_appels_base NbMaxEnr_t511");

	for (vl_i=0; vl_i < NbMaxEnr_t511; vl_i++)
	{
		vg_appel_t511[vl_i].NumPIC = 0;
	}

	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : pic_init_appels_base C_NB_MAX_PIC");

	for (vl_i=0; vl_i < C_NB_MAX_PIC; vl_i++)
	{
		vg_pic[vl_i].Utilise=FALSE;
		vg_pic[vl_i].Perturbation=(T_PERT_PIC *)NULL;
		strcpy(vg_pic[vl_i].Bandeau,"");
		vg_pic[vl_i].Flash = 0;
		strcpy(vg_pic[vl_i].Picto,"");
	}
	vg_priorite_retenue = 0;

	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : pic_init_appels_base");

	return(XDC_OK);
}

/******************************************************
* DESCRIPTION DE LA FONCTION :
* libere les ressources
*
******************************************************
* SEQUENCE D'APPEL :
* pic_libere_ressources();
*
* Arguments
*
* Codes retour
*
* Mecanismes
*
******************************************************/
extern int pic_libere_ressources ()
{
  XZST_03EcritureTrace (XZSTC_FONCTION, "pic_libere_ressources: OUT");
  return (XDC_OK);
}

/*****************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZAD_TDP
******************************************************
* DESCRIPTION DE LA FONCTION :
* fct utilisateur passee dans XZAO419
* utilisee pour creer un PIC
*
******************************************************
* SEQUENCE D'APPEL :
* ajouter_pic)
*
* Arguments
*
* Codes retour
*
* Mecanismes
* Pointeur de fonction.
******************************************************/
static int ajouter_pic(	XDY_Eqt 		va_numero_pic,
						XDY_District	va_district_pic,
						XDY_Autoroute 	va_autoroute_pic,
						XDY_PR 			va_pr_pic,
						XDY_Sens		va_sens_pic,
						XDY_District	va_district,
						int				va_priorite,
						XDY_Horodate	va_horodate,
						char			*va_perturbation,
						int				va_amont,
						int				*pa_resultat)
{
	T_BOOL vl_cr=FALSE;

	/* on ne retient que les pic du district dont la partie texte est disponible */
	if (((va_district_pic == va_district) || (va_district == vg_SitePrincipal)) && (etat_pic(va_horodate, va_numero_pic) == XDC_OK))
	{
		if ( (vg_Utilisation->DispoTexte & ( XDC_EQT_MAJEUR
											| XDC_EQT_CRITIQUE
											| XDC_EQT_DESACTIVE
											| XDC_EQT_HS
											| XDC_EQT_INHIBE)) == 0)
		{
			vg_Utilisation->NumPIC = va_numero_pic;
			vg_Utilisation->DistrictPIC = va_district_pic;
			vg_Utilisation->AutoroutePIC = va_autoroute_pic;
			vg_Utilisation->PRPIC = va_pr_pic;
			vg_Utilisation->SensPIC = va_sens_pic;
			vg_Utilisation->PrioriteC = va_priorite;	
			vg_Utilisation->Amont  = va_amont;	
			vl_cr = creer_pic(va_perturbation);
		}
    	/* on se place en file d'attente si le PIC est indispo et pas site CI */
		else if (va_district_pic != XDC_CI)
		/* Ne s'applique pas sur COFIROUTE, uniquement sur ESCOTA */
		/* Sur COFIROUTE, le site principal peut piloter les PMV */
		{
			/*XZAP21_Placer_Evt_File_Attente(	vg_IE_NumPA, va_numero_pic, XDC_EQT_PIC,
			va_priorite, va_distance_pic);*/
			vl_cr = 0; /*temporaire a virer JMG*/
		}
	}
	*pa_resultat=vl_cr;
	return XDC_OK;
}

extern void le_PIC_null ()
{
  return;
}

/*------------------------------------------------------
* SERVICE RENDU :
*  teste si il y a une sortie entre le PIC et le debut de la FMC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern XDY_Booleen util_pic_amont_sortie (	int Dist_PIC,
											int Sens_PIC,
											int	Dist_sortie_amont,
											int	Dist_sortie_aval,
											int	Sens_fmc)
/*
* ARGUMENTS EN ENTREE :
*
* ARGUMENTS EN SORTIE :
*   T_BOOL      sortie entre PIC et FMC
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*   fonction appelee depuis les regles du Rtie
*
* FONCTION
*   renvoie TRUE si il y a une sortie entre le PIC et la FMC.
*
------------------------------------------------------*/
{
	static char *version = "drol_pic_util.c : util_pic_amont_sortie" ;

	/* si le sens de la FMC est inconnu et si le PIC est en sens sud, le PR de tete devient le PR de queue */
	if ( (XDY_Sens) Sens_fmc == XDC_SENS_INCONNU && (XDY_Sens) Sens_PIC == XDC_SENS_SUD)
		Dist_sortie_amont = Dist_sortie_aval;

	/* si Dist_sortie_amont est nulle c'est qu'il n'y a pas de sortie amont ( REPERE1 ) alors on retourne FALSE */
	if ( Dist_sortie_amont == 0 )
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne FALSE car pas de sortie amont (dist=0) ", version);
		return (FALSE);
		/*Dist_sortie_amont = Dist_sortie_aval;*/
	}
	
	/* on compare les distances */
	if ( Dist_PIC > Dist_sortie_amont)
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne TRUE", version);
		return (TRUE);
	}
	else 
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : %s retourne FALSE", version);
		return (FALSE);
	}
}

/*------------------------------------------------------
* SERVICE RENDU :
*  teste si un PIC est en amont d'une FMC
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern XDY_Booleen util_pic_amont_fmc (	int	PR_PIC,
										int	Sens_PIC,
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
*   renvoie TRUE si le PIC est en amont de la FMC, FALSE sinon.
*
------------------------------------------------------*/
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "util_pic_amont_fmc %d,%d,%d,%d,%d,%d",
										PR_PIC,
										Sens_PIC,
										PR_Queue,
										PR_Tete,
										Sens_fmc,
										Autoroute_inversee);
	
	/* si le sens de la FMC est inconnu et si le PIC est en sens sud, le PR de tete devient le PR de queue */
	if ((XDY_Sens) Sens_fmc == XDC_SENS_INCONNU && (XDY_Sens) Sens_PIC == XDC_SENS_SUD)
		PR_Queue = PR_Tete;

	/* le sens sud est le sens des PR croissant (sauf si autoroute inversee) */
	if ((((XDY_Sens) Sens_PIC == XDC_SENS_SUD) && (PR_PIC*Autoroute_inversee < PR_Queue*Autoroute_inversee))
		|| (((XDY_Sens) Sens_PIC == XDC_SENS_NORD) && (PR_PIC*Autoroute_inversee > PR_Queue*Autoroute_inversee))) 
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : util_pic_amont_fmc retourne TRUE");
		return (TRUE);
	}
	else
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : util_pic_amont_fmc retourne FALSE");
		return (FALSE);
	}
}

//XDY_Entier vitesseTextToVitesse (XDY_VitesseRegulTxt v_vitesseText)
//{
//	XDY_Entier vl_vitesse=0;
//
//	if(!strncmp(v_vitesseText , "L50",3))
//		vl_vitesse = 50;
//	else if(!strncmp(v_vitesseText , "L70",3))
//		vl_vitesse = 70;
//	else if(!strncmp(v_vitesseText , "L90",3))
//		vl_vitesse = 90;
//	else if(!strncmp(v_vitesseText , "L110",4))
//		vl_vitesse = 110;
//	else if(!strncmp(v_vitesseText , "L130",4))
//		vl_vitesse = 130;
//
//	return vl_vitesse;
//}

//void vitesseToVitesseText(XDY_Entier va_vitesseIn, XDY_VitesseRegulTxt va_vitesseOut)
//{
//        XDY_VitesseRegulTxt vl_vitesseText="";
//
//        if(va_vitesseIn == 50)
//                strcpy(va_vitesseOut, "L50");
//        else if(va_vitesseIn == 70)
//                strcpy(va_vitesseOut , "L70");
//        else if(va_vitesseIn == 90)
//                strcpy(va_vitesseOut , "L90");
//        else if(va_vitesseIn == 110)
//                strcpy(va_vitesseOut , "L110");
//        else if(va_vitesseIn == 130)
//                strcpy(va_vitesseOut , "L130");
//}
