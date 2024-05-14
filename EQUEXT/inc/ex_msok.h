/*E*/
/* Fichier : $Id: ex_msok.h,v 1.12 2021/05/03 13:38:16 pc2dpdy Exp $      Release : $Revision: 1.12 $        Date : $Date: 2021/05/03 13:38:16 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_msok.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier de manipulation des
* sockets UNIX, commun au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	12 Aug 1994	: Creation
* Mismer.D	version 1.2	15 Nov 1994	:
* Mismer.D	version 1.3	02 Dec 1994	:
* Mismer.D	version 1.4	06 Jan 1995	:
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	02 Jun 1995	: Modif fonction ex_envoyer_commande_ST
* Mismer.D	version 1.7	07 Jul 1995	: Modif ex_acpt_cnxex_cnx_test_ST
* Mismer.D	version 1.8	07 Fev 1996	: Ajout de ex_presence_machine et ex_cnx_test_ST
*
* JPL		24/02/11 : Migration architecture HP ia64 (DEM 975) : declaration de la fonction 'ex_select'  1.9
* JMG		02/02/16 : ST 40 ports 1.10
* JPL		12/02/18 : Migration Linux 64 bits (DEM 1274) : ex_acpt_cnx : argument 'type' de type XDY_Mot  1.11
* ABE		16/02/21 : Ajout client/serveur IPV4 DEM-SAE155 1.12
------------------------------------------------------ */

#ifndef EX_MSOK
#define EX_MSOK

/* fichiers inclus */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/time.h>

#include "xdc.h"
#include "xdy.h"
#include "xdf.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzss.h"
#include "xzex.h"
#include "xzec.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Insertion d'un elements dans le masque des sockets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ex_ins_masque   ( 	XZEXT_MASK * pa_Enr,
				XZEXT_MASK * *pa_debut);
/*
* ARGUMENTS EN ENTREE :
*   pa_Enr 	: Pointeur sur enregistrement a inserer
*   pa_debut	: Pointeur sur sur le debut de la liste
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Allocation de memoire pour le nouvel element de la liste
*   Recherche de l'emplacement d'insertion de l'element
*   Mise a jour de la liste 
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Suppression d'un elements dans le masque des sockets
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
extern void	ex_sup_masque ( 	int 		va_DescSock, 
					XZEXT_MASK *	*pa_debut );

/*
* ARGUMENTS EN ENTREE :
*   va_DescSock	: Descripteur de la socket de l'enregistrement a supprimer
*   pa_debut	: Pointeur sur sur le debut de la liste
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION 
*   Recherche de l'element a supprimer
*   Mise a jour de la liste 
*   Liberation de la memoire
*
------------------------------------------------------*/


/*A definition de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction memorise le repertoire des sockets de la tache
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void ex_rep_socket ( XDY_NomMachine	pa_NomMachine );

/*
* ARGUMENTS EN ENTREE :
*  XDY_NomMachine	pa_NomMachine  : Nom de la machine
*
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*  Memorisation en global du repertoire des sockets de la tache et
*  creation du repertoire s'il n'existe pas .
------------------------------------------------------*/


/*X------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet la connexion en mode serveur
*   sur une socket d'≈coute
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_cnx_serveur ( 
			char		*pa_sockaddr,
			XZEXT_MASK *	*pa_masque,
			int		*pa_sd_sock );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sockaddr	:	adresse (port) du serveur.
*   pa_masque	:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_sd_sock	:	descripteur de la socket d'≈coute.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		connexion serveur impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Cr≈ation d'une socket d'≈coute
*   Attachement de la socket d'≈coute » une addresse
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la r≈ception des message
*   Ouverture du service d'≈coute des demandes de connexion
*
------------------------------------------------------*/



/*X------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet la connexion en mode client
*   sur une socket de donn≈es.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_cnx_client ( 
			char		*pa_sockaddr_clt,
			char		*pa_sockaddr_srv,
			XZEXT_MASK *	*pa_masque,
			int		*pa_sd_sock_don );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sockaddr_clt	:	adresse (port) du client.
*   pa_sockaddr_srv	:	adresse (port) du serveur.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_don	:	descripteur de la socket de donn≈es.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Cr≈ation d'une socket de donn≈es
*   Attachement de la socket de donn≈es » une addresse
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la r≈ception des message
*   Demande connexion au serveur
*
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet la connexion en mode serveur
*   sur une socket d'≈coute
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_cnx_serveur_LT ( 
		char		*pa_NomST,
		int		va_Port,
		XZEXT_MASK *	*pa_masque,
		int		*pa_sd_sock_srv);

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomST		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_don	:	descripteur de la socket de donn≈es.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Cr≈ation d'une socket de donn≈es
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la r≈ception des message
*   Demande connexion au serveur
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet la connexion en mode client
*   sur une socket de donn≈es.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_cnx_client_ST ( 
			char		*pa_NomST,
			int		va_Port,
			XZEXT_MASK *	*pa_masque,
			int		*pa_sd_sock_don );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_NomST		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_don	:	descripteur de la socket de donn≈es.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Cr≈ation d'une socket de donn≈es
*   Insertion du descripteur de socket dans le masque
*   de lecture pour la r≈ception des message
*   Demande connexion au serveur
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction ferme la socket sp≈cifi≈e.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern void	ex_fin_cnx ( 
		int		va_sd_sock,
		XZEXT_MASK *	*pa_masque );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_masque	:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_sd_sock	:	descripteur de la socket.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		fermeture socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Retirer la socket du masque de lecture
*   fermer la socket
*
------------------------------------------------------*/



/*X------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet d'accepter une connexion 
*   avec un client
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_acpt_cnx ( 	XDY_Mot		va_Type,
 				int		pa_sd_sock_ect,
				XZEXT_MASK *	*pa_masque,
 				int		*pa_sd_sock_clt );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sd_sock_ect	:	descripteur de la socket d'≈coute.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sd_sock_clt	:	descripteur de la socket du client.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK 		acceptation socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Acceptation par le serveur de la demande de connexion du client.
*   Insertion du descripteur de socket dans le masque de lecture.
*   D≈terminer le type d'≈quipement en fonction de la tache
*   traitement cliente.
*
------------------------------------------------------*/



/*X------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet d'envoyer un message par 
*   une socket.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_env_msg (
 		char		*pa_ident_dest,
 		XZEXT_MASK *	*pa_masque,
		char 		*pa_message,
 		int		*pa_cr_emission );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_ident_dest	:	Identit≈ du destinataire du message.
*   pa_masque		:	Masque de lecture
*   message		:	message » envoyer.
*
* ARGUMENTS EN SORTIE :
*
*   cr_emission	:	compte-rendu d'≈mission.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		emission message impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Identifier la socket du client.
*   Emettre le message » envoyer.
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet de lire un message dans 
*   la socket sp≈cifi≈e. 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_lire_msg (
 		int		va_sd_sock,
		char 		*pa_message,
 		int		*pa_lg_msg,
 		XZEXT_MASK *  	*pa_masque,
		int		*pa_deconnex );

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donn≈es.
*   pa_message	:	pointeur de r≈c≈ption du message.
*   pa_masque	:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque	:	masque de lecture.
*   pa_message	:	pointeur de r≈c≈ption du message.
*   pa_lg_msg	:	pointeur de r≈c≈ption de la longueur du message.
*   pa_deconnex	:	indicateur de d≈connexion.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		lecture message socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Extraire un message d'une socket de donn≈es.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet de determiner un evenement en lecture sur  
*   la socket sp≈cifi≈e. 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_select (
 		int              va_sd_sock,
 		double		va_TimeOut );

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donn≈es.
*   va_TimeOut	:	Time out
*
* ARGUMENTS EN SORTIE :
*
*
* CODE RETOUR : 
*
*   XDC_OK		evenement sur la socket
*   XDC_NOK		buffer socket vide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction se met en attente d'un message 
*   sur la liste des sockets 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_attendre_msg (
 		int		pa_sd_sock_ect,
 		XZEXT_MASK * 	*pa_masque,
		int		*pa_evenement );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sd_sock_ect	:	descripteur de la socket d'≈coute.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_evenement	:	existence ou non d'≈v≈nement sur une socket.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Attendre des messages en mode non bloquant sur le
*   masque de lecture.
*   Retourner le masque des sockets.
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet de lire un message dans 
*   la socket console serveur de terminaux. 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_lire_console (
 		int		va_sd_sock,
		char 		*pa_message,
 		int		*pa_lg_msg );

/*
* ARGUMENTS EN ENTREE :
*
*   va_sd_sock	:	descripteur de la socket de donn≈es.
*   pa_message	:	pointeur de r≈c≈ption du message.
*
* ARGUMENTS EN SORTIE :
*
*   pa_message	:	pointeur de r≈c≈ption du message.
*   pa_lg_msg	:	pointeur de r≈c≈ption de la longueur du message.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XDC_NOK		lecture message socket impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Extraire un message d'une socket de donn≈es.
*   Retourner le message et la longueur du message.
*
------------------------------------------------------*/


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction se met en attente d'un message 
*   sur la liste des sockets d'un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int  ex_attendre_msg_ST (
 		int		pa_sd_sock_ect,
 		XZEXT_MASK * 	*pa_masque,
 		double		va_TimeOut,
		int		*pa_evenement,	
		double		*pa_ResteTimeOut );

/*
* ARGUMENTS EN ENTREE :
*
*   pa_sd_sock_ect	:	descripteur de la socket d'≈coute.
*   pa_masque		:	masque de lecture.
*   va_TimeOut		:	Time-out
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_evenement	:	existence ou non d'≈v≈nement sur une socket.
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Attendre des messages en mode non bloquant sur le
*   masque de lecture.
*   Retourner le masque des sockets.
*
------------------------------------------------------*/




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction se met en attente d'un message 
*   sur la liste des sockets d'un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int  ex_ouvrir_console_ST (     char		*pa_NomST,
					int		*pa_sd_sock,
 					XZEXT_MASK * 	*pa_masque,
					char *va_machine);

/*
* ARGUMENTS EN ENTREE :
*
*  char			*pa_NomST		:  Nom du serveur de terminaux
*
* ARGUMENTS EN SORTIE :
*
*  int			*pa_sd_sock	: socket console
*  XZEXT_MASK * 	*pa_masque	: masque console
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Ouvrir un connexion avec la console du serveur de terminaux.
*
------------------------------------------------------*/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction se met en attente d'un message 
*   sur la liste des sockets d'un serveur de terminaux
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int  ex_envoyer_commande_ST (   char		*pa_Commande,
					int		 va_sd_sock,
 					XZEXT_MASK * 	*pa_masque );

/*
* ARGUMENTS EN ENTREE :
*
*  char			*pa_Commande		:  Commande a envoyer
*  int			 va_sd_sock	        : socket console
*
* ARGUMENTS EN SORTIE :
*
*  XZEXT_MASK * 	*pa_masque	: masque console
*
* CODE RETOUR : 
*
*   XDC_OK		succ≈s
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Envoyer une commande a la console du serveur de terminaux.
*
------------------------------------------------------*/

extern int ex_presence_machine ( XDY_NomMachine );

extern int ex_cnx_test_ST ( char *, int	 );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet la connexion en mode client
*   sur une socket IPV4
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_cnx_client_IPV4 ( 
		char		*pa_Nom,
		int		va_Port,
		XZEXT_MASK *	*pa_masque,
		int		*pa_sock_client);

/*
* ARGUMENTS EN ENTREE :
*
*   pa_Nom		:	Nom du serveur pour rÈcuperer l'adresse.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sock_client	:	descripteur de la socket de donnees.
*
* CODE RETOUR : 
*
*   XDC_OK		succes
*   XDC_NOK		connexion client impossible
*   XZEXC_ARG_INV	argument invalide
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Cration d'une socket IPV4 (client)
*   Retourne le descripteur de socket							
*
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*   Cette fonction permet la connexion en mode serveur
*   sur une socket IPV4
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int ex_cnx_serveur_IPV4 ( 
		int		va_Port	,
		XZEXT_MASK *	*pa_masque,
		int		*pa_sock_srv);

/*
* ARGUMENTS EN ENTREE :
*
*   pa_Nom		:	Nom du serveur de terminaux.
*   va_Port		:	Numero du port a connecter.
*   pa_masque		:	masque de lecture.
*
* ARGUMENTS EN SORTIE :
*
*   pa_masque		:	masque de lecture.
*   pa_sock_srv :	descripteur de la socket serveur
*
* CODE RETOUR : 
*
*   XDC_OK		success
*   XDC_NOK		Creation server impossible
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
*   Cration d'une socket IPV4 (serveur)
*   Retourne le descripteur de socket
*
------------------------------------------------------*/

#endif
