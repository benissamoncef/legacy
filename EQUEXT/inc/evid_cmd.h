/*Fichier :  $Id: evid_cmd.h,v 1.8 2017/02/28 14:50:05 devgfi Exp $      Release : $Revision: 1.8 $        Date : $Date: 2017/02/28 14:50:05 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEVID * FICHIER evid_cmd.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de callbacks de la
*  tache VIDEO.
*
------------------------------------------------------
* HISTORIQUE :
*
* Magiel.E	version 1.1	02 Nov 1994	: Creation
* Nagiel.E	version 1.2	26 Dec 1994	:
* Volcic.F	version 1.3	24 Jan 1995	:
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.5	08 Jan 1999	: Modif pour Trace cmd Magnetoscope (DEM/APG16) Fiche ANA75
* Mismer.D      version 1.6     29 Mai 2000     : Modif pour basculement PC2 
* Mismer.D      version 1.7     29 Mai 2000     : Modif pour basculement PC2 Suite et fin (DEM/65)
* JPL		09/11/16 : Plus de callbacks de controle d'etat; ajout fonction de commande Systeme video (DEM 1189)  1.8
*
------------------------------------------------------ */

#ifndef evid_cmd
#define evid_cmd

/* fichiers inclus */

#include	"evid_don.h"

/* definitions de constantes exportees */

extern XDY_NomMachine		vg_NomMachine;

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

 
void	ev_cmd_VIDEO_cb ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG);

void	ev_CMD_LCR ( T_IPC_CONN			va_Cnx,
		     T_IPC_CONN_PROCESS_CB_DATA		data,
		     T_CB_ARG				arg);


/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Envoi d'une commande a un destinataire de type Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int	ev_sysv_commande_sysv (
	int		va_num_sysV,
	int		va_type_cmde,
	char		*pa_commande,
	int		va_lgr_cmde);

/*
* ARGUMENTS EN ENTREE :
*   va_num_sysV		: Identifiant du destinataire de la commande.
*   va_type_cmde	: Type de commande au systeme video.
*   pa_commande		: Donnees de la commande.
*   pl_lgr_cmde		: Taille de la commande.
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*  Fonction appelee lors de la reception du message
*  XDM_ECMD_VIDEO
*
* FONCTION 
*  Tente d'envoyer la commande a l'exemplaire du Systeme video indique.
*  En cas d'erreur, emet un avertissement et clot la connexion vers le serveur.
*
------------------------------------------------------------------------------*/

#endif
