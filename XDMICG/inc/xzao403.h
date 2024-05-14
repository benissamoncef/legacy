/*E*/
/* Fichier : $Id: xzao403.h,v 1.4 2018/11/05 12:52:49 devgfi Exp $        $Revision: 1.4 $        $Date: 2018/11/05 12:52:49 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao403.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer	10 Fev 1997     : Creation (DEM/1395)
* JPL		25/09/17 : Degroupee  1.2
* JPL		25/09/17 : Type du champ 'Code' change en XDY_Mot (DEM 1236)  1.3
* JPL		26/10/18 : Ajout indicateur de suppression (DEM 1305)  1.4
------------------------------------------------------ */

#ifndef xzao403
#define xzao403

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO403_RPC_NAME "XZAO403"

#define XZAOC_XZAO403_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
	
	XDY_NomEqt	Nom;
	XDY_NomAuto	Autoroute;
        XDY_PR          PR_Debut;
        XDY_PR          PR_Fin;
	XDY_Sens	Sens;
   	XDY_NomEqt	NomServeur;
   	XDY_Mot		Code;
        XDY_NomSite     Site;
        XDY_Nom         Libelle;
        XDY_TDP         TDP_Normal;
        XDY_TDP         TDP_Max;
        XDY_Entier      Seuil_Bas;
        XDY_Entier      Seuil_Haut;
        XDY_Booleen     Type_Zone;
        XDY_Booleen	Inhibition;
	XDY_Booleen	Supprime;

} XZAOT_ConfEqtZDP;		

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO403_Ajouter_Eqt_ZDP (	XDY_Basedd,
					XZAOT_ConfEqtZDP,
					XDY_ResConf * );



#endif
