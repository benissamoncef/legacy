/*E*/
/*Fichier :  $Id: xzao623.h,v 1.1 2008/02/13 13:24:32 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:32 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao623.h
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

#ifndef xzao623
#define xzao623

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO623_RPC_NAME "XZAO;623"

#define XZAOC_XZAO623_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom     NomSequence;
                 XDY_NomEqt  NomENR;
                 XDY_Mot     Adresse;
                 XDY_Octet   BitComm;
                 XDY_Octet   BitDiscordance;
                 XDY_Booleen Creation;
               } XZAOT_ConfSeqAutENR;     

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO623_Ajouter_SeqAut_ENR ( XDY_Basedd,
                                        XZAOT_ConfSeqAutENR,
                                        XDY_ResConf *);

#endif
