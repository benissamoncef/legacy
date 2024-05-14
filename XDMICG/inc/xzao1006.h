/*E*/
/*Fichier :  $Id: xzao1006.h,v 1.2 2008/10/14 14:31:32 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/10/14 14:31:32 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1006.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ      28 Nov 2007    : Creation DEM 662
* JBL      13 Oct 2008      : Ajout PRInfluence
------------------------------------------------------ */

#ifndef xzao1006
#define xzao1006

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1006_RPC_NAME "XZAO;1006"

#define XZAOC_XZAO1006_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    
    XDY_NomEqt    NomLtn;
    XDY_NomAuto   Autoroute;
    XDY_NomEqt    NomServeur;
    XDY_PR        PR;
    XDY_PR        PRInfluence;
    XDY_Sens      Sens;
    XDY_Mot       AdresseEtat;
    XDY_Mot       AdresseCommande;
    XDY_Mot       AdresseDiscordance;
    XDY_Mot       AdresseTelealarme;
    
} XZAOT_ConfLTNG;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1006_Ajouter_LTN ( XDY_Basedd,
                                  XZAOT_ConfLTNG,
                                  XDY_ResConf *);



#endif
