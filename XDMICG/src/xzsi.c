/*E*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSI        * FICHIER xzsi.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Module contenant: 
*   - les fonctions d'envoi de
*     messages RT-Works aux datagroups de 
*     rafraichissement de synoptiques d'administration
*   - une fonction d'affichage de message systeme 
*
* Reference DCG-023 chapitre 3.1
*
******************************************************
* HISTORIQUE :
*
*
* T.Milleville	16 Nov 94 	: Creation
* T.Milleville	08 Mar 1995     : Modification de la fonction XZSI_06
*	Ajout d'un argument pour specifier le destinataire et appel 
*	pour affichage dans la boite d'affichage V1.2
* T.Milleville	15 Mar 1995     : Remplacement de la fonction XZIA_06
*		par XZIA_08   V 1.3
* T.Milleville	20 Mar 1995     : Remplacement de la fonction XZIA_08
*	par XZIA08_AffMsgAdm  V1.5
* T.Milleville	20 Mar 1995     : Mpodification pour SCCS V1.6
* T.Milleville	19 Avr 95	: Modification des fonctions XZSI_06
*	et XZSI_08 pour differencier l'envoi d'un msg sur la fenetre
*	operateur ou administarteur.Donc pour etre en correlation
*	avec XZIA , XZSI_06 destine a l'operateur et XZSI_08 
*	destine a l'administrateur V1.7
* T.Milleville	25 Jul 95	: Ajout de commentaires V1.8
* T.Milleville	12 Oct 95	: Modification code XZSI_06 et XZSI-08 V1.9
* T.Milleville	19 Oct 95	: MOdification XZSI_04 pour recueperer le type
*	du peripherique et envoyer dans le msg XDM_ADM_ARC_PERI v1.10
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : utiliser 'vprintf' avec une liste d'args.  1.11
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

/* fichiers inclus */


#include "xdc.h"
#include "xdm.h"
#include "xdl.h"
#include "xdg.h"
#include "xzsi.h"
#include "xzsa.h"


/* declaration de variable locales */
static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine:     nom du calculateur sur lequel se 
*                 trouve le sous-systeme.
* NomSSysteme:    nom du sous-systeme
* EtatSSysteme:   etat du sous-systeme
*                 (XZSIC_OK ou XZGAC_NOK)
*
* Fonction
* La fonction envoie le message XDM_ADM_SSS au 
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom de la machine,
* le nom du sous-systeme et son etat, au format:
* %s %s %d.
*
* Codes retour
* XDC_OK	: ok
* XDC_NOK	: pas ok
*
* Mecanismes
*
******************************************************/

int XZSI_01DiffEtatSSysteme(XZSCT_NomMachine	va_NomMachine,
		        XZSCT_NomSSysteme	va_NomSSysteme,
		        int			va_EtatSSysteme)
{
  static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_01DiffEtatSSysteme";

  XZSCT_Datagroup  vl_nomDatagroup;

  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_01DiffEtatSSysteme : IN  \
	-- Nom Mach %s -- Nom SSyst %s -- Eta ssyst %d \n",
	va_NomMachine,va_NomSSysteme,va_EtatSSysteme);

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_SSS),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomMachine,
		    T_IPC_FT_STR, va_NomSSysteme,
		    T_IPC_FT_INT4, va_EtatSSysteme,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_01DiffEtatSSysteme : OUT \n");

  return(XDC_OK);

}	/* Fin XZSI_01DiffEtatSSysteme */



/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine:     nom du calculateur sur lequel se
*                 trouve la tache.
* NomTache:       nom de la tache
* EtatTache:      etat de la tache (XZSIC_OK ou
*                 XZSIC_NOK)
*
* Fonction
* La fonction envoie le message XDM_ADM_TAC au
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom de la machine,
* le nom de la tache et son et at, au format: %s %s %d.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_02DiffEtatTache(XZSCT_NomMachine	va_NomMachine,
		     XZSCT_NomTache	va_NomTache,
		     int		va_EtatTache)
{
  static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_02DiffEtatTache";

  XZSCT_Datagroup  vl_nomDatagroup;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZSI_02DiffEtatTache" );

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_TAC),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomMachine,
		    T_IPC_FT_STR, va_NomTache,
		    T_IPC_FT_INT4, va_EtatTache,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  return(XDC_OK);
}



/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine:     nom du serveur de communication ou
*                 de donnees.
* EtatMachine:    etat du calculateur (XZSIC_VEILLE,
*                 XZSIC_NOMACT, XZGAC_NOMPAS ou
*                 XZSIC_DEGRAD)
*
* Fonction
* La fonction envoie le message XDM_ADM_ARC_SER au 
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom de la machine 
* et son etat, au format: %s %d.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_03DiffEtatServeur(
	XZSCT_NomMachine	va_NomMachine,
	int		va_EtatMachine)
{
  static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_03DiffEtatServeur";
  XZSCT_Datagroup  vl_nomDatagroup	= "";
  int	vl_ValRet	= XDC_OK;

  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_03DiffEtatServeur : IN  \
	-- Nom Mach %s  -- Eta ssyst %d \n",
	va_NomMachine,va_EtatMachine);

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  XZST_03EcritureTrace( XZSTC_MESSAGE, 
	"Envoie du Msg XDM_ADM_ARC_SER -- Nom Mach %s \
	Eta Mach %d \n",va_NomMachine,va_EtatMachine);

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_ARC_SER),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomMachine,
		    T_IPC_FT_INT4, va_EtatMachine,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_03DiffEtatServeur : OUT  \
	-- Code Retour %d \n",vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSI_03DiffEtatServeur */


/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomPeriph:      nom du peripherique
* EtatPeriph:     etat du peripherique (XZSIC_OK ou
*                 XZSIC_NOK)
*
* Fonction
* La fonction envoie le message XDM_ADM_ARC_PER au 
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom du peripherique 
* et son etat, au format: %s %d.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_04DiffEtatPeriph(XZSCT_NomPeriph	va_NomPeriph,
		      int		va_EtatPeriph)
{
  static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_04EtatDiffPeriph";
  int	vl_TypeEntite	= 0;


  XZSCT_Datagroup  vl_nomDatagroup;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZSI_04DiffEtatPeriph" );

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

/* Modif V1.10 */
	/* Recuepration du type du peripherique */
	XZSA_26TypeEntite(va_NomPeriph,&vl_TypeEntite);


  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_ARC_PER),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomPeriph,
		    T_IPC_FT_INT4, vl_TypeEntite,
		    T_IPC_FT_INT4, va_EtatPeriph,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  return(XDC_OK);
}


/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomInterface:   nom de l'interface
* EtatInterface:  etat de l'interface (XZSIC_OK ou
*                 XZSIC_NOK)
*
* Fonction
* La fonction envoie le message XDM_ADM_ARC_INT au
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom de l'interface
* et son etat, au format: %s % d.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_05DiffEtatInterface(XZSCT_NomInterface	va_NomInterface,
		         int	va_EtatInterface)
{
  static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_05DiffEtatInterface";

  XZSCT_Datagroup  vl_nomDatagroup;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZSI_05DiffEtatInterface" );

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_ARC_INT),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomInterface,
		    T_IPC_FT_INT4, va_EtatInterface,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  return(XDC_OK);
}



/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* va_destinataire Destinataire du Msg
* NumTexte:       indice du message d'administration
* P0,.., P9       chaines a ajouter au message
*
* Fonction
* On recupere la langue courante; en fonction de 
* celle-ci, on recupere dans le fic hier FIC_MSG_FR
* ou FIC_MSG_AL le libelle du message d'indice NumTexte.
* On execute un printf a ce format recupere des 9
* parametres P1, ..., P9.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_06AffMsgSyst(
	XZIAT_destinataire va_destinataire,
	int   va_NumTexte, ...) 
{
  	static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_06AffMsgSyst";

   char 		vl_leformat[255]	= "";
   XDLT_SystemeAlarme 	vl_SysAlarme;
   char			vl_msgPourAffichage[255]	= "";
   va_list 		vl_ap;
   int			vl_cr	= 0;
   
	XZST_03EcritureTrace( XZSTC_INTERFACE,"XZSI_06AffMsgSyst : IN \
		-- Num Texte %d \n",va_NumTexte);


	/* Recuperation des arguments variables */
   	va_start (vl_ap, va_NumTexte);

	/* Teste si msg d'alarme, cad numero est different de 0 */
	if (va_NumTexte != 0)
	{

   /* recuperation du format d'ecriture a partir de va_NumTexte   */
   vl_cr = XDL_01SystemeAlarme( va_NumTexte, &vl_SysAlarme );

   if ( vl_cr == XDC_NOK )
   {
	XZST_03EcritureTrace( XZSTC_WARNING,
	      "XZSI_06AffMsgSyst: pb a l'appel de XDL_01SystemeAlarme du texte %d \n",va_NumTexte);

	      return (XDC_NOK);
   }
   else
   {
   	strcpy( vl_leformat, vl_SysAlarme.FormatFranc );
	if ( strcmp( vl_leformat, "" ) == 0 )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING,
	      "XZSI_06AffMsgSyst: format de message vide");
	}
	else
	{
   	  /* ecriture avec le format precedent des arguments dans le tableau */
   	  vsprintf ( vl_msgPourAffichage, vl_leformat, vl_ap);
	}
   }
	}
	else
	{
   	  	/* ecriture avec le format precedent des arguments dans le tableau */
   	  	vsprintf ( vl_msgPourAffichage, "%s", vl_ap);
	}	

	/* Termine la strutation des arguments varaibles */
   	va_end (vl_ap);


	XZST_03EcritureTrace( XZSTC_INFO ,
			" Num Texte = %d  -- Texte %s \n",va_NumTexte,vl_msgPourAffichage);

	/*A Envoi d'un Msg Systeme  pou affichage dans la fenetre Systeme */
	XZIA_06AffMsgSyst(va_destinataire,vl_msgPourAffichage);


	XZST_03EcritureTrace(XZSTC_INFO,"Affichage Msg %s \n",
		vl_msgPourAffichage );

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSI_06AffMsgSyst : OUT \n");

	return (XDC_OK);

}	/* Fin XZSI_06AffMsgSyst */




/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NivTrace:       niveau de trace
* VisuTrace:      visualisation trace (ecran ou non)
* NomTache:       nom de la tache
* NomMachine:     nom de la machine
*
* Fonction
* La fonction envoie le message XDM_ADM_TRA au 
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le niveau de trace, 
* la visualisation, le nom de la tache et le nom de
* la machine, au format: %d %d %s %s.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_07DiffNivVisuTrace(XZSTT_NiveauTrace     va_NivTrace,
		        int                   va_VisuTrace,
		        XZSCT_NomTache        va_NomTache,
		        XZSCT_NomMachine      va_NomMachine)
{
  static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_07DiffNivVisuTrace";

  XZSCT_Datagroup  vl_nomDatagroup;
  int 	vl_ValRet 	= XDC_OK;



  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_07DiffNivVisuTrace  : IN \
  -- Niv Trace %x -- Visu Trace %d -- Nom Tache %s -- Nom Mach %s \n",
  va_NivTrace,va_VisuTrace,va_NomTache,va_NomMachine);

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_TRA),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_INT4, va_NivTrace,
		    T_IPC_FT_INT4, va_VisuTrace,
		    T_IPC_FT_STR, va_NomTache,
		    T_IPC_FT_STR, va_NomMachine,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_07DiffNivVisuTrace  : OUT \
  -- Code Retour %d \n",vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSI_07DiffNivVisuTrace */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'aficher un Msg d'alarme sur le poste d'administation
*	dans la fenetre d'administrrtion 
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSI_08AffMsgAdm(
	XZIAT_destinataire va_destinataire,
	int   va_NumTexte, ...) 

/*
* ARGUMENTS EN ENTREE :
* va_destinataire Destinataire du Msg
* NumTexte:       indice du message d'administration
* P0,.., P9       chaines a ajouter au message
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere les arguments dans la liste variable, construit le msg
*	a afficher puis appel de la fonction d'afichage 
*
------------------------------------------------------*/
{
	static char *version="$Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ : XZSI_08AffMsgAdm " ;
   	char 			vl_leformat[255]	= "";
   	XDLT_SystemeAlarme 	vl_SysAlarme;
   	char			vl_msgPourAffichage[255]	= "";
   	va_list 		vl_ap;
   	int			vl_cr		= 0;
   
	XZST_03EcritureTrace( XZSTC_INTERFACE,"XZSI_08AffMsgAdm : IN \
		-- Num Texte %d \n",va_NumTexte);


	/*A Positionnement en debut de liste variable */
   	va_start (vl_ap, va_NumTexte);

	/* Teste si msg d'alarme , cad numero est different de 0 */
	if (va_NumTexte != 0)
	{
   		/*A  recuperation du format d'ecriture a partir de va_NumTexte   */
   		vl_cr = XDL_01SystemeAlarme( va_NumTexte, &vl_SysAlarme );

   		if ( vl_cr == XDC_NOK )
   		{
			XZST_03EcritureTrace( XZSTC_WARNING,
	      "XZSI_08AffMsgAdm: pb a l'appel de XDL_01SystemeAlarme pour texte %d \n",va_NumTexte);

			return (XDC_OK);
   		}
   		else
   		{
   			strcpy( vl_leformat, vl_SysAlarme.FormatFranc );
			if ( strcmp( vl_leformat, "" ) == 0 )
			{
	   			XZST_03EcritureTrace( XZSTC_WARNING,
	      				"XZSI_08AffMsgAdm: format de message vide");
			}
			else
			{
   	  			/* ecriture avec le format precedent des arguments dans le tableau */
   	  			vsprintf ( vl_msgPourAffichage, vl_leformat, vl_ap);
			}
   		}
	}
	else
	{
   	  	/* ecriture du texte */
   	  	vsprintf ( vl_msgPourAffichage, "%s", vl_ap);
	}

	/* Termine scrutation arguments variables */
   	va_end (vl_ap);

	XZST_03EcritureTrace( XZSTC_INFO ,
			" Num Texte = %d  -- Texte %s \n",va_NumTexte,vl_msgPourAffichage);

	/*A Envoi d'un Msg Systeme  pou affichage dans la fenetre Systeme */
	XZIA08_AffMsgAdm(va_destinataire,vl_msgPourAffichage);


	XZST_03EcritureTrace(XZSTC_INFO,"Affichage Msg %s \n",
		vl_msgPourAffichage );


	XZST_03EcritureTrace(XZSTC_INTERFACE, " XZSI_08AffMsgAdm : OUT  \
		-- Code Retour %d \n",vl_cr);

	return(vl_cr);

}	/* Fin XZSI_08AffMsgAdm */




/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine:     nom du calculateur
* ModeReconstr    mode reconstruction ou non (XZSIC_ON,
*                 XZSIC_OFF)
* ModeInstr       mode instruction ou non (XZSIC_ON,
*                 XZSIC_OFF)
*
* Fonction
* La fonction envoie le message XDM_ADM_ARC_STA_M au
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom de la station,
* son etat de reconstruction et d'instruction, au
* format: %s %d %d.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_10DiffModeStation(XZSCT_NomMachine va_NomMachine,
		       int              va_ModeInstr)
{
  	static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_10DiffModeStation";

  XZSCT_Datagroup  vl_nomDatagroup;

  XZST_03EcritureTrace( XZSTC_FONCTION, "IN: XZSI_10DiffModeStation" );

  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_ARC_STA_M),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomMachine,
		    T_IPC_FT_INT4, va_ModeInstr,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  return(XDC_OK);
}


/*X*/
/* Fichier : $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $	      Release : $Revision: 1.11 $        Date : $Date: 2010/12/07 17:11:08 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine:     nom du calculateur.
* EtatStation:    etat de la station (XZSIC_VEILLE, 
*                 XZSIC_NOMACT, XZGAC_NOMPAS ou
*                 XZSIC_DEGRAD );

* Fonction
* La fonction envoie le message XDM_ADM_ARC_STA_E au 
* Datagroup XDG_IHM_ADM.
* Le message est construit avec le nom de la machine
* et son etat, au format: %s %d %d.
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


int XZSI_11DiffEtatStation(XZSCT_NomMachine va_NomMachine,
		       int              va_EtatStation)
{
  	static char *version="  $Id: xzsi.c,v 1.11 2010/12/07 17:11:08 gesconf Exp $ :  XZSI_11DiffEtatStation";
  	XZSCT_Datagroup  vl_nomDatagroup	= "";
	int	vl_ValRet	= XDC_OK;


  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_11DiffEtatStation : IN  \
	-- Nom Mach %s  -- Eta Station %d \n",
	va_NomMachine,va_EtatStation);


  /* Construction du datagroup   */
  strcpy( vl_nomDatagroup, XDG_IHM_ADM );

  XZST_03EcritureTrace( XZSTC_MESSAGE, 
	"Envoie du Msg XDM_ADM_ARC_STA_E -- Nom Mach %s \
	Eta Mach %d \n",va_NomMachine,va_EtatStation);

  /* Construction du message  */
  TipcSrvMsgWrite(  vl_nomDatagroup,
		    XDM_IdentMsg(XDM_ADM_ARC_STA_E),
		    XDM_FLG_SRVMSGWRITE,
		    T_IPC_FT_STR, va_NomMachine,
		    T_IPC_FT_INT4, va_EtatStation,
		    NULL );

  /* Envoi du message  */
  TipcSrvFlush();

  XZST_03EcritureTrace( XZSTC_INTERFACE, "XZSI_11DiffEtatStation : IN  \
	-- Code Retour %d \n",vl_ValRet);

  return(vl_ValRet);

}		/* Fin XZSI_11DiffEtatStation */

