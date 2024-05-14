/*E*/
/*Fichier :  $Id: xzac108.h,v 1.1 2008/10/07 14:26:30 devgtie Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/10/07 14:26:30 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac101_gtc.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JBL		SEPT 2008 : DEM 662, ajout type d'ouvrage 
------------------------------------------------------ */

#ifndef xzac108
#define xzac108

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC108_RPC_NAME "XZAC;101"

#define XZACC_XZAC108_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

int XZAC108_Commande_Seq_ECL (   XDY_Entier    va_NumEvt_in,
                                 XDY_Octet     va_CleEvt_in,
                                 XDY_Operateur va_Operateur_in,
                                 XDY_Octet     va_TypEqt_in,
                                 XDY_Eqt       va_NumEqt_in,
                                 XDY_Horodate  va_HeureLancement_in,
                                 XDY_Booleen   va_Manuelle_in,
                                 XDY_Octet     va_Priorite_in,
                                 XDY_Entier    va_DistanceEvt_in,
                                 XDY_Octet	va_Sequence_in);

#endif
