/*  Fichier : $Id: xzao510.h
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao510.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* GGY	16/02/24	: Creation
*
------------------------------------------------------ */
#ifndef xzao510
#define xzao510

/* fichiers inclus 						*/
/* definitions de constantes exportees	*/
#define XZAOC_XZAO510_RPC_NAME "XZAO510"
#define XZAOC_XZAO510_NB_PARM_RETURN 1

/* definitions de types exportes 		*/
typedef struct {
	XDY_Octet		Numero;
	XDY_Nom			NomBatiment;
	XDY_Abrev		Abreviation;
	XDY_NomAuto		Autoroute;
	XDY_PR			PR;
	XDY_Sens		Sens;
	XDY_Booleen		Creation;
} XZAOT_ConfBat;

/* definition de macro exportees 	*/
/* declarations de donnees globales */
/* delaration de fonctions externes */
extern int XZAO510_Ajouter_Batiment (	XDY_Basedd,
										XZAOT_ConfBat,
										XDY_ResConf *);

#endif
