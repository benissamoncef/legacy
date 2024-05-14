/*E*/
/* Fichier : $Id: xzao467.h,v 1.1 2012/11/08 09:54:24 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/11/08 09:54:24 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao467.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR	05/06/12: Creation (DEM/1014 PMA)
------------------------------------------------------ */

#ifndef xzao467
#define xzao467

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO467_RPC_NAME "XZAO467"

#define XZAOC_XZAO467_NB_PARM_RETURN 0

/* definitions de types exportes */

typedef struct {
	XDY_Eqt		NumeroPMA;
	XDY_Eqt		Num_Destination_un_proche;
	XDY_Nom		Nom_Destination_un_proche;
	XDY_Eqt		Num_Destination_un_lointaine;
	XDY_Nom		Nom_Destination_un_lointaine;
	XDY_Eqt		Num_Destination_deux_proche;
	XDY_Nom		Nom_Destination_deux_proche;
	XDY_Eqt		Num_Destination_deux_lointaine;
	XDY_Nom		Nom_Destination_deux_lointaine;

} XZAOT_PMA_DEST_TDP;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO467_Lire_Liste_PMVA(	XDY_Basedd,
					XDY_NomEqt,
					XDY_FonctionInt,
					XDY_Entier * );

#endif
