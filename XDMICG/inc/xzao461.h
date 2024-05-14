/*E*/
/*  Fichier : @(#)xzao461.h	1.2      Release : 1.2        Date : 10/11/11
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao461.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* VR       10 Nov 2011   1.1  : Creation (DEM/1014)
* VR	   27 Jui 2012	 1.2  : Lecture Nom de zones au lieu des Id
------------------------------------------------------ */

#ifndef xzao461
#define xzao461

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO461_RPC_NAME "XZAO461"

#define XZAOC_XZAO461_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_Nom		Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Nom			NomDest;
	XDY_Octet		TypeDest;
	XDY_Sens		SensDest;
	XDY_Booleen		Inhibition;
	XDY_Nom			Nom_ZDP;
	int			Init;
} XZAOT_ConfDestPMV;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO461_Ajouter_Destination_TDP (	XDY_Basedd,
					XZAOT_ConfDestPMV,
					XDY_ResConf * );



#endif
