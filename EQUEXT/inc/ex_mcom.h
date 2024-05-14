/*E Fichier : $Id: ex_mcom.h,v 1.10 2020/09/24 12:15:21 pc2dpdy Exp $      Release : $Revision: 1.10 $        Date : $Date: 2020/09/24 12:15:21 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  : EQUEXT
------------------------------------------------------
* MODULE COMMUN * FICHIER ex_mcom.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Structure des donnees du fichier des fonctions communes
*  au Sous-Systeme EQUEXT
*
------------------------------------------------------
* HISTORIQUE :
*
* Flaidar.A	version 1.1	27 oct 1994	: Creation
* Mismer.D	version 1.2	28 Nov 1994	: Remplacement XZECT_ENG_CONF_LIGNE par XZEXT_ENG_CONF_LIGNE
* Volcic.F	version 1.3	25 Jan 1995	: 
* Fontaine.C	version 1.4	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.5	14 Nov 1995	: Suppression constantes non utilisees
* Mismer.D	version 1.6	08 Fev 1996	: Ajout declaration fonction ecom_traitement_liaison_ST
* JMG		version 1.7	24/06/16	: ajout ex_st40 
* JMG		version 1.8	27/02/18	: ajout IP DEM1284 DEM1306
* JMG		version 1.9	27/02/19	: Port devient PortIP
* JMG		version 1.10 17/11/2021 : correction proto ecom_purge_cmd
------------------------------------------------------ */

#ifndef ex_mcom
#define ex_mcom

/* fichiers inclus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

#include <xdy.h>
#include <xdc.h>
#include <xdf.h>
#include <xdl.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzex.h>
#include <xzec.h>
#include <xzss.h>
#include <xzsm.h>
#include <xzamc.h>

#include	<rtworks/common.h>
#include	<rtworks/ipc.h>


/* definitions de constantes exportees */

#define ELxiC_ST_NOK		9	/* retour fonc. init si comms. avec ST impossible */

/* definitions de types exportes */


/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Lecture configuration de la ligne serie recu en argument.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int ecom_lect_config_ligne_serie( XDY_NomMachine		va_NomMachine,
*				  XZEXT_ENG_CONF_LIGNE 		*pa_ConfigLigne,
*				  int				va_NoLigneSerie )
*
*
* ARGUMENTS EN ENTREE :
*   va_NomMachine	: Nom de la machine
*   va_NoLigneSerie	: No. de la ligne serie dont on lit la config.
*
* ARGUMENTS EN SORTIE :
*   pa_ConfigLigne	: pointeur sur structure stokage contenu configuration de la ligne serie
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   lecture chaque ligne fichier config. pour trouver la config. de la ligne serie recue en par.   
*   retour a la decouverte de la config.
------------------------------------------------------*/

extern int ecom_lect_config_ligne_serie( XDY_NomMachine	,
				  XZEXT_ENG_CONF_LIGNE 	*,
				  int	);
				  
				  
/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Affichage contnue d'une structure de stokage config. ligne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_aff_config_ligne_serie	( XZEXT_ENG_CONF_LIGNE 	*pa_ConfigLigne )
*
*
* ARGUMENTS EN ENTREE :
*   pa_ConfigLigne	: pointeur sur structure stokage contenu config. de la ligne serie   
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*   
*   NEANT
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   
*   
------------------------------------------------------*/
extern void ecom_aff_config_ligne_serie	( XZEXT_ENG_CONF_LIGNE 	* );

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Traitement de la connexion avec le serveur de terminaux
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*/
extern void ecom_traitement_liaison_ST( XZEXT_ST_Config *, 
					XZEXT_MASK **, 
					XZEXT_ST_Sockets *,
					int * );
/*
*
* ARGUMENTS EN ENTREE :
*   
* pa_Config	: pointeur sur structure stokage config non initialisee.
* pa_SockList	: pointeur sur la liste des sockets.
* pa_desSocks	: pointeur sur les sockets specifiques taches TELxi.
*
* ARGUMENTS EN SORTIE :
* pa_Config	: pointeur sur structure config mise a jour
* pa_SockList	: pointeur sur la liste des sockets mise a jour
* pa_desSocks	: pointeur sur les sockets specifiques taches TELximises a jour.
*
* 
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
* 
* Appel de cette fonction si la liaison avec le serveur de terminaux est HS
*
* FONCTION 
*
* Essai de connexion sur la ligne serie
*
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Initialisation tache protocole utilisant le Serveur de Terminaux.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int ecom_init( 	XZEXT_ST_Config *pa_Config, 
*			XZEXT_MASK **pa_SockList, 
*			XZEXT_ST_Sockets *pa_desSocks )
*
*
* ARGUMENTS EN ENTREE :
*   
* pa_Config	: pointeur sur structure stokage config non initialisee.
* pa_SockList	: pointeur sur la liste des sockets.
* pa_desSocks	: pointeur sur les sockets specifiques taches TELxi.
*
* ARGUMENTS EN SORTIE :
* pa_Config	: pointeur sur structure config mise a jour
* pa_SockList	: pointeur sur la liste des sockets mise a jour
* pa_desSocks	: pointeur sur les sockets specifiques taches TELximises a jour.
*
* CODE RETOUR : 
*   XDC_OK	succes
*   XDC_NOK	echec
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*
*   lecture fichier config. lignes pour recuperer ds. pa_ConfigLigne la config de la ligne geree 
*   connexion en mode client sur la socket serveur des terminaux.
*   
------------------------------------------------------*/
extern int ecom_init( XZEXT_ST_Config *pa_Config, XZEXT_MASK **, XZEXT_ST_Sockets * );




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Recherche adresse client selon soket active ds. la liste des sockets
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  int ecom_detecte_clnt( XZEXT_ST_Sockets *pa_DesSocks, XZEXT_MASK **pa_SockList )
*
* ARGUMENTS EN ENTREE :
*
* pa_DesSocks	: pointeur sur les descs. sockets specifiques tache TELMi.
* pa_SockLis	: pointeur sur liste sockets.
*
* ARGUMENTS EN SORTIE :
* NEANT
* 
* CODE RETOUR : 
*
* XDC_NOK si la liste contient seulement XZEXC_NBR_MIN_SOCK_TELxi ou moins.
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
* - recherche ds. la liste des sockets l'adresse de la socket active et
*   la stoke ds. le champ pa_DesSocks->curr_addr.
*
------------------------------------------------------*/
extern int ecom_detecte_clnt( XZEXT_ST_Sockets *, XZEXT_MASK ** );
				  

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* colle une cmde. LCR a la fin de la liste des cmdes. LCR pointee par pa_List.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
*  void ecom_append_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*
*
*
------------------------------------------------------*/
extern void ecom_append_cmd_LCR( XZEXT_Cmde_LCR *, XZEXT_Liste_Cmdes_LCR ** );
extern void ecom_append_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *, XZEXT_Liste_Cmdes_LCR_IP ** );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
* Recuperation premier elem. de la liste des cmdes. LCR.
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_getfirst_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
* 
* pa_DebListe 	: pointeur sur le debut de la liste.
*
* ARGUMENTS EN SORTIE :
*
* pa_DebListe	: pointeur sur le debut de la liste mise a jour
* pa_CmdeLCR	: cmde. LCR extraite .
*
* CODE RETOUR : 
*
* NEANT.
*
* CONDITION D'UTILISATION
*  
* liste non vide : a tester avant appel de cette fonction.
*
* FONCTION 
*
* elimine le 1er elem. de la liste de cmdes. LCR et stoke l'info. utile ds. pa_CmdeLCR.
*
*
------------------------------------------------------*/
extern void	ecom_getfirst_cmd_LCR( XZEXT_Cmde_LCR *, XZEXT_Liste_Cmdes_LCR ** );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* insertion d'une cmde. LCR ds. la liste selon la priorite attachee a la commande.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_insert_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*
*
*
------------------------------------------------------*/
extern void ecom_insert_cmd_LCR( XZEXT_Cmde_LCR *, XZEXT_Liste_Cmdes_LCR ** );
extern void ecom_insert_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *, XZEXT_Liste_Cmdes_LCR_IP ** );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* insertion d'une cmde. LCR en debut de la liste.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void ecom_insertfirst_cmd_LCR( XZEXT_Cmde_LCR *pa_CmdeLCR, XZEXT_Liste_Cmdes_LCR **pa_DebListe )
*
* ARGUMENTS EN ENTREE :
*
* pa_CmdeLCR	: pointeur sur cmde. LCR a inserer ds. la liste.
* pa_DebListe	: pointeur sur une liste chainee de cmdes. LCR
*
* ARGUMENTS EN SORTIE :
* pa_DebListe	: la liste mise a jour avec le nouvel element.
*
* CODE RETOUR : 
*
* NEANT
*
* CONDITION D'UTILISATION
*  
*
* FONCTION 
*
*
*
------------------------------------------------------*/
extern void ecom_insertfirst_cmd_LCR( XZEXT_Cmde_LCR *, XZEXT_Liste_Cmdes_LCR ** );
extern void ecom_insertfirst_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *, XZEXT_Liste_Cmdes_LCR_IP ** );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Lecture horodate systeme.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* char  *ecom_lire_horodate( XZSMT_Horodate *pa_Horodate );
*
*
* ARGUMENTS EN ENTREE :
*   pa_Horodate	: pointeur sur horodate.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR : 
*   pointeur sur horodate convetie en format jj/mm/aaaa hh:mm:ss si tout OK.
*   pointeur sur chaine "?" si la lecture de l'horodate s'est mal pasee.
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   lecture chaque ligne fichier config. pour trouver la config. de la ligne serie recue en par.   
*   retour a la decouverte de la config.
------------------------------------------------------*/

extern char  *ecom_lire_horodate( XZSMT_Horodate * );



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Destruction liste commandes LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_destroy_list_cmd_LCR( XZEXT_Liste_Cmdes_LCR **pa_DebListe );
*
*
* ARGUMENTS EN ENTREE :
*   pa_DebListe	: pointeur sur le debut de la liste de commandes.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR : 
*   NEANT.
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   eliberation de la memoire ocuppee par la liste de cmdes. lCR non traitees.
*
------------------------------------------------------*/

extern void	ecom_destroy_list_cmd_LCR( XZEXT_Liste_Cmdes_LCR ** );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Destruction liste de sockets.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* void	ecom_detruire_liste_sock( XZEXT_MASK **pa_DebListe );
*
*
* ARGUMENTS EN ENTREE :
*   pa_DebListe : pointeur sur la liste de sockets.
*
* ARGUMENTS EN SORTIE :
*   NEANT.
*
* CODE RETOUR : 
*  NEANT.
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*   eliberation memoire occuppee par la liste de socket passee en argument.
*   
------------------------------------------------------*/

void	ecom_detruire_liste_sock( XZEXT_MASK ** );




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Extraction d'une demande LCR d'un msg. socket ds. une structure cmde. LCR.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* int	ecom_extraire_cmd_LCR( XZEXT_Msg_Socket *pa_MsgSock, XZEXT_Cmde_LCR *pa_CmdeLCR,
*				 int va_DesSockClnt)
*
*
* ARGUMENTS EN ENTREE :
*    pa_MsgSock	: pointeur sur le message contenant la demande LCR d'un tache TEzzz.
*    va_DesSockClnt : desc. socket tache cliente TEzzz ayant fait la demande LCR.
*
* ARGUMENTS EN SORTIE :
*   
*    pa_CmdLCR	: pointeur sur la zone de stokage de la demande LCR depuillee.
*
* CODE RETOUR : 
*   XDC_OK si tout est OK.
*   XDC_NOK Sinon.
* CONDITION D'UTILISATION
*   
*   pa_MsgSock pointe sur un message recu sur une socket donnees contenant une demande LCR.
*
* FONCTION 
*   depouillement de la demande LCR et stokage ds. la structure cmde. LCR.
*   
------------------------------------------------------*/
extern int	ecom_extraire_cmd_LCR( 	XZEXT_Msg_Socket *, XZEXT_Cmde_LCR *, 
				int  );
extern int	ecom_extraire_cmd_LCR_IP( 	XZEXT_Msg_Socket *, XZEXT_Cmde_LCR_IP *, 
				int  );


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* Met une trame contenant des cars. non imprimable en format lisible.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
*
* char	*ecom_rendre_trame_lisible( char *pa_Trame, int va_LongTrame, char *pa_TrameLisible );
*
*
* ARGUMENTS EN ENTREE :
*   pa_Trame : pointeur sur la liste de sockets.
*   va_LongTrame : longueur de la trame.
*   pa_TrameLisible	: pointeur sur buffer stockage trame lisible.
*
* ARGUMENTS EN SORTIE :
*   pa_TrameLisible	: pointeur sur la trame en format lisible.
*
* CODE RETOUR : 
*  pl_TrameLisible : pointeur sur la trame en format lisible.
* CONDITION D'UTILISATION
*  pa_Trame doit etre de longueur  va_LongTrame avec va_LongTrame >= 0.
*
* FONCTION 
*  Remplit pl_TrameLisible avec pa_Trame en remplacant les cars. non imprimable par 
*  leur code hexa.
*   
------------------------------------------------------*/

extern char	*ecom_rendre_trame_lisible( char *, int, char * );

extern int ex_st40 (char *);

extern int ecom_init_IP(     XZEXT_IP_Config *pa_Config,
                       XZEXT_MASK **pa_SockList,
                       XZEXT_ST_Sockets *pa_desSocks
                        );

extern int      ecom_extraire_cmd_DIASER(  XZEXT_Msg_Socket *, XZEXT_Cmde_DIASER *,
                                int  );

int ecom_existe_cmd_LCR_IP(XDY_AdresseIP , XDY_PortIP ,
                                        XZEXT_Liste_Cmdes_LCR_IP *, XZEXT_Cmde_LCR_IP *);

void ecom_delete_cmd_LCR_IP(XDY_AdresseIP , XDY_PortIP ,
                XZEXT_Liste_Cmdes_LCR_IP **);

XDY_Octet ecom_getfirst_cmd_LCR_IP( XZEXT_Cmde_LCR_IP *, 
			XZEXT_Liste_Cmdes_LCR_IP **, 
                        XDY_AdresseIP , XDY_PortIP );
XDY_Octet ecom_getfirst_cmd_LCR_IP2 ( XZEXT_Cmde_LCR_IP *,
		XZEXT_Liste_Cmdes_LCR_IP **,
		char *);
int ecom_purge_cmd(XDY_AdresseIP , XDY_PortIP , XZEXT_Liste_Cmdes_LCR_IP **, XZEXT_Cmde_LCR_IP *);
#endif

