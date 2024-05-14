/*E Fichier : $Id: xzca.h,v 1.2 1995/01/25 10:29:18 mercier Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/25 10:29:18 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GECONF
------------------------------------------------------
* MODULE ZCA * FICHIER ZCA.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	06 Jan 1995	: Creation
************
* MERCIER	12 Jan 1995	: Ajout de xzca_Sauvegarder_Hebdo	V 1.2
------------------------------------------------------ */

#ifndef ZCA
#define ZCA

/* fichiers inclus */
	/* include system */
	#include        <stdio.h>
	#include        <string.h>
	#include        <ctype.h>

	/* Include service migrazur */
	 #include "xdc.h"
	 #include "xdy.h"
	 #include "xdm.h"
	 #include "xdf.h"
	 #include "xzst.h"
	 #include "xzsc.h"

/* definitions de constantes exportees */
#define 	XZCAC_ERR_LECTURE	-1
#define 	XZCAC_ERR_ANNEE		-1
#define 	XZCAC_ERR_MOIS		-2
/* definitions de types exportes */

typedef struct
{
	XDY_Entier	BandeID;
	XDY_Entier      NumCopie;
}XZCAT_ComposantCAT;

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*
*	xzca01_Lecture_Catalogue : prend un argument une fonction utilisateur
* que l on va appeler pour chaque occurence du fichier du catalogue d archivage.
*/

extern int xzca01_Lecture_Catalogue (XDY_FonctionInt , int *);
/*
*	xzca02_Sauvegarder_Archive permet de declencher l ordre de
*	sauvegarde du mois de l annee passes en argument . 
*	Algo : 	Verification de la valeur de l annee et du mois recu recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CSV_ARC.
*/
extern int xzca02_Sauvegarder_Archive (XDY_Entier, XDY_Entier);

/*
*	xzca03_Restaurer_Archive permet de declencher l ordre de
*	retauration du mois de l annee passe en argument . 
*	Algo : 	Verification de la valeur de l annee et du mois recu recue,
*	constitue un msg RTwks et envoie le message RTWks XDM_CSV_RES.
*
*/

extern int xzca03_Restaurer_Archive (XDY_Entier, XDY_Entier);

/*
* Fonction permettant de simuler le declenchement de la base CFG en globale
* et de la base histo en incrementale
*/
extern int xzca_Sauvegarder_Hebdo ();

#endif
