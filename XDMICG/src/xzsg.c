/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE * FICHIER xzsg.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les librairies de mise en place
* et d'arret des tests reseaux pour une machine donnee.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  30 Sep 1994    : Creation
* T.Milleville  25 Jul 1995    : Ajout de commentaires V1.2
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : retour C.R. OK ou NOK pour plusieurs fonctions  1.3
*******************************************************/




/* fichiers inclus */
#include "xdc.h"
#include "xdg.h"
#include "xdm.h"
#include "xzst.h"
#include "xzsg.h"




/* declaration des constantes locales */
static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG";




/* definition des types locaux */
typedef struct {
		  XZSCT_NomMachine  NomMachine;
                  void              *TestPrec;
                  void              *TestSuiv;
               } XZSGT_TEST_RES; 




/* declaration des variables locales  */
	/* Premier element de la liste des tests a effectuer */
XZSGT_TEST_RES *XZSGP_PremTest; 
	/* Dernier element de la liste des tests a effectuer */
XZSGT_TEST_RES *XZSGP_DernTest; 


/* declaration de fonctions internes */


/*X*/
/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE xzsg
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Insere un nouveau test reseau (sur une nouvelle machine)
* dans la liste des tests reseau courante.
*
******************************************************
* SEQUENCE D'APPEL :
*/
int XZSG_InsereTest(XZSCT_NomMachine pa_NomMachine)
/*
* PARAMETRES EN ENTREE : - pa_NomMachine : nom de la machine
*	sur laquelle il faut faire des tests reseau.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG_InsereTest";
   	XZSGT_TEST_RES *pl_NouvTest	= NULL;
   


   /* creation du test supplementaire */
   pl_NouvTest = malloc(sizeof(XZSGT_TEST_RES));
   strcpy(pl_NouvTest -> NomMachine, pa_NomMachine);
   pl_NouvTest -> TestSuiv = NULL;

   /* ajout du test supplementaire dans la liste */
   if (XZSGP_PremTest == NULL)
   {
      /* premier element de la liste */
      XZSGP_PremTest = pl_NouvTest;
      pl_NouvTest -> TestPrec = NULL;
   }
   else
   {
      /* on relie le dernier element de la liste a ce nouvel element */
      XZSGP_DernTest -> TestSuiv = pl_NouvTest; /* chainage avant */
      pl_NouvTest -> TestPrec = XZSGP_DernTest; /* chainage arriere */
   }
   XZSGP_DernTest = pl_NouvTest;

   return(XDC_OK);
}	/* XZSG_InsereTest */	



/*X*/
/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Supprime un test reseau (sur une machine donnee)
* dans la liste des tests reseau courante.
*
******************************************************
* SEQUENCE D'APPEL :
*/
int XZSG_SupprimeTest(XZSCT_NomMachine pa_NomMachine)
/*
* PARAMETRES EN ENTREE : - pa_NomMachine : nom de la machine
*	sur laquelle il faut supprimer les tests reseau.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG_SupprimeTest";
   	XZSGT_TEST_RES *pl_Test	= NULL;
   	int vl_TestTrouve = XDC_FAUX;
   
   /* recherche du test dans la liste */
   for (pl_Test = XZSGP_PremTest; 
	(pl_Test != NULL) && (!vl_TestTrouve);
	pl_Test = pl_Test -> TestSuiv)
   {
      if (!strcmp(pl_Test -> NomMachine, pa_NomMachine))
      {
	 /* suppression du test de la liste */
	 if (pl_Test == XZSGP_PremTest)
	 {
	    XZSGP_PremTest = XZSGP_DernTest = NULL;
         }
	 else
	 {
	    /* on relie l'element precedent a l'element suivant */
	    ((XZSGT_TEST_RES *)(pl_Test -> TestPrec)) -> TestSuiv = pl_Test -> TestSuiv; /* chainage avant */
	    /* si element a suprimer n est pas le dernier de la liste */
	    /*    on recalcule le chainage arriere */
	    /* sinon maj de XZSGP_DernTest */
	    if (pl_Test != XZSGP_DernTest)
	       ((XZSGT_TEST_RES *)(pl_Test -> TestSuiv)) -> TestPrec = pl_Test -> TestPrec; /* chainage arriere */
	    else
	       XZSGP_DernTest = pl_Test -> TestPrec;
         }
	 free(pl_Test);
	 vl_TestTrouve = XDC_VRAI;
      }
   }

   return(XDC_OK);
}	/* XZSG_SupprimeTest */	



/*X*/
/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction envoie a la tache spng.x de la machine
* locale, la liste de toutes les demandes de tests
* reseau courante.
*
******************************************************
* SEQUENCE D'APPEL :
*/
int XZSG_EnvoiServices( T_IPC_CONN va_Cnx,
                    T_IPC_CONN_PROCESS_CB_DATA pa_Data,
                    T_CB_ARG pa_Arg)
/*
* PARAMETRES EN ENTREE : - paramatres specifiques a un
*	callback RTWORKS.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG_EnvoiServices";
   XZSGT_TEST_RES *pl_Test;
   XZSCT_NomMachine pl_NomMachineLoc;
   XZSCT_Datagroup pl_NomDatagroup;

	XZST_03EcritureTrace(XZSTC_FONCTION,
		 "XZSG_00EnvoieServices : IN ");


   /* recuperation nom machine locale */
   XZSC_07NomMachine(pl_NomMachineLoc);

   /* preparation du datagroup de spng local*/
   XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPPING,pl_NomMachineLoc);

   /* envoi de tous les services en cours */
   for (pl_Test = XZSGP_PremTest; pl_Test != NULL; pl_Test = pl_Test -> TestSuiv)
   {
       if(!TipcSrvMsgWrite(pl_NomDatagroup
		      ,XDM_IdentMsg(XDM_TSG_PING)
		      ,XDM_FLG_SRVMSGWRITE
		      ,T_IPC_FT_STR, pl_Test -> NomMachine 
   		      ,NULL))
      {
	 XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSG_EnvoiServices : Echec TipcSrvMsgWrite pour machine %s",
				pl_Test -> NomMachine);
	 return(XDC_NOK);
      }
   }

	XZST_03EcritureTrace(XZSTC_FONCTION,
		 "XZSG_00EnvoieServices : OUT ");

   return(XDC_OK);
}	/* Fin XZSG_EnvoieServices **/




/* definition de fonctions externes */



/*X*/
/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction realise les inits de la fonctionnalitee
* tests reseau. Elle effectue :
*	- L'abonnement au datagroup NPINGINIT_NomMachine
*	- L'armement du callback sur le message XDM_TSG_INIT.
*
******************************************************
* SEQUENCE D'APPEL :
*/
int XZSG_00InitServices()
/*
* PARAMETRES EN ENTREE : neant
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG_00InitServices";
   	XZSCT_Datagroup 	pl_NomDatagroup		= "\0";
   	XZSCT_NomMachine 	pl_NomMachineLoc	= "\0";

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		 "XZSG_00InitServices : IN ");


   /* recuperation nom machine locale */
   XZSC_07NomMachine(pl_NomMachineLoc);

   /* preparation du datagroup de demande des services en cours */
   XDG_EncodeDG2(pl_NomDatagroup, XDG_NPINGINIT, pl_NomMachineLoc);

   /* abonnement au datagroup */
   if (!XDG_JoindreDG(pl_NomDatagroup))
   {
	XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSG_00InitServices : Echec XDG_JoindreDG pour datagroup %s",
				pl_NomDatagroup);
   }

   /* on arme le callback de demande des services en cours */
   if (TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TSG_INIT), 
			      (T_IPC_CONN_PROCESS_CB_FUNC) XZSG_EnvoiServices,
			      (T_CB_ARG) NULL) == NULL)
   {
	XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSG_00InitServices : Echec TipcSrvProcessCbCreate pour message XDM_TSG_INIT");
	return(XDC_NOK);
   }

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		 "XZSG_00InitServices : OUT ");

   return(XDC_OK);
}	/* XZSG_00...*/



/*X*/
/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction permet de demander une surveillance 
* reseau pour une machine donnee.
*
******************************************************
* SEQUENCE D'APPEL :
*/
int XZSG_01SurveillerReseau(XZSCT_NomMachine pa_NomMachine) 
/*
* PARAMETRES EN ENTREE : - pa_NomMachine : nom de la machine
*	sur laquelle on demande une surveillance reseau.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : - XDC_OK : si la demande a put etre
*	prise en compte.
*			- XDC_NOK : sinon
*
* CONDITION D'ERREUR : - impossible d'envoyer le message a
*	la tache spng.x
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
   static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG_01SurveillerReseau";
   
   XZSCT_Datagroup  pl_NomDg;
   XZSCT_NomMachine pl_NomMachineLoc;
   int              vl_ValRet = XDC_OK;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
			"IN :XZSG_01SurveillerReseau(%s)", 
			pa_NomMachine);
  
   /* memorisation du test reseau */
   XZSG_InsereTest(pa_NomMachine);

   /* recuperation nom machine locale */
   XZSC_07NomMachine(pl_NomMachineLoc);

   /* preparation du datagroup de spng local*/
   XDG_EncodeDG2(pl_NomDg,XDG_NSUPPING,pl_NomMachineLoc);

   
   /* envoi du message XDM_TSG_PING */
   if(!TipcSrvMsgWrite(pl_NomDg
		      ,XDM_IdentMsg(XDM_TSG_PING)
		      ,XDM_FLG_SRVMSGWRITE
		      ,T_IPC_FT_STR, pa_NomMachine
   		      ,NULL))
   {
      vl_ValRet = XDC_NOK;
   }

   XZST_03EcritureTrace(XZSTC_INTERFACE,
			"OUT :XZSG_01SurveillerReseau \n");

   return(vl_ValRet);
}	/* XZSG_01...*/



/*X*/
/* Fichier : $Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $	Release : $Revision: 1.3 $        Date : $Date: 2010/12/08 22:07:36 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction permet de demander un arret de surveillance 
* reseau pour une machine donnee.
*
******************************************************
* SEQUENCE D'APPEL :
*/
int XZSG_02ArretSurveiller(XZSCT_NomMachine pa_NomMachine) 
/*
* PARAMETRES EN ENTREE : - pa_NomMachine : nom de la machine
*	sur laquelle il faut arreter les tests reseau.
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : - XDC_OK : si la demande a put etre
*	prise en compte.
*			- XDC_NOK : sinon
*
* CONDITION D'ERREUR : - impossible d'envoyer le message a
*	la tache spng.x
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version="$Id: xzsg.c,v 1.3 2010/12/08 22:07:36 gesconf Exp $:  XZSG_02ArretSurveiller";
   
   XZSCT_Datagroup      pl_NomDg;
   XZSCT_NomMachine pl_NomMachineLoc;
   int             vl_ValRet = XDC_OK;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
			"IN :XZSG_02ArretSurveiller(%s)", 
			pa_NomMachine);
  
   /* suppression du test de la liste des tests */
   XZSG_SupprimeTest(pa_NomMachine);

   /* recuperation nom machine locale */
   XZSC_07NomMachine(pl_NomMachineLoc);

   /* preparation du datagroup de spng local*/
   XDG_EncodeDG2(pl_NomDg,XDG_NSUPPING,pl_NomMachineLoc);

   
   /* envoi du message XDM_TSG_STOP_PING */
   if (!TipcSrvMsgWrite(pl_NomDg
		       ,XDM_IdentMsg(XDM_TSG_STOP_PING)
		       ,XDM_FLG_SRVMSGWRITE
		       ,T_IPC_FT_STR, pa_NomMachine
    		       ,NULL))
   {
      vl_ValRet = XDC_NOK;
   }

   XZST_03EcritureTrace(XZSTC_INTERFACE,
			"OUT :XZSG_02ArretSurveiller \n");

   return(vl_ValRet);
} 	/* Fin XZSG_02...*/





