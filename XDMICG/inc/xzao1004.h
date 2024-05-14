/*E*/
/*Fichier :  $Id: xzao1004.h,v 1.2 2008/10/14 14:31:31 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/10/14 14:31:31 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1004.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ      28 Nov 2007    : Creation DEM 662
* JBL      Oct 20008      : Ajout PRInfluence
------------------------------------------------------ */

#ifndef xzao1004
#define xzao1004

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1004_RPC_NAME "XZAO;1004"

#define XZAOC_XZAO1004_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    
    XDY_NomEqt    NomLit;
    XDY_NomAuto   Autoroute;
    XDY_NomEqt    NomServeur;
    XDY_PR        PR;
    XDY_PR        PRInfluence;
    XDY_Sens      Sens;
    XDY_Mot       AdresseEtat;
    XDY_Mot       AdresseCommande;
    XDY_Mot       AdresseDiscordance;
    XDY_Mot       AdresseTelealarme;
    
} XZAOT_ConfLitG;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1004_Ajouter_LIT ( XDY_Basedd,
                                  XZAOT_ConfLitG,
                                  XDY_ResConf *);



#endif
