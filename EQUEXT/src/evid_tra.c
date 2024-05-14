/*E*/
/* Fichier : $Id: evid_tra.c,v 1.15 2017/02/28 14:54:12 devgfi Exp $      Release : $Revision: 1.15 $        Date : $Date: 2017/02/28 14:54:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_tra.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*   Module de traitement des trames provenant des eqt VIDEO
*
------------------------------------------------------
* HISTORIQUE :
*
* Nagiel.E	version 1.1	04 Sep 1994	: Creation
* Nagiel.E	version 1.2	23 Dec 1994	:
* Nagiel.E	version 1.3	26 Dec 1994	:
* Mismer.D	version 1.4	17 Jan 1995	:
* Volcic.F	version 1.5	24 Jan 1995	:
* Mismer.D	version 1.6	09 Fev 1995	:
* Mismer.D	version 1.7	18 Mai 1995	: Modif gestion compte rendu commande, reprise historique
* Volcic.F	version 1.8	03 Sep 1995	: Modif traces camera ou magnetoscope non trouve
* Mismer.D	version 1.9	01 Fev 1995	: Modif recherche camera affectee
* Mismer.D	version 1.10	24 Avr 1997	: Commande magneto EQT2 (DEM/1427)
* Mismer.D	version 1.11	20 Jan 1998	: Modif pour basculement PC2
* Hilmarcher    version 1.12                    : modif pour utilisation cam pc niv 2 depuis CI
* Hilmarcher    version 1.13    24/11/2004      : modif pour dem 380
* JMG   13/11/07        : ajout site de gestion DEM715 
* JPL		04/11/16 : Lecture et traitement d'une reponse du Systeme video (DEM 1189)  1.15
------------------------------------------------------*/

/* fichiers inclus */

#include "evid_tra.h"

/* definitions de constantes */

/* Status et code associe retournes par le Systeme video */
#define		CM_STATUS_ACK		"ACK"
#define		CM_STATUS_NAK		"NAK"

#define		CM_CODE_OK		0
#define		CM_CODE_CMD_ERR		1
#define		CM_CODE_CAM_INV		11
#define		CM_CODE_MON_INV		12
#define		CM_CODE_POS_INV		13
#define		CM_CODE_ERR_OPE		21


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_tra.c,v 1.15 2017/02/28 14:54:12 devgfi Exp $ $Revision: 1.15 $ : evid_dir" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Decodage et traitement de la reponse a une commande.
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int em_traitement_trame ( int va_socket, int va_type_cmde, int *va_cr_reception )

/*
* ARGUMENTS EN ENTREE :
*   va_num_sysV		: Identifiant du destinataire de la commande.
*
* ARGUMENTS EN SORTIE :
*   va_cr_reception	: Compte-rendu de la recepction du message (OK / NOK).
*
*
* CODE RETOUR : 
*
* CONDITION D'UTILISATION
*  Sur reception d'un message de reponse d'un systeme video
*
* FONCTION 
*
*  Lire le message recu et le traiter selon la commande envoyee.
*
------------------------------------------------------------------------------*/
{
	XZEXT_MSG_SOCKET	vl_Msg;				/* Donnees du message recu */
	int			vl_LgMsg;

	char			vl_status[32] = "";		/* status textuel extrait */
	int			vl_codeErr = (-1);		/* code numerique extrait */

	char			*pl_espace;
	int			vl_lg_status = 0;
	int			vl_lg_code = 0;
	int			vl_cr;

	static char *version = "$Id: evid_tra.c,v 1.15 2017/02/28 14:54:12 devgfi Exp $ $Revision: 1.15 $ : em_traitement_trame" ;

	XZST_03EcritureTrace (XZSTC_FONCTION, " IN : %s ", version);

	/*A Lire le message recu dans la socket */
	sem_wait(&vg_semaphore); 
	do {
		vl_cr = (int) read ( va_socket, (void *) vl_Msg, sizeof(vl_Msg) - 1 );
	} while ( (vl_cr < 0)  &&  (errno == EINTR) );
	sem_post(&vg_semaphore); 

	if (vl_cr >= 0) {
		*va_cr_reception = XDC_OK;
		vl_LgMsg = vl_cr;
		vl_Msg[vl_LgMsg] = XDC_FIN_CHN;
	}

	/* En cas d'echec */
	if (vl_cr <= 0) {
		XZST_03EcritureTrace (XZSTC_WARNING, "Erreur a la lecture de la reponse a la commande %d (%d/%d).",
		                         va_type_cmde, vl_cr, errno);
		*va_cr_reception = XDC_NOK;
		XZST_03EcritureTrace (XZSTC_FONCTION, " OUT : %s ", version);
		return (XDC_NOK);
	}


	/*A Tracer et analyser la reponse */
	XZST_03EcritureTrace (XZSTC_MESSAGE, "Reponse recue :%s.", vl_Msg);

	/* Si l'espace separateur est trouve */
	if ( NULL != (pl_espace = strchr (vl_Msg, ' ')) ) {
		vl_lg_status = (pl_espace - vl_Msg);
		vl_lg_code = strspn (pl_espace + 1, "1234567890");

		/* et est suivi d'un code numerique et seulement de ce code alors */
		if ( (vl_lg_code > 0)  &&  (vl_lg_status + 1 + vl_lg_code == vl_LgMsg) ) {
			if (vl_lg_status < sizeof(vl_status)) {
				/* extraire le status et le code numerique */
				strncpy (vl_status, vl_Msg, vl_lg_status);
				vl_status[vl_lg_status] = XDC_FIN_CHN;
				vl_codeErr = atoi (pl_espace + 1);
			}
		}
	}


	/*A Verifier la validite de la reponse */
	if ( (0 != strcmp (vl_status, CM_STATUS_ACK))  &&  (0 != strcmp (vl_status, CM_STATUS_NAK)) ) {
		XZST_03EcritureTrace (XZSTC_WARNING, "Reponse incomprehensible :%s.", vl_Msg);
		XZST_03EcritureTrace (XZSTC_FONCTION, " OUT : %s ", version);
		return (XDC_NOK);
	}

	switch (va_type_cmde) {
		case XDC_AFFECT_IMAG_JOYSTICK :
		case XDC_CMD_POS :
			if ( (0 != strcmp (vl_status, CM_STATUS_ACK)) ) {
				XZST_03EcritureTrace (XZSTC_WARNING, "Reponse NAK a commande d'affectation :%s.", vl_Msg);
			} else if (CM_CODE_OK != vl_codeErr) {
				XZST_03EcritureTrace (XZSTC_WARNING, "Reponse NAK a commande de positionnement :%s.", vl_Msg);
			}
			break;


		case EVIDC_DEMANDE_ETAT :
			if ( (0 != strcmp (vl_status, CM_STATUS_ACK))  ||  (CM_CODE_OK != vl_codeErr) ) {
				XZST_03EcritureTrace (XZSTC_WARNING, "Reponse a demande d'etat inattendue :%s.", vl_Msg);
			}
			break;
	}

	XZST_03EcritureTrace (XZSTC_FONCTION, " OUT : %s ", version);
	return (XDC_OK);
}
