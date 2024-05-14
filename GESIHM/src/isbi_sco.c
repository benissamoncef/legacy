/*E*/
/*Fichier : $Id: isbi_sco.c,v 1.2 2012/05/29 10:09:26 pc2dpdy Exp $      Release :  $Revision: 1.2 $       Date :  $Date: 2012/05/29 10:09:26 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  GESIHM
------------------------------------------------------
* MODULE MSCO * FICHIER isbi_sco.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier isbi_sco.h >
*
------------------------------------------------------
* HISTORIQUE :
*
-----------------------------------------------------*/

/* fichiers inclus */
#include "isbi_sco.h"
#include "isbi_sra_rafobj.h"
#include "search.h"

/* definitions de constantes */
#define CM_BUFF_SOCK 600
#define CM_TAILLE_INITIALE_HTABLE	100
#define CM_INCREMENT_HTABLE	50

/* definitions de types locaux */
typedef struct {
               char nom[256];
               float valeur;
               int   ref;
	       XDY_Horodate horodate;
               float coef[CM_NB_MAX_FENETRES];
               } tm_vagcourbe;

tm_vagcourbe *vm_vagcourbe = NULL;
int vm_vagIndiceSuivant = 0;
int vm_nb_elem_HTable = 0;
int vm_capacite_HTable = 0;

/* definition de macro locales */

/* declaration de variables locales */
int vm_fenetre[CM_NB_MAX_FENETRES] = {CM_COURBE_AUCUNE, CM_COURBE_AUCUNE, CM_COURBE_AUCUNE}; // courbe affichee dans chaque fenetre
char vl_msg[CM_BUFF_SOCK];

const char c_correspFenetre[CM_NB_MAX_FENETRES] = {'G','D','L'};

/* declaration de fonctions internes */
int MSCO_liberer_fenetre();
void MSCO_arreter_courbe();

/* definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Rappel de la courbe dans applix
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSCO_rappel_courbe(va_chaine)
							char *va_chaine;

/*
* ARGUMENTS EN ENTREE :
*  va_chaine : chaine des infos necessaires au rappel de la courbe
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sco.c	1.43 09/28/07 : MSCO_rappel_courbe " ;
int vl_type_courbe;
int vl_fenetre;
char vl_arg[20];
char vl_str[20];

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_rappel_courbe() chaine=%s", va_chaine);

	if (!strcmp(va_chaine, "L"))
		vl_fenetre = CM_FENETRE_LARGE;
	else if (!strcmp(va_chaine, "G"))
		vl_fenetre = CM_FENETRE_GAUCHE;
	else if (!strcmp(va_chaine, "D"))
		vl_fenetre = CM_FENETRE_DROITE;
	else
		XZST_03EcritureTrace(   XZSTC_WARNING, "MSCO_rappel_courbe - Type de Fenetre %s INCONNU", va_chaine);

	/*recup type de courbe*/
	vl_type_courbe = MSCO_Type_Courbe(vl_fenetre+1);

	if (vl_type_courbe != CM_COURBE_AUCUNE){
		switch (vl_type_courbe) {
			case CM_COURBE_TDP:
			case CM_COURBE_TDP_MZ:
			case CM_COURBE_INRETS:
				XZST_03EcritureTrace(   XZSTC_INFO, "MSCO_rappel_courbe - Pas de rappel possible pour ce type de courbe");
				break;
			case CM_COURBE_FT:
			case CM_COURBE_FT_REF:
				XZST_03EcritureTrace(   XZSTC_INFO, "MSCO_rappel_courbe - Courbe F(T)");
				sprintf( vl_arg , "rappel|%d" , vl_fenetre+1 );
				/*A envoie du message XDM_IAX_NEWTASK */
				MSOP_Envoyer( XDC_MACRO_MTCV , vl_arg );
				break;
			default:
				break;
		}

	}
	else
		XZST_03EcritureTrace(   XZSTC_WARNING, "MSCO_rappel_courbe - Aucune courbe à rappeler pour la fenetre %s", va_chaine);

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO_rappel_courbe");
	return XDC_OK;

}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSCO01_demarer_courbe_ft(va_type_ft,
		                     va_chaine)
							int va_type_ft;
							char *va_chaine;

/*
* ARGUMENTS EN ENTREE :
*  va_chaine : chaine des infos necessaires a l'elaboration des courbes
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sco.c	1.43 09/28/07 : MSCO01_demarer_courbe_ft " ;
tm_varcaract 	vl_varcaract[4];
XDY_Octet   	vl_nb_courbes=-1;
XDY_Booleen 	vl_tempsreel;
XZSMT_Horodate	vl_horodeb;
XDY_Octet   	vl_fenetre=-1;
XDY_Octet		vl_type_ech=-1;
XDY_Booleen		vl_periode=-1;
XDY_Texte       pl_chaine;
char  vl_tampon[30][25];
//char *vl_tampon;
int i, j;
char vl_str[20];

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO01_demarer_courbe_ft (%d) chaine=%s", va_type_ft, va_chaine);

	/*Lecture de la chaine*/
    sscanf(va_chaine, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]",
							vl_tampon[0],
							vl_tampon[1],
							vl_tampon[2],
							vl_tampon[3],
							vl_tampon[4],
							vl_tampon[5],
							vl_tampon[6],
							vl_tampon[7],
							vl_tampon[8],
							vl_tampon[9],
							vl_tampon[10],
							vl_tampon[11],
							vl_tampon[12],
							vl_tampon[13],
							vl_tampon[14],
							vl_tampon[15],
							vl_tampon[16],
							vl_tampon[17],
							vl_tampon[18],
							vl_tampon[19],
							vl_tampon[20],
							vl_tampon[21],
							vl_tampon[22],
							vl_tampon[23],
							vl_tampon[24],
							vl_tampon[25]);

    for(i=0; i < 26; i++)
    {
		XZST_03EcritureTrace(XZSTC_WARNING, "> Tampon[%d] = [%s] (lng=%d)", i, vl_tampon[i], strlen(vl_tampon[i]));
    }


    for(i=0; i < 4; i++)
	{
		j = i*5;
		XZST_03EcritureTrace(XZSTC_WARNING, "boucle %d tampon[%d]=[%s](%d)", i, j, vl_tampon[j], strlen(vl_tampon[j]));

		if (strcmp(vl_tampon[j], "-1")){
			strcpy(vl_varcaract[i].nom_station, vl_tampon[j]);
			vl_varcaract[i].no_station = atoi(vl_tampon[j+1]);
			vl_varcaract[i].mesure = atoi(vl_tampon[j+2]);
			vl_varcaract[i].voie = atoi(vl_tampon[j+3]);
			vl_varcaract[i].sens = atoi(vl_tampon[j+4]);
		}else{
			strcpy(vl_varcaract[i].nom_station, "");
			vl_varcaract[i].no_station = 0;
			vl_varcaract[i].mesure = 0;
			vl_varcaract[i].voie = 0;
			vl_varcaract[i].sens = 0;
		}
	}
    j=j+5;

	vl_nb_courbes = atoi(vl_tampon[j++]);
	vl_tempsreel = atoi(vl_tampon[j++]);
	strcpy(vl_horodeb,vl_tampon[j++]);
	vl_fenetre = atoi(vl_tampon[j++]);
	vl_type_ech = atoi(vl_tampon[j++]);
	vl_periode = atoi(vl_tampon[j]);


	XZST_03EcritureTrace(XZSTC_WARNING, "\nstation 1:%s|%d|%d|%d|%d|\nstation 2:%s|%d|%d|%d|%d|\nstation 3:%s|%d|%d|%d|%d|\nstation 4:%s|%d|%d|%d|%d|\nreste:%d|%d|%s|%d|%d|%d",
								vl_varcaract[0].nom_station,
								vl_varcaract[0].no_station,
								vl_varcaract[0].mesure,
								vl_varcaract[0].voie,
								vl_varcaract[0].sens,
								vl_varcaract[1].nom_station,
								vl_varcaract[1].no_station,
								vl_varcaract[1].mesure,
								vl_varcaract[1].voie,
								vl_varcaract[1].sens,
								vl_varcaract[2].nom_station,
								vl_varcaract[2].no_station,
								vl_varcaract[2].mesure,
								vl_varcaract[2].voie,
								vl_varcaract[2].sens,
								vl_varcaract[3].nom_station,
								vl_varcaract[3].no_station,
								vl_varcaract[3].mesure,
								vl_varcaract[3].voie,
								vl_varcaract[3].sens,
								vl_nb_courbes,
								vl_tempsreel,
								vl_horodeb,
								vl_fenetre,
								vl_type_ech,
								vl_periode);


	if (vl_fenetre < 1 || vl_fenetre > 3)
    {
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO01_demarer_courbe_ft : fenetre incorrecte : %d", vl_fenetre);
		return XDC_NOK;
    }

    if (MSCO_liberer_fenetre(vl_fenetre-1) == XDC_NOK){
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO01_demarer_courbe_ft : ABANDON");
		return XDC_NOK;
	}

    if (va_type_ft == CM_COURBE_FT)
    {
		vm_fenetre[vl_fenetre-1] = CM_COURBE_FT;
		if (MSCO01_init_courbe_temps(vl_varcaract,
								 vl_nb_courbes,
								 vl_tempsreel,
								 vl_horodeb,
								 vl_fenetre,
								 vl_type_ech,
								 vl_periode) == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "MSCO01_demarer_courbe_ft : ABANDON");
			vm_fenetre[vl_fenetre-1] = CM_COURBE_AUCUNE;
			return XDC_NOK;
		}
    }else
    {
		vm_fenetre[vl_fenetre-1] = CM_COURBE_FT_REF;
		if (MSCO_init_courbe_ft_ref(vl_varcaract,
								 vl_nb_courbes,
								 vl_tempsreel,
								 vl_horodeb,
								 vl_fenetre,
								 vl_type_ech,
								 vl_periode) == XDC_NOK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "MSCO01_demarer_courbe_ft_Ref : ABANDON");
			return XDC_NOK;
		}

    }
		return XDC_OK;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO01_demarer_courbe_ft");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSCO04_demarer_courbe_INRETS(va_chaine)
                    char *va_chaine;

/*
* ARGUMENTS EN ENTREE :
*  va_chaine : chaine des infos necessaires a l'elaboration des courbes
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sco.c	1.43 09/28/07 : MSCO04_init_courbe_INRETS " ;
char vl_date_debut[20];
int vl_pr_debut, vl_pr_fin;
int vl_mesure;
int vl_no_autoroute;
XDY_NomAuto vl_nom_autoroute;
int vl_sens;
int vl_valeur;
int vl_fenetre;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO04_demarer_courbe_INRETS chaine=%s", va_chaine);

	/*Lecture de la chaine*/
	sscanf(va_chaine, "%[^|]|%d|%d|%d|%d|%d|%[^|]|%d|%d",
			vl_date_debut,
			&vl_pr_debut,
			&vl_pr_fin,
			&vl_mesure,
			&vl_fenetre,
			&vl_no_autoroute,
			vl_nom_autoroute,
			&vl_valeur,
			&vl_sens);

	XZST_03EcritureTrace(   XZSTC_DEBUG1, "variables :\n date_debut=%s\n PR= %d a %d\n mesure=%d\n fenetre=%d\n autoroute=%d(%s)\n valuer=%d\n sens=%d",
			vl_date_debut,
			vl_pr_debut,
			vl_pr_fin,
			vl_mesure,
			vl_fenetre,
			vl_no_autoroute,
			vl_nom_autoroute,
			vl_valeur,
			vl_sens);

	//    vl_mesure = { XDC_GR_INRETS, XDC_GR_INRETS_BOUCHON, XDC_GR_BOUCHON }
    if (vl_mesure == XDC_GR_INRETS)
    	vl_fenetre = CM_FENETRE_LARGE+1;

    if (vl_fenetre < 1 || vl_fenetre > 3)
    {
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_INRETS : fenetre incorrecte : %d", vl_fenetre);
		return XDC_NOK;

    }
    if (MSCO_liberer_fenetre(vl_fenetre-1) == XDC_NOK){
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_INRETS : ABANDON");
		return XDC_NOK;
	}

    vm_fenetre[vl_fenetre-1] = CM_COURBE_INRETS;
	if (MSCO04_init_courbe_INRETS(vl_date_debut,
								vl_pr_debut,
								vl_pr_fin,
								vl_mesure,
								vl_fenetre,
								vl_no_autoroute,
								vl_nom_autoroute,
								vl_valeur,
								vl_sens) == XDC_NOK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_INRETS : ABANDON");
		vm_fenetre[vl_fenetre-1] = CM_COURBE_AUCUNE;
		return XDC_NOK;
	}

	return XDC_OK;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO04_demarer_courbe_INRETS");
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSCO04_demarer_courbe_TDP(va_no_tdp)
					XDY_Eqt va_no_tdp;

/*
* ARGUMENTS EN ENTREE :
*  va_no_tdp : info necessaire a l'elaboration des courbes
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sco.c	1.43 09/28/07 : MSCO04_demarer_courbe_TDP " ;
int vl_fenetre;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO04_demarer_courbe_TDP no_tdp=%d", va_no_tdp);

   	vl_fenetre = CM_FENETRE_LARGE+1;

    if (vl_fenetre < 1 || vl_fenetre > 3)
    {
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_TDP : fenetre incorrecte : %d", vl_fenetre);
		return XDC_NOK;

    }
    if (MSCO_liberer_fenetre(vl_fenetre-1) == XDC_NOK){
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_TDP : ABANDON");
		return XDC_NOK;
	}

    vm_fenetre[vl_fenetre-1] = CM_COURBE_TDP;
	if (MSCO04_init_courbe_tdp(va_no_tdp) == XDC_NOK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_TDP : ABANDON");
		vm_fenetre[vl_fenetre-1] = CM_COURBE_AUCUNE;
		return XDC_NOK;
	}

	return XDC_OK;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO04_demarer_courbe_TDP");
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*   Initialisation de la courbe temps
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int MSCO04_demarer_courbe_TDP_MZ(va_chaine)
					char *va_chaine;

/*
* ARGUMENTS EN ENTREE :
*  va_chaine : info necessaire a l'elaboration des courbes
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isbi_sco.c	1.43 09/28/07 : MSCO04_demarer_courbe_TDP_MZ " ;
int vl_fenetre;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO04_demarer_courbe_TDP_MZ chaine=%s", va_chaine);

   	vl_fenetre = CM_FENETRE_LARGE+1;

    if (vl_fenetre < 1 || vl_fenetre > 3)
    {
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_TDP_MZ : fenetre incorrecte : %d", vl_fenetre);
		return XDC_NOK;

    }
    if (MSCO_liberer_fenetre(vl_fenetre-1) == XDC_NOK){
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_TDP_MZ : ABANDON");
		return XDC_NOK;
	}

    vm_fenetre[vl_fenetre-1] = CM_COURBE_TDP_MZ;
	if (MSCO04_init_courbe_tdp_mz(va_chaine) == XDC_NOK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO04_demarer_courbe_TDP_MZ : ABANDON");
		vm_fenetre[vl_fenetre-1] = CM_COURBE_AUCUNE;
		return XDC_NOK;
	}

	return XDC_OK;

	XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT  : MSCO04_demarer_courbe_TDP_MZ");
}

int MSCO_liberer_fenetre(int va_fenetre_b0){

	XZST_03EcritureTrace(XZSTC_FONCTION, "IN   : MSCO_liberer_fenetre");
	if (va_fenetre_b0 < CM_FENETRE_GAUCHE || va_fenetre_b0 > CM_FENETRE_LARGE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_liberer_fenetre : fenetre incorrecte %d", va_fenetre_b0);
		return XDC_NOK;
	}


	if (va_fenetre_b0 == CM_FENETRE_LARGE)
	{
		if(vm_fenetre[CM_FENETRE_LARGE] != CM_COURBE_AUCUNE)
		{
			MSCO_arreter_courbe(va_fenetre_b0, vm_fenetre[CM_FENETRE_LARGE]);
			vm_fenetre[CM_FENETRE_LARGE] = CM_COURBE_AUCUNE;
		}
		else
		{
			if(vm_fenetre[CM_FENETRE_GAUCHE] != CM_COURBE_AUCUNE)
			{
				MSCO_arreter_courbe(va_fenetre_b0, vm_fenetre[CM_FENETRE_GAUCHE]);
				vm_fenetre[CM_FENETRE_GAUCHE] = CM_COURBE_AUCUNE;
			}
			if(vm_fenetre[CM_FENETRE_DROITE] != CM_COURBE_AUCUNE)
			{
				MSCO_arreter_courbe(va_fenetre_b0, vm_fenetre[CM_FENETRE_DROITE]);
				vm_fenetre[CM_FENETRE_DROITE] = CM_COURBE_AUCUNE;
			}
		}
	}
	else
	{
		if (va_fenetre_b0 == CM_FENETRE_GAUCHE)
			if(vm_fenetre[CM_FENETRE_GAUCHE] != CM_COURBE_AUCUNE)
			{
				MSCO_arreter_courbe(va_fenetre_b0, vm_fenetre[CM_FENETRE_GAUCHE]);
				vm_fenetre[CM_FENETRE_GAUCHE] = CM_COURBE_AUCUNE;
			}
		if (va_fenetre_b0 == CM_FENETRE_DROITE)
			if(vm_fenetre[CM_FENETRE_DROITE] != CM_COURBE_AUCUNE)
			{
				MSCO_arreter_courbe(va_fenetre_b0, vm_fenetre[CM_FENETRE_DROITE]);
				vm_fenetre[CM_FENETRE_DROITE] = CM_COURBE_AUCUNE;
			}
		if(vm_fenetre[CM_FENETRE_LARGE] != CM_COURBE_AUCUNE)
		{
			MSCO_arreter_courbe(va_fenetre_b0, vm_fenetre[CM_FENETRE_LARGE]);
			vm_fenetre[CM_FENETRE_LARGE] = CM_COURBE_AUCUNE;
		}
	}

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : MSCO_liberer_fenetre");
	return XDC_OK;
}

int MSCO_demande_arret_courbe(char va_fenetre){
int vl_fenetre;

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT   : MSCO_demande_arret_courbe : fenetre=%c", va_fenetre);

	if (va_fenetre != 'G' && va_fenetre != 'D' && va_fenetre != 'L')
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_liberer_fenetre : fenetre incorrecte %d", va_fenetre);
		return XDC_NOK;
	}

	switch (va_fenetre){
		case 'G' :
			vl_fenetre = CM_FENETRE_GAUCHE;
			break;
		case 'D' :
			vl_fenetre = CM_FENETRE_DROITE;
			break;
		case 'L' :
			vl_fenetre = CM_FENETRE_LARGE;
			break;
	}

	MSCO_liberer_fenetre(vl_fenetre);

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT   : MSCO_demande_arret_courbe");
}

void MSCO_arreter_courbe(int va_fenetre_b0, int va_type_courbe)
{
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN   : MSCO_arreter_courbe");
	switch (va_type_courbe)
	{
	case CM_COURBE_TDP:
	case CM_COURBE_TDP_MZ:
		MSCO04_arret_courbe_tdp();
		break;
	case CM_COURBE_FT:
		MSCO01_arret_courbe_temps(va_fenetre_b0+1);
		break;
	case CM_COURBE_FT_REF:
		MSCO_arret_courbe_temps_ref(va_fenetre_b0+1);
		break;
	case CM_COURBE_INRETS:
		MSCO04_arret_courbe_INRETS(va_fenetre_b0+1);
		break;
	default:
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_arreter_courbe : Courbe inconnue %d", va_type_courbe);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT  : MSCO_arreter_courbe");
}

int MSCO00_Signaler_Fin_Init(va_fenetre, va_type_courbe)
					int va_fenetre;
					int va_type_courbe;
{
	if (va_fenetre < CM_FENETRE_GAUCHE || va_fenetre > CM_FENETRE_LARGE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO00_Signaler_Fin_Init : fenetre incorrecte %d", va_fenetre);
		return XDC_NOK;
	}

        XZST_03EcritureTrace(   XZSTC_WARNING ,"fin INIT");
//	sprintf(vl_msg, "%d|%d|%c", CMD_FLASH_FIN_INIT_COURBE, vm_fenetre[va_fenetre], c_correspFenetre[va_fenetre]);
	sprintf(vl_msg, "%d|%d|%c", CMD_FLASH_FIN_INIT_COURBE, va_type_courbe, c_correspFenetre[va_fenetre]);
	if (send_message_courbe(vl_msg, CM_BUFF_SOCK) == -1)
	{
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible d'envoyer le message %s" , vl_msg);
	   return XDC_NOK;
	}
	return XDC_OK;
}

int MSCO_Type_Courbe(va_fenetre)
		int va_fenetre;
{
	int vl_fenetre = va_fenetre-1;
	XZST_03EcritureTrace(XZSTC_INFO, "MSCO_Type_Courbe : fenetre =%d", vl_fenetre);
	if (vl_fenetre < CM_FENETRE_GAUCHE || vl_fenetre > CM_FENETRE_LARGE)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "MSCO_Type_Courbe : fenetre incorrecte %d", vl_fenetre);
		return XDC_NOK;
	}
	return vm_fenetre[vl_fenetre];
}




/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Insere le nom de la varibale dans la table de hash
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Insere_vag(
          pa_nom,
          va_coef,
          va_fenetre)

          char  *pa_nom;
          float va_coef;
          int va_fenetre;


/*
* ARGUMENTS EN ENTREE :
*   pa_nom : Nom de la vag
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_Insere_vag" ;
   tm_vagcourbe  *pl_vag;
   tm_vagcourbe  *pl_tmp;
   int i;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Insere_vag %s",  pa_nom );

   if (vm_capacite_HTable == 0)
   {
	   vm_capacite_HTable = CM_TAILLE_INITIALE_HTABLE;
	   if (! (vm_vagcourbe = malloc(sizeof(tm_vagcourbe)*vm_capacite_HTable)))
	   {
	         XZST_03EcritureTrace(   XZSTC_WARNING , "MSCO_Insere_vag: Impossible de creer la table des vag" );
	         return XDC_NOK;
	   }
	   vm_vagIndiceSuivant = 0;
   }
   else if (vm_nb_elem_HTable >= vm_capacite_HTable)
   {
	   pl_vag = vm_vagcourbe;

	   if (! (pl_tmp = malloc(sizeof(tm_vagcourbe)*vm_capacite_HTable+CM_INCREMENT_HTABLE)))
	   {
	         XZST_03EcritureTrace(   XZSTC_WARNING , "MSCO_Insere_vag: Impossible d'augmenter la taille de la table des vag" );
	         return XDC_NOK;
	   }
	   vm_vagcourbe = pl_tmp;
	   //recopie
	   memcpy(vm_vagcourbe, pl_vag, sizeof(tm_vagcourbe)*vm_capacite_HTable);
	   //liberation
	   free(pl_vag);
	   vm_capacite_HTable = vm_capacite_HTable+CM_INCREMENT_HTABLE;
   }

   /*A Test de l'existence de la variable ou pas */
   i=0;
   pl_vag = NULL;
   while (i < vm_vagIndiceSuivant && pl_vag == NULL){
	   if (!strcmp(vm_vagcourbe[i].nom, pa_nom)){
		   pl_vag = &vm_vagcourbe[i];
	   }
	   i++;
   }

   if (pl_vag == NULL)
   {
      /*B La variable n'existe pas donc creation */
      /*B Insertion */
	  pl_vag = &vm_vagcourbe[vm_vagIndiceSuivant];
	  vm_vagIndiceSuivant++;
	  vm_nb_elem_HTable++;

	  /*B Mise a jour de l'element */
	  strcpy(pl_vag->nom , pa_nom );
	  pl_vag->valeur = (float) 0.0;
	  pl_vag->ref = 1;
	  pl_vag->coef[va_fenetre-1] = va_coef;
   }
   else
   {
      pl_vag->ref = pl_vag->ref + 1;
      pl_vag->coef[va_fenetre-1] = va_coef;
      XZST_03EcritureTrace(   XZSTC_WARNING , "MSCO_Insere_vag la vag %s existe deja %d fois ", pl_vag->nom, pl_vag->ref-1 );
   }


   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Insere_vag" );
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Detruit la varibale dans la table de hash
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

 MSCO_Detruire_vag(
          pa_nom,
          pa_detruite)

          char  *pa_nom;
          XDY_Booleen *pa_detruite;


/*
* ARGUMENTS EN ENTREE :
*   pa_nom : Nom de la vag
*   pa_detruite : booleen si oui ou non la vag est detruite
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 :  MSCO_Detruire_vag" ;
   tm_vagcourbe  *pl_vag;
   int i;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  :  MSCO_Detruire_vag *pa_nom_vue:%s",  pa_nom );

   /*A Test de l'existence de la variable ou pas */
   i=0;
   pl_vag = NULL;
XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Detruire_vag - rech vag %s de %d à %d", pa_nom, i, vm_vagIndiceSuivant );
   while (i < vm_vagIndiceSuivant && pl_vag == NULL){
	   if (!strcmp(vm_vagcourbe[i].nom, pa_nom)){
		   pl_vag = &vm_vagcourbe[i];
XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Detruire_vag - trouvé [%d] = %s", i, vm_vagcourbe[i].nom );
	   }else
		   i++;
   }

   if ( pl_vag == NULL)
   {
      /*B Variable inexistante */
      XZST_03EcritureTrace(   XZSTC_WARNING , " la vag %s n'existe pas ", pa_nom );
      *pa_detruite = XDC_NON;
      return XDC_NOK;
   }
   else
   {
      pl_vag->ref = pl_vag->ref - 1;
      *pa_detruite = XDC_NON;
      if ( pl_vag->ref == 0 )
      {
    	  /*B liberation de la place memoire */
XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Detruire_vag - Deplacement des autres VAG de %d à %d", i, vm_vagIndiceSuivant-1  );
    	  while (i < vm_vagIndiceSuivant-1){
    		  XZST_03EcritureTrace(   XZSTC_DEBUG1, "MSCO_Detruire_vag - %s deplacé en %d ieme position", vm_vagcourbe[i+1].nom, i);

    		  vm_vagcourbe[i] = vm_vagcourbe[i+1];
    		  i++;
    	  }
    	  vm_vagIndiceSuivant--;
    	  vm_nb_elem_HTable--;
          *pa_detruite = XDC_OUI;
      }
   }

   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT :  MSCO_Detruire_vag" );
   return XDC_OK;
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Mise a jour d'une vag courbe
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Maj_vag(
          pa_nom,
          va_valeur)

          char  *pa_nom;
          float va_valeur;


/*
* ARGUMENTS EN ENTREE :
*   pa_nom : Nom de la vag
*   va_valeur : valeur
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_Maj_vag" ;
   tm_vagcourbe  *pl_vag;
   int i;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Maj_vag *pa_nom_:%s; va_valeur = %f",  pa_nom,va_valeur );

   /*A Test de l'existence de la variable ou pas */
   i=0;
   pl_vag = NULL;
   while (i < vm_vagIndiceSuivant && pl_vag == NULL){
	   if (!strcmp(vm_vagcourbe[i].nom, pa_nom)){
		   pl_vag = &vm_vagcourbe[i];
	   }
	   i++;
   }

   if ( pl_vag == NULL)
   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de trouver la vag %s" , pa_nom);
	   return XDC_NOK;
   }else
   {
      /*A Maj de la vag */
      pl_vag->valeur = va_valeur;

   }
   return XDC_OK;
}

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Retourne la valeur de la vag
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Valeur_vag(
          pa_nom_vag,
          pa_valeur)

          char    *pa_nom_vag;
          float *pa_valeur;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_vag : Nom de la vag
*
*
* ARGUMENTS EN SORTIE :
*   pa_valeur : pointeur sur valeur vag
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 :MSCO_Valeur_vag ";
   tm_vagcourbe  *pl_vag;
   int i;
   float vl_valeur;


   /*A Test de l'existence de la variable ou pas */
   i=0;
   pl_vag = NULL;
   while (i < vm_vagIndiceSuivant && pl_vag == NULL){
	   if (!strcmp(vm_vagcourbe[i].nom, pa_nom_vag)){
		   pl_vag = &vm_vagcourbe[i];
	   }
	   i++;
   }

   if ( pl_vag == NULL)
   {
      /*A Vag inexistante ! */
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de trouver la vag %s" , pa_nom_vag);
      return XDC_NOK;
   }else
   {
      /*A Recuperation de la valeur*/
      *pa_valeur = pl_vag->valeur;
   }

   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Mise a jour horodate d'une vag courbe
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Maj_vag_horodate(
          pa_nom,
          va_valeur,va_horodate)

          char  *pa_nom;
          float va_valeur;
	XDY_Horodate va_horodate;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom : Nom de la vag
*   va_valeur : valeur
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 : MSCO_Maj_vag_horodate" ;
   tm_vagcourbe  *pl_vag;
   int i;

   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Maj_vag_horodate *pa_nom_:%s; va_valeur = %f; va_horodate=%lf",  pa_nom,va_valeur,va_horodate );

   /*A Test de l'existence de la variable ou pas */
   i=0;
   pl_vag = NULL;
   while (i < vm_vagIndiceSuivant && pl_vag == NULL){
	   if (!strcmp(vm_vagcourbe[i].nom, pa_nom)){
		   pl_vag = &vm_vagcourbe[i];
	   }
	   i++;
   }

   if ( pl_vag == NULL)
   {
	   XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de trouver la vag %s" , pa_nom);
	   return XDC_NOK;
   }else
   {
      /*A Maj de la vag */
      pl_vag->valeur = va_valeur;
	pl_vag->horodate = va_horodate;
   }
//   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCO_Maj_vag_horodate" );
   return XDC_OK;
}



/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Retourne la valeur de la vag + horodate
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

MSCO_Valeur_vag_horodate(
          pa_nom_vag,
          pa_valeur,
	  pa_horodate)

          char    *pa_nom_vag;
          float *pa_valeur;
	  XDY_Horodate *pa_horodate;

/*
* ARGUMENTS EN ENTREE :
*   pa_nom_vag : Nom de la vag
*
*
* ARGUMENTS EN SORTIE :
*   pa_valeur : pointeur sur valeur vag
*
*
* CODE RETOUR :
*   XDC_OK
*   XDC_NOK
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction
*
* FONCTION
*
*
------------------------------------------------------*/
{static char *version = "@(#)isyn_sco.c	1.43 09/28/07 :MSCO_Valeur_vag ";
   tm_vagcourbe  *pl_vag;
   int i;
   float vl_valeur;

//   XZST_03EcritureTrace(   XZSTC_FONCTION, "IN  : MSCO_Valeur_vag_horodate pa_nom_vag:%s ; " , pa_nom_vag  );

   /*A Test de l'existence de la variable ou pas */
   i=0;
   pl_vag = NULL;
   while (i < vm_vagIndiceSuivant && pl_vag == NULL){
	   if (!strcmp(vm_vagcourbe[i].nom, pa_nom_vag)){
		   pl_vag = &vm_vagcourbe[i];
	   }
	   i++;
   }

   if ( pl_vag == NULL)
   {
      /*A Vag inexistante ! */
      XZST_03EcritureTrace(   XZSTC_WARNING , "Impossible de trouver la vag %s" , pa_nom_vag);
      return XDC_NOK;
   }else
   {
      /*A Recuperation de la valeur*/
      *pa_valeur = pl_vag->valeur;
      *pa_horodate=pl_vag->horodate;
   }

//   XZST_03EcritureTrace(   XZSTC_FONCTION, "OUT : MSCP_Valeur_vag_horodate valeur:%f " , *pa_valeur);
  return (XDC_OK);
}
