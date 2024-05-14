/*E*/
/*Fichier :  $Id: xzao601.h,v 1.1 2008/02/13 13:24:30 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:30 $
------------------------------------------------------
* GTIEPROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao601.h
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

#ifndef xzao601
#define xzao601

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO601_RPC_NAME "XZAO;601"

#define XZAOC_XZAO601_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_NomEqt  NomECL;
                 XDY_NomAuto Autoroute;
                 XDY_NomEqt  NomServeur;
                 XDY_PR      PR;
                 XDY_Sens    Sens;
                 XDY_Mot     AdresseEtat;
                 XDY_Mot     AdresseCommande;
                 XDY_Mot     AdresseDiscordance;
                 XDY_Mot     AdresseTelealarme; 
               } XZAOT_ConfECL;       

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO601_Ajouter_ECL ( XDY_Basedd,
                                 XZAOT_ConfECL,
                                 XDY_ResConf *);



#endif
