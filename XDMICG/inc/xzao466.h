/*E*/
/* Fichier : $Id: xzao466.h,v 1.1 2012/11/08 09:54:24 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/11/08 09:54:24 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao466.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       05 Juin 2012     : Creation (DEM/1014 PMA)
------------------------------------------------------ */

#ifndef xzao466
#define xzao466

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO466_RPC_NAME "XZAO466"

#define XZAOC_XZAO466_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

typedef struct {
	XDY_Nom			Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Nom			NomDest;
	XDY_Octet		TypeDest;
	XDY_Octet		SensDest;
	XDY_Booleen		Inhibition;
	XDY_Nom			ZDP;
	int				Init;
} XZAOT_ConfDestPMA;

/* delaration de fonctions externes */

extern int XZAO466_Ajouter_Destination_TDP_PMA (	XDY_Basedd,
					XZAOT_ConfDestPMA,
					XDY_ResConf * );



#endif
