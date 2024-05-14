/*E*/
/* Fichier : $Id: xzao73.h,v 1.5 2018/11/06 14:57:45 devgfi Exp $        $Revision: 1.5 $        $Date: 2018/11/06 14:57:45 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao73.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Oct 1994	: Creation
* JPL		19/07/16	: Nom correct de la fonction  1.2
* JPL		26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.4
* JPL		06/11/18 : Procédure dégroupée  1.5
------------------------------------------------------ */

#ifndef xzao73
#define xzao73

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO73_RPC_NAME "XZAO73"

#define XZAOC_XZAO73_NB_PARM_RETURN 1

/* definitions de types exportes */
   	
typedef struct {

        XDY_TypeAlarme	Alarme;
   	XDY_TypeAlerte  Alerte;
   	XDY_NomEqt	NomETOR;

} XZAOT_ETOR;

/*typedef struct {

   	XDY_NomEqt	NomMiniGTC;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		Rgs;
   	XZAOT_ETOR	ListeETOR[32];
   	
} XZAOT_ConfMiniGTC;*/

typedef struct {

   	XDY_NomEqt	NomMiniGTC;
   	XDY_NomEqt	NomServeur;
   	XDY_Octet	Liaison;
   	XDY_Rgs		Rgs;
   	XDY_TypeAlarme	Alarme;
   	XDY_TypeAlerte  Alerte;
   	XDY_NomEqt	NomETOR;
	XDY_Booleen	Supprime;
   	
} XZAOT_ConfMiniGTC;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO73_Ajouter_MiniGTC (	XDY_Basedd,
					XZAOT_ConfMiniGTC,
					XDY_ResConf *);

#endif
