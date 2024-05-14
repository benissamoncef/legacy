/*E*/
/* Fichier : $Id: evid_com.c,v 1.1 2017/02/08 18:32:47 devgfi Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/02/08 18:32:47 $
--------------------------------------------------------------------------------
* ESCOTA *  PROJET MIGRAZUR
--------------------------------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
--------------------------------------------------------------------------------
* MODULE TEVID * FICHIER evid_com.c
--------------------------------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module de communication avec les serveurs du Systeme Video.
*
--------------------------------------------------------------------------------
* HISTORIQUE :
*
* JPL	02/11/16	: Creation (DEM 1189)  1.1
*
------------------------------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "evid_com.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: evid_com.c,v 1.1 2017/02/08 18:32:47 devgfi Exp $ : evid_com" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */



/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Connexion a l'un des points d'acces du Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_sysv_connecter ( int  va_NumSysVideo )

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumSysVideo	: Identifiant de l'exemplaire a connecter.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_ARG_INV si un parametre est invalide.
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Si le point d'acces est configure et si la connexion n'est pas etablie
*   alors tenter la connexion au serveur et en cas de succes,
*   mettre a jour les donnees de liaison.
*
------------------------------------------------------------------------------*/
{
	EVID_DONNEES_SYSTVIDEO	*pl_sysVideo;
	int			vl_cr;
	int			vl_sockSysVid;

	/* Verifier les parametres d'entree */
	if ( (va_NumSysVideo != EVIDC_SYSV1)  &&  (va_NumSysVideo != EVIDC_SYSV2) )
	{
		return (XZEXC_ARG_INV);
	}
	pl_sysVideo = &vg_sysvid[va_NumSysVideo];

	/* Verifier que la configuration est valide */
	if ( ! estConfigure (va_NumSysVideo) )
	{
		return (XDC_NOK);
	}

	/* Verifier si la connexion est deja etablie */
	if ( estConnecte (va_NumSysVideo) )
	{
		return (XDC_OK);
	}

	/* Connecter au serveur declare */
	vl_cr = ex_cnx_client_ST ( pl_sysVideo->Config.Nom_machine,
	                              pl_sysVideo->Config.No_port,
	                              &pl_sysVideo->Masque,
	                              &vl_sockSysVid);

	return (vl_cr);
}





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Purger les donnees eventuelles en provenance du Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_sysv_purger ( int  va_NumSysVideo )

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumSysVideo	: Identifiant de l'exemplaire a purger.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_ARG_INV si un parametre est invalide.
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Lit toutes donnees en arrivee (avec un delai maximal) sur le canal
*   de communication avec le serveur, si la liaison est etablie.
*
------------------------------------------------------------------------------*/
{
	XZEXT_MSG_SOCKET	vl_message;
	int			vl_nb_car_lus = 0;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN: ec_sysv_purger");

	/* Verifier les parametres d'entree */
	if ( (va_NumSysVideo != EVIDC_SYSV1)  &&  (va_NumSysVideo != EVIDC_SYSV2) )
	{
		return (XZEXC_ARG_INV);
	}

	/* Verifier que la configuration est valide */
	if ( ! estConfigure (va_NumSysVideo) )
	{
		return (XZEXC_ARG_INV);
	}

	/* Verifier si la connexion est etablie */
	if ( ! estConnecte (va_NumSysVideo) )
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT: ec_sysv_purger (non connecte)");
		return (XDC_OK);
	}

	do {
		vl_nb_car_lus = 0;
		/* Si la lecture ne bloquerait pas alors lire toutes donnees en attente */
		if ( ex_select ( vg_sysvid[va_NumSysVideo].Masque->desc_sock, (double) 0.010 ) == XDC_OK ) {
			sem_wait(&vg_semaphore); 
			do {
				vl_nb_car_lus = read (vg_sysvid[va_NumSysVideo].Masque->desc_sock, (void *) vl_message, sizeof(vl_message));
			} while ( (vl_nb_car_lus < 0)  &&  (errno == EINTR) );
			sem_post(&vg_semaphore); 
		}
	} while (vl_nb_car_lus > 0);

	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT: ec_sysv_purger");
	return (XDC_OK);
}





/*X*/
/*------------------------------------------------------------------------------
* SERVICE RENDU :
*
*  Ecriture d'un message vers un point d'acces au Systeme video.
*
--------------------------------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int	ec_sysv_ecrire ( int va_NumSysVideo, char *pa_Message, int va_LgrMsg )

/*
* ARGUMENTS EN ENTREE :
*
*   va_NumSysVideo	: Identifiant de l'exemplaire a qui envoyer le message ;
*   va_Message		: Donnees du message ;
*   va_LgrMsg		: Longueur du message.
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   XDC_ARG_INV si un parametre est invalide.
*   XDC_OK en cas de succes, XDC_NOK sinon.
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Ecrit les donnees sur le canal de communication avec le Systeme video
*   jusque envoi complet du message ou erreur d'ecriture.
*
------------------------------------------------------------------------------*/
{
	int			vl_nbCarEcrits = 0;
	int			vl_cr;

	XZST_03EcritureTrace (XZSTC_FONCTION, "IN: ec_sysv_ecrire");

	/* Verifier les parametres d'entree */
	if ( (va_NumSysVideo != EVIDC_SYSV1)  &&  (va_NumSysVideo != EVIDC_SYSV2) )
	{
		return (XZEXC_ARG_INV);
	}

	if ( va_LgrMsg <= 0 )
	{
		return (XZEXC_ARG_INV);
	}

	/* Verifier si la connexion est etablie */
	if ( ! estConnecte (va_NumSysVideo) )
	{
		XZST_03EcritureTrace (XZSTC_FONCTION, "OUT: ec_sysv_ecrire (non connecte)");
		return (XDC_NOK);
	}

	do {
		/* Ecrire sur le canal de communication avec le serveur les donnees restantes du message */
		sem_wait(&vg_semaphore); 
		do {
			vl_cr = (int) write ( vg_sysvid[va_NumSysVideo].Masque->desc_sock,
			                         (void *) (pa_Message + vl_nbCarEcrits),
			                         (size_t) (va_LgrMsg - vl_nbCarEcrits) );
			/* ... en considerant comme erreur l'engorgement du canal de communication */
		} while ( (vl_cr < 0)  &&  ( (errno == EINTR) /*|| (errno == EAGAIN) || (errno == EWOULDBLOCK)*/ ) );
		sem_post(&vg_semaphore); 

		if ( vl_cr < 0 )
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "Impossible d'envoyer la commande au Systeme video %d", va_NumSysVideo);
		} else {
			vl_nbCarEcrits = vl_nbCarEcrits + vl_cr;
		}
	} while ( (vl_cr >= 0)  &&  (vl_nbCarEcrits < va_LgrMsg) );


	XZST_03EcritureTrace (XZSTC_FONCTION, "OUT: ec_sysv_ecrire (cr=%d)", vl_cr);
	if (vl_cr < 0)
	{
		return (XDC_NOK);
	}
	return (XDC_OK);
}
