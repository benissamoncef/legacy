/*E*/
/*Fichier :  $Id: xzao1008.h,v 1.2 2008/10/14 14:31:32 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/10/14 14:31:32 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao1008.h
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

#ifndef xzao1008
#define xzao1008

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO1008_RPC_NAME "XZAO;1008"

#define XZAOC_XZAO1008_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct {
    
    XDY_NomEqt    NomSys;
    XDY_NomAuto   Autoroute;
    XDY_NomEqt    NomServeur;
    XDY_PR        PR;
    XDY_PR        PRInfluence;
    XDY_Sens      Sens;
    XDY_Mot       AdresseEtat;
    XDY_Mot       AdresseCommande;
    XDY_Mot       AdresseDiscordance;
    XDY_Mot       AdresseTelealarme;
    
} XZAOT_ConfSYSG;        

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO1008_Ajouter_SYS ( XDY_Basedd,
                                  XZAOT_ConfSYSG,
                                  XDY_ResConf *);



#endif
