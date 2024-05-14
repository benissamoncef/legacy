/*E*/
/*Fichier :  $Id: xzao621.h,v 1.1 2008/02/13 13:24:32 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:32 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao621.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* AAZ     06 Sep 2007 : Creation DEM 662
------------------------------------------------------ */

#ifndef xzao621
#define xzao621

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO621_RPC_NAME "XZAO;621"

#define XZAOC_XZAO621_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom     NomSequence;
                 XDY_NomEqt  NomECL;
                 XDY_Mot     Adresse;
                 XDY_Octet   BitComm;
                 XDY_Octet   BitDiscordance;
                 XDY_Booleen Creation;
               } XZAOT_ConfSeqAutECL;     

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO621_Ajouter_SeqAut_ECL ( XDY_Basedd,
                                        XZAOT_ConfSeqAutECL,
                                        XDY_ResConf *);

#endif
