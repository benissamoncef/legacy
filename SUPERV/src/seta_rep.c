/*E*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP * FICHIER seta_rep.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Module de reponse aux demandes de l'interface               
* homme-machine.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 24 Oct 1994	: Creation
* T.Milleville 08 MAr 199( 	: Modification XZSI_xx en XZSI_xx V1.2
* T.Milleville 25 Avr 1995 	: Ajout de traces V1.3
* T.Milleville 25 Jul 1995 	: Ajout de commentaires V1.5
* T.Milleville 12 Oct 1995 	: Modification fonction seta_synopag_cb 
*	 Par default on appele la mise a jour du peripherique
*	Modification seta_diffetat pour tenir compte du nouveau
*	msg XDM_TSE_PERI V1.8
* T.Milleville 23  Oct 1995 	: Modification du test pour les demandes
*	de synoptiques. On demande egalement si la machine
*	est en mode DEGRASE V1.9
* T.Milleville 14 Dec 1995 	: Prise en compte de la surveillance inter-SC
*	dans la demande de diffusion des etats   V1.10
* P.Niepceron	23 Jan 1995	: Dans la demande de diff si la machine est la machine courante, 
				alors on diffuse l'etat de la machine quelque soit le type v1.11
***
* P.NIEPCERON	22 Aou 1996	: Ajout du type machine PC simplifie v1.12
* P.NIEPCERON	22 Aou 1996	: Ajout du type machine PC 2 +STA v1.13 1.14
* P.Niepceron   01 Jul 1998     : Ajout du type de machine Historique (dem/1697) v1.15
* JPL		11/01/11	: Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.16
*******************************************************/

/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

/* fichiers inclus */

#include <rtworks/ipc.h>

#include "xdc.h"
#include "xdm.h"
#include "xzsa.h"
#include "xzsc.h"
#include "xzse.h"
#include "xzst.h"
#include "xzsi.h"

#include "seta_don.h"
#include "seta_rep.h"
#include "seta_sur.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_rep" ;

/* declaration de fonctions internes */
int seta_synopsite(XZSCT_NomMachine pa_NomMachine,
                 int va_NumOrdre);

/* definition de fonctions externes */


/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'initialisation du
* synoptique d'architecture generale.
* Elle est appelee sur reception du message XDM_TSE_ISAG.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_synopag_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun-
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucun
*
* CONDITIONS LIMITES :
* 	Aucun
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_synopag_cb" ;
   int vl_EtatMachine	= 0; /* etat de la machine locale */
   int vl_NumMachine	= 0;
   int vl_NumEntite	= 0;
   int vl_TypeMachine	= 0;


   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopag_cb : IN ");


   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);

/* Modif V1.9 */ 
   if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF)
    	|| (vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
   {
      /* diffusion de l'etat des machines vers les synoptiques */
      for (vl_NumMachine = 0; vl_NumMachine < SEV_NB_MACH; vl_NumMachine++)
      {
         /* recuperation du type de la machine */
         XZSA_17TypeMachine(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
							&vl_TypeMachine);

         /* diffusion de l'etat de la machine vers les 
	 *			synoptique selon son type */
         switch (vl_TypeMachine)
         {
            case XZSAC_TYPEM_SD:
            case XZSAC_TYPEM_SC:
	    case XZSAC_TYPEM_PC2:
	    case XZSAC_TYPEM_PC2LT:
            case XZSAC_TYPEM_PCS:	/* ajout PCS modif 1.12 */
               
               XZSI_03DiffEtatServeur(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
                                      SEP_ETAT_MACH[vl_NumMachine].Etat);
               break;


            case XZSAC_TYPEM_ST:
            case XZSAC_TYPEM_STA:
	    case XZSAC_TYPEM_HIS:
            case XZSAC_TYPEM_LT:
            case XZSAC_TYPEM_SP:

               XZSI_11DiffEtatStation(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
                                      SEP_ETAT_MACH[vl_NumMachine].Etat);
               break;

         }

         XZSI_10DiffModeStation(SEP_ETAT_MACH[vl_NumMachine].NomMachine, 
                                      SEP_ETAT_MACH[vl_NumMachine].EtatModeIns);
      } 


      /* diffusion de l'etat des entites vers les synoptiques */
      for (vl_NumEntite = 0; vl_NumEntite < SEV_NB_ENTI; vl_NumEntite++)
      {
         /* diffusion de l'etat de l'entite vers les synoptique 
						selon son type */
         switch (SEP_ETAT_ENTI[vl_NumEntite].TypeEntite)
         {
            case XZSAC_TYPE_PERI:
               
               XZSI_04DiffEtatPeriph(SEP_ETAT_ENTI[vl_NumEntite].NomEntite, 
                                     SEP_ETAT_ENTI[vl_NumEntite].Etat);
               break;


            case XZSAC_TYPE_INTE:

               XZSI_05DiffEtatInterface(SEP_ETAT_ENTI[vl_NumEntite].NomEntite, 
                                        SEP_ETAT_ENTI[vl_NumEntite].Etat);
                
               break;

	    default :
		XZSI_04DiffEtatPeriph(SEP_ETAT_ENTI[vl_NumEntite].NomEntite,
					  SEP_ETAT_ENTI[vl_NumEntite].Etat);
		break;

         } 
      } 
   }

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopag_cb : IN ");

}	/* Fin seta_synopag_cb */


/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'initialisation d'un
* synoptique de sous-systemes d'un site.
* Elle est appelee sur reception du message XDM_TSE_ISSS
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_synopss_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_synopss_cb" ;
   int vl_ValRet 	= XDC_OK;
   int vl_EtatMachine	= 0;       /* etat de la machine locale */
#ifndef _TIBCO_EMS
   T_STR pl_NomSite	= ""; /* nom du site */
#else
	XDY_NomSite pl_NomSite = ""; /* nom du site */
#endif
   int vl_NbElt		= 0;             /* necessaire aux appels aux fonctions de XZSA */


   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopss_cb : IN ");


   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);

   
/* Modif V1.9 */ 
   if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF)
    	|| (vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
   {
      /* recuperation du nom du site */
      TipcMsgRead(pa_Data -> msg,
                  T_IPC_FT_STR, &pl_NomSite,
		  NULL);

      /* diffusion de l'etat des sites vers les synoptiques */
      vl_ValRet = XZSA_19ListeMachineSite(pl_NomSite, 
			&seta_synopsite, &vl_NbElt, NULL);
   }

   if (vl_ValRet != XDC_OK)
   {
      XZST_03EcritureTrace(XZSTC_WARNING,"seta_synopss_cb: erreur detectee");
   }

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopss_cb : OUT  ");

}	/* Fin seta_synopss_cb */



/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'initialisation d'un
* synoptique de taches d'une machine.
* Elle est appelee sur reception du message XDM_TSE_ISTC.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_synoptc_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucun
*
* CONDITIONS LIMITES :
* 	Aucun
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_synoptc_cb" ;
   int vl_EtatMachine	= 0; /* etat de la machine locale */
   int vl_NumTache	= 0;
   int vl_NumEntite	= 0;
#ifndef _TIBCO_EMS
   T_STR  pl_NomMachine	= "";     /* nom de la machine dont on veut rafraichir
                                          le synoptique des taches */
#else
	XDY_NomMachine pl_NomMachine = "";
#endif
	/* nom de la machine d'une tache temporaire */
   XZSCT_NomMachine pl_NomMachineTemp; 
   XZSCT_NomTache   pl_NomTacheTemp;   /* Nom d'un tache temporaire */


   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synoptc_cb : IN   ");
   
   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);

   
/* Modif V1.9 */ 
   if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF)
    	|| (vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
   {
      /* recuperation du nom de la machine */
      TipcMsgRead(pa_Data -> msg,
                  T_IPC_FT_STR, &pl_NomMachine,
		  NULL);

   	XZST_03EcritureTrace(XZSTC_MESSAGE , "Reception msg XDM_TSE_ISTC \
		-- Nom Machine %s \n",pl_NomMachine);


      /* diffusion de l'etat des taches vers les synoptiques */
      for (vl_NumTache = 0; vl_NumTache < SEV_NB_TACH; vl_NumTache++)
      {

	XZST_03EcritureTrace(XZSTC_DEBUG3,"Diffusion etat tache %s -- %d \n",
				SEP_ETAT_TACH[vl_NumTache].NomTache,
				SEP_ETAT_TACH[vl_NumTache].Etat);

         /* diffusion de l'etat de la tache vers les synoptiques */
         if (!strcmp(SEP_ETAT_TACH[vl_NumTache].NomMachine, pl_NomMachine))
         {
		XZST_03EcritureTrace(XZSTC_INFO ,"Diffusion etat tache sur Nom Machine %s -- %d -- %s \n",
				SEP_ETAT_TACH[vl_NumTache].NomTache,
				SEP_ETAT_TACH[vl_NumTache].Etat,SEP_ETAT_TACH[vl_NumTache].NomMachine);
               XZSI_02DiffEtatTache(SEP_ETAT_TACH[vl_NumTache].NomMachine, 
                                    SEP_ETAT_TACH[vl_NumTache].NomTache,
                                    SEP_ETAT_TACH[vl_NumTache].Etat);
         }
      } 

      /* diffusion de l'etat des taches temporaires vers les synoptiques */
      for (vl_NumEntite = 0; vl_NumEntite < SEV_NB_ENTI; vl_NumEntite++)
      {
         /* diffusion de l'etat de la tache temporaire vers les synoptiques */
         if (SEP_ETAT_ENTI[vl_NumEntite].Etat == XZSAC_TYPE_TTMP)
         {
               /* recuperation du nom de la machine et du nom de la tache
                  temporaire */
               sscanf(SEP_ETAT_ENTI[vl_NumEntite].NomEntite,
		      "%s %s",
		      pl_NomMachineTemp,
		      pl_NomTacheTemp);

               if (!strcmp(pl_NomMachine, pl_NomMachineTemp))
               {
                  XZSI_05DiffEtatInterface(
				SEP_ETAT_ENTI[vl_NumEntite].NomEntite, 
                                 SEP_ETAT_ENTI[vl_NumEntite].Etat);
               }


         	/* diffusion de l'etat de la tache vers les synoptiques */
               XZSI_02DiffEtatTache(SEP_ETAT_TACH[vl_NumEntite].NomMachine, 
                                    SEP_ETAT_TACH[vl_NumEntite].NomTache,
                                    SEP_ETAT_TACH[vl_NumEntite].Etat);
         }
      } 

   }

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synoptc_cb : OUT    ");

}	/* Fin seta_synoptc_cb */



/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction demande l'initialisation d'un
* synoptique de traces d'une machine.
* Elle est appelee sur reception du message XDM_TST_INIT.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_synopts_cb(
	T_IPC_CONN va_Conn,
        T_IPC_CONN_PROCESS_CB_DATA pa_Data,
        T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucun
*
* CONDITIONS LIMITES :
* 	Aucun
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_synopts_cb" ;
   int vl_EtatMachine	= 0; /* etat de la machine locale */
   int vl_NumTache	= 0;
#ifndef _TIBCO_EMS
   T_STR  pl_NomMachine	= "";
#else
   XDY_NomMachine  pl_NomMachine	= "";
#endif

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopts_cb : IN    ");

   /* recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);

   
/* Modif V1.9 */ 
   if ( (vl_EtatMachine == XZSEC_ETAT_NACTIF)
    	|| (vl_EtatMachine == XZSEC_ETAT_DEGRADE) )
   {
      /* recuperation du nom de la machine */
      TipcMsgRead(pa_Data -> msg,
                  T_IPC_FT_STR, &pl_NomMachine,
		  NULL);

      /* diffusion de l'etat des traces des taches vers les synoptiques */
      for (vl_NumTache = 0; vl_NumTache < SEV_NB_TACH; vl_NumTache++)
      {
         /* diffusion de l'etat des traces d'une tache vers les synoptiques */
         if (!strcmp(SEP_ETAT_TACH[vl_NumTache].NomMachine, pl_NomMachine))
         {
            XZSI_07DiffNivVisuTrace(SEP_ETAT_TACH[vl_NumTache].NivTrace,
				    SEP_ETAT_TACH[vl_NumTache].VisuTrace,
                                    SEP_ETAT_TACH[vl_NumTache].NomTache,
                                    SEP_ETAT_TACH[vl_NumTache].NomMachine);
                   
         }
      } 

   }

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopts_cb : OUT     ");

}	/* Fin seta_synopts_cb */


/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction permet d'appeler la fonction seta_diffetat
* sur callback via le message XDM_TSE_DIFE. 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_diffetat_cb(
	T_IPC_CONN va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* PARAMETRES EN ENTREE :
* Arguments specifiques a un callback IPC.
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_diffetat_cb" ;
#ifndef _TIBCO_EMS
   T_STR  pl_NomMachine	= "";
#else
   XDY_NomMachine  pl_NomMachine	= "";
#endif

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_diffetat_cb : IN      ");

   /* recuperation du nom de la machine destinataire des demandes d'historisation */
   TipcMsgRead(pa_Data -> msg,
               T_IPC_FT_STR, &pl_NomMachine,
                NULL);

   XZST_03EcritureTrace(XZSTC_MESSAGE , "Recuperation msg XDM_TSE_DIFFE \
	-- Nom Machine %s \n",pl_NomMachine);

	/*A Diffusion de l'etat */
   	seta_diffetat(pl_NomMachine);

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_diffetat_cb : IN      ");

}	/* Fin seta_diffetat_cb */


/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction envoi tous les etats des machines    
* supervisees par la machine locale exceptees pour les          
* machines de niveau superieur.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void seta_diffetat(
	XZSCT_NomMachine pa_NomMachine)

/*
* PARAMETRES EN ENTREE :
* pa_NomMachine : nom de la machine destinataire des etats. 
*
*
* PARAMETRES EN SORTIE :
* 	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucun
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_diffetat" ;
   int vl_EtatMachine	 = 0; /* etat de la machine locale */
   int vl_NumElt	= 0;
   T_INT4 vl_TypeMsg	= 0;
   XZSCT_Datagroup vl_NomDatagroup	= "";


   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_diffetat : IN  -- \
		Nom Machine %s \n",pa_NomMachine);

   /*A recuperation de l'etat de la machine locale */
   XZSE_09EtatMachine(&vl_EtatMachine);

   /*A construction du nom du datagroup d'envoi des etats */
   XDG_EncodeDG2 (vl_NomDatagroup, XDG_NSUPETAT, pa_NomMachine);

   /*A Traitement different si le calculateur est ACTIF ou PASSIF */
   if  ( (vl_EtatMachine == XZSEC_ETAT_NACTIF) 
   	 || (vl_EtatMachine == XZSEC_ETAT_DEGRADE) 
	 || (vl_EtatMachine == XZSEC_ETAT_NPASSIF) ) 
   {
	/*B Calculatuer ACTIF, donc on historise toutes les machines
	*	surveillees */
      /* demande d'historisation de l'etat et du mode des machines */
      for (vl_NumElt = 0; vl_NumElt < SEV_NB_MACH; vl_NumElt++)
      {
/* Modif V1.10	Prise en compte de la surveillance inter-SC */ 
	 if (
	    (strcmp(SEP_ETAT_MACH[vl_NumElt].NomMachine, SEP_NOM_SERVEUR1) &&
	     strcmp(SEP_ETAT_MACH[vl_NumElt].NomMachine, SEP_NOM_SERVEUR2) &&
	     (SEP_ETAT_MACH[vl_NumElt].TypeSurveillance != SEC_SURV_ACTIVE_SC) )   || 

	     (!strcmp(SEP_ETAT_MACH[vl_NumElt].NomMachine, SEP_NOM_MACHINE))            ) /* modif 1.11: si la machine est la machine courante, alors on diffuse l'etat de la machine */
         {

   	XZST_03EcritureTrace(XZSTC_DEBUG1 , "seta_diffetat :  Envoi du msg XDM_TSE_MACH pour \
		machine %s -- etat %d \n",SEP_ETAT_MACH[vl_NumElt].NomMachine,SEP_ETAT_MACH[vl_NumElt].Etat);
	
            /* demande d'historisation de l'etat de la machine */     
            TipcSrvMsgWrite(vl_NomDatagroup,
                         XDM_IdentMsg(XDM_TSE_MACH),
                         XDM_FLG_SRVMSGWRITE,
                         T_IPC_FT_STR, SEP_ETAT_MACH[vl_NumElt].NomMachine,
                         T_IPC_FT_INT4, SEP_ETAT_MACH[vl_NumElt].Etat,
                         NULL);

            /* demande d'historisation du mode de la machine */     
            TipcSrvMsgWrite(vl_NomDatagroup,
                         XDM_IdentMsg(XDM_TSE_MODE),
                         XDM_FLG_SRVMSGWRITE,
                         T_IPC_FT_STR, SEP_ETAT_MACH[vl_NumElt].NomMachine,
                         T_IPC_FT_INT4, SEP_ETAT_MACH[vl_NumElt].EtatModeIns,
                         T_IPC_FT_INT4, SEP_ETAT_MACH[vl_NumElt].EtatModeRec,
                         NULL);
         }
      } 


      /* demande d'historisation de l'etat des sous-systemes */
      for (vl_NumElt = 0; vl_NumElt < SEV_NB_SSYS; vl_NumElt++)
      {
	 if(strcmp(SEP_ETAT_SSYS[vl_NumElt].NomMachine, SEP_NOM_SERVEUR1) &&
	    strcmp(SEP_ETAT_SSYS[vl_NumElt].NomMachine, SEP_NOM_SERVEUR2))
         {
            /* demande d'historisation de l'etat du sous-systeme */     
            TipcSrvMsgWrite(vl_NomDatagroup,
                         XDM_IdentMsg(XDM_TSE_SSYS),
                         XDM_FLG_SRVMSGWRITE,
                         T_IPC_FT_STR, SEP_ETAT_SSYS[vl_NumElt].NomMachine,
                         T_IPC_FT_STR, SEP_ETAT_SSYS[vl_NumElt].NomSSysteme,
                         T_IPC_FT_INT4, SEP_ETAT_SSYS[vl_NumElt].Etat,
                         NULL);
         }
      } 
      
      /* demande d'historisation de l'etat des taches de l'etat de leurs traces */
      for (vl_NumElt = 0; vl_NumElt < SEV_NB_TACH; vl_NumElt++)
      {
	 if(strcmp(SEP_ETAT_TACH[vl_NumElt].NomMachine, SEP_NOM_SERVEUR1) &&
	    strcmp(SEP_ETAT_TACH[vl_NumElt].NomMachine, SEP_NOM_SERVEUR2))
         {

	 XZST_03EcritureTrace(XZSTC_DEBUG1,"Historisation tache \
		-- Nom Mach %s -- Nom Tach %s -- Etat %d \n",
		SEP_ETAT_TACH[vl_NumElt].NomMachine,
		SEP_ETAT_TACH[vl_NumElt].NomTache,
		SEP_ETAT_TACH[vl_NumElt].Etat);

            /* demande d'historisation de l'etat de la tache */     
            TipcSrvMsgWrite(vl_NomDatagroup,
                         XDM_IdentMsg(XDM_TSE_TACH),
                         XDM_FLG_SRVMSGWRITE,
                         T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomMachine,
                         T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomTache,
                         T_IPC_FT_INT4, SEP_ETAT_TACH[vl_NumElt].Etat,
                         NULL);

            /* demande d'historisation de l'etat des traces de la tache */     
            TipcSrvMsgWrite(vl_NomDatagroup,
                         XDM_IdentMsg(XDM_TST_TRAC),
                         XDM_FLG_SRVMSGWRITE,
                         T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomMachine,
                         T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomTache,
                         T_IPC_FT_INT4, SEP_ETAT_TACH[vl_NumElt].NivTrace,
                         T_IPC_FT_INT4, SEP_ETAT_TACH[vl_NumElt].VisuTrace,
                         NULL);
         }
      } 

      /* demande d'historisation de l'etat des entites annexes */
      for (vl_NumElt = 0; vl_NumElt < SEV_NB_ENTI; vl_NumElt++)
      {
         /* selection du type du message de demande d'historisation 
            selon le type de l'entite */
         switch(SEP_ETAT_ENTI[vl_NumElt].TypeEntite)
         {
            case XZSAC_TYPE_PERI:
               vl_TypeMsg = XDM_TSE_PERI;
               break;

            case XZSAC_TYPE_INTE:
               vl_TypeMsg = XDM_TSE_INTE;
               break;

            case XZSAC_TYPE_TTMP:
               vl_TypeMsg = XDM_TSE_TTMP;
               break;

            default :
		vl_TypeMsg = XDM_TSE_PERI;
		break;

         }

         /* envoi du message de demande d'historisation */     
	 if (vl_TypeMsg == XDM_TSE_TTMP)
	 {
            	TipcSrvMsgWrite(vl_NomDatagroup,
                            XDM_IdentMsg(vl_TypeMsg),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, SEP_ETAT_ENTI[vl_NumElt].NomEntite,
                            T_IPC_FT_STR, SEP_ETAT_ENTI[vl_NumElt].NomEntite,
                            T_IPC_FT_INT4, SEP_ETAT_ENTI[vl_NumElt].Etat,
                            NULL);

	 }
	 else if (vl_TypeMsg == XDM_TSE_INTE)
	 {
            TipcSrvMsgWrite(vl_NomDatagroup,
                            XDM_IdentMsg(vl_TypeMsg),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, SEP_ETAT_ENTI[vl_NumElt].NomEntite,
                            T_IPC_FT_INT4, SEP_ETAT_ENTI[vl_NumElt].Etat,
                            NULL);
         }
	 else if (vl_TypeMsg == XDM_TSE_PERI)
	 {
            TipcSrvMsgWrite(vl_NomDatagroup,
                            XDM_IdentMsg(vl_TypeMsg),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, SEP_ETAT_ENTI[vl_NumElt].NomEntite,
                            T_IPC_FT_INT4, SEP_ETAT_ENTI[vl_NumElt].Etat,
                            T_IPC_FT_INT4, SEP_ETAT_ENTI[vl_NumElt].TypeEntite,
                            NULL);
         }
      }


      }
#if 0
	else if (vl_EtatMachine == XZSEC_ETAT_NPASSIF)
	{

		XZST_03EcritureTrace(XZSTC_DEBUG1,"Historisation Mach \
			%s \n",SEP_NOM_MACHINE);

		/*B Calculatuer PASSIF , donc on historise que les donnees
		*	concernat cette machine */
		/* demande d'historisation de l'etat de la machine */

      		for (vl_NumElt = 0; vl_NumElt < SEV_NB_MACH; vl_NumElt++)
      		{
	 		if(!strcmp(SEP_ETAT_MACH[vl_NumElt].NomMachine,
							SEP_NOM_MACHINE))
			{
				TipcSrvMsgWrite(vl_NomDatagroup,
		   			XDM_IdentMsg(XDM_TSE_MACH),
					XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_STR, SEP_ETAT_MACH[vl_NumElt].NomMachine,
			T_IPC_FT_INT4, SEP_ETAT_MACH[vl_NumElt].Etat,
						 NULL);

				/* demande d'historisation du mode 
				*			de la machine */
				TipcSrvMsgWrite(vl_NomDatagroup,
					XDM_IdentMsg(XDM_TSE_MODE),
					XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_STR, SEP_ETAT_MACH[vl_NumElt].NomMachine,
			T_IPC_FT_INT4, SEP_ETAT_MACH[vl_NumElt].EtatModeIns,
			T_IPC_FT_INT4, SEP_ETAT_MACH[vl_NumElt].EtatModeRec,
				  NULL);

				break;
			}
			
		}

      		/* demande d'historisation de l'etat des sous-systemes */
      		for (vl_NumElt = 0; vl_NumElt < SEV_NB_SSYS; vl_NumElt++)
      		{
	 		if(!strcmp(SEP_ETAT_SSYS[vl_NumElt].NomMachine,
							SEP_NOM_MACHINE))
			{
            			/* demande d'historisation de l'etat 
				*		du sous-systeme */     
            			TipcSrvMsgWrite(vl_NomDatagroup,
                         		XDM_IdentMsg(XDM_TSE_SSYS),
                         		XDM_FLG_SRVMSGWRITE,
                         T_IPC_FT_STR, SEP_ETAT_SSYS[vl_NumElt].NomMachine,
                         T_IPC_FT_STR, SEP_ETAT_SSYS[vl_NumElt].NomSSysteme,
                         T_IPC_FT_INT4, SEP_ETAT_SSYS[vl_NumElt].Etat,
                         		NULL);
			}
		}

      		/* demande d'historisation de l'etat des sous-systemes */
      		for (vl_NumElt = 0; vl_NumElt < SEV_NB_TACH; vl_NumElt++)
      		{
	 		if(!strcmp(SEP_ETAT_TACH[vl_NumElt].NomMachine,
							SEP_NOM_MACHINE))
			{
		 		/* demande d'historisation de l'etat 
				*			de la tache */
		 		TipcSrvMsgWrite(vl_NomDatagroup,
		    			XDM_IdentMsg(XDM_TSE_TACH),
					XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomMachine,
			T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomTache,
			T_IPC_FT_INT4, SEP_ETAT_TACH[vl_NumElt].Etat,
					   NULL);

				/* demande d'historisation de l'etat 
					des traces de la tache */
	   			TipcSrvMsgWrite(vl_NomDatagroup,
					XDM_IdentMsg(XDM_TST_TRAC),
					XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomMachine,
			T_IPC_FT_STR, SEP_ETAT_TACH[vl_NumElt].NomTache,
			T_IPC_FT_INT4, SEP_ETAT_TACH[vl_NumElt].NivTrace,
			T_IPC_FT_INT4, SEP_ETAT_TACH[vl_NumElt].VisuTrace,
					   NULL);

			 }
		}
	}
#endif



      if (TipcSrvFlush() != TRUE)
      {
         XZST_03EcritureTrace(XZSTC_WARNING,
             "seta_diffetat: erreur d'envoi des messages d'historisation");
      }


   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_diffetat : OUT \n");

}	/* Fin seta_diffetat */



/* definition de fonctions internes */

/*X*/
/* Fichier : $Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $	Release : $Revision: 1.17 $        Date : $Date: 2020/11/03 17:53:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SETA_REP
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction diffuse vers un synoptique de
* sous-systemes l'etat des sous-systeme d'un machine.
* Elle est appelee par une fonction du type XZSA.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int seta_synopsite(
	XZSCT_NomMachine pa_NomMachine,
        int va_NumOrdre)

/*
* PARAMETRES EN ENTREE :
* pa_nomMachine : nom de la machine
* va_NumOrdre   : numero d'ordre de la machine (non
*                 utilise ici)
*
*
* PARAMETRES EN SORTIE :
*	Aucun
*
* VALEUR(S) DE RETOUR :
* 	Aucune
*
* CONDITION D'ERREUR :
* 	Aucune
*
* CONDITIONS LIMITES :
* 	Aucune
*
******************************************************/
{
   static char *version = "$Id: seta_rep.c,v 1.17 2020/11/03 17:53:56 pc2dpdy Exp $ : seta_synopsite" ;
   int vl_NumSSysteme	= 0;
   int vl_ValRet 	= XDC_OK;

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopsite : IN \
   	-- Nom Mach %s -- Num Ordre %d \n",pa_NomMachine,va_NumOrdre);

   for (vl_NumSSysteme = 0; vl_NumSSysteme < SEV_NB_SSYS; vl_NumSSysteme++)
   {
      /* diffusion de l'etat du sous-systeme vers le synoptique */
      if (!strcmp(SEP_ETAT_SSYS[vl_NumSSysteme].NomMachine, pa_NomMachine))
      {
         XZSI_01DiffEtatSSysteme(SEP_ETAT_SSYS[vl_NumSSysteme].NomMachine,
                                  SEP_ETAT_SSYS[vl_NumSSysteme].NomSSysteme,
                                  SEP_ETAT_SSYS[vl_NumSSysteme].Etat);
      }
   }

   XZST_03EcritureTrace(XZSTC_FONCTION , "seta_synopsite : IN \
   	-- Nom Mach %s -- Num Ordre %d \n",pa_NomMachine,va_NumOrdre);

   return (vl_ValRet);
}	/* Fin seta_synopsite */
