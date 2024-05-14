/*E*/
/* Fichier : $Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $	Release : $Revision: 1.3 $        Date : $Date: 1995/07/26 11:50:41 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_CON * FICHIER seta_con.c 
******************************************************
* DESCRIPTION DU MODULE :
*
* Module qui permet de modifier et diffuser le mode d'une machine
*
* seta_stmodei_cb	: Callback qui fait passer la machine
*	en mode instruction ou normale
* seta_stmoder_cb	: Callback qui fait passer la machine
*	en mode reconstruction ou normale.
* seta_stmode		: Diifuse le mode de la machine locale
*	a une tache de cette machine
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  24 Oct 1994	: Creation
* T.Milleville  25 Jul 1995	: Ajout de commentaires V1.2
* T.Milleville  26 Jul 1995	: Mise a jour Entete V1.3
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

/* fichiers inclus */

#include <rtworks/ipc.h>

#include "xzsa.h"
#include "xzse.h"

#include "seta_don.h"
#include "seta_con.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
int vm_ModeIns = XZSEC_MODE_NOR;
int vm_ModeRec = XZSEC_MODE_NOR;

static char *version = "$Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $ : SETA_CON" ;

/* declaration de fonctions internes */
int seta_stmode(
	XZSCT_NomTache pa_NomTache,
      	int va_NumOrdre);


/* definition de fonctions externes */



/*X*/
/* Fichier : $Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $	Release : $Revision: 1.3 $        Date : $Date: 1995/07/26 11:50:41 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_CON
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction fait passer une machine en mode
* instruction ou normale.
* Elle est appelee sur reception du message XDM_TSE_MODI.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_stmodei_cb(
	T_IPC_CONN ca_Cnx,
      	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG Arg)

/*
* PARAMETRES EN ENTREE :
*	Argument d'une callback
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* Aucune
*
* CONDITIONS LIMITES :
*
* Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $ : seta_stmodei_cb" ;
   int vl_ModeIns	= 0;
   int vl_NbElt		= 0; /* variable necessaire aux appels des focntions    
		    de XZSA */

	XZST_03EcritureTrace(XZSTC_FONCTION ,
	  	"seta_stmodei_cb: IN \n");

	/*A Recuperation du Mode Instruction */
   TipcMsgRead(pa_Data -> msg, T_IPC_FT_INT4, &vl_ModeIns ,NULL);

   /* selection du nouveau mode */
   if (vl_ModeIns == XZSEC_MODE_TOGGLE)
   {
      /* inversion du mode d'instruction */
      if (vm_ModeIns == XZSEC_MODE_NOR)
      {
	 vm_ModeIns = XZSEC_MODE_INS;
      }
      else
      { 
	 vm_ModeIns = XZSEC_MODE_NOR;
      }
   }
   else
   {
      /* passage au mode demande */
      vm_ModeIns = vl_ModeIns;
   }

   /*A diffusion du mode de la machine a toutes les taches de la machine */
   if (XZSA_03ListeTacheMachine(SEP_NOM_MACHINE, 
		(void (*)())seta_stmode, &vl_NbElt, NULL) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, 
	   "seta_stmodei_cb: erreur de diffusion mode machine aux taches");
   }      

   /*A diffusion des modes de la machine au noyau de supervision 
   *	du serveur du site */
   if (XZSE_17EnvoieModeStation(SEP_NOM_MACHINE, vm_ModeIns, vm_ModeRec) 
								!= XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, 
	   "seta_stmodei_cb: erreur de diffusion mode machine au serveur");
   }      


	XZST_03EcritureTrace(XZSTC_FONCTION ,
	  	"seta_stmodei_cb: OUT  \n");

}	/* Fin seta_stmodei_cb */



/*X*/
/* Fichier : $Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $	Release : $Revision: 1.3 $        Date : $Date: 1995/07/26 11:50:41 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_CON
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction fait passer une machine en mode
* reconstruction ou normale.
* Elle est appelee sur reception du message XDM_TSE_MODR.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_stmoder_cb(
	T_IPC_CONN ca_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG Arg)

/*
* PARAMETRES EN ENTREE :
*	Argument d'une callback IPC
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR : 
*	Aucune
*
* CONDITION D'ERREUR :
*
* 	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $ : seta_stmoder_cb" ;
   int vl_ModeRec;
   int vl_NbElt; /* variable necessaire aux appels des focntions    
		    de XZSA */

	XZST_03EcritureTrace(XZSTC_FONCTION ,
	  	"seta_stmoder_cb: IN \n");

	/*A Recuperation du Mode Reconstruction */
   	TipcMsgRead(pa_Data -> msg,
	       T_IPC_FT_INT4, &vl_ModeRec ,NULL);

   /* selection du nouveau mode */
   if (vl_ModeRec == XZSEC_MODE_TOGGLE)
   {
      /* inversion du mode d'instruction */
      if (vm_ModeRec == XZSEC_MODE_NOR)
      {
	 vm_ModeRec = XZSEC_MODE_REC;
      }
      else
      { 
	 vm_ModeRec = XZSEC_MODE_NOR;
      }
   }
   else
   {
      /* passage au mode demande */
      vm_ModeRec = vl_ModeRec;
   }

   /* diffusion du mode de la machine a toutes les taches de la machine */
   if (XZSA_03ListeTacheMachine(SEP_NOM_MACHINE, (void(*)())seta_stmode, 
						&vl_NbElt, NULL) != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, 
	   "seta_stmoder_cb: erreur de diffusion mode machine aux taches");
   }      

   /* diffusion des modes de la machine au noyau 
   *		de supervision du serveur du site */
   if (XZSE_17EnvoieModeStation(SEP_NOM_MACHINE, vm_ModeIns, vm_ModeRec) 
								!= XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING, 
	   "seta_stmoder_cb: erreur de diffusion mode machine au serveur");
   }      

	XZST_03EcritureTrace(XZSTC_FONCTION ,
	  	"seta_stmoder_cb: OUT  \n");

}	/* Fin seta_stmoder_cb */



/*X*/
/* Fichier : $Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $	Release : $Revision: 1.3 $        Date : $Date: 1995/07/26 11:50:41 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_CON
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction diffuse le mode de la machine locale 
* a une tache de cette machine.
* Elle est appelee via un fonction du type XZSA.
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_stmode(
	XZSCT_NomTache pa_NomTache,
        int va_NumOrdre)

/*
* PARAMETRES EN ENTREE :
* pa_NomTache : nom de la tache 
* va_NumOrdre : numero d'ordre de la tache (non utilise
*               ici)
*
*
* PARAMETRES EN SORTIE :
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_OK 	: ok
*
* CONDITION D'ERREUR :
*
*  	Aucune
*
* CONDITIONS LIMITES :
*
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_con.c,v 1.3 1995/07/26 11:50:41 milleville Exp $ : seta_stmode" ;
   int vl_ValRet 	= XDC_OK;
   int vl_EtatMachine	= 0; /* etat de la machine locale */
   XZSCT_Datagroup pl_NomDatagroup	= "";

	XZST_03EcritureTrace(XZSTC_FONCTION ,
	  	"seta_stmode: IN -- Nom Tache  %s -- Num Ordre %d \n",
		pa_NomTache, va_NumOrdre );

   /* Recuperation de l'etat de la machine */
   XZSE_09EtatMachine(&vl_EtatMachine);

   if (vl_EtatMachine == XZSEC_ETAT_NACTIF)
   {
      /* envoi du message XDM_TSE_MAJM au datagroup XDG_NomMachine_NomTache */
      XDG_EncodeDG2(pl_NomDatagroup, SEP_NOM_MACHINE, pa_NomTache);

      TipcSrvMsgWrite(pl_NomDatagroup,
		      XDM_IdentMsg(XDM_TSE_MAJM),
		      XDM_FLG_SRVMSGWRITE,
		      T_IPC_FT_INT4, vm_ModeIns,
		      T_IPC_FT_INT4, vm_ModeRec,
		      NULL);

      if (TipcSrvFlush() != TRUE)
      {
	 vl_ValRet = XDC_NOK;
	 XZST_03EcritureTrace(XZSTC_WARNING,
			      "seta_stmode: erreur envoi message XDM_TSE_MAJM");
      }
   }

	XZST_03EcritureTrace(XZSTC_FONCTION ,
	  	"seta_stmode: OUT -- Code Retour %d \n",vl_ValRet);

   return (vl_ValRet);

}	/*  Fin seta_stmode */


