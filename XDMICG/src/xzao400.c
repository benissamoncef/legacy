/*E*/
/*Fichier : $Id: xzao400.c,v 1.6 2017/11/15 18:43:22 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2017/11/15 18:43:22 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao400.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer       06 Fev 1997     : Creation (DEM/1395)
* Mismer       14 Fev 1997     : Ajout code zone (DEM/1395)
* JMG		02/10/08	: ajout indice et nombre dans fct util DEM/837
* JMG		05/04/16 : ajout nom eqt dans fct util TDP ASF 1.4
* JPL		26/09/17 : Redirige vers xzao4001 qui la remplace (DEM 1236)  1.6
------------------------------------------------------*/

/* fichiers inclus */

#include "xzao.h"

static char *version = "$Id: xzao400.c,v 1.6 2017/11/15 18:43:22 devgfi Exp $ : xzao400.c" ;


/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU :
*
*  Appelle XZAO400SP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZAO400_Lire_Liste_Zone_De_Parcours (	XDY_Basedd	va_Basedd_in,
						XDY_District	va_Site_in,
						XDY_FonctionInt pa_FonctionUtilisateurZDP_in,
						XDY_Entier 	*va_Resultat_out )

/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd	va_Basedd_in
* XDY_District	va_Site_in
* XDY_Fonction  pa_FonctionUtilisateurZDP_in
*
* PARAMETRES EN SORTIE :
*
* XDY_Entier	va_Resultat_out
*
* VALEUR RENDUE :
*
* Retourne la liste des zones de parcours
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Pour chaque zone de parcours appel de la fonction utilisateur avec XDY_ZDP
*
* MECANISMES :
*
------------------------------------------------------*/

{
	static char *version = "$Id: xzao400.c,v 1.6 2017/11/15 18:43:22 devgfi Exp $ : XZAO400_Lire_Liste_Zone_De_Parcours" ;

	return XZAO4001_Liste_Zones_De_Parcours ( va_Basedd_in,
	                                          va_Site_in,
	                                          pa_FonctionUtilisateurZDP_in,
	                                          va_Resultat_out );
}




