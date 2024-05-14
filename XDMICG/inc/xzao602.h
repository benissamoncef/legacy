/*E*/
/*Fichier :  $Id: xzao602.h,v 1.1 2008/02/13 13:24:31 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:31 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao602.h
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

#ifndef xzao602
#define xzao602

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO602_RPC_NAME "XZAO;602"

#define XZAOC_XZAO602_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_NomEqt  NomVEN;
                 XDY_NomAuto Autoroute;
                 XDY_NomEqt  NomServeur;
                 XDY_PR      PR;
                 XDY_Sens    Sens;
                 XDY_Mot     AdresseEtat;
                 XDY_Mot     AdresseCommande;
                 XDY_Mot     AdresseDiscordance;
                 XDY_Mot     AdresseTelealarme; 
               } XZAOT_ConfVEN;       

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO602_Ajouter_VEN ( XDY_Basedd,
                                 XZAOT_ConfVEN,
                                 XDY_ResConf *);



#endif
