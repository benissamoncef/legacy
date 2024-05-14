/*E*/
/*Fichier :  $Id: xzao603.h,v 1.1 2008/02/13 13:24:31 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:31 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao603.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ      05 Sep 2007 : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao603
#define xzao603

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO603_RPC_NAME "XZAO;603"

#define XZAOC_XZAO603_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_NomEqt  NomENR;
                 XDY_NomAuto Autoroute;
                 XDY_NomEqt  NomServeur;
                 XDY_PR      PR;
                 XDY_Sens    Sens;
                 XDY_Mot     AdresseEtat;
                 XDY_Mot     AdresseCommande;
                 XDY_Mot     AdresseDiscordance;
                 XDY_Mot     AdresseTelealarme; 
               } XZAOT_ConfENR;       

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO603_Ajouter_ENR ( XDY_Basedd,
                                 XZAOT_ConfENR,
                                 XDY_ResConf *);



#endif
