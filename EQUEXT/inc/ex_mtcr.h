/*E Fichier : $Id: ex_mtcr.h,v 1.2 1995/05/23 17:35:00 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/23 17:35:00 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mtcr.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier d'encapsulation des
*  messages LCR, communs au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	24 oct 1994	: Creation
* Fontaine.C	version 1.2	23 Mai 1995	: Reprise entete
*
------------------------------------------------------ */

#ifndef EX_MTCR
#define EX_MTCR

/* fichiers inclus */
#include <stdlib.h>
#include <stdio.h>
#include <xzex.h>
#include <xdc.h>
#include <xzst.h>

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Calcul valeur CRC de la chaine des cars. recue en argument
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	char EX_calcul_CRC( char *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_MsgTEDI	: pointeur sur chaine de caracteres contenant le message TEDI sans le CRC.
*
* ARGUMENTS EN SORTIE :
*   neant
*
* CODE RETOUR : 
*   vl_ValCRC	: la valeur du caractÉre CRC sur 7 bits significatifs.
* 
* CONDITION D'UTILISATION
*   la chaine passe en argument doit se terminer avec '\0'.
*
* FONCTION 
*   calcule la somme des caracteres de la chaine recu en argument sur 8 bits.
*
------------------------------------------------------*/
extern char EX_calcul_CRC( char * );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Encapsulation d'une trame LCR en trame TEDI/LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	int EX_encapsuler_LCR( XZEXT_Comp_Trame_TEDI *pa_BriquesTEDI, XZEXT_Msg_TEDI *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_BriquesTEDI : pointeur sur struct. donnees contenant les composants de le future trame TEDI   
*
* ARGUMENTS EN SORTIE :
*   pa_MsgTEDI	: pointeur sur structure message TEDI/LCR ( trame et sa longueur )
*
* CODE RETOUR : 
*   XDC_OK 	: si pas de probleme d'encapsulation.
*   XDC_NOK	: si requette avec no. bloc != 0 ou type message != requette, info ou acquit.
* 
* CONDITION D'UTILISATION
*   pa_BriquesTEDI->trame_LCR pointe sur une chaine de max. 247 cars. (256-247=9 cars. min. nec. pour l'encapsulation).
*   les chaines pointees par pa_BriquesTEDI->trame_LCR et pa_BriquesTEDI->adr_RGSB doivent de terminer avec '\0'.
*   si encaps. mess. TEDI acquitt. => acquitt. ( [ACK]b ou [NAK]b ) stoke ds. pa_BriquesTEDI->adr_RGSB
*
* FONCTION 
*   encapsule selon protocole TEDI/LCR pa_BriquesTEDI en pa_MsgTEDI->mesgTEDI en utilisant les cars. speciaux de
*   controle du protocole et la fonction EX_calcule_CRC().
*
------------------------------------------------------*/
extern int EX_encapsuler_LCR( 	XZEXT_Comp_Trame_TEDI *, XZEXT_Msg_TEDI * );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Mise en format lisible d'un trame TEDI/LCR
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	char * EX_affiche_TEDI( XZEXT_Msg_TEDI *pa_MsgTEDI )
*
*
* ARGUMENTS EN ENTREE :
*   pa_MsgTEDI	: pointeur sur struct. donnees contenant la trame TEDI   
*
* ARGUMENTS EN SORTIE :
*   NEANT
*
* CODE RETOUR : 
*   char * 	: pointeur sur la chaine contenant la trame TEDI en format lisible
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   met en format lisible une trame TEDI/LCR
*   
*
------------------------------------------------------*/
extern char * EX_affiche_TEDI( XZEXT_Msg_TEDI * );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Saisie au clavier d'une chaine de cars. contenant des espaces
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	void EX_saisie_cmd_LCR( char *pa_CmdLCR, int va_MaxLong )
*
*
* ARGUMENTS EN ENTREE :
*   va_MaxLong	: longueur max. de la chaine a saisir
*
* ARGUMENTS EN SORTIE :
*   pa_CmdLCR	: pointeur sur la zone mem. pour stoker la chaine saisie.
*
* CODE RETOUR : 
*   NEANT
*
* CONDITION D'UTILISATION
*   pa_CmdLCR	: pointe sur un buffer de min. va_MaxLong + 1 cars.
*
* FONCTION 
*   Saisie d'une chaine de max va_MaxLong cars. au clavier. Fin de la chaine = touche Entree.
*   
*
------------------------------------------------------*/
void EX_saisie_cmd_LCR( char *, int  );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Verification du CRC et de la structure TEDI de la chaine passee en argument.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*	int ex_verif_TEDI_CRC( XZEXT_Msg_Socket *pa_MsgSock, char *pa_DebutMsg, char *pa_NoBloc)
*
* ARGUMENTS EN ENTREE :
*
* pa_MsgSock	: pointeur sur message recu sur socket ligne serie.
* pa_DebutMsg	: debut de message ([ENQ]rgs ou [STX]rgs) a verifier.
* pa_NoBloc	: no. bloc attendu pour les acquitt.
*
* ARGUMENTS EN SORTIE :
* 
* pa_NoBloc	: indique le no. du bloc du message recu.
*
* CODE RETOUR : 
*
* XZEXC_MSG_OK ou XZEXC_BLC_OK		 : si structure TEDI correcte (cars. speciaux et CRC ).
* XZEXC_ERR_CRC_BLC ou XZEXC_ERR_CRC_MSG : si erreur CRC message ou bloc interm.
* XZEXC_ACK_OK ou XZEXC_NAK_OK		 : si acquitt. pos. ou neg. correcte.
* XZEXC_TEDI_NOK : si message receptionne sans structure TEDI ou no. bloc incorrecte pour les acquitt.
*
* CONDITION D'UTILISATION
* pa_MsgSock->msg de longueur pa_MsgSock->len.
* si le code retour est un _OK, pa_MsgSock->msg contient le msg. LCR (sans pref., suff., DDM, FDM).
*
* FONCTION 
*
* - verification du CRC.
* - verification presence des caracteres speciaux et du debut de message correcte.
* - le debut du message et donne par pa_DebutMsg - [STX]rgs pour TELMi, [ENQ]rgs pour TELEi.
*
------------------------------------------------------*/
extern int ex_verif_TEDI_CRC( XZEXT_Msg_Socket *, char *, char * );

#endif
