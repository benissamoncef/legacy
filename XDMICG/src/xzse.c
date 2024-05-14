/*E*/
/* Fichier : $Id: xzse.c,v 1.16 2020/11/03 17:57:57 pc2dpdy Exp $        Release : $Revision: 1.16 $        Date : $Date: 2020/11/03 17:57:57 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSE * FICHIER xzse.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSE
*        - Declaration des fonctions externes
*        - Declaration des variables externes
*
*   Reference DCG-021 chapitre 3.3 
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville    23 Aou 1994     : Creation
*  T.Milleville    18 Mai 19945    : Modification XZSE_13 V1.5
*	Ajout du test si PASSIF 
*  T.Milleville    25 Jul 19945    : Ajout de commentaires V1.6
*  T.Milleville    25 Aou 19945    : Ajout dune priorite sur envoie
*	de l'etat d'une tache une priorite sur envoie
*	de l'etat d'une tache V1.7
*  T.Milleville    12 Oct 1995    : Modification XZSE_14. Recuperation du 
*	type de peripherique et l'information avec le msg XDM_TSE_PERI V1.8
* Niepceron2	23 aut 1996 :	ajout XZSE_Abt_Etat_Comm_Site (RADT) V1.9
* Niepceron	23 aut 1996 :  correction sur XZSE_Abt_Etat_Comm_Site et creation Callback sur tous
				les type de message de XDG_IHM_ADM v1.11
* Niepceron	13 Sep 1996 : Utilisation de XZSA_23SiteMachine pour connaitre le nom du site de la machine v1.12
* Niepceron     16 Dec 1996 : On ne tient compte que des serveurs dont le nom se termine pas 1 (pas les esclave) v1.13
* JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : commentaires ; fichiers d'en-tete 1.15
* JMg		07/02/20 :; EMS 1.16
*******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
* NOM DU RELECTEUR : 
* COMMENTAIRES     :
*
*
*******************************************************/


/* fichiers inclus */

#include "xdc.h"
#include "xdg.h"
#include "xdm.h"
#include "xdy.h"
 
#include <rtworks/common.h>
#include <rtworks/ipc.h>

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "xzst.h"
#include "xzsc.h"
#include "xzse.h"  
#include "xzsa.h"
#include "xzsm.h"

/* declaration des constantes locales */

/* declaration des variables locales  */
static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE " ;

int 			XZSEV_ModeStationIns 	= XZSEC_MODE_NOR;
int 			XZSEV_ModeStationRec 	= XZSEC_MODE_NOR;

/*int 			XZSEV_EtatMachine 	= XZSEC_ETAT_NACTIF; */
int 			XZSEV_EtatMachine 	=  XZSEC_SANS_ETAT; 

XDY_Fonction		pg_FonctUtil_XZSE = NULL;
T_CB			vg_CB_XZSE         = NULL;
T_CB                    vg_CB_XZSE_STA_E= NULL,
			vg_CB_XZSE_STA_M= NULL,
			vg_CB_XZSE_PER= NULL,
			vg_CB_XZSE_INT=NULL,
			vg_CB_XZSE_SSS=NULL,
			vg_CB_XZSE_TRA=NULL,
			vg_CB_XZSE_TAC=NULL,
			vg_CB_XZSE_REINIT=NULL;


XZSCT_NomMachine	XZSEV_NomSCActif 	= "" ;
XZSCT_NomMachine	XZSEV_NomSDActif 	= "" ;
void			(*XZSEV_FonctionBasc)() = NULL;
void			(*XZSEV_FonctionBascSD)() = NULL;

/* declaration de fonctions internes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour les modes d'instructions
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void XZSE_ActualiserMode(
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*   Arg specifiques a un Callback RTWorks
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere les donnes et met a jour les variables globales
*
------------------------------------------------------*/
{
	static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_ActualiserMode " ;
	int vl_ModeIns, vl_ModeRec;

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserMode : IN \n ");

   /* Si la recuperation de ModeIns et ModeRec s'effectue 
   *	sans probleme  mise a jour de XZSEV_ModeIns et de XZSEV_ModeRec */
   if (TipcMsgRead(pa_Data->msg,
	           T_IPC_FT_INT4,&vl_ModeIns,
	           T_IPC_FT_INT4,&vl_ModeRec,
	           NULL)) 
   {
  	XZSEV_ModeStationIns = vl_ModeIns;
   	XZSEV_ModeStationRec = vl_ModeRec;
   }
   else 
	XZST_03EcritureTrace(XZSTC_WARNING,"XZSE_ActualiserMode:TipcMsgRead");

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserMode : OUT \n ");

}	/* Fin XZSE_ActualiserMode */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour l'etat d'une machine
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void XZSE_ActualiserEtat(
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*   Arg specifiques a un callback RTWorks
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere l'etat du calculateur, puis la compare a la valeur courante
*	et dans le cas d'une difference , declenche un basculement
*
------------------------------------------------------*/
{
	static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_ActualiserEtat " ;
	int vl_EtatMachine	= 0;

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserEtat : IN \n ");

   /* Si la recuperation de l'etat de la machine s'effectue sans probleme */
   if (TipcMsgRead(pa_Data->msg,
	           T_IPC_FT_INT4,&vl_EtatMachine,
	           NULL)) 
   {

   	if ((vl_EtatMachine != XZSEV_EtatMachine) 
			&& (XZSEV_EtatMachine != XZSEC_SANS_ETAT)) 
	{
	    if (XZSEV_FonctionBasc != NULL) (*XZSEV_FonctionBasc)();
	}	
        XZSEV_EtatMachine = vl_EtatMachine;	
   }
   else 
	XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSE_ActualiserEtat:TipcMsgRead");

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserEtat : OUT \
   -- Etat Machine %d \n", XZSEV_EtatMachine);

}	/* Fin XZSE_ActualiserEtat */





/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour le nom du Serveur de Donnees 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void XZSE_ActualiserNomSD(
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*   Arg specifiques a un callback RTWorks
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucune
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere le nom de la machine , puis la compare au  nom du 
*	serveur de donnees courant. Si ill existe une differnce 
*	alors basculement.
*
------------------------------------------------------*/
{
	static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_ActualiserNomSD " ;
#ifndef _TIBCO_EMS
	T_STR	 pl_NomMachine;
#else
	XDY_NomMachine pl_NomMachine;
#endif
   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserNomSD : IN \n");
   
   /* Si la recuperation de NomMachine s'effectue sans probleme 
   *			mise a jour de XZSEV_NomSDActif */
   if (TipcMsgRead(pa_Data->msg,
	           T_IPC_FT_STR,&pl_NomMachine,
	           NULL)) 
   {
   if (XZSEV_NomSDActif != NULL) 
   {
    	if (XZSEV_FonctionBasc != NULL) (*XZSEV_FonctionBascSD)();
   }	
   
	strcpy(XZSEV_NomSDActif,pl_NomMachine);
        XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSE_ActualiserNomSD %s",XZSEV_NomSDActif);
   }
   else 
	XZST_03EcritureTrace(XZSTC_WARNING,"XZSE_ActualiserNomSD:TipcMsgRead");

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserNomSD : OUT  \n");

}	/* Fin XZSE_ActualiserNomSD */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de mettre a jour le nom du Serveur de Communications  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void XZSE_ActualiserNomSC(
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_ARG)

/*
* ARGUMENTS EN ENTREE :
*   Arg specifiques a un callback RTWorks
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   Aucune
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Recupere le nom de la machine , puis la compare au  nom du 
*	serveur de donnees courant. Si ill existe une differnce 
*	alors basculement.
*
------------------------------------------------------*/
{
	static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_ActualiserNomSC " ;
#ifndef _TIBCO_EMS
	T_STR	 pl_NomMachine;
#else
	XDY_NomMachine pl_NomMachine;
#endif

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserNomSC : IN \n");

   /* Si la recuperation de NomMachine s'effectue sans probleme 
   *		mise a jour de XZSEV_NomSCActif */
   if (TipcMsgRead(pa_Data->msg,
	           T_IPC_FT_STR,&pl_NomMachine,
	           NULL)) 
   {
	strcpy(XZSEV_NomSCActif,pl_NomMachine);
        XZST_03EcritureTrace(XZSTC_FONCTION,
		"XZSE_ActualiserNomSC %s",XZSEV_NomSCActif);
   }
   else 
	XZST_03EcritureTrace(XZSTC_WARNING,"XZSE_ActualiserNomSC:TipcMsgRead");

   XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSE_ActualiserNomSC : OUT \n");

}	/* Fin XZSE_ActualiserNomSC */




/* definition de fonctions externes */


/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* ModeIns	: XZSEC_MODE_INS ou XZSEC_MODE_NOR ou 
*		  XZSEC_MODE_TST
*
* Fonction
* La fonction appelle la primitive XZSC_07. La fonction 
* envoie le message XDM_TSE_MODI au datagroup 
* XDG_NSUPETAT_NomMachine. Ce message contient 
* l'argument ModeIns au format %d.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOM_INV		Nom machine invalide 
*			(compte-rendu de XZSC_07)
* XZSEC_ARG_INV 	Argument ModeIns invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

int XZSE_01PassageModeExpIns(int va_ModeIns)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 :  XZSE_01PassageModeExpIns";

   int 			vl_ValRet = XDC_OK;
   XZSCT_NomMachine 	pl_NomMachine;
   XZSCT_Datagroup 	pl_NomDatagroup;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_01PassageModeExpIns : In  ModeIns:%d \n", va_ModeIns);
   
   /* verification de ModeIns */
   if ((va_ModeIns == XZSEC_MODE_INS) 
		|| (va_ModeIns == XZSEC_MODE_NOR) 
		|| (va_ModeIns  == XZSEC_MODE_TST))
   {
  	/* recuperation du nom de la machine */
  	vl_ValRet = XZSC_07NomMachine(pl_NomMachine);

   	if (vl_ValRet == XDC_OK)
   	{
     	    /*B construction du nom du datagroup XDG_NSUPETAT_NomMachine */
            XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPETAT, pl_NomMachine);

            /*B envoi du message XDM_TSE_MODI */
            if (!TipcSrvMsgWrite(pl_NomDatagroup,
			        XDM_IdentMsg(XDM_TSE_MODI),
			        XDM_FLG_SRVMSGWRITE,
			        T_IPC_FT_INT4, va_ModeIns,
			        NULL))
      	    {
	    	vl_ValRet = XDC_NOK;
	    	XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_01PassageModeExpIns:\
			TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%u)",
				 pl_NomDatagroup,
			         XDM_IdentMsg(XDM_TSE_MODI),
				 va_ModeIns);
            }
            TipcSrvFlush();
   	}
	else
		vl_ValRet = XZSEC_NOM_INV;
		

   }
   else 
	vl_ValRet = XZSEC_ARG_INV;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_01PassageModeExpIns : OUT  Code Retour %d \n",
		vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_01... */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* ModeIns :	Mode instruction
*
* Fonction
* Cette fonction retourne le contenu de la variable
* XZSEV_ModeStationIns dans l'argument ModeIns.
*
* Codes retour
* XDC_OK	 ok.
*
* Mecanismes
******************************************************/

int XZSE_03ModeIns(int *pa_ModeIns)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_03ModeIns";
   int 		vl_ValRet 	= XDC_OK;


	XZST_03EcritureTrace(XZSTC_INTERFACE,
			"XZSE_03ModeIns : IN \n");

   /* recuperation du contenu de la variable XZSEV_ModeStationIns */
   *pa_ModeIns = XZSEV_ModeStationIns; 

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_03ModeIns : OUT  - Mode Ins %d -- Code Retour %d\n"			,*pa_ModeIns, vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_03ModeIns */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomSite :	Nom du site.
*
* Fonction
* Cette fonction envoie le message XDM_TSE_BASC au 
* datagroup XDG_NSUPHIST_NomSite contenant la chaine
* "BASC_SC_NomSite" au format %s.
* L'existence du site est verifie avec la primitive 
* XZSA_22.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOA_INV		Argument NomSite invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

int XZSE_05BasculementSC(XZSCT_NomSite	va_NomSite)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_05BasculementSC";

   int                  vl_ValRet = XDC_OK;
   char			pl_NomSite[20];
   XZSCT_Datagroup      pl_NomDatagroup;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_05Basculement : IN - NomSite:%s \n", va_NomSite);
   
   /* verification du nom du site */
   vl_ValRet = XZSA_22ExisterSite(va_NomSite);

   if (vl_ValRet == XDC_OK)
   {
      /* construction du nom du datagroup XDG_NSUPHIST_NomSite */
      XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPHIST, va_NomSite);

      /* construction de la chaine contenue dans le message BASC_SC_NomSite */
      sprintf(pl_NomSite, "%s_%s", "BASC_SC", va_NomSite);
   
      /* envoi du message XDM_TSE_BASC */
      if (!TipcSrvMsgWrite(pl_NomDatagroup,
                          XDM_IdentMsg(XDM_TSE_BASC),
                          XDM_FLG_SRVMSGWRITE,
                          T_IPC_FT_STR, pl_NomSite,
                          NULL))
      {
         /* trace de l'erreur */
         vl_ValRet = XDC_NOK;
	 XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_05BasculementSC:\
		TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s)",
				 pl_NomDatagroup,
			         XDM_IdentMsg(XDM_TSE_BASC),
				 pl_NomSite);
      }

      TipcSrvFlush();

      /*A Message de Basculement dans le Journal D'administartyion */
   }
   else
	vl_ValRet = XZSEC_NOA_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_05Basculement : OUT - Code retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_05.. */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction
* Cette fonction envoie le message XDM_TSE_BASC au 
* datagroup XDG_NSUPHIST contenant la chaine "BASC_SD" 
* au format %s.
* 
* Codes retour
* XDC_OK		Ok.
* XZSEC_ARG_INV		Argument invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_07BasculementSD()
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_07BasculementSD";
   int                  vl_ValRet = XDC_OK;


   XZST_03EcritureTrace(XZSTC_INTERFACE, "XZSE_07BasculementSD : IN \n");

   /* envoi du message XDM_TSE_BASC */
   if (!TipcSrvMsgWrite(XDG_NSUPHIST,
                       XDM_IdentMsg(XDM_TSE_BASC),
                       XDM_FLG_SRVMSGWRITE,
                       T_IPC_FT_STR, "BASC_SD",
                       NULL))

   {
         /* trace de l'erreur */
         vl_ValRet = XDC_NOK;
	 XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_07BasculementSD:\
		TipcSrvMsgWrite(NSUPHIST,%u,XDM_FLG_SRVMSGWRITE,,BASC_SD)",
			         XDM_IdentMsg(XDM_TSE_BASC));
   }
   TipcSrvFlush();

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_07BasculementSD : OUT - Code retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_07BasculementSD */




/*X*/
/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* EtatMachine :	XZSEC_ETAT_NACTIF ou XZSEC_ETAT_NPASSIF.
* 
* Fonction
* Cette fonction retourne le contenu de la variable 
* XZSEV_EtatMachine dans l'argument EtatMachine.
* 
* Codes retour
* XDC_OK	ok.
*
* Mecanismes
********************************************************/

int XZSE_09EtatMachine(int *pa_EtatMachine)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_09EtatMachine";
   int	vl_ValRet = XDC_OK;
   
   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_09EtatMachine : IN \n");

   /* verification de la valeur de l'etat de la machine */
   if ((XZSEV_EtatMachine == XZSEC_ETAT_NACTIF) 
	|| (XZSEV_EtatMachine == XZSEC_ETAT_NPASSIF))
   {
	/* affectation de l'etat de la machine 
	*			a la variable XZSEV_EtatMachine */
   	*pa_EtatMachine = XZSEV_EtatMachine;
   }   
   else 
	vl_ValRet = XDC_ARG_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_09EtatMachine : OUT - Eta Mach %d -- Code retour %d \n",
		*pa_EtatMachine, vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_09EtatMachine */


/*X*/
/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* 
* Fonction
* La fonction arme un callback sur reception des 
* messages XDM_TSE_MAJM. Ce callback est la fonction 
* interne suivante XZSE_ActualiserMode.
*   XZSE_ActualiserMode((IN)(T_MSG_HEADER *)msg_header,         
*			(OUT)(char *)msg_data)
*   Les arguments sont specifiques a un callback IPC. 
*   Cette fonction recupere les informations ModeIns 
*   et ModeRec et actualise les variables XZSEV_ModeIns
*   et XZSEV_ModeRec.
* La fonction arme un callback sur reception des 
* messages XDM_TSE_MAJE. Ce callback est la fonction 
* interne suivante XZSE_ActualiserEtat.
*   XZSE_ActualiserEtat((IN)(T_MSG_HEADER *)msg_header, 
*   (OUT)(char *)msg_data)
*   Les arguments sont specifiques a un callback IPC. 
*   Cette fonction recupere l'informations EtatMachine.
*   Si EtatMachine <> XZSEV_EtatMachine et 
*   XZSEV_EtatMachine <> XZSEC_SANS_ETAT (basculement)
*	Elle appelle la fonction XZSEV_FonctionBasc si
*	different de NULL.
*   Fsi
*   XZSEV_EtatMachine = EtatMachine
* La fonction arme un callback sur reception des messages
* XDM_TSE_MAJD. Ce callback est la fonction interne 
* suivante XZSE_ActualiserNomSD.
*   XZSE_ActualiserNomSD((IN)(T_MSG_HEADER *)msg_header,
* 			(OUT)(char *)msg_data)
*   Les arguments sont specifiques a un callback IPC. 
*   Cette fonction recupere l'informations NomMachine.
*   Si XZSEV_NomSDActif <> NULL et 
*   XZSEV_NomSDActif <> NomMachine (basculement)
*	Elle appelle la fonction XZSEV_FonctionBascSD 
*	si different de NULL.
*   Fsi
*   XZSEV_NomSDActif = NomMachine.
* La fonction arme un callback sur reception des
* messages XDM_TSE_MAJC. Ce callback est la fonction 
* interne suivante XZSE_ActualiserNomSC.
*   XZSE_ActualiserNomSC((IN)(T_MSG_HEADER *)msg_header,
*			(OUT)(char *)msg_data)
*   Les arguments sont specifiques a un callback IPC. 
*   Cette fonction recupere l'informations NomMachine et 
*   actualise la variable XZSEV_NomSCActif.
*
* Codes retour
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS3.
********************************************************/

int XZSE_10AutoriseMajModeEtat()
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_10AutoriseMajModeEtat";
   int                  vl_ValRet = XDC_OK;
  
   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_10AutoriseMajModeEtat : IN  \n");


   /*A armement d'un callback sur reception du message XDM_TSE_MAJM */ 
  if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MAJM),
			XZSE_ActualiserMode,(T_CB_ARG)NULL) == NULL) 
	vl_ValRet = XDC_NOK;  
   
   
   /* armement d'un callback sur reception du message XDM_TSE_MAJE */ 
   if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MAJE),
			XZSE_ActualiserEtat,(T_CB_ARG)NULL) == NULL) 
	vl_ValRet = XDC_NOK;  
 
 
   /* armement d'un callback sur reception du message XDM_TSE_MAJD */ 
   if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MAJD),
		XZSE_ActualiserNomSD,(T_CB_ARG)NULL) == NULL) 
	vl_ValRet = XDC_NOK;  
   
   
   /* armement d'un callback sur reception du message XDM_TSE_MAJC */ 
   if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSE_MAJC),
		XZSE_ActualiserNomSC,(T_CB_ARG)NULL) == NULL) 
	vl_ValRet = XDC_NOK;  
  
  
   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_10AutoriseMajModeEtat : OT - Code Retour %d \n",
		vl_ValRet);

   return(vl_ValRet);

}	/* XZSE_10AutoriseMajModeEtat */



/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine   : Nom de la machine du sous-systeme.
* NomSSysteme  : Nom du sous-systeme.
* EtatSSysteme : XZSEC_ETAT_ NOK , XZSEC_ETAT_OK ou
*		 XZSEC_SANS_ETAT.
*
* Fonction
* Cette fonction appelle la primitive XZSC_04 pour 
* trouver le nom du site de la machine.
* Puis elle envoie le message XDM_TSE_SSYS au datagroup
* XDG_NSUPETAT_NomMachine contenant les elements
* "NomMachine" "NomSSysteme" " EtatSSysteme" au 
* format %s %s %d.
* Le nom de la machine est verifie avec la primitive 
* XZSA_05.
* Le nom du sous-systeme est verifie avec la primitive
* XZSA_06.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOS_INV		Nom du sous-systeme invalide.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument EtatSSysteme invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_11EnvoieEtatSSysteme(XZSCT_NomMachine	va_NomMachine,
			  XZSCT_NomSSysteme	va_NomSSysteme,
			  int			va_EtatSSysteme)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_11EnvoieEtatSSysteme";
   int                  vl_ValRet = XDC_OK;
   XZSCT_Datagroup      pl_NomDatagroup;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_11EnvoieEtatSSysteme : IN \
		-- NomMach %s -- NomSSysteme: %s -- EtatSSysteme: %d \n",
		va_NomMachine, va_NomSSysteme, va_EtatSSysteme);
   
   /* verification du nom de la machine */
   vl_ValRet = XZSA_05ExisterMachine(va_NomMachine);
   if (vl_ValRet == XDC_OK)
   {
   	/* verification du nom du sous systeme */
   	vl_ValRet = XZSA_06ExisterSSysteme(va_NomSSysteme);

	/* test de l'etat du systeme */
	if ((vl_ValRet == XDC_OK) && 
		((va_EtatSSysteme == XZSEC_ETAT_NOK) 
			|| (va_EtatSSysteme == XZSEC_ETAT_OK) 
			|| (va_EtatSSysteme == XZSEC_SANS_ETAT)))
   	{
	  	/* construction du nom du datagroup 
		*	XDG_NSUPETAT_NomMachine pour le seta
		*  de la machine concernee */
      		XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPETAT, va_NomMachine);
          	
		/* envoi du message XDM_TSE_SSYS */
/*           	if (!TipcSrvMsgWrite(pl_NomDatagroup,
                                    XDM_IdentMsg(XDM_TSE_SSYS),
                                    XDM_FLG_SRVMSGWRITE,
                                    T_IPC_FT_STR, va_NomMachine,
                                    T_IPC_FT_STR, va_NomSSysteme,
                                    T_IPC_FT_INT4, va_EtatSSysteme,
                                    NULL))
      	   	{
*/           	/* trace de l'erreur */
/*           	vl_ValRet = XDC_NOK;
	        	XZST_03EcritureTrace(XZSTC_WARNING,
		           "XZSE_11EnvoieEtatSSysteme:\
			TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%s,,%u)",
			     pl_NomDatagroup, XDM_IdentMsg(XDM_TSE_SSYS),
			 va_NomMachine, va_NomSSysteme, va_EtatSSysteme);
      	   	}
*/      		TipcSrvFlush();
       	}
        else 
		vl_ValRet = XZSEC_NOS_INV;	
   }
   else
	vl_ValRet = XZSEC_NOM_INV;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_11EnvoieEtatSSysteme : OUT -- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_11... */


/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine : Nom de la machine de la tache.
* NomTache   : Nom de la tache.
* EtatTache  : XZSEC_ETAT_ NOK , XZSEC_ETAT_OK 
*		ou XZSEC_SANS_ETAT.
*
* Fonction
* Cette fonction appelle la primitive XZSC_04 pour 
* trouver le nom du site de la machine.
* Puis elle envoie le message XDM_TSE_TACH au 
* datagroup XDG_NSUPETAT_NomMachine contenant les elements 
* "NomMachine" "NomTache" "EtatTache" 
* au format %s %s %d.
* Le nom de la machine est verifie avec la primitive 
* XZSA_05.
* Le nom de la tache est verifie avec la primitive 
* XZSA_07.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOT_INV		Nom de tache invalide.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument EtatTache invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_12EnvoieEtatTache(XZSCT_NomMachine	va_NomMachine,
		       XZSCT_NomTache	va_NomTache,
		       int		va_EtatTache,
		       int		va_Priorite)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 :  XZSE_12EnvoieEtatTache";

   int                  vl_ValRet 		= XDC_OK;
   XZSCT_Datagroup      pl_NomDatagroup		= "";
   T_IPC_MSG 		vl_Msg;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_12EnvoieEtatTache : IN \
	-- NomMach %s -- NomTache %s -- EtatTache %d \n",
		va_NomMachine, va_NomTache, va_EtatTache);
   
   /* verification du nom de la machine */
   vl_ValRet = XZSA_05ExisterMachine(va_NomMachine);
   if (vl_ValRet == XDC_OK)
   {
   	/* verification du nom de la tache */
   	vl_ValRet = XZSA_07ExisterTache(va_NomTache);
	/* verification de l'etat de la tache */
   	if ((vl_ValRet == XDC_OK) && 
		((va_EtatTache == XZSEC_ETAT_OK) 
				|| (va_EtatTache == XZSEC_ETAT_NOK) 
				|| (va_EtatTache == XZSEC_SANS_ETAT)))
   	{
	  	/* construction du nom du datagroup 
		*		XDG_NSUPETAT_NomMachine pour le seta
		*  de la machine concernee */
      		XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPETAT, va_NomMachine);


           	/* envoi du message XDM_TSE_TACH */
           	if (!TipcSrvMsgWrite(pl_NomDatagroup,
                                    XDM_IdentMsg(XDM_TSE_TACH),
                                    XDM_FLG_SRVMSGWRITE,
                                    T_IPC_FT_STR, va_NomMachine,
                                    T_IPC_FT_STR, va_NomTache,
                                    T_IPC_FT_INT4, va_EtatTache,
                                    NULL))
      	   	{
           		vl_ValRet = XDC_NOK;
	        	XZST_03EcritureTrace(XZSTC_WARNING, 
				"XZSE_12EnvoieEtatTache:\
			TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%s,,%u)",
				 pl_NomDatagroup,
			         XDM_IdentMsg(XDM_TSE_TACH),
				 va_NomMachine,
				 va_NomTache,
				 va_EtatTache);
      	   	}
      		TipcSrvFlush();
   	}
   	else 
		vl_ValRet = XZSEC_NOT_INV;
   }
   else 
	vl_ValRet = XZSEC_NOM_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_12EnvoieEtatTache : OUT -- CodeRetour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_12... */


/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine  :	Nom du calculateur.
* EtatMachine :	XZSEC_SANS_ETAT ou
*		XZSEC_ETAT_VEILLE ou       
*		XZSEC_ETAT_NACTIF ou       
*		XZSEC_ETAT_NPASSIF ou      
*		XZSEC_ETAT_DEGRADE.
*
* Fonction
* Cette fonction appelle la primitive XZSC_04 pour 
* trouver le nom du site de la machine.
* Puis elle envoie le message XDM_TSE_APPLI au 
* datagroup XDG_NSUPHIST_NomSite contenant les elements 
* NomMachine et EtatMachine au format %s %d.
* Le nom du calculateur est verifie avec la primitive
* XZSA_05.
* 
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument EtatMachine invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_13EnvoieEtatMachine(
	XZSCT_NomMachine	va_NomMachine,
	int			va_EtatAppli)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_13EnvoieEtatMachine";
   int                  vl_ValRet 	= XDC_OK;
   XZSCT_Datagroup      pl_NomDatagroup	= "";



   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_13EnvoieEtatAppli : IN \
		-- NomMach %s -- EtatAppli %d\n",
		va_NomMachine, va_EtatAppli);
   
   /* verification du nom de la machine */
   vl_ValRet = XZSA_05ExisterMachine(va_NomMachine);

    /* verification de l'etat de la machine */
   if((vl_ValRet == XDC_OK)
	&& ((va_EtatAppli == XZSEC_ETAT_NACTIF)
			|| (va_EtatAppli == XZSEC_ETAT_DEGRADE)
			|| (va_EtatAppli == XZSEC_ETAT_NPASSIF)
			|| (va_EtatAppli == XZSEC_SANS_ETAT)))
   {  
	   /* construction du nom du datagroup XDG_NSUPETAT_NomMachine */
      	   XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPETAT, va_NomMachine);


	XZST_03EcritureTrace(XZSTC_MESSAGE," XZSE_13 : \
		Envoie XDM_TSE_APPLI : -- Nom mach %s -- Etat mach %d \n",
		va_NomMachine,va_EtatAppli);

		/* envoi du message XDM_TSE_APPLI */
		if (!TipcSrvMsgWrite(pl_NomDatagroup,
			    	XDM_IdentMsg(XDM_TSE_APPLI),
				XDM_FLG_SRVMSGWRITE,
			    	T_IPC_FT_STR, va_NomMachine,
				T_IPC_FT_INT4, va_EtatAppli,
				NULL))
		{
           		/* trace de l'erreur */
           		vl_ValRet = XDC_NOK;

           	XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_13EnvoieEtatMachine:\
			TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%u)",
				 pl_NomDatagroup,
			         XDM_IdentMsg(XDM_TSE_APPLI),
                                 va_NomMachine,
                                 va_EtatAppli);
      	   }
      	TipcSrvFlush();
   }
   else
	vl_ValRet = XZSEC_NOM_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_13EnvoieEtatM&achine : OUT -- CodeRetour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_13... */



/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomPeriph  : Nom du peripherique.
* EtatPeriph : XZSEC_ETAT_ NOK ou XZSEC_ETAT_OK.
*
* Fonction
* Cette fonction envoie le message XDM_TSE_PERI au 
* datagroup XDG_NSUPHIST contenant les elements 
* NomPeriph et EtatPeriph au format %s %d.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_ARG_INV		Argument EtatPeriph invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/


int XZSE_14EnvoieEtatPeriph(
	XZSCT_NomPeriph      va_NomPeriph,
	int		va_EtatPeriph)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_14EnvoieEtatPeriph";
   int  vl_ValRet = XDC_OK;
   int 		vl_TypeEntite	= 0;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_14EnvoieEtatPeriph : IN \
		-- NomPeriph %s -- EtatPeriph %d \n",
			va_NomPeriph, va_EtatPeriph);
	/*B Recuepration du type de peripherique */
	if ( (vl_ValRet = XZSA_26TypeEntite (va_NomPeriph, &vl_TypeEntite)) != XDC_OK)
	{
   		XZST_03EcritureTrace(XZSTC_WARNING,
			" XZSE_14EnvoieEtatPeriph :  Probleme recuperation type entite %s \n",va_NomPeriph);
	}

	if (vl_ValRet == XDC_OK)
	{

   /* verification de l'etat du peripherique */
   if ((va_EtatPeriph == XZSEC_ETAT_NOK) ||(va_EtatPeriph == XZSEC_ETAT_OK))
   {

#if 0
	/* envoi du message XDM_TSE_PERI */
   	if (!TipcSrvMsgWrite("NSUPHIST",
                       	    XDM_IdentMsg(XDM_TSE_PERI),
                       	    XDM_FLG_SRVMSGWRITE,
                       	    T_IPC_FT_STR, va_NomPeriph,
                       	    T_IPC_FT_INT4, va_EtatPeriph,
                       	    NULL))
#endif

#if 1
	/* envoi du message XDM_TSE_PERI */
   	if (!TipcSrvMsgWrite("NSUPHIST",
                       	    XDM_IdentMsg(XDM_TSE_PERI),
                       	    XDM_FLG_SRVMSGWRITE,
                       	    T_IPC_FT_STR, va_NomPeriph,
                       	    T_IPC_FT_INT4, va_EtatPeriph,
                       	    T_IPC_FT_INT4, vl_TypeEntite,
                       	    NULL))
#endif
      	{  
           /* trace de l'erreur */
           vl_ValRet = XDC_NOK;
	   XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_14EnvoieEtatPeriph:\
		TipcSrvMsgWrite(NSUPHIST,%u,XDM_FLG_SRVMSGWRITE,,%s,,%u,%u)",
			         XDM_IdentMsg(XDM_TSE_PERI),
				 va_NomPeriph,
				 va_EtatPeriph, vl_TypeEntite);
        }  
        TipcSrvFlush();
   }
   else 
	vl_ValRet = XZSEC_ARG_INV;

	}

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_14EnvoieEtatPeriph : OUT  -- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);


}	/* XZSE_14... */


/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomInterface    : Nom de l'interface.
* EtatInterface   : XZSEC_ETAT_ NOK ou XZSEC_ETAT_OK.
*
* Fonction
* Cette fonction envoie le message XDM_TSE_INTE au 
* datagroup XDG_NSUPHIST contenant les elements 
* NomInterface et EtatInterface au format %s %d.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_ARG_INV		Argument EtatInterface invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_15EnvoieEtatInterface(
	XZSCT_NomInterface	va_NomInterface,
   	int			va_EtatInterface)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : ZSE_15EnvoieEtatInterface";
   int  vl_ValRet = XDC_OK;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_15EnvoieEtatInterface : IN \
	-- NomInterface %s -- EtatInterface %d \n",
			va_NomInterface, va_EtatInterface);

   /* verification de l'etat de l'interface */
   if ((va_EtatInterface == XZSEC_ETAT_NOK) 
		||(va_EtatInterface == XZSEC_ETAT_OK))
   {
   	/* envoi du message XDM_TSE_INTE */
   	if (!TipcSrvMsgWrite("NSUPHIST",
                            XDM_IdentMsg(XDM_TSE_INTE),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, va_NomInterface,
                            T_IPC_FT_INT4, va_EtatInterface,
                            NULL))
      	{  
           /* trace de l'erreur */
           vl_ValRet = XDC_NOK;
	   XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_15EnvoieEtatInterface:\
		TipcSrvMsgWrite(NSUPHIST,%u,XDM_FLG_SRVMSGWRITE,,%s,,%u)",
			         XDM_IdentMsg(XDM_TSE_INTE),
				 va_NomInterface,
				 va_EtatInterface);
      	}  
      	TipcSrvFlush();
  }
  else vl_ValRet = XDC_NOK;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_15EnvoieEtatInterface : OUT -- Code Retour %d \n",vl_ValRet);

  return(vl_ValRet);

}	/* Fin XZSE_15EnvoieEtatInterface */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine	: Nom de la machine.
* ModeIns	: XZSEC_MODE_INS ou XZSEC_MODE_NOR.
* ModeRec	: XZSEC_MODE_REC ou XZSEC_MODE_NOR.
*
* Fonction
* Cette fonction appelle la primitive XZSC_04 pour 
* trouver le nom du site de la machine.
* Puis elle envoie le message XDM_TSE_MODE au datagroup
* XDG_NSUPHIST_NomSite contenant les elements NomMachine 
* et ModeIns et ModeRec au format %s %d %d.
* Le nom de la machine est verifie avec la primitive 
* XZSA_05.
*
* Codes retour
* XDC_OK 		Ok.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument ModeIns ou ModeRec 
*			invalide.
*
* Mecanismes
* RTworks : RTS2. 
********************************************************/

int XZSE_17EnvoieModeStation(XZSCT_NomMachine	va_NomMachine,
			 int			va_ModeIns,
			 int			va_ModeRec)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_17EnvoieModeStation";
   int                  vl_ValRet 	= XDC_OK;
   XZSCT_NomSite	pl_NomSite	= "";
   XZSCT_Datagroup      pl_NomDatagroup	= "";

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_17EnvoieModeStation : IN \
		-- NomMachine %s -- ModeIns %d -- ModeRec %d \n ",
			va_NomMachine, va_ModeIns, va_ModeRec);

   /* verification du nom de la machine */
   vl_ValRet = XZSA_05ExisterMachine(va_NomMachine);
   /* verification de ModeIns et de ModeRec */
   if ((vl_ValRet == XDC_OK) 
	&& ((va_ModeIns == XZSEC_MODE_INS) || (va_ModeIns == XZSEC_MODE_NOR)) 
	&& ((va_ModeRec == XZSEC_MODE_REC) || (va_ModeRec == XZSEC_MODE_NOR))) 
   {
      	/* recherche du nom du site de la machine */
      	vl_ValRet = XZSC_04NomSite(pl_NomSite);
      	if (vl_ValRet == XDC_OK)
      	{
	   /* construction du nom du datagroup XDG_NSUPHIST_NomSite */
      	   XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPHIST, pl_NomSite);

           /* envoi du message XDM_TSE_MODE */
           if (!TipcSrvMsgWrite(pl_NomDatagroup,
                               XDM_IdentMsg(XDM_TSE_MODE),
                               XDM_FLG_SRVMSGWRITE,
                               T_IPC_FT_STR, va_NomMachine,
                               T_IPC_FT_INT4, va_ModeIns,
                               T_IPC_FT_INT4, va_ModeRec,
                               NULL))
      	   {
           	/* trace de l'erreur */
           	vl_ValRet = XDC_NOK;
	    	XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_17EnvoieModeStation:\
			TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%u,,%u)",
				 pl_NomDatagroup,
			         XDM_IdentMsg(XDM_TSE_MODE),
				 va_NomMachine,
				 va_ModeIns,
				 va_ModeRec);
      	   }
      	TipcSrvFlush();
	}
   	else 
		vl_ValRet = XZSEC_NOA_INV;
   }
   else 
	vl_ValRet = XZSEC_NOM_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_17EnvoieModeStation : OUT -- Code Retour %d\n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_17... */



/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine : Nom du serveur de communication actif.
*
* Fonction
* Cette fonction retourne le contenu de la variable 
* XZSEV_NomSCActif dans l'argument NomMachine.
*
* Codes retour
* XDC_OK		ok.
*
* Mecanismes
********************************************************/

int XZSE_18NomSCActif(XZSCT_NomMachine	va_NomMachine)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_18NomSCActif";
  int 	vl_ValRet 		= XDC_OK;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZSE_18NomSCActif : IN \n");

   /* recuperation du contenu de la variable XZSEV_NomSCActif */
   strcpy(va_NomMachine,XZSEV_NomSCActif);

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZSE_18NomSCActif : OUT \
		-- Nom SC %s -- Code Retour %d\n",va_NomMachine,vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_18NomSCActif */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine :	Nom du serveur de donnees actif.
*
* Fonction
* Cette fonction retourne le contenu de la variable
* XZSEV_NomSDActif dans l'argument NomMachine.
*
* Codes retour
* XDC_OK		ok.
*
* Mecanismes
********************************************************/

int XZSE_19NomSDActif(XZSCT_NomMachine	va_NomMachine)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_19NomSDActif";
   int 	vl_ValRet 	= XDC_OK;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_19NomSDActif : IN \n");


   	/* recuperation du contenu de la variable XZSEV_NomSDActif */
   	strcpy(va_NomMachine,XZSEV_NomSDActif);

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZSE_19NomSDActif : OUT \
		-- Nom SD %s -- Code Retour %d\n",va_NomMachine,vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_19NomSDActif */



/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction
* Cette fonction envoie le message XDM_TSE_ISAG au 
* datagroup XDG_NSUPETAT.
*
* Codes retour
* XDC_OK                                Ok.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_20InitSynoptiqueAG()
{
   static char *version="@(#)xzse.c	1.13 09/29/97 :  XZSE_20InitSynoptiqueAG";
   int  vl_ValRet = XDC_OK;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_20InitSynoptiqueAG : IN \n");

   /* envoi du message XDM_TSE_ISAG */
   if (!TipcSrvMsgWrite(XDG_NSUPETAT,
                       XDM_IdentMsg(XDM_TSE_ISAG),
                       XDM_FLG_SRVMSGWRITE,
                       NULL))
      {  
         /* trace de l'erreur */
         vl_ValRet = XDC_NOK;
	 XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_20InitSynoptiqueAG:\
		TipcSrvMsgWrite(NSUPETAT,%u,XDM_FLG_SRVMSGWRITE)",
			      XDM_IdentMsg(XDM_TSE_ISAG));
      }  
      TipcSrvFlush();

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZSE_20InitSynoptiqueAG : OUT -- Code Retour %d\n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin 	XZSE_20InitSynoptiqueAG */ 



/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomSite :	Nom du site.
*
* Fonction
* Cette fonction envoie le message XDM_TSE_ISSS au 
* datagroup XDG_NSUPETAT avec l'element NomSite au 
* format %s.
* L'existence du site est verifie avec la primitive
*  XZSA_22.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOA_INV		Argument NomSite invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_21InitSynoptiqueSS(XZSCT_NomSite	va_NomSite)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_21InitSynoptiqueSS";
   int                  vl_ValRet = XDC_OK;
   XZSCT_NomSite	pl_NomSite;

   XZST_03EcritureTrace(XZSTC_INTERFACE, "XZSE_21InitSynoptiqueSS : IN \
	-- NomSite %s\n",va_NomSite);
   
   /* verification de l'existence du site */
   vl_ValRet = XZSA_22ExisterSite(va_NomSite);
   if (vl_ValRet == XDC_OK)
   {
	/* envoi du message XDM_TSE_ISSS */
        if (!TipcSrvMsgWrite(XDG_NSUPETAT,
                            XDM_IdentMsg(XDM_TSE_ISSS),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, va_NomSite,
                            NULL))
      	{
        	/* trace de l'erreur */
        	vl_ValRet = XDC_NOK;
		XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_21InitSynoptiqueSS:\
			TipcSrvMsgWrite(NSUPETAT,%u,XDM_FLG_SRVMSGWRITE,,%s)",
			         XDM_IdentMsg(XDM_TSE_ISSS),
				 va_NomSite);
      	}
      	TipcSrvFlush();
   }
   else
	vl_ValRet = XZSEC_NOA_INV;

	XZST_03EcritureTrace(XZSTC_INTERFACE, "XZSE_21InitSynoptiqueSS : OUT \
		-- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_21InitSynoptiqueSS */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine :		Nom de la machine.
*
* Fonction
* Cette fonction envoie le message XDM_TSE_ISTC au 
* datagroup XDG_NSUPETAT avec l'element NomMachine au 
* format %s.
* Le nom de la machine est verifie avec la primitive
* XZSA_05.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_NOM_INV		Nom de la machine invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_22InitSynoptiqueTache(XZSCT_NomMachine	va_NomMachine)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_22InitSynoptiqueTache";
   int                  vl_ValRet 	= XDC_OK;
   XZSCT_NomSite	pl_NomSite	= "";


   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_22InitSynoptiqueTache : IN \
		-- NomMachine %s \n", va_NomMachine);
  
   /* verification du nom de la machine */
   vl_ValRet = XZSA_05ExisterMachine(va_NomMachine);
   if (vl_ValRet == XDC_OK)
   {
	/* envoi du message XDM_TSE_ISTC */
        if (!TipcSrvMsgWrite(XDG_NSUPETAT,
                            XDM_IdentMsg(XDM_TSE_ISTC),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, va_NomMachine,
                            NULL))
      	{
        	/* trace de l'erreur */
        	vl_ValRet = XDC_NOK;
		XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_22InitSynoptique:\
			TipcSrvMsgWrite(NSUPETAT,%u,XDM_FLG_SRVMSGWRITE,,%s)",
			         XDM_IdentMsg(XDM_TSE_ISTC),
				 va_NomMachine);
      	}
      	TipcSrvFlush();
   }
   else
	vl_ValRet = XZSEC_NOM_INV;

	XZST_03EcritureTrace(XZSTC_INTERFACE, 
		"XZSE_22InitSynoptiqueTache : OUT -- Code Retour %d \n",
			vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_22InitSynoptiqueTache */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* EtatMachine :		XZSEC_ETAT_NACTIF ou
*			XZSEC_ETAT_NPASSIF.
*
* Fonction
* Cette fonction affecte le contenu de la variable
* XZSEV_EtatMachine avec l'argument EtatMachine.
*
* Codes retour
* XDC_OK		ok.
*
* Mecanismes
*******************************************************/

int  XZSE_25ActualiserEtatMachine(int	va_EtatMachine)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 :  XZSE_25ActualiserEtatMachine";
   int 	vl_ValRet = XDC_OK;
  

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_25ActualiserEtatMachine : IN \
		- EtatMachine %d \n", va_EtatMachine);

   /* verification de l'etat de la machine */
   if ((va_EtatMachine == XZSEC_ETAT_NACTIF) 
		|| (va_EtatMachine == XZSEC_ETAT_NPASSIF))
   {
	/* affectation de l'etat de la machine 
	*		a la variable XZSEV_EtatMachine */
   	XZSEV_EtatMachine = va_EtatMachine;
   }   
   else 
	vl_ValRet = XDC_NOK;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_25ActualiserEtatMachine : OUT  \
		-- Code Retour %d \n",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_25ActualiserEtatMachine */





/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine :	Nom de la machine de la tache.
* NomTache   :	Nom de la tache.
* NivTrace   :	Niveau de trace de la tache.
* VisuTrace  :	XZSTC_ECRAN ou XZSTC_NECRAN.
*
* Fonction
* Cette fonction appelle la primitive XZSC_04 pour 
* trouver le nom du site de la machine.
* Puis elle envoie le message XDM_TST_TRAC au datagroup 
* XDG_NSUPHIST_NomSite contenant les elements 
* "NomMachine" "NomTache" "NivTrace" "VisuTrace" au 
* format %s %s %d %d.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_ARG_INV		Argument VisuTrace invalide.
*
* Mecanismes
* RTworks : RTS2.
*******************************************************/

int XZSE_26EnvoieEtatTrace(
	XZSCT_NomMachine	va_NomMachine,
       	XZSCT_NomTache		va_NomTache,
	XZSTT_NiveauTrace	va_NivTache,
	int			va_VisuTrace)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_26EnvoieEtatTrace";
   int 			vl_ValRet 	= XDC_OK;
   XZSCT_NomSite 	pl_NomSite	= "";
   XZSCT_Datagroup 	pl_NomDatagroup	= "";



   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_26iEnvoieEtatTrace : IN \
	-- NomMach %s -- NomTache %s -- NivTache %d -- VisuTrace:%d \n",
			va_NomMachine ,va_NomTache, va_NivTache, va_VisuTrace);

   /* verification de VisuTrace */ 
   if ((va_VisuTrace == XZSTC_ECRAN) || (va_VisuTrace == XZSTC_NECRAN))
   {
           /* construction du nom du datagroup XDG_NSUPETAT_NomMachine */
           XDG_EncodeDG2(pl_NomDatagroup,XDG_NSUPETAT, va_NomMachine);

           /* envoi du message XDM_TST_TRAC */
           if (!TipcSrvMsgWrite(pl_NomDatagroup,
			       XDM_IdentMsg(XDM_TST_TRAC),
			       XDM_FLG_SRVMSGWRITE,
			       T_IPC_FT_STR, va_NomMachine,
			       T_IPC_FT_STR, va_NomTache,
			       T_IPC_FT_INT4, va_NivTache,
			       T_IPC_FT_INT4, va_VisuTrace,
			       NULL))
           {
	   	/* trace de l'erreur */
	   	vl_ValRet = XDC_NOK;
	        XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_26EnvoieEtatTrace:\
		 TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%s,,%u,,%u)",
				 pl_NomDatagroup,
			         XDM_IdentMsg(XDM_TST_TRAC),
			         va_NomMachine,
			         va_NomTache,
			         va_NivTache,
			         va_VisuTrace);
           }
      	TipcSrvFlush();
   }
   else 
	vl_ValRet = XDC_ARG_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_26iEnvoieEtatTrace : OUT -- Code Retour %d \n ",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_26iEnvoieEtatTrace */




/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction
* Appel primitive XZSC_07 pour recuperer le nom de la
* machine.
* Appel primitive XZSC_05 pour recuperer le type de 
* la machine.
* Si Type de la machine = XZSCC_TYPEM_SD
*	Affectation du contenu de la variable
*	XZSEV_NomSDActif avec le nom de la machine.
* Fsi
* Affectation du contenu de la variable XZSEV_NomSCActif
* avec le nom de la machine.
*
* Codes retour
* XDC_OK		ok.
*
* Mecanismes
********************************************************/

int XZSE_27ActualiserNomMachineActive(char *pa_NomMachine)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_27ActualiserNomMachineActive";
  
   int 			vl_ValRet 	= XDC_NOK;
   int		 	pl_TypeMachine	= 0;	

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_27ActualiserNomMachineActive : IN -- \
	Nom Machine Active %s \n", pa_NomMachine);

   /* recuperation du type de la machine locale */
   vl_ValRet = XZSC_05TypeMachine(&pl_TypeMachine);
   if (vl_ValRet == XDC_OK)
   {
      if (pl_TypeMachine == XZSCC_TYPEM_SD )
      {
         strcpy(XZSEV_NomSDActif,pa_NomMachine);
      }

      strcpy(XZSEV_NomSCActif,pa_NomMachine);
   }
   else
	vl_ValRet = XZSEC_NOM_INV;

   XZST_03EcritureTrace(XZSTC_INTERFACE,
	"XZSE_27ActualiserNomMachineActive : OUT -- Code Retour %d \n ",
		 vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_27ActualiserNomMachineActive */


/*X*/
/* Fichier : @(#)xzse.c	1.13      Release : 1.13        Date : 09/29/97
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachine :	Nom de la machine de la tache.
* NomTache   :	Nom de la tache.
* EtatTache  :	XZSEC_ETAT_NOK , XZSEC_ETAT_OK ou 
*		XZSEC_SANS_ETAT.
*
* Fonction
* Cette fonction envoie le message XDM_TSE_TTMP au 
* datagroup XDG_NSUPHIST contenant les elements 
* "NomMachine" "NomTache" "EtatTache" au 
* format %s %s %d.
* Aucune verification est effectuee sur les noms des 
* arguments car les taches temporaires sont considerees 
* comme des entites annexes telles que les interfaces, 
* les peripheriques ou les concentrateurs.
*
* Codes retour
* XDC_OK		Ok.
* XZSEC_ARG_INV		Argument EtatTache invalide.
*
* Mecanismes
* RTworks : RTS2.
********************************************************/

int XZSE_28EnvoieEtatTacheTmp(XZSCT_NomMachine	va_NomMachine,
			  XZSCT_NomTache	va_NomTache,
			  int			va_EtatTache)
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_28EnvoieEtatTacheTmp";
   int vl_ValRet = XDC_OK;
   XZSCT_NomEntite pl_NomEntite;


   XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_28EnvoieEtatTacheTmp : IN \
		-- NomMach %s -- NomTache %s -- EtatTache:%d \n",
			va_NomMachine, va_NomTache, va_EtatTache);

   /* verification de l'etat de la tache */
   if ((va_EtatTache == XZSEC_ETAT_NOK) 
	|| (va_EtatTache == XZSEC_ETAT_OK) 
	|| (va_EtatTache == XZSEC_SANS_ETAT))
   {
	/* construction du nom de l'entite */
	sprintf(pl_NomEntite, "%s %s", va_NomMachine, va_NomTache);

      	/* envoi du message XDM_TSE_TTMP */
      	if (!TipcSrvMsgWrite(XDG_NSUPHIST,
		          XDM_IdentMsg(XDM_TSE_TTMP),
		          XDM_FLG_SRVMSGWRITE,
		          T_IPC_FT_STR, pl_NomEntite,
		          T_IPC_FT_INT4, va_EtatTache,
		          NULL))
    	{
	   /* trace de l'erreur */
	   vl_ValRet = XDC_NOK;
	   XZST_03EcritureTrace(XZSTC_WARNING, "XZSE_28EnvoieEtatTache:\
		TipcSrvMsgWrite(NSUPHIST,%u,XDM_FLG_SRVMSGWRITE,,%s,,%s,,%u)",
			         XDM_IdentMsg(XDM_TSE_TTMP),
	                 va_NomMachine, va_NomTache, va_EtatTache); 
	 }
        TipcSrvFlush();
   }
   else vl_ValRet = XDC_NOK;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
			"XZSE_28EnvoieEtatTacheTmp : OUT  \
			-- Code Retour %d \n ",vl_ValRet);

   return(vl_ValRet);

}	/* Fin XZSE_28EnvoieEtatTacheTmp */


/*X*/
/******************************************************
* STERIA *  PROJET MIGRZUR
******************************************************
* MODULE XZSE
******************************************************
* ALGORITHME :
*
* Arguments
* pa_NomMachine : Nom de la machine dont on veut connaitre
* les informations demandees.
*
* Fonction
* Cette fonction permet de recuperer via les messages
* appropries l'etat d'une machine, de ses sous-systemes et
* de ses taches.
*
* Codes retour
* XDC_OK                Ok.
* XDC_NOK
*
* Mecanismes
********************************************************/

int XZSE_30DemanderEtatMachineAppli(XZSCT_NomMachine pa_NomMachine)
{
   	static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_30DemanderEtatMachineAppli";
	XZSCT_Datagroup pl_NomDatagroup;
   	XZSCT_NomMachine vl_NomMachine;
      	int vl_ValRet = XDC_OK;


	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_30DemanderEtatMachineAppli  : IN \
		--Nom Mach %s \n",pa_NomMachine);

	/* recuperation du nom de la machine locale */
	XZSC_07NomMachine(vl_NomMachine);

	/* construction du nom du datagroup d'envoi de la demande */
	XDG_EncodeDG2(pl_NomDatagroup, XDG_NSUPETAT, pa_NomMachine);

       XZST_03EcritureTrace(XZSTC_MESSAGE,
		    "Envoie msg XDM_TSE_DIFE sur DataGroup %s \
		    -- Arg NomMachineDem %s \n",pl_NomDatagroup,
		    vl_NomMachine);

	if (!TipcSrvMsgWrite(pl_NomDatagroup,
	 	XDM_IdentMsg(XDM_TSE_DIFE),
		XDM_FLG_SRVMSGWRITE,
		T_IPC_FT_STR, vl_NomMachine, NULL))
	{			
	       XZST_03EcritureTrace(XZSTC_WARNING,
		    "XZSE_30DemanderEtatMachineAppli: \
		    TipcSrvMsgWrite XDM_TSE_DIFE : NomMachine %s" ,
		    pa_NomMachine);

		vl_ValRet = XDC_NOK;
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSE_30DemanderEtatMachineAppli  : OUT \
		--Code Retour %d \n",vl_ValRet);

	return(vl_ValRet);

} 	/* Fin XZSE_30DemanderEtatMachineAppli */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZSE_Etat_Comm_Site ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ADM_ARC_SER 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_Etat_Comm_Site";
   XZSET_EComm_Site	vl_Mes;
   int			vl_Resultat = XDC_OK;
   
#ifndef _TIBCO_EMS
	T_STR	 vl_NomMac;
  T_INT4        vl_EtatMac;
#else
	XDY_NomMachine vl_NomMac;
  	XDY_Entier        vl_EtatMac;
#endif
	XDY_NomMachine vl_machine="";

          if (!TipcMsgRead(pa_Data->msg
                ,T_IPC_FT_STR,&vl_NomMac
                ,T_IPC_FT_INT4,&vl_EtatMac
                ,NULL))
        {
    XZST_03EcritureTrace( XZSTC_WARNING,
		"XZSE_Etat_Comm_Site : Pb lecture message XDM_ADM_ARC_SER" );
    return;
  }

  if ( vl_NomMac[strlen(vl_NomMac)-1] != '1' )
    {
    XZST_03EcritureTrace( XZSTC_INFO, " %s n'est pas un serveur actif, alors on n'en tient pas cpte!",vl_NomMac);
    return;
    }

  if ( XZSA_23SiteMachine(vl_NomMac, vl_Mes.Site) != XDC_OK )
    {
    XZST_03EcritureTrace( XZSTC_WARNING,
			"Site introuvable pour la machine :%s", vl_NomMac);
    return;
    }

    XZST_03EcritureTrace ( XZSTC_FONCTION, "IN  : XZSE_Etat_Comm_Site M = %s /// S = %s // etat = %d",
							vl_NomMac,vl_Mes.Site,vl_EtatMac);
    switch ( vl_EtatMac )
    {
    case XZSEC_ETAT_VEILLE :
    case XZSEC_SANS_ETAT   :
    		vl_Mes.Etat = (XDY_Booleen) XDC_NOK;
    		break;
    case XZSEC_ETAT_NACTIF :
    case XZSEC_ETAT_NPASSIF:
    case XZSEC_ETAT_DEGRADE:
    		vl_Mes.Etat = (XDY_Booleen) XDC_OK;
    		break;
    default :
   	        XZST_03EcritureTrace( XZSTC_WARNING, "Etat inconnu : %d",vl_EtatMac);
		vl_Mes.Etat = (XDY_Booleen) XDC_OK;
		break;
    }
   
   if (XZSC_GetDestinataire(pa_Data, vl_machine)!=XDC_OK) {
	XZST_03EcritureTrace(XZSTC_WARNING,"XZSE_Etat_Comm_Site : probleme dans recuperation machine destinataire du message");
	  }

   /*A Appel de la fonction utilsateur avec la structure XZSET_EComm_Site en argument */
   pg_FonctUtil_XZSE ( vl_Mes, vl_machine );
   XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZSE_Etat_Comm_Site " );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void	XZSE_Etat_Autre ( 
        T_IPC_CONN 			va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_ARG )
 
/*
* ARGUMENTS EN ENTREE :
*  Message RTworks
*
* ARGUMENTS EN SORTIE :
*   aucun
*
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*  Fonction appel�e lors de la r�ception du message
*  XDM_ADM_ARC_SER 
*
* FONCTION 
*
*
------------------------------------------------------*/
{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_Etat_Autre";
#ifndef _TIBCO_EMS
	T_STR	 	vl_NomMac;
	T_INT4		vl_EtatMac;
#else
	XDY_NomMachine 	vl_NomMac;
  	XDY_Entier		vl_EtatMac;
#endif
		  
   XZST_03EcritureTrace ( XZSTC_FONCTION,  "IN : XZSE_Etat_Autre " );

   /* on consomme le message mais on n'en fait rien */
	if (!TipcMsgRead(pa_Data->msg
					,T_IPC_FT_STR,&vl_NomMac
					,T_IPC_FT_INT4,&vl_EtatMac
					,NULL))
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "XZSE_Etat_Autre : Pb lecture message XDM_ADM_ARC_SER" );
		return;
	}

   XZST_03EcritureTrace ( XZSTC_FONCTION,  "OUT : XZSE_Etat_Autre " );

}


int XZSE_Abt_Etat_Comm_Site(XDY_Booleen va_abonnement, XDY_Fonction pa_fonction)

/*X*/
/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Fonction	pa_fonction		: Nom de la fonction utilisateur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etat de communication des Sites
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre le Datagroup correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre le Datagroup
********************************************************/

{
   static char *version="@(#)xzse.c	1.13 09/29/97 : XZSE_Abt_Etat_Comm_Site";
XDY_Datagroup	pl_NomDG;
int 		vl_ValRet = XDC_OK;
T_BOOL		vl_EtatDg = FALSE;

     XZST_03EcritureTrace ( XZSTC_FONCTION, 
     "IN  :  XZSE_Abt_Etat_Comm_Site Abt:%d Fonct:%ld .",
     		va_abonnement,
     		pa_fonction);
     		
     /* Memorisation en globale du nom de la fonction utilisateur */		
     pg_FonctUtil_XZSE = pa_fonction;
     
     /*A Si demande d'abonnement */
     if ( va_abonnement == XZEAC_DEBUT_ABONNEMENT )
     {
	/*A  Armement Callback sur Reception XDM_ADM_ARC_SER */
	if ( vg_CB_XZSE == NULL )
	{
	   if ( ( vg_CB_XZSE = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_ARC_SER),
							      	XZSE_Etat_Comm_Site,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_ARC_SER ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_ARC_SER ." );
	   }
	   
	   if ( ( vg_CB_XZSE_STA_E = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_ARC_STA_E),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_ARC_STA_E ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_ARC_STA_E ." );
	   }
	   if ( ( vg_CB_XZSE_STA_M = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_ARC_STA_M),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_ARC_STA_M ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_ARC_STA_M ." );
	   }
	   if ( ( vg_CB_XZSE_PER = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_ARC_PER),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_ARC_PER ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_ARC_PER ." );
	   }
	   if ( ( vg_CB_XZSE_INT = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_ARC_INT),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_ARC_INT ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_ARC_INT ." );
	   }
	}
	   if ( ( vg_CB_XZSE_SSS = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_SSS),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_SSS ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_SSS ." );
	   }
	   if ( ( vg_CB_XZSE_TRA = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_TRA),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_TRA ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_TRA ." );
	   }
	
	   if ( ( vg_CB_XZSE_TAC = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_TAC),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_TAC ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_TAC ." );
	   }
	   if ( ( vg_CB_XZSE_REINIT = TipcSrvProcessCbCreate (	XDM_IdentMsg( XDM_ADM_REINIT),
							      	XZSE_Etat_Autre,
								(T_CB_ARG)NULL ) ) == NULL ) 
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec armement Callback sur XDM_ADM_REINIT ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Armement Callback sur XDM_ADM_REINIT ." );
	   }
	

	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s", XDG_IHM_ADM );
   	/*A Joindre ce datagroup s'il n'est pas deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( !vl_EtatDg )
   	{
   	   if ( !XDG_JoindreDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec abonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Abonnement au Datagroup %s ", pl_NomDG );
	   }
	}
	/*A Demande l'etat des machines */
	XZSE_20InitSynoptiqueAG();
	
     }
     else
     {
        if ( ( va_abonnement == XZEAC_FERMER_ABONNEMENT ) && ( vg_CB_XZSE != NULL ) )
        {
           /*A  Desarmement Callback sur Reception XDG_IHM_ADM */
	   if ( TutCbDestroy ( vg_CB_XZSE ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_STA_E ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_STA_E = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_STA_M ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_STA_M = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_PER ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_PER = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_INT ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_INT = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_SSS ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_SSS = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_TRA ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_TRA = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_TAC ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_TAC = NULL;
	   }
	   if ( TutCbDestroy ( vg_CB_XZSE_REINIT ) == FALSE )  
	   {	
	      /*B Ecriture Trace */
	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desarmement Callback sur XDG_IHM_ADM ." );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desarmement Callback surXDG_IHM_ADM  ." );
	      vg_CB_XZSE_REINIT = NULL;
	   }
	}

	/*A Desabonnement au Datagroup XDG_IHM_ADM */
	/*A Construction du nom du DataGroup */
	sprintf ( pl_NomDG, "%s", XDG_IHM_ADM );
   	/*A Disjoindre ce datagroup s'il est deja joint */
   	TipcSrvSubjectGetSubscribe ( pl_NomDG, &vl_EtatDg );
   	if ( vl_EtatDg )
   	{
   	   if ( !XDG_QuitterDG( pl_NomDG ) )
   	   {
	      /*B Ecriture Trace */
      	      XZST_03EcritureTrace ( XZSTC_WARNING, "Echec desabonnement au Datagroup %s ", pl_NomDG );
	      vl_ValRet = XDC_NOK;
	   }
	   else
	   {
      	      XZST_03EcritureTrace ( XZSTC_DEBUG1, "Desabonnement au Datagroup %s ", pl_NomDG );
	   }
	}
	
     }
     
   XZST_03EcritureTrace ( XZSTC_FONCTION, "OUT :  XZSE_Abt_Etat_Comm_Site Retour:%d.", vl_ValRet );
   return ( vl_ValRet );				
				
}
