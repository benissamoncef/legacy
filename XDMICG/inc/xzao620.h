/*E*/
/*Fichier :  $Id: xzao620.h,v 1.1 2008/02/13 13:24:32 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/02/13 13:24:32 $
------------------------------------------------------
* GTIE PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao620.h
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

#ifndef xzao620
#define xzao620

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO620_RPC_NAME "XZAO;620"

#define XZAOC_XZAO620_NB_PARM_RETURN 1

/* definitions de types exportes */

typedef struct { XDY_Nom     NomSequence;
                 XDY_NomEqt  NomSIG;
                 XDY_Mot     Adresse;
                 XDY_Octet   BitComm;
                 XDY_Octet   BitDiscordance;
                 XDY_Booleen Creation;
               } XZAOT_ConfSeqAutSIG;     

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern int XZAO620_Ajouter_SeqAut_SIG ( XDY_Basedd,
                                        XZAOT_ConfSeqAutSIG,
                                        XDY_ResConf *);

#endif
