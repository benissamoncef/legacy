/*E Fichier : $Id: eimi_pro.h,v 1.1 2019/01/29 11:05:55 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2019/01/29 11:05:55 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TELMi * FICHIER eimi_pro.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier protocole de la
*  tache TELMi.
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   01/03/18 :      Creation CTRL_FEUX 1.1 DEM1284
*
------------------------------------------------------ */

#ifndef eimi_pro
#define eimi_pro

/* fichiers inclus */

#include "eimi_mdon.h"
#include "ex_mtcr.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* type local EPRO qui decrit les conditions d'envoi d'une trame TEDI/LCR */
typedef struct EPROT_Conditions
{
    int		nb_essais;	/* condition = nbr. essais d'envoi trame TEDI/LCR */
    int		nb_err_crc;	/* nbr. errs. CRC detectees */
    int		nb_non_rep;	/* nbre. non reponses aux trames envoyees */
    double	val_timer;	/* valeur timer pour attente reponse */
}EPROT_Conditions;

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Envoi d'une trame TEDI/LCR suite a une demande d'un client TEzzz
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_envoi_trame_LCR(  XZEXT_Cmde_LCR *pa_CmdeLCR )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a envoyer.
*
* ARGUMENTS EN SORTIE :
* NEANT
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. LCR a envoyer.
*
* FONCTION 
*
* - envoi trame LCR et attente reponse
* - empilement des eventuelles demandes des clients TEzzz ds. une liste lors
*   de l'attente de la reponse au mesage TEDI/LCR
*
------------------------------------------------------*/
extern void epro_envoi_trame_LCR_IP(  XZEXT_Cmde_LCR_IP *, XDY_Booleen );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement d'un message recu sur la socket comms. avec le serveur de terminaux.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  int epro_traitement_msg_lser(  XZEXT_Cmde_LCR *pa_CmdeLCR , 
*				XZEXT_Msg_Socket *pa_MsgSock,
*				EPROT_Conditions *pa_Cond,
*				XZEXT_Msg_TEDI  *pa_MsgTEDI )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR dont on recoit la reponse sur la ligne serie.
* pa_MsgSock	: pointeur sur le message recu sur la socket comms; avec la ligne serie geree.
* pa_Cond	: pointeur sur la structure des conds. pour gerer le protocole.
* pa_MsgTEDI	: pointeur sur le message TEDI a envoyer en cas d'erreur CRC bloc intermed.
*
* ARGUMENTS EN SORTIE :
* 
* pa_Cond	: les conditions mises a jour en fonction de la trame TEDI/LCR recue.
* pa_MsgTEDI	: msg. TEDI (acquit. neg. a envoyer en cas de err. CRC blc. interm. 
*
* CODE RETOUR : 
*
* XDC_OK	: si pas d'erreur CRC ou TEDI detectee.
* XDC_NOK	: sinon.
*
* CONDITION D'UTILISATION
* pa_CmdeLCR doit contenir les composantes de la cmde. LCR dont on recoit la reponse.
* pa_MsgSock doit contenir la reponse a pa_CmdeLCR.
*
* FONCTION 
*
* - detection type reponse ( message bloc unique, bloc intermediaire, aquittement ).
* - si c'est le cas, envoi compte rendu a la atche TEzzz ayant envoye pa_CmdeLCR.
*
------------------------------------------------------*/
extern int epro_traitement_msg_lser(  XZEXT_Cmde_LCR * , 
				XZEXT_Msg_Socket *,
				EPROT_Conditions *,
				XZEXT_Msg_TEDI *);



/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement d'un message recu sur une sock. comms. avec un client TEzzz.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void epro_traitement_msg_donn( XZEXT_Msg_Socket *pa_MsgSock )
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: pointeur sur le message recu sur la socket donnees active.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR : 
*
*   NEANT.
*
* CONDITION D'UTILISATION
*
* pa_MsgSock doit contenir un message de la part d'un client TEzzz.
*
* FONCTION 
* - si demande arret appel ecfg_sortir().
* - si demande init appel ecom_init().
* - si demande LCR :
*  	depouillement du message recu du client TEzzz ds. une struct type XZEXT_Cmde_LCR.
*  	empilement de cette meme demande ds. la liste des demandes a traiter.
*
------------------------------------------------------*/
void epro_traitement_msg_donn( XZEXT_Msg_Socket * );


#endif
