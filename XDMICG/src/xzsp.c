/*E*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSP * FICHIER xzsp.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface xzsp
*        - Declaration des fonctions externes
*        - Declaration des variables externes
*
*   Reference DCG-021 chapitre 3.5  
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  5 Oct 1994	: Creation
*
*  T.Milleville  11 Jan 1995	: Modification de la fonction 
*					XZSP_08   V1.8
*
*  T.Milleville  17 Jan 1995	: Modification de la fonction
*			XZSP_05. Ajout de l'argument de la Machine et
*			suppression de  la fonction sp_adm_init V1.9 
*  T.Milleville  10 Fev 1995	: Ajout argument a la fonction
*				XZSS_04  V1.11
*  T.Milleville  25 Jul 1995	: Ajout  de commentaires V1.13
*  T.Milleville  6 Oct  1995	: Ajout d'un test lors de la lecture
*	des versions logicielles. On tetst egalement sur le champ
*	TypeFic   V1.14
*  P.NIEPCERON	8  Dec 1995	: On ne tient plus compte des errno des write
*			lors de l'ecriture version log. v1.15
*  P.NIEPCERON  8 Fev 1996	: Ajout du caractere '_' ds le scanf de lecture du journale v1.17
*  P.NIEPCERON  15 Avr 1996     : Ajout des caracteres accentu�s et ! ds le scanf de lecture du journale v1.18
* JMG		216/06/10	: linux DEM/934
* JPL		07/12/10 : Migration architecture HP ia64 (DEM 961) : adresses des champs 'scanf'  1.20
*******************************************************/

/* fichiers inclus */

#include <stdlib.h>
#include <sys/stat.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#ifdef _HPUX_SOURCE
#include <rtworks/ipc_proto.h>
#endif

#include "xzsa.h"
#include "xzsp.h"

/* declaration des constantes locales */

static char*		version		="$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP";

/* declaration des variables locales  */

/* declaration de fonctions internes */
int sp_adm_init(
	XZSCT_NomMachine        va_NomMachine,
	int                     va_NumOrdre,
	void                    *pa_Args);




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int 	XZSP_AppliqueFonction 	(
	void		(*pa_FonctionUtil) (), 
	int		va_NumTexte, 
	XZSPT_Horodate	va_Horodate,
	int		va_NumEnr,
	char*		va_Args)

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   < Valeur et descriptif des parametres en sortie >
*
*
* CODE RETOUR : 
*   <Valeur de retour et un commentaire explicatif >
*
* CONDITION D'UTILISATION
*   < Explication succinte sur les conditions  d'utilisation
*     de la fonction 
*
* FONCTION 
*   < Bref descriptif fonctionnel >
*
------------------------------------------------------*/
{

  /* Variables Locales */
  static char*		version		="$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_AppliqueFonction";
  register int 		vl_ValRet = XDC_OK,
  			vl_i = 0;
  char*			tl_args [XZSPC_MAX_ARG];
  char*			pl_arg = va_Args;
 

	XZST_03EcritureTrace(XZSTC_FONCTION,"XZSP_AppliqueFonction : IN \
		-- Num Texte %d --  Horoadte %s -- Num Enr %d \n",
		va_NumTexte,va_Horodate,va_NumEnr);

if (va_NumTexte == 0)
{
	/*B Appel de la Fonction Utilisatuer
	*	avec comme argument un Texte */
	pa_FonctionUtil(va_Horodate, va_NumEnr, va_Args);
}
else
{


  /* decoupage de la liste des arguments */
  while ((tl_args [vl_i] = strtok (pl_arg, " ")) != NULL)
    {
      pl_arg = NULL;
      vl_i += 1;
    }


  /* appel de la fonction user avec le nombre d'argument qui va bien */
  /* void FonctionUtil (int numTexte, XZSCT_Horodate horodate, int NoEnr, ...) */
  switch (vl_i)
    {
    case 0:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, NULL);
      
      break;

    case 1:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], NULL);
      break;

    case 2:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], NULL);
      break;

    case 3:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], NULL);
      break;

    case 4:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], NULL);
      break;

    case 5:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], 
		       tl_args [4], NULL);
      break;

    case 6:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], 
		       tl_args [4], tl_args [5], NULL);
      break;

    case 7:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], 
		       tl_args [4], tl_args [5], tl_args [6], NULL);
      break;

    case 8:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], 
		       tl_args [4], tl_args [5], tl_args [6], tl_args [7]
		       , NULL);
      break;

    case 9:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], 
		       tl_args [4], tl_args [5], tl_args [6], tl_args [7], 
		       tl_args [8], NULL);
      break;
      
    case 10:
      pa_FonctionUtil (va_NumTexte, va_Horodate, va_NumEnr, 
		       tl_args [0], tl_args [1], tl_args [2], tl_args [3], 
		       tl_args [4], tl_args [5], tl_args [6], tl_args [7], 
		       tl_args [8], tl_args [9], NULL);
      break;
      
    default:
      vl_ValRet = XDC_NOK;
      break;
    }

}	

	XZST_03EcritureTrace(XZSTC_FONCTION,"XZSP_AppliqueFonction : OUT \
		-- Code retour %d \n",vl_ValRet);

  return (vl_ValRet);


}	/* Fin XZSP_AppliqueFonction */



/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine  :	Nom de la machine sur lequel on veut 
*		commander l'application.
* MarcheArret :	Indicateur de lancement ou d'arret
*		XZSPC_MARCHE ou XZSPC_ARRET
*
* Fonction 
* La fonction envoie le message XDM_TSP_APPL au 
* datagroup XDG_NSUPAPLG_NomMachine.
* Ce message contient "MarcheArret" au format %d.
* L'existence de la machine est teste avec la primitive
* XZSA_05.
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine invalide.
* XZSPC_ARG_INV		Argument MarcheArret invalide.
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_01CmdApplication (
	XZSCT_NomMachine	va_NomMachine,
	int			va_MarcheArret)

{

  /* Donnees Locales */
  register int 		vl_ValRet 	= XDC_OK;
  static char*		version		="$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_01CmdApplication";
  XZSCT_Datagroup	vl_nsupaplg 	= "";


      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_01CmdApplication IN : machine  : %s -- \
		Ind M/A : %d \n", va_NomMachine,  va_MarcheArret);


  /* test de l'existence de la machine */
  if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      	XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_01CmdApplication: machine %s invalide", va_NomMachine);

      	vl_ValRet = XZSPC_NOM_INV;
    }
  /* Envoi du message XDM_TSP_APPL au datagroup XDG_NSUPAPLG_NomMachine */
  else 
    {
      /* contruction du nom du datagroup */
      XDG_EncodeDG2 (vl_nsupaplg, XDG_NSUPAPLG, va_NomMachine);
      
      /* Envoi et balek ! */
      if (!TipcSrvMsgWrite (vl_nsupaplg, 
			    XDM_IdentMsg (XDM_TSP_APPL),
			    XDM_FLG_SRVMSGWRITE,
			    T_IPC_FT_INT4, va_MarcheArret,
			    NULL))
      {
		XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_01CmdApplication: erreur d'ecriture du msg XDM_TSP_APPL");

		vl_ValRet = XDC_NOK;
      }
    }
      
	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_01CmdApplication OUT  : Code Retour %d \n",
		vl_ValRet );
  
	return (vl_ValRet);

}	/* Fin XZSP_01... */


/*X*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine  :	Nom de la machine sur lequel on veut 
*		commander le sous-systeme.
* NomSSysteme :	Nom du sous-systeme a arreter ou 
*		demarrer.
* MarcheArret :	Indicateur de lancement ou d'arret 
*		XZSPC_MARCHE ou XZSPC_ARRET
*
* Fonction
* La fonction envoie le message XDM_TSP_SSYS au 
* datagroup XDG_NSUPAPLG_NomMachine.
* Ce message contient "NomSSysteme" et "MarcheArret" 
* au format %s %d.
* L'existence de la machine et du sous-systeme sont 
* testes avec les primitives XZSA_05 et XZSA_06.
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine invalide.
* XZSPC_NOS_INV		Nom du sous-systeme invalide.
* XZSPC_ARG_INV		Argument MarcheArret invalide.
* XDC_OK 		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_02CmdSSyst (
	XZSCT_NomMachine	va_NomMachine,
	XZSCT_NomSSysteme	va_NomSSysteme,
	int			va_MarcheArret)

{   
  /* Donnees Locales */
  register int 		vl_ValRet = XDC_OK;
  static char*		version="$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_02CmdSSyst";
  XZSCT_Datagroup	vl_nsupaplg = "";

      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_02CmdSSyst IN : machine  : %s -- \
		Ss-Syst %s  -- Ind M/A : %d \n", 
		va_NomMachine, va_NomSSysteme, va_MarcheArret);


  /* test de l'existence de la machine */
  if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
	"XZSP_02CmdSSyst: machine %s invalide", va_NomMachine);

      vl_ValRet = XZSPC_NOM_INV;
    }
  else if (XZSA_06ExisterSSysteme (va_NomSSysteme) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
	"XZSP_02CmdSSyst: sous-systeme %s invalide", va_NomSSysteme);

      vl_ValRet = XZSPC_NOS_INV;
    }
      
  /* Envoi du message XDM_TSP_SSYS au datagroup XDG_NSUPAPLG_NomMachine */
  else 
    {
      /* contruction du nom du datagroup */
      XDG_EncodeDG2 (vl_nsupaplg, XDG_NSUPAPLG, va_NomMachine);
      
      /* Envoi et balek ! */
      if (!TipcSrvMsgWrite (vl_nsupaplg, 
			    XDM_IdentMsg (XDM_TSP_SSYS),
			    XDM_FLG_SRVMSGWRITE,
			    T_IPC_FT_STR, va_NomSSysteme,
			    T_IPC_FT_INT4, va_MarcheArret,
			    NULL))
	{
	  XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_02CmdSSyst: erreur d'ecriture du msg XDM_TSP_SSYS");

	  vl_ValRet = XDC_NOK;
	}
    }
      
      	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_02CmdSSyst OUT  : Code Retour %d \n",
		vl_ValRet );
  
	return (vl_ValRet);

}	/* Fin XZSP_02... */


/*X*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine  :	Nom de la machine sur lequel on veut 
*		commander la tache.
* NomTache    :	Nom de la tache a arreter ou demarrer.
* MarcheArret :	Indicateur de lancement ou d'arret 
*		XZSPC_MARCHE ou XZSPC_ARRET
*
* Fonction
* La fonction envoie le message XDM_TSP_TACH au 
* datagroup XDG_NSUPAPLG_NomMachine.
* Ce message contient "NomTache" et "MarcheArret" au 
* format %s %d.
* L'existence de la machine et de la tache sont testes 
* avec les primitives XZSA_05 et XZSA_07.
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine
* XZSPC_NOT_INV		Nom de la tache invalide.
* XZSPC_ARG_INV		Argument MarcheArret invalide.
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS2.
*******************************************************/

extern int XZSP_03CmdTache (XZSCT_NomMachine	va_NomMachine,
			    XZSCT_NomTache      va_NomTache,
			    int			va_MarcheArret)
{
  /* Donnees Locales */
  register int 		vl_ValRet = XDC_OK;
  static char*		version = "$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_03CmdTache";
  XZSCT_Datagroup	vl_nsupaplg = "";

      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_03CmdTache IN : machine  : %s -- \
		tache : %s -- Ind M/A %d \n ",
		va_NomMachine, va_NomTache,va_MarcheArret);

  /* test de l'existence de la machine */
  if (XZSA_05ExisterMachine (va_NomMachine) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_03CmdTache: machine %s invalide", va_NomMachine);

      vl_ValRet = XZSPC_NOM_INV;
    }
  /* Envoi du message XDM_TSP_TACH au datagroup XDG_NSUPAPLG_NomMachine */
  else 
    {
      /* contruction du nom du datagroup */
      XDG_EncodeDG2 (vl_nsupaplg, XDG_NSUPAPLG, va_NomMachine);
      
      /* Envoi et balek ! */
      if (!TipcSrvMsgWrite (vl_nsupaplg, 
			    XDM_IdentMsg (XDM_TSP_TACH),
			    XDM_FLG_SRVMSGWRITE,
			    T_IPC_FT_STR,  va_NomTache,
			    T_IPC_FT_INT4, va_MarcheArret,
			    NULL))
	{
	  XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_03CmdTache: erreur d'ecriture du msg XDM_TSP_TACH");

	  vl_ValRet = XDC_NOK;
	}
    }
      
	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_03CmdTache OUT  : Code Retour %d \n",
		vl_ValRet );

  	return (vl_ValRet);
  
}	/* Fin XZSP_03.. */



/*X*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine  :	Nom de la machine que l'on veut arreter.
* IndicRelance:	Indicateur de relance automatique 
* 		XZSPC_MARCHE ou XZSPC_ARRET.
*
* Fonction
* La fonction appelle la primitive XZSP_01 avec 
* "NomMachine" et XZSPC_ARRET, puis elle envoie le 
* message XDM_TSP_STOP au datagroup 
* XDG_NSUPAPLG_NomMachine. 
* Ce message contient "IndicRelance" au format %d.
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine invalide.
* XZSPC_ARG_INV		Argument IndicRelance invalide.
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_04ArretMachine (
	XZSCT_NomMachine	va_NomMachine,
	int 			va_IndicRelance)

{

  /* Donnees Locales */
  register int 		vl_ValRet = XDC_OK;
  static char*		version = "$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_04ArretMachine";
  XZSCT_Datagroup	vl_nsupaplg = "";

      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_04ArretMachine IN : machine  : %s -- \
		Ind Relance %d \n ",
		va_NomMachine, va_IndicRelance );


  /* test de l'existence de la machine */
  if (XZSP_01CmdApplication (va_NomMachine, XZSPC_ARRET) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_04ArretMachine: machine %s invalide", va_NomMachine);

      vl_ValRet = XZSPC_NOM_INV;
    }
  /* Envoi du message XDM_TSP_STOP au datagroup XDG_NSUPAPLG_NomMachine */
  else 
    {
      /* contruction du nom du datagroup */
      XDG_EncodeDG2 (vl_nsupaplg, XDG_NSUPAPLG, va_NomMachine);
      
      /* Envoi et balek ! */
      if (!TipcSrvMsgWrite (vl_nsupaplg, 
			    XDM_IdentMsg (XDM_TSP_STOP),
			    XDM_FLG_SRVMSGWRITE,
			    T_IPC_FT_INT4, va_IndicRelance,
			    NULL))
	{
	  XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_04ArretMachine: erreur d'ecriture du msg XDM_TSP_STOP");

	  vl_ValRet = XDC_NOK;
	}
    }

      	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_04ArretMachine OUT  : Code Retour %d \n",
		vl_ValRet );

  	return (vl_ValRet);
  
}	/* Fin XZSP_04... */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction permet de mettre a jour le fichier 
*	d'administration
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSP_05MajFichierAdm(XZSCT_NomMachine va_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*   va_NomMachine :	Nom de la Machine
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK  : ok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Envoie du message XDM_TSA_ADMI sur le datagroup XDG_NSUPDIFA_Nommachine
*	Ensuite envoie du message XDM_TSP_INIT 
*	au datagroup XDG_NSUPAPLG_NomMachine
*	Ensuite envoie du message XDM_TSE_INIT 
*	au datagroup XDG_NSUPETAT_NomMachine
*
------------------------------------------------------*/
{
  	static char*		version		="$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_05MajFichierAdm";
	int 			vl_ValRet	= XDC_OK;
	XZSCT_Datagroup         vl_DataGroup 	= "";
	XZSCT_NomMachine        vl_NomMachine 	= "";
	int                  vl_NbElts       = 0;


      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_05MajFichierAdm IN : Nom Mach %s \n",
		va_NomMachine);


	/*A Recuperation du nom de la machine **/
	if ( (vl_ValRet = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
			"XZSP_05MajFichierAdm : Pb recuperationNom Machine \n");

	}
	else
	{

	/*A Envoie du message XDM_TSA_ADMI sur 
	*		le dg XDG_NSUPDIFA_NomMachine */
	XDG_EncodeDG2(vl_DataGroup, XDG_NSUPDIFA, va_NomMachine);
	if (!TipcSrvMsgWrite(vl_DataGroup
			      ,XDM_IdentMsg(XDM_TSA_ADMI)
			       ,XDM_FLG_SRVMSGWRITE
			       ,NULL) )
	{
	       /* Affichage Trace */
	       XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSP_05MajFichierAdm : Pb envoie msg XDM_TSA_ADMI \n");
			
		vl_ValRet = XZSCC_RTW_INV;
	}	
		
	/*A Envoie du message XDM_TSP_INIT sur 
	*		le dg XDG_NSUPAPLG_NomMachine */
	XDG_EncodeDG2(vl_DataGroup, XDG_NSUPAPLG, va_NomMachine);

	if (!TipcSrvMsgWrite(vl_DataGroup
			      ,XDM_IdentMsg(XDM_TSP_INIT)
			       ,XDM_FLG_SRVMSGWRITE
			       ,NULL) )
	{
	       XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSP_05MajFichierAdm : Pb envoie msg XDM_TSP_INIT \n");
		
		vl_ValRet = XZSCC_RTW_INV;
	}	

	/*A Envoie du message XDM_TSE_INIT sur 
	*		le dg XDG_NSUPETAT_NomMachine */
	XDG_EncodeDG2(vl_DataGroup, XDG_NSUPETAT, va_NomMachine);

	if (!TipcSrvMsgWrite(vl_DataGroup
			      ,XDM_IdentMsg(XDM_TSE_INIT)
			       ,XDM_FLG_SRVMSGWRITE
			       ,NULL) )
	{
	       XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSP_05MajFichierAdm : Pb envoie msg XDM_TSE_INIT \n");

		vl_ValRet = XZSCC_RTW_INV;
	}	

	/*A Envoie du message XDM_ADM_REINIT sur 
	*		le dg XDG_IHM_ADM */
	strcpy(vl_DataGroup, XDG_IHM_ADM);

	if (!TipcSrvMsgWrite(vl_DataGroup
			      ,XDM_IdentMsg(XDM_ADM_REINIT)
			       ,XDM_FLG_SRVMSGWRITE
			       ,NULL) )
	{
	       XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSP_05MajFichierAdm : Pb envoie msg XDM_ADM_REINIT \n");

		vl_ValRet = XZSCC_RTW_INV;
	}

	}

      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_05MajFichierAdm OUT : \n");


	return (vl_ValRet);


}	/* Fin XZSP_05MajFichierAdm */




/*X*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* ALGORITHME :
*
* Arguments
* FonctionUtil : Pointeur sur fonction utilisateur.
* NbEnrg       : Nombre d'enregistrements du journal
*
* Fonction
* La fonction rapatrie le journal de l'administration 
* systeme (XDF_NSUP_JOURADM) dans le repertoire XDC_REP_TMP 
* (primitive XZSS_07) a partir du serveur de donnees 
* actif (primitive XZSE_19), puis elle ouvre le journal
* et applique la fonction utilisateur sur chaque enregistrement lu 
* dans le fichier . Ensuite elle referme le journal de l'administration. 
* La fonction retourne le nombre d'enregistrements du journal.
*
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN)(int) NumTexte, 		no du libelle
*		(IN)(XZSPT_Horodate) Horodate,  horodate du message
*		(IN)(int) NumOrdre, 		no d'enregistrement dans le journal
*		...				liste d'arg. variable,
*		NULL)
* 
* L'algorithme de lecture est le suivant :
* Les enregistrements sont lus dans l'ordre chronologique.
* Ouverture du fichier XDF_NSUP_JOURADM 
* Lecture de l'entete du fichier par la fonction
* sx_jouradm_lecture contenant :
*	- TailleMax 	: Taille maximale du fichier.
*	- NbOctetEcrit	: pointeur d'ecriture courant.
* PtrLecture = NbOctetEcrit.
* AvantEcriture = 0
* Tant que caractere lu <> '\t'
*        Lecture d'un caractere.
*        Si fin de fichier
*                PtrLecture = sizeof de la structure 
*			      SXT_EnteteJourAdm.
*                AvantEcriture = 1
*        Sinon
*                Incrementation de PtrLecture.
*        Fsi
* Ftantque
* Positionnement en arriere d'un caractere.
* Tant que caractere de debut correct
*        Lecture du caractere de debut d'enregistrement
*	 '\t'
*        Si caractere de debut correct
*                Lecture de la longueur utile de 
*		 l'enregistrement
*                Lecture de l'enregistrement et appel de 
*		 la fonction utilisateur
*                Incrementation du nombre 
*		 d'enregistrements lus.
*                Incrementation de PtrLecture du nombre 
*		 d'octets lus
*                Si PtrLecture = NbOctetEcrit
*                        caractere de debut incorrect
*			 (sortie de boucle).
*                Fsi
*        Sinon
*                Si Init = 0
*                        PtrLecture = sizeof de la 
*			 structure SXT_EnteteJourAdm.
*                        caractere de debut correct 
*			 (continuer la boucle).
*                        AvantEcriture = 1
*                fsi
*        Fsi
* Ftantque
* Fermeture du fichier XDF_NSUP_JOURADM (primitive XZSS_13)
* 
* Codes retour
* XZSPC_FIC_INV		Probleme de lecture du journal 
*			de l'administration systeme.
* XDC_OK 		ok.
*
* Mecanismes
* RTworks : RTS2
******************************************************/

extern int XZSP_07ConsultJournalAdmin (
	void	(*pa_FonctionUtil)(),
       	int*	pa_NumEnr)

{
  /* Donnees Locales */
  register int 		vl_ValRet = XDC_OK,
  			vl_rembobine = XDC_FAUX,
			cr	= 0;
  int 			vl_numTexte = 0,
  			vl_lgUtile = 0;
  XZSPT_Horodate	vl_horodate = "",
  			vl_heure = "",
  			vl_date = "";  
  XZSCT_NomMachine	vl_serveurActif = "",	/* SD ou SC */
  			vl_machineLocale = "";
  FILE*			pl_idJournal = NULL;
  char			vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
  char			vl_pathJournalTmp [XDC_PATH_ABS_SIZE] = "";
  char			vl_args [XDC_LINE_CMD_SIZE] = "";
  char                  vl_commande[XDC_LINE_CMD_SIZE];
  FILE			*vl_cr;
  static char*		version = "$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_07ConsultJournalAdmin";



      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_07ConsultJournalAdmin IN : Fct Utile %s \n",
		pa_FonctionUtil);


  /* Initialisations */
  (*pa_NumEnr) = 0;
  /* Recuperation du nom du seveur de donnees actif */
  XZSE_19NomSDActif (vl_serveurActif);
  XZST_03EcritureTrace (XZSTC_DEBUG1,"Nom SD %s -- Long %d \n",
		vl_serveurActif,strlen(vl_serveurActif));
  strcpy (vl_serveurActif, "SDCI1");
  if (strlen(vl_serveurActif) == 0)
  {
      	XZST_03EcritureTrace (XZSTC_WARNING, 
	    "XZSP_07ConsultJourAdm: Pb Serveur SD Actif  \n");

  	XZSE_18NomSCActif (vl_serveurActif);
  XZST_03EcritureTrace (XZSTC_DEBUG1,"Nom SC %s -- Long %d \n",
		vl_serveurActif,strlen(vl_serveurActif));
  	if (strlen(vl_serveurActif) == 0)
	{
      		XZST_03EcritureTrace (XZSTC_WARNING, 
	    "XZSP_07ConsultJourAdm: Pb Serveur SC Actif  \n");

      		vl_ValRet = XDC_NOK;
	    	return (vl_ValRet);
  	}
   }

  /* Recuperation du nom de la machine locale */
  if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
	    "XZSP_07ConsultJourAdm: nom de machine invalide");
      vl_ValRet = XDC_NOK;
    }
  /* Recuperation du fichier XDF_NSUP_JOURADM du */
  /* serveur de donnees actif dans XDC_REP_TMP         */
  else if (XZSS_07TransfertFichier (	vl_serveurActif, 
					XDF_NSUP_JOURADM, 
					XDC_NSUP_PATHOPER,
				    	vl_machineLocale, 
					XDF_NSUP_JOURADM_TMP, 
					XDC_REP_TMP) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
	"XZSP_07ConsultJourAdm: Transfert du fichier %s incorrect de %s a %s", 
	 XDF_NSUP_JOURADM, vl_serveurActif, vl_machineLocale);
      vl_ValRet = XZSPC_FIC_INV;
    }
  else 
    {
      /* On ne garde que les lignes commencant par une tabulation */
      /* Puis on trie le fichier selon horodate */
      sprintf( vl_commande, 
     "sed '/^[\t]/!d' %s/%s | nl | sort -k2,2 -k3,3 | sed 's/^[ 0-9]*[\t]*[\t]//'> %s/%s",
			XDC_REP_TMP,XDF_NSUP_JOURADM_TMP , 
			XDC_REP_TMP, XDF_NSUP_JOURADM );

      XZST_03EcritureTrace (XZSTC_INFO, 
	"Trie du Journal Adinistration : Cmd %s \n",vl_commande);

      /* Lancement de la commande */
      vl_cr = popen( vl_commande, "r");
      if ( vl_cr == NULL )
	 XZST_03EcritureTrace (XZSTC_WARNING, 
			"XZSP_07ConsultJourAdm: pb au popen ");
      else
      {
	 errno = 0;
         if (pclose( vl_cr ) != 0)
	 {
		XZST_03EcritureTrace (XZSTC_WARNING,
			"XZSP_07ConsultJourAdm: pb au pclose %d \n ", errno);		 }


	/*B Destruction du Fichier Temporaire */
	sprintf (vl_pathJournalTmp, "%s/%s", XDC_REP_TMP, XDF_NSUP_JOURADM_TMP);
	if (XZSS_06DetruireFichier( vl_pathJournalTmp))
	{
		XZST_03EcritureTrace( XZSTC_WARNING,
			"pb a l'appel de XZSS_06DetruireFichier %s \n", 
			vl_pathJournalTmp);
	}	

         /* Ouverture en lecture de ce fichier */
         sprintf (vl_pathJournal, "%s/%s", XDC_REP_TMP, XDF_NSUP_JOURADM);
	 if ( (vl_ValRet = XZSS_16OuvrirFichierStream ( 
		XZSSC_FIC_RD_STREAM,vl_pathJournal,&pl_idJournal ) ) != XDC_OK )
	{	

	     XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_07ConsultJourAdm: nom du fichier %s \
		invalide ou fichier deja ouvert", XDF_NSUP_JOURADM);

	     vl_ValRet = XZSPC_FIC_INV;
	}
         /* Positionnement au premier enregistrement */
         else if (fseek (pl_idJournal, 0, SEEK_SET) < 0)
	   {
	     XZST_03EcritureTrace (XZSTC_WARNING, 
	        "XZSP_07ConsultJourAdm: Positionnement impossible \
		au debut de %s", XDF_NSUP_JOURADM);

	     vl_ValRet = XDC_NOK;
	   }

         /* lecture de la ligne courante jusqu'a la fin du fichier */
         else
	 while ((cr = fscanf (pl_idJournal, 
			  	"%s %s %[/ a-zA-Z0-9,;._-+:������!?�{}'()]s", 
				vl_date, 
				vl_heure, 
				vl_args)) != EOF) /* modif 1.17 */
	   {	    
	         XZST_03EcritureTrace (XZSTC_DEBUG1, 
			"Lecture Journal Admin : CR %d \n",cr);

	     /* applik la fonktion user */
	     sprintf (vl_horodate, "%s %s", vl_date, vl_heure);

	     if (XZSP_AppliqueFonction (pa_FonctionUtil, 
				     vl_numTexte, 
				     vl_horodate,
				     *pa_NumEnr,
				     vl_args) != XDC_OK)
	       {
	         vl_ValRet = XDC_NOK;
	         XZST_03EcritureTrace (XZSTC_WARNING, 
			"XZSP_07ConsultJournalAdm:  erreur application \
			de la fonction user %s", pa_FonctionUtil);
	       }
	     (*pa_NumEnr) ++;
	   }
       }
    }

	/*A Fermeture du Fichier */
	if ( (vl_ValRet = XZSS_17FermerFichierStream (pl_idJournal)) != XDC_OK )
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
			"XZSP_07ConsultJourAdm: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}

	/* On detruit le fichier */
  	unlink (vl_pathJournal);
      
      	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_07ConsultJournalAdmin OUT : Nb Enregis : %d \n",
		*pa_NumEnr);

	return (vl_ValRet);
  
}	/* Fin XZSP_07... */



/*X*/
/* Fichier : $Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $	Release : $Revision: 1.21 $        Date : $Date: 2017/05/16 16:40:34 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* ALGORITHME :
*
* Arguments
* FonctionUtil	: Pointeur sur fonction utilisateur.
* IndHisto	: XZSPC_ACTUEL ou XZSPC_HISTO
* NbElem	: Nombre d'elements de la liste.
*
* Fonction
* Cette fonction applique la fonction utilisateur 
* passee en argument a chaque element de la liste.
* L'appel de la fonction utilisateur est le suivant :
* FonctionUtil ((IN) (XZSCT_TypeFichier      TypeFichier		
*		(IN) (XZSCT_NomMachine)      NomMachine,
* 		(IN) (XZSCT_NomTache)        NomTache,
* 		(IN) (XZSCT_NumVersion)      NumVersion,
* 		(IN) (XZSPT_Horodate)        Horodate,
* 		(IN) (int)                   NumOrdre)
* Chaque element de la liste est recupere dans le 
* fichier de gestion des versions logicielles 
* XDF_NSUP_VERSLOG.
* La fonction rapatrie le fichier XDF_NSUP_VERSLOG
* dans le repertoire XDC_REP_TMP (primitive XZSS_07) a partir
* de PODFA , puis * elle ouvre le fichier.
* Pour chaque enregistrement XZSPT_ComposantVL du fichier
*        Si IndHisto = XZSPC_ACTUEL
*                Appel primitive XZSA_05 pour tester la 
*		 machine et XZSA_07 pour tester la tache
*                Si test d'existence positif
*                        Application fonction utilisateur
*                Fsi
*        Sinon
*                Application fonction utilisateur
*        Fsi
* Fpour
* Fermeture du fichier.
*
* La fonction retourne le nombre d'enregistrements 
* lus du fichier.
* La structure d'un enregistrement SXT_VersLog est 
* decrit DCG Tome2.1 -  Description des modules communs.
*
* Codes retour
* XDC_OK 		ok.
* XZSPC_FIC_INV		Erreur lecture fichier des 
*			versions logicielles.
*
* Mecanismes
* Pointeur de fonction.
********************************************************/

extern int XZSP_08ListeVersionLog	(
	void	(*pa_FonctionUtil) (),
	int 	va_IndHisto,
	int*	pa_NumElem)

{

  /* Variables Locales */
  register int 		vl_ValRet = XDC_OK,
  			cr = 0;
  XZSPT_Horodate	vl_heure = "",
  			vl_date = "";
  XZSCT_NomMachine	vl_machineLocale = "";
  int			pl_idVersion = NULL;
  char			vl_pathVersion [XDC_PATH_ABS_SIZE] = "";
  char			vl_args [XDC_LINE_CMD_SIZE] = "";
  static char*		version = "$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_08ListeVersionLog";
  XZSPT_ComposantVL     vl_ENRGLEC;
  XZSCT_NomMachine	vl_NomMachineMemo	= "";
  XZSPT_NomFichier	vl_NomFichierMemo	= "";
  XZSPT_TypeFichier	vl_TypeFicMemo		= "";



      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_08ListeVersionLog IN : Fct Utile %s -- \
		Ind Histo %d \n",pa_FonctionUtil, va_IndHisto );

  /* Initialisation */
  (*pa_NumElem) = 0;

  /* Recuperation du nom de la machine locale */
  if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_08ListeVersLog: nom de machine invalide");
      vl_ValRet = XDC_NOK;
    }
  /* Recuperation du fichier XDF_NSUP_VERSLOG 
		du serveur de donnees actif dans XDC_REP_TMP */
  else if (XZSS_07TransfertFichier (XDC_MACHINE_GECONF, XDF_NSUP_VERSLOG, 
		XDC_NSUP_PATHOPER, vl_machineLocale, 
			XDF_NSUP_VERSLOG, XDC_REP_TMP) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
	"XZSP_08ListeVersLog:a Transfert du fichier %s incorrect de %s a %s", 
	    XDF_NSUP_VERSLOG, XDC_MACHINE_GECONF, vl_machineLocale);

      vl_ValRet = XZSPC_FIC_INV;
    }
  /* Ouverture en lecture de ce fichier */
  else 
    {
      sprintf (vl_pathVersion, "%s/%s", XDC_REP_TMP, XDF_NSUP_VERSLOG);
      if ( (vl_ValRet = XZSS_12OuvrirFichier ( 
	XZSSC_FIC_RD, vl_pathVersion, &pl_idVersion ) ) != XDC_OK )
     {
	  XZST_03EcritureTrace (XZSTC_WARNING, 
		"XZSP_08ListeVersLog: nom du fichier %s invalide \
		ou fichier deja ouvert", XDF_NSUP_VERSLOG);
												vl_ValRet = XZSPC_FIC_INV;
	}
      /* lecture des enregistrements du fichier */
      else
      while (read(pl_idVersion,&vl_ENRGLEC,sizeof(vl_ENRGLEC)) > 0)
	{
	  if (va_IndHisto == XZSPC_ACTUEL)
	    {
		/*B Teste si c'est la derniere version correspondant 
		*	a la machine et au nom du fichier */
			/* Modif V1.14*/
		if ( (strcmp(vl_ENRGLEC.NomMachine,vl_NomMachineMemo)) 
		   || (strcmp(vl_ENRGLEC.NomFichier,vl_NomFichierMemo)) 
		   || (strcmp(vl_ENRGLEC.TypeFic,vl_TypeFicMemo)) )
		{
			/* On applique la fonction utilisateur */
			vl_ValRet = XDC_OK;

			/*B On memorise les nouveaux de machine et de fichier */
			strcpy(vl_NomMachineMemo,vl_ENRGLEC.NomMachine);
			strcpy(vl_NomFichierMemo, vl_ENRGLEC.NomFichier);
			strcpy(vl_TypeFicMemo, vl_ENRGLEC.TypeFic);
		}
		else
		{
			/*B On ne tient pas compte de ces versions */
			vl_ValRet = XDC_NOK;
		}
	    }


	  XZST_03EcritureTrace (XZSTC_DEBUG1, 
		"Avant appel fonction Utile %d \n",vl_ValRet);


	  if (vl_ValRet != XDC_NOK)
	    {

	  XZST_03EcritureTrace (XZSTC_DEBUG1, 
		"appel fonction Utile NB Elts %d \n",*pa_NumElem);

	      pa_FonctionUtil (vl_ENRGLEC,
			       *pa_NumElem);
	     (*pa_NumElem)++;
	    }

	} /* Fin du While */
    }

    /*A Fermeture du Fichier */
    if ( (vl_ValRet = XZSS_13FermerFichier (pl_idVersion)) != XDC_OK )
    {
	XZST_03EcritureTrace (XZSTC_WARNING,
		    "XZSP_07ConsultJourAdm: Pb femeture Fichier \n");

	vl_ValRet = XZSPC_FIC_INV;
    }


      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_08ListeVersionLog OUT  : Nb Elts %d -- Code Retour %d \n",		*pa_NumElem, vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSP_08ListeVersionLog **/



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction permet d'ecrire un enregistrement dans
*	le fichier de version logicielle 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSP_09EcritVersionLog(
	XZSPT_ComposantVL	va_Enr )

/*
* ARGUMENTS EN ENTREE :
*   va_Enr : Enregistrement a ecrire
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK :  ok
*   XZSPC_FIC_INV : Erreur ecriture fichier des versions logicielles
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Teste l'existence du fichier de version logicielle, puis ouvre celui-ci
*	Ensuite lecture des enregistrements et si le nom de la machine
*	lue est identique au  nom de machine a enregistrer alors
*	enregistrement a l'emplacement precedent, sinon enregistrement
*	en fin .
*
------------------------------------------------------*/
{
  	static char*		version = "$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_09EcritVersionLog";
	int 			vl_IdVersion	= 0;
	XZSPT_ComposantVL	vl_ENRGLEC	;
	off_t			vl_Offset	= 0;
	int			vl_MacTrouvee	= XDC_NON;
	int 			vl_ValRet	= XDC_OK;
	int			vl_NbBytes 	= 0;
	mode_t			vl_ModeVersion	= 0;
	int			vl_IndicEcrit	= XDC_NON;
	char 			vl_NomFichier [50] = "";



      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_09EcritVersionLog IN : Enreg  : \
		Type Fic : %s -- Machine : %s -- Fichier %s -- \
		Version %s -- Horodate : %s \n",
		va_Enr.TypeFic,va_Enr.NomMachine, 
		va_Enr.NomFichier, va_Enr.NumVersion,va_Enr.Horodate);

	/*A Initialisation de la structure de lecture */
	memset(&vl_ENRGLEC,0,sizeof(XZSPT_ComposantVL));


	/*A Teste si le fichier de version logicielle existe */
	/*B Creation du Nom du fichier de version logicielle 
	*	dans le repertoire XDC_NSUP_PATHOPER   */
	sprintf(vl_NomFichier, "%s/%s",XDC_NSUP_PATHOPER,XDF_NSUP_VERSLOG);
 	if (access(vl_NomFichier, F_OK) == 0)
	{
		/*B Ouverture du Fichier Version Logicielle */
		if ( (vl_ValRet = XZSS_12OuvrirFichier ( 
			(XZSSC_FIC_RDWR ) , 
			vl_NomFichier, &vl_IdVersion ) ) != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING,
			"XZSP_09EcritVersionLog: nom du fichier %s invalide \
			ou fichier deja ouvert", vl_NomFichier);

			vl_ValRet = XZSPC_FIC_INV;
		}
	}
	else
	{
		/*B Creation du Fichier Version Logicielle */
		if ( (vl_ValRet = XZSS_12OuvrirFichier ( 
				(XZSSC_FIC_RDWR | XZSSC_FIC_CR )  ,
				vl_NomFichier, &vl_IdVersion ) ) != XDC_OK )
		{
			XZST_03EcritureTrace (XZSTC_WARNING,
			"XZSP_09EcritVersionLog: nom du fichier %s invalide \
				ou fichier deja ouvert", vl_NomFichier);

			vl_ValRet = XZSPC_FIC_INV;
		}
		else
		{
			/*B On modifie les droits du fichier en Lecture et
			*	en Ecriture **/
			vl_ModeVersion = S_IRUSR | S_IWUSR | S_IROTH | S_IWOTH;
			fchmod(vl_IdVersion,vl_ModeVersion);
			/*B Si une erreur est detectee */
			errno = 0;
			if (errno)
			{
				/*B Probleme grave */
				XZST_03EcritureTrace(XZSTC_WARNING,
					"XZSP_09EcritVersionLog : \
					Pb modification droits  code %d \n",
					errno);

				errno = 0;
			}
		}
	}
     
	/*A On lit le fichier et on recupere les enregistrements */
	while(read(vl_IdVersion,&vl_ENRGLEC,sizeof(vl_ENRGLEC)) > 0)
	{
		/*B Teste si Nom machine lue == Nom machine a enregistrer */
		if ( (!strcmp(vl_ENRGLEC.NomMachine,va_Enr.NomMachine))
		   && (!strcmp(vl_ENRGLEC.NomFichier,va_Enr.NomFichier))
		   && (!strcmp(vl_ENRGLEC.TypeFic,va_Enr.TypeFic))  )
     		{
			/*B On positionne le pointeur de 1
			*       enregistrement precedent 
			*	Attention on utilise 2 fois la fonction lssek 
			*	pour enregistrer en debut de fichier 
			* En effet la fonction lssek(-,0,SEEK_CUR) est inoperante */
			vl_Offset = lseek (vl_IdVersion ,
				- sizeof(vl_ENRGLEC),SEEK_CUR );
			lseek (vl_IdVersion , vl_Offset, SEEK_SET );
								
			if (vl_Offset == -1)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,
					"Impossible lseek fichier version");
				break;
			}

			/*B Ecriture du nouvel Enregistrement */
			vl_NbBytes = 
				write(vl_IdVersion, &va_Enr,sizeof(va_Enr) );
			/*B Si une erreur est detectee */
			/*if (errno)*/
			if (0) /* modif v1.15*/
			{
				/*B Probleme grave */
				XZST_03EcritureTrace(XZSTC_WARNING,
					"XZSP_09EcritVersionLog : \
					Pb ecriture enregistrement code %d \n",
					errno);

				errno = 0;
			}
			else
			{
				/*B Initialisation Machine trouvee */
				vl_MacTrouvee = XDC_NON;


				/*B On memorise le fait que l'on a ecrit
				*	l'enregistrement */
				vl_IndicEcrit = XDC_OUI;

				/*B On reecrit les enregistrements 
				*	deja  existants */
				memcpy(&va_Enr,&vl_ENRGLEC,
						sizeof(XZSPT_ComposantVL));
			}
		}
		else 
		{
		  if ( (!strcmp(vl_ENRGLEC.NomMachine,va_Enr.NomMachine))
				&& (vl_IndicEcrit == XDC_NON) )
		  {
			/*B Memorisation Machine trouvee */
			vl_MacTrouvee = XDC_OUI;
		  }
		  else
		  {
			/*B Si machine trouvee */
			if ( (vl_MacTrouvee == XDC_OUI)
			|| (vl_IndicEcrit == XDC_OUI) )
			{
				/*B On positionne le pointeur de 1 
				*	enregistrement precedent 
				*	Attention on utilise 2 fois la fonction lssek 
				*	pour enregistrer en debut de fichier 
				* En effet la fct lssek(-,0,SEEK_CUR) est inoperante */
				vl_Offset = lseek (vl_IdVersion ,
					- sizeof(vl_ENRGLEC),SEEK_CUR );
				lseek (vl_IdVersion ,vl_Offset ,SEEK_SET );
				if (vl_Offset == -1)
				{
					XZST_03EcritureTrace(XZSTC_WARNING,
					"Impossible lseek fichier version");
					break;
				}

				/*B Ecriture du nouvel Enregistrement */
				vl_NbBytes = write(vl_IdVersion,
						&va_Enr,sizeof(va_Enr) );
				/*B Si une erreur est detectee */
				if (0)	/* modif v1.15*/
				{
					/*B Probleme grave */
					XZST_03EcritureTrace(XZSTC_WARNING,
						"XZSP_09EcritVersionLog : \
					Pb ecriture enregistrement code %d \n",
					errno);

					errno = 0;
				}
				else
				{
					/*B Initialisation Machine trouvee */
					vl_MacTrouvee = XDC_NON;

					vl_IndicEcrit = XDC_OUI;

					/*B On reecrit les enregistrements 
					*	deja  existants */
					memcpy(&va_Enr,&vl_ENRGLEC,
						sizeof(XZSPT_ComposantVL));
				}

			}
		  }

		} /* Fin else 1*/

		/*A Initialisation de la structure de lecture */
		memset(&vl_ENRGLEC,0,sizeof(XZSPT_ComposantVL));

	}	/* Fin While*/

	/*B Si l'enregistrement n'a pas encore ete ecrit **/
	/*B Ecriture du nouvel Enregistrement */
	vl_NbBytes = write(vl_IdVersion, &va_Enr,sizeof(va_Enr) );
	/*B Si une erreur est detectee */
	if (errno)
	{
		/*B Probleme grave */
		XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSP_09EcritVersionLog : \
			Pb ecriture enregistrement code %d \n", errno);
	}

	/*A Fermeture du fichier de version logicielle */
	if ( (vl_ValRet = XZSS_13FermerFichier (vl_IdVersion)) != XDC_OK )
	{
		XZST_03EcritureTrace (XZSTC_WARNING,
			"XZSP_09EcritVersionLog: Pb femeture Fichier \n");

		vl_ValRet = XZSPC_FIC_INV;
	}


      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_09EcritVersionLog OUT \n");

	return (vl_ValRet);

}	/* Fin XZSP_09EcritVersionLog */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'imprimer le Journal D'administartion
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSP_10ImprimeJournalAdmin()

/*
* ARGUMENTS EN ENTREE :
*	Aucun
*
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
*   Recupere le nom du Serveur de Donnees ou de Communications
*	actif, puis transfert le Journal d'administration dans
*	un fichier temporaire, puis trie celui ci selon les dates
*	croissantes et enfin demande l'impression sur l'imprimante
*	locale
------------------------------------------------------*/
{
	static char *version="$Id: xzsp.c,v 1.21 2017/05/16 16:40:34 gesconf Exp $ : XZSP_10ImprimeJournalAdmin " ;
  register int 		vl_ValRet = XDC_OK;
  XZSCT_NomMachine	vl_serveurActif = "",	/* SD ou SC */
  			vl_machineLocale = "";
  FILE*			pl_idJournal = NULL;
  char			vl_pathJournal [XDC_PATH_ABS_SIZE] = "";
  char			vl_pathJournalTmp [XDC_PATH_ABS_SIZE] = "";
  char			vl_ImpJournal [XDC_PATH_ABS_SIZE] = "";
  char			vl_args [XDC_LINE_CMD_SIZE] = "";
  char                  vl_commande[XDC_LINE_CMD_SIZE];
  FILE			*vl_cr;



      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_10ImprimeJournalAdmin IN \n");

  /* Initialisations */
  /* Recuperation du nom du seveur de donnees actif */
  if (XZSE_19NomSDActif (vl_serveurActif) != XDC_OK)
  {
      	XZST_03EcritureTrace (XZSTC_WARNING, 
	    "XZSP_10ImprimeJournalAdmin: Pb Serveur Actif  \n");

  	if (XZSE_18NomSCActif (vl_serveurActif) != XDC_OK)
  	{
      		XZST_03EcritureTrace (XZSTC_WARNING, 
	    "XZSP_10ImprimeJournalAdmin: Pb Serveur SC Actif  \n");

      		vl_ValRet = XDC_NOK;
	    	return (vl_ValRet);
  	}
   }
  /* Recuperation du nom de la machine locale */
  else if (XZSC_07NomMachine (vl_machineLocale) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, 
	    "XZSP_10ImprimeJournalAdmin: nom de machine invalide");
      vl_ValRet = XDC_NOK;
    }
  /* Recuperation du fichier XDF_NSUP_JOURADM du */
  /* serveur de donnees actif dans XDC_REP_TMP         */
  else if (XZSS_07TransfertFichier (	vl_serveurActif, 
					XDF_NSUP_JOURADM, 
					XDC_NSUP_PATHOPER,
				    	vl_machineLocale, 
					XDF_NSUP_JOURADM_TMP_IMP, 
					XDC_REP_TMP) != XDC_OK)
    {
      XZST_03EcritureTrace (XZSTC_WARNING, "XZSP_10ImprimeJournalAdmin: \
	Transfert du fichier %s incorrect de %s a %s", 
	 XDF_NSUP_JOURADM, vl_serveurActif, vl_machineLocale);
      vl_ValRet = XZSPC_FIC_INV;
    }
  else 
    {
      /* On ne garde que les lignes commencant par une tabulation */
      /* Puis on trie le fichier selon horodate */
      sprintf( vl_commande, 
     "sed '/^[\t]/!d' %s/%s | nl | sort -k2,2 -k3,3 | sed 's/^[ 0-9]*[\t]*[\t]//'> %s/%s",
			XDC_REP_TMP,XDF_NSUP_JOURADM_TMP_IMP , 
			XDC_REP_TMP, XDF_NSUP_JOURADM_IMP );

      XZST_03EcritureTrace (XZSTC_INFO, 
	"Trie du Journal Adinistration : Cmd %s \n",vl_commande);

      /* Lancement de la commande */
      vl_cr = popen( vl_commande, "r");
      if ( vl_cr == NULL )
	 XZST_03EcritureTrace (XZSTC_WARNING, 
			"XZSP_10ImprimeJournalAdmin: pb au popen ");
      else
      {
	 errno = 0;
         if (pclose( vl_cr ) != 0)
	 {
		XZST_03EcritureTrace (XZSTC_WARNING,
		    "XZSP_10ImprimeJournalAdmin: pb au pclose %d \n ", errno);		 }


	/*B Destruction du Fichier Temporaire */
	sprintf (vl_pathJournalTmp, "%s/%s", 
			XDC_REP_TMP, XDF_NSUP_JOURADM_TMP_IMP);
	if (XZSS_06DetruireFichier( vl_pathJournalTmp))
	{
		XZST_03EcritureTrace( XZSTC_WARNING,
			"pb a l'appel de XZSS_06DetruireFichier %s \n", 
			vl_pathJournalTmp);
	}

	/*B Demande d'Impression du Fichier XDF_NSUP_JOURADM_IMP */
	sprintf(vl_ImpJournal,"%s/%s",XDC_REP_TMP,XDF_NSUP_JOURADM_IMP);
	if (XZSS_04ImprimerFichier( XZSSC_TYPE_IMPRIMA, 
		XDF_NSUP_JOURADM_IMP,XDC_REP_TMP,XDC_IMP_ASCII ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING,
			"pb a l'a ppel de XZSS_04ImprimerFichier %s \n",
			 vl_ImpJournal);
		vl_ValRet = XDC_NOK;
	}
	else
	{
		/* supression du fichier */
		if (XZSS_06DetruireFichier( vl_ImpJournal))
		{
			XZST_03EcritureTrace( XZSTC_WARNING,
			    "pb a l'appel de XZSS_06DetruireFichier %s \n",
			    vl_ImpJournal);
			vl_ValRet = XDC_NOK;
		}
	}
      }
  }
      XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSP_10ImprimeJournalAdmin IN \n");

	return (vl_ValRet);

}	/* Fin XZSP_10ImprimeJournalAdmin */


