/*E Fichier : $Id: elei_pro.h,v 1.3 1995/05/23 09:45:44 cornu Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/05/23 09:45:44 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELEi * FICHIER elei_pro.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Structure des donnees du fichier protocole de la tache TELEi.
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	26 Oct 1994	: Creation
* Flaidar.A	version 1.2	02 Dec 1994	:
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise entete, historique
*
------------------------------------------------------ */

#ifndef elei_pro
#define elei_pro

/* fichiers inclus */

#include "elei_mdon.h"
#include "ex_mtcr.h"

/* definitions de constantes exportees */
#define EPROC_TXT_ALM_TELEi	"ALM. TELEI"

/* definitions de types exportes */

/* type local EPRO qui decrit les conditions d'envoi d'une trame TEDI/LCR */
typedef struct EPROT_Conditions
{
    int		nb_essais;	/* condition = nbr. essais d'envoi trame TEDI/LCR */
    int		nb_err_crc;	/* nbr. errs. CRC detectees */
    int		nb_non_rep;	/* nbre. non reponses aux trames envoyees */
    int		val_timer;	/* valeur timer pour attente reponse */
}EPROT_Conditions;

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* Traitement des messages equipements maitres.
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*  void epro_traitement_msg_equip_maitre( XZEXT_Msg_Socket *pa_MsgSock )
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: message recu sur la socket a verifier.
*
* ARGUMENTS EN SORTIE :
*
* pa_MsgSock	: pa_MsgSock->msg contient le trame LCR si le message est correcte TEDI.
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
* 
* pa_MsgSock	: doit contenir le message recu sur la socket ligne serie.
*
* FONCTION 
*
* - verification du message contnu ds. pa_MsgSock->msg.
* - seule une REQUETTE ( [ENQ]rgs0 ... [ETX][BCC] ) est reconnue comme message correcte 
*   d'un point de vue TEDI.
* - toute autre trame est une erreur et XDC_NOK est retourne.
*
------------------------------------------------------*/
extern void epro_traitement_msg_equip_maitre( XZEXT_Msg_Socket * );

#endif
