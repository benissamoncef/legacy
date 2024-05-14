/*E*/
/*  Fichier : $Id: xzeo.h,v 1.2 1995/05/24 17:54:40 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/24 17:54:40 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE XZEO  * FICHIER xzeo.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Structure des donnees du fichier xzeo.c >
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille.M	version 1.1	27 Oct 1994	: Creation
* Fontaine.C	version 1.2	24 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef XZEO
#define XZEO

/* fichiers inclus  */

#include "xdc.h"
#include "xdm.h"
#include "xdy.h"
#include "xdg.h"
#include "xzec.h"

#include "xzst.h"


/* definitions de constantes exportees */

/* definitions de types exportes */
                                
/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Demande d'appel d'un intervenant par telephone ou Eurosignal BIP
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZEO01_Appel_Op_Num(XDY_NomSite 	va_nomSite,
                             XDY_Tel	va_NumOp,
                             XDY_Tel	va_NumTel);
/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumOp : no de l'Operateur a appeler
* va_NumTel : no de telephone a transmettre
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : Erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Commande d'envoi numero telephone a messagerie operateur
*
------------------------------------------------------*/



int XZEO02_Appel_Op_AlphaNum(XDY_NomSite 	va_nomSite,
                             XDY_Tel		va_NumOp,
                             XDY_Msg_ES80	va_Message);
/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_NumOp : no de l'Operateur a appeler
* va_Message : message a transmettre
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : Erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Commande d'envoi message a messagerie alphanumerique
*
------------------------------------------------------*/




int Envoyer_Mess_Oper(	XDY_NomSite 	va_nomSite,
		      	int		va_codecmd,
                        XDY_Tel		va_NumOp,
                        XDY_Msg_ES80	va_Message);
/*
* ARGUMENTS EN ENTREE :
* va_nomSite : nom du site pour construire le datagroup
* va_codecmd : code de la commande a envoyer
* va_NumOp : no de l'Operateur a appeler
* va_Message : message a transmettre
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   XDC_OK			succes
*   XDC_NOK			echec : Erreur envoi message datagroup
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Envoie le message XDM_ECMD_OPER sur le 
*   datagroup XDG_ECMD_OPER_<nomSite>
*
------------------------------------------------------*/

#endif
