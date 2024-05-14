/*E*/
/*Fichier : $Id: xzao123.h,v 1.6 2013/02/05 17:10:50 devgfi Exp $      Release : $Revision: 1.6 $        Date : $Date: 2013/02/05 17:10:50 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao123.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	31 Mai 1995	: Creation
* torregrossa	26 Oct 1995	: Ajout de l'info sens inverse de l'autoroute (A57) (1.2)
* torregrossa	28 Nov 1995	: Suppression ajout de l'info sens inverse de l'autoroute (A57) (1.3)
*                                 (repris dans xzao316.h)
* JPL		04/10/2010	: Degroupee (renommee sans ";") (pour DEM 930)  1.4
* JPL		15/12/2010	: Transfert type XZAOT_Liste_Auto depuis xzao316.h (suite DEM 930)  1.5
* JPL		05/02/2013	: Redefinition types ; ajout XZAO123_Liste_Complete_Autoroutes  1.6
------------------------------------------------------ */

#ifndef xzao123
#define xzao123

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO123_RPC_NAME "XZAO123"

#define XZAOC_XZAO123_NB_PARM_RETURN 0

/* definitions de types exportes */
typedef struct
{
	XDY_Autoroute	numero;
	XDY_NomAuto	nom;
	XDY_Mot		inversee;
	XDY_Octet	escota;
	XDY_PR		debut;
	XDY_PR		fin;
	XDY_PR		debut_concession;
	XDY_PR		fin_concession;
} XZAOT_Autoroute;

/*!!! Pour compatibilite seulement !!! */
typedef XZAOT_Autoroute	XZAOT_Liste_Auto;


/* definition de macro exportees */


/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO123_Liste_Autoroute (	XDY_Basedd,
					XDY_FonctionInt );

extern int XZAO123_Liste_Complete_Autoroutes (	XDY_Basedd,
						XDY_FonctionInt );

#endif
