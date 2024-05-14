/*E*/
/*Fichier :  $Id: xzac105.h,v 1.2 2008/10/07 14:26:29 devgtie Exp $      Release : $Revision: 1.2 $        Date : $Date: 2008/10/07 14:26:29 $
------------------------------------------------------
*  * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAC * FICHIER xzac105.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Actions >
*
------------------------------------------------------
* HISTORIQUE :
*
* JBL		Nov 2007	:	DEM 662
* JBL   	Sept 2008	: Ajout du type d'ouvrage
------------------------------------------------------ */

#ifndef xzac105
#define xzac105

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZACC_XZAC105_RPC_NAME "XZAC;105"

#define XZACC_XZAC105_NB_PARM_RETURN 0

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

int XZAC105_Commande_Seq_EXP (    XDY_Entier    va_NumEvt_in,
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
