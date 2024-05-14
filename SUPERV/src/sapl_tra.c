/*E*/
/* Fichier : $Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $	Release : $Revision: 1.13 $        Date : $Date: 2020/11/03 17:53:24 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_TRA * FICHIER apl_tra.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module appartient a la tache tsaplg
* Il propose diverses routines callbacks pour commander l'arret (stop)
* ou le demarrage (start) des taches applicatives du systeme MIGRAZUR.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  10 Oct 1994	: Creation
* T.Milleville  25 Jul 1995	: Ajout de commentaires V1.3
* T.Milleville  26 Jul 1995	: Ajout le cas ARRET_MARCHE lors de la 
*	commande d'une tache. On stoppe la tache avec -2 comme argument
*	ce qui permettra de tuer la tache, mais elle sera relancer
*	par SUPERV V1.4
* P.NIEPCERON	 8 dec 1995	: ajout de nohup ds les cmd de reboot v1.5
* P.NIEPCERON   19 dec 1995     : ajout de remsh ds les cmd de reboot v1.6
**
* P.NIEPCERON	22 Aou 1996	: ajout du type PC simplifie v1.7
* P.NIEPCERON   10 Dec 1997	: on boucle tant qu'on arrive pas a rebooter v1.8
* P.NIEPCERON   16 Dec 1997	: ajout du type PC2 v1.9
* JMG    10/02/10        : portage Linuc DEM/934
* JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.12
* LCL		20/12/21	: MOVIS P2 ems SAE-312
*******************************************************/


/* fichiers inclus */
#include "xdc.h"
#include "xdg.h"
#include "xdm.h"
#include "xdl.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzsp.h"

#include "sapl_com.h"
#include "sapl_tra.h"
#include "sapl_dir.h"
#include "sapl_don.h"
#include "sapl_ini.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $ : SAPL_TRA" ;


/* declaration de fonctions externes */



/*X*/
/* Fichier : $Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $	Release : $Revision: 1.13 $        Date : $Date: 2020/11/03 17:53:24 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Commande marche-arret d'une tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_cmdtach_cb (	
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Impossible de lire le message RTWorks
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $ : sp_cmdtach_cb" ;
#ifndef _TIBCO_EMS
  	T_STR		vl_NomTache	= "";
  	T_INT4	vl_onoff 		= 0;
#else
  	XDY_NomTache		vl_NomTache	= "";
  	XDY_Entier	vl_onoff 		= 0;
#endif


  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : sp_cmdtach_cb") ;

  if ( TipcMsgRead( pa_Data->msg,	
	T_IPC_FT_STR,	&vl_NomTache,
	T_IPC_FT_INT4, 	&vl_onoff, NULL ) )
  {
        XZST_03EcritureTrace(	XZSTC_MESSAGE,
				"Reception tache %s, onoff=%d",
				vl_NomTache , vl_onoff );

	if ( vl_onoff == XZSPC_MARCHE )
	{
		/* Lance de la tache */
		sp_startTache( vl_NomTache, -1 ) ;
	}
	else if (vl_onoff == XZSPC_ARRET)
	{
		/* Arret complet de la tache */
		sp_stopTache( vl_NomTache, -1 ) ;
	}
	else if  (vl_onoff == XZSPC_ARRET_MARCHE)
	{
		/* Arret de la tache + relance SUPERV */
		sp_stopTache( vl_NomTache, -2 ) ;
	}
  }
  else
  {
        XZST_03EcritureTrace(	XZSTC_MESSAGE,
				"erreur lecture message invalide"); 
  }

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : sp_cmdtach_cb") ;

} /* sp_cmdtach_cb */



/*X*/
/* Fichier : $Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $	Release : $Revision: 1.13 $        Date : $Date: 2020/11/03 17:53:24 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Commande marche-arret d'un sous-systeme
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_cmdssys_cb (	
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Impossible de lire le message RTWorks
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $ : sp_cmdssys_cb" ;
#ifndef _TIBCO_EMS
  	T_STR		vl_NomSSysteme	= "";
  	T_INT4		vl_onoff 	= 0;
#else
  	XDY_NomSysteme		vl_NomSSysteme	= "";
  	XDY_Entier		vl_onoff 	= 0;
#endif


  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : sp_cmdssys_cb") ;

  if ( TipcMsgRead( pa_Data->msg,	
		T_IPC_FT_STR,	&vl_NomSSysteme,
		T_IPC_FT_INT4, 	&vl_onoff, NULL ) )
  {
        XZST_03EcritureTrace(	XZSTC_MESSAGE,
				"Reception sous-systeme %s, onoff=%d",
				vl_NomSSysteme , vl_onoff );

	if ( vl_onoff == XZSPC_MARCHE )
		sp_startSSysteme( vl_NomSSysteme, 0 ) ;
	else
		sp_stopSSysteme( vl_NomSSysteme, 0 ) ;
  }
  else
  {
        XZST_03EcritureTrace(	XZSTC_MESSAGE,
				"erreur lecture message invalide"); 
  }

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : sp_cmdssys_cb") ;

} /* sp_cmdssys_cb */



/*X*/
/* Fichier : $Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $	Release : $Revision: 1.13 $        Date : $Date: 2020/11/03 17:53:24 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Commande marche-arret d'application
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_cmdappli_cb (	
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Impossible de lire le message RTWorks
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $ : sp_cmdappli_cb" ;
#ifndef _TIBCO_EMS
  	T_INT4	vl_onoff 	= 0;
#else
  	XDY_Entier	vl_onoff 	= 0;
#endif


  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : sp_cmdappli_cb") ;

  if ( TipcMsgRead( pa_Data->msg,	
		T_IPC_FT_INT4,	&vl_onoff, NULL ) )
  {
        XZST_03EcritureTrace(	XZSTC_MESSAGE,
				"Reception appli onoff=%d",
				vl_onoff);

	if ( vl_onoff == XZSPC_MARCHE )
		{
		sp_startAppli() ;
		}
	else if ( vl_onoff == XZSPC_ARRET )
		{
		sp_stopAppli() ;
		}
	else if ( vl_onoff == XZSPC_FIN_MIGRAZUR )
		{
		sp_sortir() ;
		}
	else
		{
        	XZST_03EcritureTrace(XZSTC_WARNING,
				"erreur valeur vl_onoff=%d",vl_onoff); 
		}
	}
  else
	{
        XZST_03EcritureTrace(XZSTC_MESSAGE,"erreur lecture message"); 
	}

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : sp_cmdappli_cb") ;

} /* sp_cmdappli_cb */



/*X*/
/* Fichier : $Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $	Release : $Revision: 1.13 $        Date : $Date: 2020/11/03 17:53:24 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Commande marche-arret d'une machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_cmdmach_cb (	
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Impossible de lire le message RTWorks
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version = "$Id: sapl_tra.c,v 1.13 2020/11/03 17:53:24 pc2dpdy Exp $ : sp_cmdmach_cb" ;
#ifndef _TIBCO_EMS
  	T_INT4			vl_onoff ;
#else
  	XDY_Entier			vl_onoff ;
#endif
  	XZSCT_NomMachine	vl_NomMachine ;
  	char			vl_commandeUnix[XDC_LINE_CMD_SIZE];
	int			vl_TypeMachine	= 0;
	XZSCT_NomSite 		vl_NomSite	= "";
	int			vl_ValRet	= XDC_OK;
	FILE *vl_ShellD;

#ifdef _HPUX_SOURCE
/* Modif v1.5 */
	#define		C_HALT_SYSTEM	"tscmd.x \"cd /;remsh `hostname` /etc/reboot -h \""
	#define		C_REBOOT_SYSTEM	"tscmd.x \"cd /;nohup remsh `hostname` /etc/reboot -r >/tmp/reboot 2>/tmp/reboot &\""
	#define		C_STOP_SERVER 	"rtstopsrv"
	#define		C_ARRET_BASE	"nohup /produits/migrazur/appliSD/exec/arret_base.sh 2>>/dev/null >>/dev/null &"
#else
        #define         C_HALT_SYSTEM   "tscmd.x \"/sbin/halt -f\""
        #define         C_REBOOT_SYSTEM "tscmd.x \"/sbin/reboot -f\""
        #define         C_STOP_SERVER   "rtstopsrv"
        #define         C_ARRET_BASE    "nohup /produits/migrazur/appliSD/exec/arret_base.sh 2>>/dev/null >>/dev/null &"

#endif


  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : sp_cmdmach_cb") ;

  if ( TipcMsgRead(pa_Data->msg,	
	T_IPC_FT_INT4,	&vl_onoff, NULL) )
  {
        XZST_03EcritureTrace(XZSTC_MESSAGE,
			"Reception machine onoff=%d",vl_onoff);

	/* recupere le nom de la machine locale */
	XZSC_07NomMachine(vl_NomMachine);

	/* recupere du nom du Site */
	XZSC_04NomSite(vl_NomSite);


        XZST_10ArchiverMsgSyst(XDL_SUP03_CMDRBOO,vl_NomMachine,NULL);
	if ( vl_onoff == XZSPC_MARCHE )
	{
		XZST_10ArchiverMsgSyst(XDL_SUP03_CMDRBOO,vl_NomMachine,NULL);
		/*A Recuperation du type de la machine */
		XZSC_05TypeMachine(&vl_TypeMachine);
		if ( 	(vl_TypeMachine == XZSCC_TYPEM_SD) ||
			(vl_TypeMachine == XZSCC_TYPEM_SC) ||
			(vl_TypeMachine == XZSCC_TYPEM_PCS)||
			(vl_TypeMachine == XZSCC_TYPEM_PC2)||
			(vl_TypeMachine == XZSCC_TYPEM_PC2LT)) /* ajout PCS modif 1.7 */  /* ajout PC2 modif 1.9*/
		{
			/*A On stoope le RTServer **/
			errno = 0;
			system(C_STOP_SERVER);
			if (errno)
			{
  				XZST_03EcritureTrace(XZSTC_WARNING,
				"IMPOSSIBLE de passer la cmd Stop Serveur \
				erreur %d \n",errno) ;
			}

			/*A On stoope la Base de donnees */
  			XZST_03EcritureTrace (XZSTC_WARNING,"arret base %s",C_ARRET_BASE) ;
			system(C_ARRET_BASE);
			
			/*if ( (vl_ValRet = XZAG04_Arret_Base(vl_NomSite)) 
						!= XDC_OK )
			{
				XZST_03EcritureTrace(XZSTC_WARNING,
				" Pb stop base sur Site : %s \n",vl_NomSite);
			}*/
		}

		/*A passage de la commande reboot */
		/* repositionement de l'uid a au reboot */
		/* attention les droit de la tache scmd.x */
		/* doit etre positionne a +s avec chmod dans le makefile */
		/* exemple : chmod +s scmd.x; chown root scmd.x */


		errno = 0;
		system(C_REBOOT_SYSTEM);
	        /*if ((vl_ShellD = popen (C_REBOOT_SYSTEM, "r")) == NULL)
	           {
 	           XZST_03EcritureTrace(   XZSTC_WARNING , "Echec de %s",C_REBOOT_SYSTEM); 
		    }
	       pclose(vl_ShellD);
*/
		if (errno)
		{
  			XZST_03EcritureTrace(XZSTC_WARNING,
				"IMPOSSIBLE de passer la commande reboot %s \
				erreur %d \n",C_REBOOT_SYSTEM,errno) ;
		}
	}
	else
	{
		XZST_10ArchiverMsgSyst(XDL_SUP08_CMDAMAC,vl_NomMachine,NULL);
		/*A Recuperation du type de la machine */
		XZSC_05TypeMachine(&vl_TypeMachine);
		if ( 	(vl_TypeMachine == XZSCC_TYPEM_SD) ||
			(vl_TypeMachine == XZSCC_TYPEM_SC) ||
			(vl_TypeMachine == XZSCC_TYPEM_PCS)||
			(vl_TypeMachine == XZSCC_TYPEM_PC2)||
			(vl_TypeMachine == XZSCC_TYPEM_PC2LT)) /* ajout PCS modif 1.7 */ /* ajout PCS modif 1.9 */
		{
			/*A On stoope le RTServer **/
			errno = 0;
			system(C_STOP_SERVER);
			if (errno)
			{
  				XZST_03EcritureTrace(XZSTC_WARNING,
				"IMPOSSIBLE de passer la cmd Stop Serveur \
				erreur %d \n",errno) ;
			}

			/*A On stoope la Base de donnees */
  			XZST_03EcritureTrace (XZSTC_WARNING,"arret base %s",C_ARRET_BASE) ;
			system(C_ARRET_BASE);
			
			/*if ( (vl_ValRet = XZAG04_Arret_Base(vl_NomSite)) 
						!= XDC_OK )
			{
				XZST_03EcritureTrace(XZSTC_WARNING,
				" Pb stop base sur Site : %s \n",vl_NomSite);
			}*/
		}


		/*A passage de la commande halt */
		/* repositionement de l'uid a au reboot */
		/* attention les droit de la tache doit etre */
		/* positionne a +s avec chmod dans le makefile */
		/* exemple : chmod +s scmd.x ;chown root scmd.x */
		while (1)
		{
		sleep(10);
		errno = 0;
		system(C_HALT_SYSTEM);

		if (errno)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,
				"IMPOSSIBLE de passer la commande halt \
				erreur %d \n", errno) ;
		}

		}
		
	}
  }
  else
  {
        XZST_03EcritureTrace(XZSTC_MESSAGE,"erreur lecture message"); 
  }

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : sp_cmdmach_cb") ;

} /* sp_cmdmach_cb */


