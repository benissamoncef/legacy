/*E*/
/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM * FICHIER xzsm.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSM
*        - Declaration des fonctions externes
*        - Declaration des variables externes
*
*   Reference DCG-021 chapitre 3.4  
*
******************************************************
* HISTORIQUE :
*
* T. Milleville		24 Aou 1994 : Creation
* T. Milleville		29 Sep 1994 : V 1.2 
*	Prise en compte du traitement de suppression du Timer memorise 
*	Reception msg XDM_TST_TSUP et traitement du callback associe
*
* T. Milleville		13 Jan 1995 : Correction de la fonction XZSM_07 v1.12 
* T. Milleville		10 Mar 1995 : Modification du Nom du Datagroup
*	pour recupere l'heure. Abonnement a XDG_TIME_NomSite.
*	Ajout d'un argument precisant le nom du site dans la fonction
*	XZSM_06... V1.14
* T. Milleville		5 Mai 1995 : Ajout d traces  V1.15
* OM+BG			22 Mai 1995 : modif gestion liste chainee dans XZSM_SupprimeReveil (1.16)
* BG                	 23 Mai 1995 : pas d'ajout si cle+DG+MSG identiques (1.17)
* BG                     22 Jun 1995 : modif timer Insere reveil (1.18)
* T. Milleville		12 Jul  1995 : Mdification derniere argument XDM_TST_PROG V1.21
* T. Milleville		25 Jul  1995 : Ajout de commentaires V1.22
* T. Milleville		14 Sep  1995 : Ajout de la suppresion du Timer lors 
*	d'une demande de reprogrammation V1.23
* JMG	26/06/10	 linux dem/934
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.30
* PNI	20/11/20	: Remise ÃƒÂ  zero du timeZone pour la conversion dans xzsm_13. DEM-SAE 191 1.33
* ABE	25/11/20	: Remise ÃƒÂ  zero du timeZone pour la conversion dans xzsm_11. DEM-SAE 191 1.34
* AMI  08/12/20  : correctif heure SAE_191  1.35
* AMI 28/01/21 : SAE_191: chantier date: retour arriere
* LCL 30/07/21 : SAE_303: correction Memory Leak 1.37
* RGR 24/08/21 : SAE_303 : correction gmtime
* LCL		20/12/21	: MOVIS P2 ems SAE-312
******************************************************/

#include <time.h>

#include <xdc.h>
#include <xdg.h>
#include <xdm.h>

#include "xzsm.h"
#include "xzems.h"

/* declaration des constantes locales */
XZSMT_CleProg	   	C_CLE_NULLE = "";
XZSCT_Datagroup	   	C_DATAGROUP_NUL = "";

/* declaration des variables locales  */
int     XZSMV_Mode                      = XZSMC_SYSTE;
int	XZSMV_InitOk			= 0 ;
double	XZSMV_DeltaHorodate 		= 0.0;
struct tm   * now;

	/* Premier element de la liste des tests a effectuer */
XZSMT_ENRG_TIMER *pg_ListeTempo=NULL;
static char *version="@(#)xzsm.c	1.23 09/21/95 xzsm.c";
/*X*/
/* Fichier : @(#)xzsm.c	1.23        Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette Fonction insere un reveil suplementaire dans
* la liste chainee du process utilisateur
*
******************************************************
* SEQUENCE D'APPEL :
*/

int XZSM_InsereReveil(XZSMT_ENRG_TIMER *pa_Enr)

/*
* PARAMETRES EN ENTREE :
* pa_Enr : reveil a inserer dans la liste
*
* PARAMETRES EN SORTIE :
*	pa_Enr :   Pointeur du Timer 
*
*
* VALEUR(S) DE RETOUR : 
*	XDC_OK  : ok pour declencher le Timer 
*	XDC_NOK : Timer deja existant
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
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_InsereReveil";
	XZSMT_ENRG_TIMER *      pl_Timer 	= NULL;
      	XZSMT_ENRG_TIMER *      pl_Enr 		= NULL;
	XZSMT_ENRG_TIMER *      pl_ExEnr 	= NULL;
	int	vl_ValRet	= XDC_OK;


	 XZST_03EcritureTrace(XZSTC_FONCTION,
		" XZSM_InsereReveil : IN -- Dg %s -- Idmsg %d --Cle %s -- Priorite %d \n",
		pa_Enr->Dg, pa_Enr->IdMsg, pa_Enr->Cle, pa_Enr->Priorite);

	/*A Test pour voir si ce timer est deja programme */
	/* BG 1.17 cf sm_Reveiller_Cb dans stim_tra.c */
	
	for (pl_Enr = pg_ListeTempo ; pl_Enr != NULL ;
		pl_Enr = pl_Enr->Suivant)
	{
		/*B Teste si identifieur, Datagroup et Cle existent */
		if ( (pl_Enr->IdMsg == pa_Enr->IdMsg) &&
			(!strcmp(pl_Enr->Dg, pa_Enr->Dg)) &&
			(!strcmp(pl_Enr->Cle,pa_Enr->Cle)) )
		{
			vl_ValRet = XDC_NOK;
			break;
		}
	}
	/*A si le timer n'est pas deja programme, on essaie de l'inserer */
	if (vl_ValRet == XDC_OK)
	/*A si l'allocaltion memoire est ok */
	if ((pl_Timer 
		= (XZSMT_ENRG_TIMER *)malloc(sizeof(XZSMT_ENRG_TIMER))) != NULL)
	{

	 	XZST_03EcritureTrace(XZSTC_INFO,
			"Enregistrement du TIMER adresse=%ld",(long)pl_Timer);
		
   		/*B copie enegistrement */
	  	memcpy(pl_Timer,pa_Enr,sizeof(XZSMT_ENRG_TIMER)) ;

		/*B pour les enregistrements deja existant */
	      	for (pl_Enr = pl_ExEnr = pg_ListeTempo ; pl_Enr != NULL ; 
				pl_ExEnr = pl_Enr,pl_Enr = pl_Enr->Suivant)
		{
		      	/* si l'horodate de declenchement est inferieure */
			if (pl_Timer->Horodate < pl_Enr->Horodate)

	 		XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Declenchement Horodate inferieure \n");

		     	/* la place d'insertion est localisee : 
			*			on stoppe la recherche */
			break ;
		}

		/* si la place d'insertion est la tete de liste */
	      	if (pl_Enr == pg_ListeTempo)
	    	{
	 		XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Debut de listes");

			/* cas particulier de la tete de liste */
		      	pl_Timer->Suivant = pg_ListeTempo ;
			pg_ListeTempo     = pl_Timer ;
	      	}
		/* sinon insertion nominal */
		else
		{	
	 	XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Insertion normale");

		 	pl_Timer->Suivant = pl_ExEnr->Suivant ;
		  	pl_ExEnr->Suivant = pl_Timer ;
		}
	}

	
	 XZST_03EcritureTrace(XZSTC_FONCTION,
		" XZSM_InsereReveil : OUT -- Code Retour %d",XDC_OK);

	return (XDC_OK);

}	/* Fin XZSM_InsereReveil */



/*X*/
/* Fichier : @(#)xzsm.c	1.23        Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction supprime une demande de reveil dans
* la liste des programmations
*
******************************************************
* SEQUENCE D'APPEL :
*/

void XZSM_SupprimeReveil(
	char *pa_Cle,
    	char *pa_Dg,
	int   va_IdMsg)

/*
* PARAMETRES EN ENTREE :
* pa_Cle        : Cle (NULL si pas de cle)
* pa_Dg         : Canal de diffusion
* va_IdMsg      : Numero de message
*
******************************************************/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_SupprimeReveil";
	XZSMT_ENRG_TIMER *        pl_Tempo	= NULL ;
   	XZSMT_ENRG_TIMER *        pl_ExTempo 	= NULL;
   	XZSMT_ENRG_TIMER *        pl_TempoALiberer 	= NULL;

	 XZST_03EcritureTrace(XZSTC_FONCTION,
		" Supprimer le Timer memorise : IN \
		Cle : %s \t DataGroup : %s \t IdMsg : %d \n ",
		pa_Cle, pa_Dg, va_IdMsg );


      	/* pour chaque enregistrement de la liste */
	for ( pl_ExTempo = pl_Tempo = pg_ListeTempo ; pl_Tempo != NULL ;
		      pl_ExTempo = pl_Tempo,pl_Tempo = pl_Tempo->Suivant,free(pl_TempoALiberer))
	{
		/*A si la Cle est precisee **/
		if ( (strcmp(pa_Cle, C_CLE_NULLE))  &&
			(!strcmp(pl_Tempo->Cle,pa_Cle)) )
		{
			/* supprime cette enregistrement */
			if (pl_Tempo == pg_ListeTempo)
			{
				pg_ListeTempo = pl_Tempo->Suivant ;
			}
			else
			{
				pl_ExTempo->Suivant = pl_Tempo->Suivant ;
			}
			free(pl_Tempo) ;
			pl_Tempo=NULL ;
			XZST_03EcritureTrace(XZSTC_INFO,"suppression timer %ld",(long)pl_Tempo);

			/* arrete, et cela afin de ne pas detruire 
			*	toutes les demandes de tempos 
			* de meme caracteristiques */
			break ;
		}
		/*A si la cle n'est pas precisee **/
		else if (!strcmp(pa_Cle, C_CLE_NULLE))
	  	  {
		  	/*B On supprime tous les enregistrements
		  	*    definis pour le Datagroup ou le Message precise */
			if (  (!strcmp(pl_Tempo->Dg,pa_Dg)) ||
				  (pl_Tempo->IdMsg == va_IdMsg) ) 
			{
				/*B Suppression de l'enregistrement **/
				if (pl_Tempo ==  pg_ListeTempo) 
				{
					pg_ListeTempo = pl_Tempo->Suivant ;
					XZST_03EcritureTrace(XZSTC_INFO,"suppression timer %ld",(long)pl_Tempo);
					/*pl_ExTempo = pl_Tempo = pg_ListeTempo;*/
					pl_TempoALiberer = pl_Tempo;
				}
				else
				{
					pl_ExTempo->Suivant = pl_Tempo->Suivant;
					XZST_03EcritureTrace(XZSTC_INFO,"suppression timer %ld",(long)pl_Tempo);
					/*pl_Tempo=pl_ExTempo;*/
					pl_TempoALiberer = pl_Tempo;
				}
			}
		}
	}

	/*B Liberation memoire */
	/*if (pl_TempoALiberer != NULL)
	{
		free(pl_TempoALiberer);
	}*/
	if (pl_Tempo != NULL)
	{
		free(pl_Tempo);
	}

	 XZST_03EcritureTrace(XZSTC_FONCTION,
		" Supprimer le Timer memorise : OUT ");

}	/* Fin XZSM_SupprimeReveil */


/*X*/
/* Fichier : @(#)xzsm.c	1.23        Release : 1.23        Date : 09/21/95
------------------------------------------------------
* SERVICE RENDU : 
*  Callback declenche sur la reception du message XDM_TST_TSUP
*	et permettant ensuite d'effectuer la suppression du 
*	Timer memorise
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void XZSM_SupTimer_Cb(
	T_IPC_CONN va_Cnx,
      	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une Callback
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
*   Appel de la fonction de suppression du Timer 
*
------------------------------------------------------*/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_SupTimer_Cb";
#ifndef _TIBCO_EMS
	char * 	vl_Cle		= NULL;
	char * 	vl_Dg		= NULL;
#else
	XDY_Nom vl_Cle          = "";
	XDY_Datagroup vl_Dg           = "";
#endif
	int	vl_IdMsg	= 0;

	 XZST_03EcritureTrace(XZSTC_FONCTION,
		" Callback de reception XDM_TST_TSUP : IN \n");

	/*A Lecture du msg XDM_TST_TSUP **/
	if (!TipcMsgRead(pa_Data->msg
		,T_IPC_FT_STR,&vl_Cle
		,T_IPC_FT_STR,&vl_Dg
		,T_IPC_FT_INT4,&vl_IdMsg
		,NULL))
	{
	     	XZST_03EcritureTrace(XZSTC_WARNING,
		      	"Erreur Lecture Msg XDM_TST_NSUP");
	}
	else
	{
		/*A Appel de la fonction de suppression du Timer **/
		XZSM_SupprimeReveil(vl_Cle, vl_Dg, vl_IdMsg);
	}

	 XZST_03EcritureTrace(XZSTC_FONCTION,
		" Callback de reception XDM_TST_TSUP : OUT \n");

}	/* Fin XZSM_SupTimer_Cb */



/*X*/
/* Fichier : @(#)xzsm.c	1.23        Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM
******************************************************
* DESCRIPTION DE LA FONCTION :
* cette fonction renvoi toutes les demandes de reveil
* a la tache STIMER
*
******************************************************
* SEQUENCE D'APPEL :
*/

void XZSM_EnvoiServices_cb(
	T_IPC_CONN va_Cnx,
      	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)
/*
* PARAMETRES EN ENTREE :
* arguments specifiques a un callback RTWorks
*
******************************************************/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_EnvoiServices_cb";
	int 			vl_cr	= 0;
   	XZSMT_ENRG_TIMER *      pl_ExTempo	= NULL;
      	XZSMT_ENRG_TIMER *      pl_Tempo	= NULL;
	XZSCT_NomMachine vl_NomMachine		= "";
	XZSCT_Datagroup vl_NomDatagroup		= "";


	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSM_EnvoiServices_cb : IN \n");

	/* recuperation du nom machine */
   	XZSC_07NomMachine(vl_NomMachine);

      	/* construction du nom du datagroup timer */
	/*XDG_EncodeDG2 (vl_NomDatagroup, XDG_NTIMEINIT, vl_NomMachine);*/
	XDG_EncodeDG2 (vl_NomDatagroup, XDG_NSUPTIME, vl_NomMachine);

	   /* demande de programmation pour chaque element de la liste */
	   for ( pl_ExTempo = pl_Tempo = pg_ListeTempo ; pl_Tempo != NULL;
		   pl_ExTempo = pl_Tempo,pl_Tempo = pl_Tempo->Suivant)
	   {
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite XDM_TST_PROG Horodate = [%lf]",pl_Tempo->Horodate);
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite XDM_TST_PROG Delai    = [%d]",pl_Tempo->Delai);
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite XDM_TST_PROG Dg       = [%s]",pl_Tempo->Dg);
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite XDM_TST_PROG IdMsg    = [%d]",pl_Tempo->IdMsg);
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite XDM_TST_PROG Cle      = [%s]",pl_Tempo->Cle);
			XZST_03EcritureTrace(XZSTC_DEBUG1,"TipcSrvMsgWrite XDM_TST_PROG Priorite = [%d]",pl_Tempo->Priorite);
		/* envoi du message TST_PROG .... */
		if (!TipcSrvMsgWrite(vl_NomDatagroup
		     	,XDM_IdentMsg(XDM_TST_PROG)
			,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_REAL8,pl_Tempo->Horodate
			,T_IPC_FT_INT4,pl_Tempo->Delai
			,T_IPC_FT_STR,pl_Tempo->Dg
			,T_IPC_FT_INT4,pl_Tempo->IdMsg
			,T_IPC_FT_STR,pl_Tempo->Cle
			,T_IPC_FT_INT2,(T_INT2)pl_Tempo->Priorite
			,NULL)) 
		{ 
	     		XZST_03EcritureTrace(XZSTC_WARNING,
		      	"Erreur envoi message XDM_TST_PROG ");
		} 
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSM_EnvoiServices_cb : OUT \n");

}	/*  Fin XZSM_EnvoiServices_cb */


/*X*/
/* Fichier : @(#)xzsm.c	1.23        Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction permet au process utilisateur de
* gerer les redemandes de programmation de reveil
*
******************************************************
* SEQUENCE D'APPEL :
*/
void XZSM_00InitServices()

/*
* PARAMETRES EN ENTREE : neant
*
*
* PARAMETRES EN SORTIE : neant
*
*
* VALEUR(S) DE RETOUR :
*
* CONDITION D'ERREUR :
*
* CONDITIONS LIMITES :
*
******************************************************/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_00InitServices";
   	XZSCT_Datagroup  pl_NomDg		= "";
   	XZSCT_NomMachine pl_NomMachine		= "";


	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSM_00InitServices : IN \n");

	/* recupere nom de la machine */
	XZSC_07NomMachine(pl_NomMachine);

	/* prepare datagroup de reception des redemandes
	* de services en cours */
	XDG_EncodeDG2(pl_NomDg, XDG_NTIMEINIT, pl_NomMachine);

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"Joindre Datagroup %s \n",pl_NomDg);

	/* abonnement au Datagroup */
	if(!XDG_JoindreDG(pl_NomDg))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				    "Erreur abonnement dg : %s",pl_NomDg);
	}
	
	/* armement du callback de redemande des services TIMER */
#ifndef _TIBCO_EMS
	if(TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TST_TINIT),
			(T_IPC_CONN_PROCESS_CB_FUNC) XZSM_EnvoiServices_cb,
				     (T_CB_ARG) NULL) == T_FALSE)
#else
	if(TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TST_TINIT),
			XZSM_EnvoiServices_cb,
				     (T_CB_ARG) NULL) == T_FALSE)
#endif
	{
	     XZST_03EcritureTrace(XZSTC_WARNING,
		      "Erreur armement du Callback sur XDM_TSG_INIT");
	}

	/*A Traitement de la suppression du Timer memorise 
	*	Modification V1.2 ***/
	/*B Armement du callback de suppression du Timer **/
#ifndef _TIBCO_EMS
	if(TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TST_TSUP),
		(T_IPC_CONN_PROCESS_CB_FUNC) XZSM_SupTimer_Cb,
				     (T_CB_ARG) NULL) == T_FALSE)
#else
	if(TipcSrvProcessCbCreate(TipcMtLookupByNum(XDM_TST_TSUP),
		 XZSM_SupTimer_Cb,
				     (T_CB_ARG) NULL) == T_FALSE)
#endif
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"Erreur armement du Callback sur XDM_TST_TSUP");
 	}

	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSM_00InitServices : OUT \n");

}	/* Fin XZSM_00IniServices */



/*X*/
/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* Fonction :
*/

int	XZSM_01DemanderProgrammation(
		double		    va_HorodateSec,
		int		    va_Delai,
		XZSCT_Datagroup     va_Datagroup,
		int		    va_IdMesg,
		XZSMT_CleProg	    va_CleProg,
		int		    va_Priorite	)
/*
* ALGORITHME :
*
* Arguments
* HorodateSec : Horodate de declenchement de la 
*		programmation.
* Delai       :	Delai de programmation cyclique en 
*		secondes.
* Datagroup   : Datagroup destinataire du message.
* IdMesg      : Identifiant du message a envoyer.
* CleProg     : Cle identifiant la programmation du 
*		timer et permettant d'annuler la 
*		programmation.
* Priorite       :  Flag indiquant une securite de programmation	
*
* Fonction
* Le reveil se declenchera a l'horodate programmee et
* se repetera cycliquement selon le delai precisee. 
* Si l'horodate est XZSMC_HORODATE_NUL et le delai non 
* nul, le premier reveil aura lieu au bout du delai. 
* Si le delai est nul, la programmation n'est pas 
* cyclique et le reveil aura lieu a l'horodate.
* Au reveil, le timer envoi le message IdMesg au 
* datagroup Datagroup. Ce message contient la cle du 
* message sous la forme CleProg, recuperable avec le 
* format %s.
* Si Horodate = XZSMC_HORODATE_NUL et Delai = 0
* 	ou	Datagroup = NULL
*	ou	IdMesg = 0
*	ou	CleProg = XZSMC_CLEPROG_NUL
*	la fonction retourne XZSMC_ARG_INV.
* La fonction envoie le message XDM_TST_PROG au 
* datagroup XDG_NSUPTIME_NomMachine contenant dans 
* l'ordre :
* Horodate Delai Datagroup IdMesg CleProg avec les 
* formats %lf %d %s %d %s.
* Le nom de la machine est recupere avec la 
* primitive XZSC_07.
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSMC_INI_INV		initialisation non effectuee
*			(primitive XZSC_07).
* XZSMC_RTW_INV         appel(s) a RT-WORKS infructueux
* XDC_OK		ok.
*
* Mecanismes
* RTworks :       RTS2
*******************************************************/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_01DemanderProgrammation ";
	int				vl_ValRet 		= XDC_OK;
	XZSCT_NomMachine		vl_NomMachine	= "";
	char				tl_NomTacheTimer[80]  = "";
	 	/* pour memorisation de la programation */
	XZSMT_ENRG_TIMER               vl_Reveil; 



	XZST_03EcritureTrace(XZSTC_INTERFACE
		    ,"XZSM_01DemanderProgrammation IN \
		    Horodate %lf -- Delai %d -- Datagroup %s -- \
		    IdMsg %d -- Cle %s \n",
		    va_HorodateSec,va_Delai,va_Datagroup,
		    va_IdMesg,va_CleProg);

	/* si les parametres ne sont pas coherents */
	if (  (va_HorodateSec == XZSMC_HORODATE_NUL && va_Delai == 0)
   		|| va_CleProg == XZSMC_CLEPROG_NUL
   		|| va_Datagroup == NULL
   		|| va_IdMesg    == 0)
	{
   		XZST_03EcritureTrace(XZSTC_WARNING
 ,"XZSM_01DemanderProgrammation : Au moins un parametre n'est pas coherent") ;
   
		/* Abandonne */
   		vl_ValRet = XZSMC_ARG_INV;
	}

	/* sinon */
	else
	{
		/*A memorisation de la demande de programmation */
		vl_Reveil.Horodate = va_HorodateSec;
	    	vl_Reveil.Delai = va_Delai;
		strcpy(vl_Reveil.Dg,va_Datagroup);
		vl_Reveil.IdMsg = va_IdMesg;
	      	strcpy(vl_Reveil.Cle,va_CleProg);
		vl_Reveil.Priorite = va_Priorite;

		/*B suppression du reveil dans la liste */
		XZSM_SupprimeReveil(va_CleProg,va_Datagroup, va_IdMesg);

		/*B Insertion du Timer */
		vl_ValRet = XZSM_InsereReveil(&vl_Reveil);

	/*B Teste  pour savoir si on declenche le Timer */
	if (vl_ValRet == XDC_OK)
	{
      		/*B Si l'on connait le nomde la machine */
      		if (XZSC_07NomMachine(vl_NomMachine) == XDC_OK)
      		{
         		/* construction du nom de la tache timer locale */
	 		XDG_EncodeDG2 (tl_NomTacheTimer, 
					XDG_NSUPTIME, vl_NomMachine);

	 		/* envoi du message TST_PROG .... */
	 		if (!TipcSrvMsgWrite(tl_NomTacheTimer
			    ,XDM_IdentMsg(XDM_TST_PROG)
			    ,XDM_FLG_SRVMSGWRITE
			    ,T_IPC_FT_REAL8,va_HorodateSec
			    ,T_IPC_FT_INT4,va_Delai
			    ,T_IPC_FT_STR,va_Datagroup
			    ,T_IPC_FT_INT4,va_IdMesg
			    ,T_IPC_FT_STR,va_CleProg
			    ,T_IPC_FT_INT2,(T_INT2)va_Priorite
			    ,NULL))
         		{
	    			XZST_03EcritureTrace(XZSTC_WARNING
	,"TipcSrvMsgWrite(%s,%u,XDM_FLG_MSGWRITE,,%f,,%u,,%s,,%u,,%s,,%u)"
				,tl_NomTacheTimer
			        ,XDM_IdentMsg(XDM_TST_PROG)
				,va_HorodateSec
				,va_Delai
				,va_Datagroup
				,va_IdMesg
				,va_CleProg
				,va_Priorite) ;
	    
				vl_ValRet =  XZSMC_RTW_INV;
	 		}
      		}
      		else 
      		{
      			XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSM_01DemanderProgrammation:XZSC_07") ;
		
			vl_ValRet = XZSMC_INI_INV;
      		}
	   }

	}

	XZST_03EcritureTrace(XZSTC_INTERFACE ,
		"XZSM_01DemanderProgrammation OUT -- Code Retour %d \n",
		    vl_ValRet);
	
	return( vl_ValRet );

}	/* fin XZSM_01DemanderProgrammation  */



/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* APPEL :
*/
int	XZSM_02AnnulerProgrammation(
		XZSMT_CleProg	   va_CleProg,
		XZSCT_Datagroup	   va_Datagroup,
		int		   va_IdMesg)
/*
* ALGORITHME :
*
* Arguments
* CleProg   :	Cle annulant la programmation.
* Datagroup :	Annulation de toutes les programmations
*		concernant ce Datagroup.
* IdMesg    :	Annulation de toutes les programmations 
*		concernant cet identifiant du message.
*
* Fonction
* Si la cle est precisee, annulation de la programmation 
* du timer.
* Si la cle est XZSMC_CLEPROG_NUL, les arguments suivants 
* servent de filtre de suppression.
* Si CleProg = XZSMC_CLEPROG_NUL
*        et      Datagroup = NULL
*        et      IdMesg = 0
*	La fonction retourne XZSMC_ARG_INV.
* La fonction envoie le message XDM_TST_ANNU au 
* datagroup XDG_NSUPTIME_NomMachine contenant dans 
* l'ordre :
* CleProg Datagroup IdMesg avec les formats %s %s %d. 
* Mettre la chaine "\0" si les chaines sont vides.
* Le nom de la machine est recupere avec la primitive 
* XZSC_07.
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSMC_INI_INV		initialisation non effectuee
*			(primitive XZSC_07).
* XDC_OK		ok.
* Aucun code retour n'est prevu si la cle fournie 
* n'existe pas. En effet cela necessiterait d'attendre
* un message provenant du timer ou de faire une gestion
* des programmations en local de la tache demandeur.
*
* Mecanismes
* RTworks :       RTS2
******************************************************/
{   
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_02AnnulerProgrammation ";
	XZSCT_NomMachine	vl_NomMachine ;
	char 			tl_NomTacheTimer[80] ;
	XZSMT_CleProg	   	va_CleProg_Env;
	XZSCT_Datagroup	   	va_Datagroup_Env;

	XZST_03EcritureTrace(XZSTC_INTERFACE
		    ,"XZSM_02AnnulerProgrammation(Cle=%s,Dg=%s,IdMsg=%u)"
		    ,va_CleProg
		    ,va_Datagroup
		    ,va_IdMesg) ;

	/* Si les arguments de la fonction sont invalide */
	if (va_Datagroup == NULL && va_CleProg == XZSMC_CLEPROG_NUL 
				 && va_IdMesg  == 0)
	{
   		XZST_03EcritureTrace(XZSTC_WARNING,
		"XZST_03EcritureTrace:Il y a au moins un parametre errone") ; 
   
	/* abandonne */
   	return(XZSMC_ARG_INV) ;
	
	}
	else
	{
		/* suppression du reveil dans la liste */
		XZSM_SupprimeReveil(va_CleProg,
				    va_Datagroup, va_IdMesg);

        	/* si la Recherche du nom de la machine est ok*/
		if (XZSC_07NomMachine(vl_NomMachine) == XDC_OK)
        	{
           		/* construction du nom du datagroup NSUPTIME */
           		XDG_EncodeDG2 (tl_NomTacheTimer, 
				XDG_NSUPTIME,vl_NomMachine);
			
			
			/** Test si la va_CleProg est NULL , alors on 
			* affecte la chaine "\0" pour l'envoi du message
			* sinon Pb avec la primitive RTWorks  ***/

			strcpy((char*)va_CleProg_Env,(char *) 
				(( va_CleProg == XZSMC_CLEPROG_NUL)  ?
				 C_CLE_NULLE : va_CleProg ) ); 

			/** Test si la va_Datagroup est NULL , alors on 
			* affecte la chaine "\0" pour l'envoi du message
			* sinon Pb avec la primitive RTWorks  ***/
			strcpy(va_Datagroup_Env,(char *) 
				(( va_Datagroup == NULL)  ?
				 C_DATAGROUP_NUL : va_Datagroup) );
 

           		/* envoi du message TST_ANNU */ 
	   		if (!TipcSrvMsgWrite(tl_NomTacheTimer
			      ,XDM_IdentMsg(XDM_TST_ANNU)
			      ,XDM_FLG_SRVMSGWRITE
			      ,T_IPC_FT_STR,va_CleProg_Env
			      ,T_IPC_FT_STR,va_Datagroup_Env
			      ,T_IPC_FT_INT4,va_IdMesg
			      ,NULL))
	   		{
	      			XZST_03EcritureTrace(XZSTC_WARNING
		  ,"TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%s,,%u)"
				  ,tl_NomTacheTimer
				  ,XDM_IdentMsg(XDM_TST_ANNU)
				  ,va_CleProg_Env
				  ,va_Datagroup_Env
				  ,va_IdMesg) ;
				
				return( XZSMC_RTW_INV );
	   		}
        	}
		
		/* sinon informe du probleme */
		else 
		{
	   		XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSM_02AnnulerProgrammation:XZSC_07NomMachine") ;
	   
			return(XZSMC_INI_INV) ;
		}
	}

	return(XZSMC_OK) ;

}	/* fin XZSM_02AnnulerProgrammation  */


/******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* APPEL :
*/

int	XZSM_05DemanderDiffusionMode(
		XZSCT_NomTache		va_NomTache,
		XZSCT_NomMachine	va_NomMachine )
/*
* ALGORITHME :
*
* Arguments
* NomTache   :	Nom de la tache dont le mode est a
*		actualiser.
* NomMachine :	Nom de la machine de la tache.
*
* Fonction
* La fonction appelle la primitive XZSE_19 pour connaitre
* le nom du serveur de donnees actif.
* Elle envoie le message XDM_TSH_SYNC au datagroup 
* XDG_NSUPHORL_NomMachine o\171 NomMachine est le nom
* du serveur trouve.
* Le message contient le nom de la tache et le nom de 
* la machine au format  %s %s.
*
* Codes retour
* XDC_OK	ok.
*
* Mecanismes
* RTworks :       RTS2.
******************************************************/
{

	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_05DemanderDiffusionMode ";
	XZSCT_NomMachine 	vl_NomSDActif;   /* nom du serveur de donnees actif */
	XZSCT_Datagroup  	vl_NomDGSUPHORL; /* nom du datagroup NSUPHORL destinataire */ 
	int			vl_ValRet;
	XZSCT_NomSite           pl_NomSite;



	XZST_03EcritureTrace(XZSTC_FONCTION,
		"XZSM_05DemanderDiffusionMode(Tache=%s,Machine=%s)",
			va_NomTache,va_NomMachine) ;

	/* Recuperation du site de la machine */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
        {
                /* Ecriture Trace */
                XZST_03EcritureTrace( XZSTC_WARNING, 
				"Recuperation du Nom du site impossible. \n" );
        }


	/* construction du nom du datagroup NSUPHORL */
	XDG_EncodeDG2 (vl_NomDGSUPHORL, XDG_NSUPHORL,pl_NomSite);
											       
	/* envoi du message TSH_SYNC au DataGroup NSUPHORL*/ 
	if (!TipcSrvMsgWrite(vl_NomDGSUPHORL
		   ,XDM_IdentMsg(XDM_TSH_SYNC)
		   ,XDM_FLG_SRVMSGWRITE
		   ,T_IPC_FT_STR,va_NomTache
		   ,T_IPC_FT_STR,va_NomMachine
		   ,NULL) )
	{
   		XZST_03EcritureTrace(XZSTC_FONCTION,
	 "XZSM_05DemanderDiffusionMode:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%s,,%s)" 
		,vl_NomDGSUPHORL
		,XDM_IdentMsg(XDM_TSH_SYNC)
		,va_NomTache,va_NomMachine) ;

   		return(XZSMC_NOK) ;
	}

	return(XZSMC_OK) ;

} /* fin XZSM_05DemanderDiffusioMode */


/*********
*	Fonction XZSM_TraiterTime
*******/

void	XZSM_TraiterTime(
		T_IPC_CONN			va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA	va_Data,
		T_CB_ARG			va_Prive)

{
#ifndef _TIBCO_EMS
	T_REAL8		vl_Horodate ;
#else
	XDY_Horodate		vl_Horodate ;
#endif
	XZSMT_Horodate  vl_heureTrace;
	int             vl_jourSemaine;
	double          vl_horodateSec = 0.0;

	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSM_TraiterTime") ;

	/* Recuperation de la valeur de l'horodate */
	if (TipcMsgRead(va_Data->msg
		,T_IPC_FT_REAL8,&vl_Horodate
		,NULL))
	{
		/* Actualiser l'horodate */
   		XZSM_10ActualiserDeltaHorodate(vl_Horodate) ;
	}

	else 
		XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSM_TraiterTime:TipcMsgRead") ;

 	/****   Lecture Heure Systeme ******/
 	XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME,(int *) &vl_jourSemaine
		      , (double *) &vl_horodateSec );

	/*  Conversion heure pour affichage trace */
 	XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heureTrace );
 	XZST_03EcritureTrace(XZSTC_INFO
		     ,"XZSM_TraiterTime:Heure Systeme (%s)"
		     , vl_heureTrace) ;



	/*****  INUTILE pour MIGRAZUR 
 ****   Lecture Heure MIGRAZUR *****
 XZSM_07LireHorodate ( XZSMC_TYPE_RTWORKS
		      , (int *) &vl_jourSemaine
		      , (double *) &vl_horodateSec );
 XZSM_11ConversionHorodate ( vl_horodateSec, &vl_heureTrace );
 XZST_03EcritureTrace(XZSTC_INFO
		     ,"XZSM_TraiterTime:Heure MIGRAZUR (%s)"
		     , vl_heureTrace) ;

	********************/



}	/* fin XZSM_TraiterTime */





/****
* Fonction
*/
void	XZSM_ActualiserMode(
		T_IPC_CONN		   	va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA   	va_Data,
		T_CB_ARG			va_Prive)

{
	int 	vl_ValRet;
	int 	vl_ModeEmis;


	XZST_03EcritureTrace(XZSTC_DEBUG1,"XZSM_ActualiserMode") ;

	/* Recuperation du mode d'emission */
	if (TipcMsgRead(va_Data->msg
	       ,T_IPC_FT_INT4,(T_INT4 *)&vl_ModeEmis,NULL) == TRUE)
	{
   		XZST_03EcritureTrace(XZSTC_DEBUG1,
				"XZSM_ActualiserMode:TipcMsgRead OK") ;

		/* Ecriture dans la variable XZMV_MODE */
	 	if ( (vl_ValRet = XZSM_09ModeHorodate(XZSMC_ECRITURE,&vl_ModeEmis)) 
						!= XDC_OK)
                	{
                                /* Affichage Trace */
                                XZST_03EcritureTrace(XZSTC_WARNING,
                                                        "Maj Emission Impossible \n");
                        }	
	}
	else 
		XZST_03EcritureTrace(XZSTC_WARNING,"XZSM_ActualiserMode:TipcMsgRead") ;

} 	/* fin XZSM_ActualiserMode */



/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* APPEL :
*/

int	XZSM_06AutoriserMajHorodate(
		int 		va_ModeEmis,
		XZSCT_NomSite	va_NomSite)

/*
* ALGORITHME :
*
* Arguments
* ModeEmis : XSSMC_RTDAQ ou XZSMC_NSUPV ou XZSMC_SYSTE
*
* Fonction
* La fonction verifie que l'indicateur XZSMV_InitOk 
* est positionne a 0 sinon erreur XZSMC_INI_INV.
* Elle abonne la tache au datagroup "XDG_TIME_NomSite".
* Elle arme le callback suivant sur reception du message 
* XDM_TSH_MODE :
*    XZSM_ActualiserMode((IN)(T_MSG_HEADER *)msg_header,         
* 			(OUT)(char *) msg_data)
*    Les arguments sont specifiques a un callback IPC.
*    Ce callback effectue le traitement suivant :
*	- Memorisation du mode du message dans la 
*	  variable XZSMV_Mode.
*	- Si XZSMV_Mode = XZSMC_NSUPV
*	  - Armement du callback suivant sur reception 
*	    du message T_MT_TIME :
*	    XZSM_TraiterTime((IN)(T_MSG_HEADER *)msg_header, 
*	  		    (OUT)(char *) msg_data)
*	    Les arguments sont specifiques a un callback 
*	    IPC. Ce callback effectue le traitement
*	    suivant :
*		- Appel primitive XZ SM_10 avec 
*		  l'horodate du message.
*	  Fsi
* La fonction positionne l'indicateur XZSMV_InitOk a 1.
*
* Codes retour
* XZSMC_INI_INV		initialisation deja effectuee.
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS1.
******************************************************/
{

	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_06AutoriserMajHorodate ";
	int 	vl_ModeEmis	= 0;
	int 	vl_ValRet	= XDC_OK;
	XZSCT_Datagroup	vl_Datagroup	= "";


	/* Affichage trace */
	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSM_06AutoriserMajHorodate : IN -- \
		Mode = %d",va_ModeEmis) ;

/* si il n'y a pas encore eu d'initialisation */
if (XZSMV_InitOk == 0)   /* if 1 */
{
   /* Memorise le mode d'emission */
   if ( (vl_ValRet = XZSM_09ModeHorodate(XZSMC_ECRITURE,&va_ModeEmis))
				!= XDC_OK )
   {
	/* Affichage Trace */
        XZST_03EcritureTrace(XZSTC_WARNING, "Maj Emission Impossible \n");
   }


   /* joint le datagroup XDG_TIME_NomSite */
   XDG_EncodeDG2 (vl_Datagroup,  XDG_TIME, va_NomSite);
   if (XDG_JoindreDG(vl_Datagroup) == TRUE) /* if 2 */
   {
      	/* armement du callback d'actualisation du mode 
	*				sur reception du message TSH_MODE */
#ifndef _TIBCO_EMS
      	TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSH_MODE),
		(T_IPC_CONN_PROCESS_CB_FUNC)XZSM_ActualiserMode,
						(T_CB_ARG)NULL);
#else
      	TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TSH_MODE),
		XZSM_ActualiserMode,
						(T_CB_ARG)NULL);
#endif
	/* Recuperation du mode d'emission */
        XZSM_09ModeHorodate(XZSMC_LECTURE,&vl_ModeEmis);

      	if ( 	(vl_ModeEmis == XZSMC_NSUPV)   ||
		(vl_ModeEmis == XZSMC_SYSTE) )
      	{
		/* Declenchement Callback sur reception message TIME */
#ifndef _TIBCO_EMS
         	if (TipcSrvProcessCbCreate(XDM_IdentMsg(T_MT_TIME),
			(T_IPC_CONN_PROCESS_CB_FUNC)XZSM_TraiterTime,
						(T_CB_ARG)NULL) == NULL)
#else
         	if (TipcSrvProcessCbCreate(XDM_IdentMsg(T_MT_TIME),
			XZSM_TraiterTime,
						(T_CB_ARG)NULL) == NULL)
#endif
         	{
	    		XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSM_ActualiserMode:TipcProcessCbCreate(T_MT_TIME)") ;
         	}
      	}

      	/* Initialisation termine */
      	XZSMV_InitOk = 1;

      	/* retourne avec succes */
      	vl_ValRet = XZSMC_OK;

   }  /* if 2 */

   /* sinon echec de l'abonnement au datagroup */
   else
   {
	XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSM_06AutoriserMajHorodate:TipcSrvDgSetJoin(_time)") ;
	 vl_ValRet = XZSMC_NOK;
   }

}   /* if 1 */

/* sinon initialisation deja effectuee */
else
{
	XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSM_06AutoriserMajHorodate : Initialisation deja effectuee") ;
   	vl_ValRet = XZSMC_INI_INV;
}

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSM_06AutoriserMajHorodate : OUT \n");

	return (vl_ValRet);

}  /* fin XZSM_06AutoriserMajHorodate */


/*X*/
/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* TypeHorloge :	XZSMC_TYPE_SYSTEME ou 
*		XZSMC_TYPE_RTWORKS
* JourSemaine : Jour de la semaine de l'horodate.
* HorodateSec : Horodate lue en secondes.
* 
* Fonction
* Selon le type d'horloge precise la fonction effectue 
* soit la lecture de l'horodate systeme, soit la lecture
* de l'horodate RTworks, soit la recuperation de la 
* variable XZSMV_Horodate.
* Si XZSMC_TYPE_SYSTEME : utilisation de la fonction : 
* TutGetWallTimeSi XZSMC_TYPE_RTWORKS
*        Si XZSMV_Mode = XZSMC_NSUPV
*                Utilisation de la fonction : 
*		 TutGetWallTime (HorodateSysteme)
*                Horodate = 
*		 XZSMV_DeltaHorodate + HorodateSysteme.
*        Sinon si XZSMV_Mode = XZSMC_RTDAQ
*                Utilisation de la fonction : 
*		 TutGetCurrentTime (Horodate).
*                Si Horodate nulle
*                   Utilisation de la fonction : 
*		    TutGetWallTime (HorodateSysteme)
*                Fsi
*        Sinon
*                Utilisation de la fonction :
*		 TutGetWallTime (HorodateSysteme)
*        Fsi
* Fsi
* Recuperation du jour de la semaine.
* Fonctions utilisees : TutTimeCvtLookup et 
* TutTimeCvtNumToStr ou les fonctions Time UNIX.
* Si les arguments sont invalides (valeur de TypeHorloge) 
* la fonction retourne XZSMC_ARG_INV.
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSMC_OK     		ok
*
* Mecanismes
* Disponibilite de l'horodate sur toutes les taches.
*
*  --------------------------
*   CONDITIONS UTILISATION MIGRAZUR 
*
*	Pour MIGRAZUR, seule l'option XZSMC_TYPE_SYSTEME
*	pour le type d'horloge est utilisee.
*    ---------------------------
*
*****************************************************
* ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION ATTENTION
* ATTENTION cette fonction retourne un nombre de secondes
* relatif au fuseau horaire en vigueur au moment de son utilisation
* Cette convention absolument pas standard est historique au logiciel SAE lui-mÃƒÂªme.
* La base de donnÃƒÂ©e stocke un nombre de seconde en fonction du fuseau horaire ...
* 
* les 2 premiers parametres ne sont plus utilises
* le code en commentaire apres la fonction est le code d'origine
******************************************************/

int	XZSM_07LireHorodate(int va_TypeHorloge,        // parametre plus utilise
                        int * pa_JourSemaine,      // parametre plus utilise
                        double * pa_HorodateSec )  // parametre retourne un nbde secondes correspondant au TZ 
{
    struct tm   * pl_InfoTemps;
    int	vl_ValRet	= XDC_OK;
#ifdef _TIBCO_EMS
    time_t                nbsec;
    double                vl_Heure;
#else
    time_t                nbsec;
#endif

// recupere le nb de secondes UTC  TutGetWallTime() => time()
#ifdef _TIBCO_EMS
    vl_Heure = TutGetWallTime() ;
    *pa_HorodateSec = vl_Heure;
#else
	/* Utilisation de la fonctoin RTWorks : TutGetWallTime */
	// recuperation de l'heure nb de seconde UTC
	nbsec  = time( NULL );
	*pa_HorodateSec  = (double)nbsec;
#endif
	nbsec  = time( NULL );

    	/* appel de localtime pour recuperer l'indicateur d'ete dans tm_isdst */  
	 pl_InfoTemps    = localtime(&nbsec) ;
	 *pa_HorodateSec  = *pa_HorodateSec +3600+3600*pl_InfoTemps->tm_isdst;

    // nous concervons le switch pour avoir le meme comportement sur code retour de la fonction
    switch(va_TypeHorloge)
    {
    /* horloge systeme */
    case XZSMC_TYPE_SYSTEME : 
    case XZSMC_TYPE_RTWORKS :
    break ;
    
    default :
        vl_ValRet = XZSMC_NOK ;
    }
    
    // nous concervons le meme code
    // pour avoir le meme comportement que celui retournÃƒÂ© avant ...
	*pa_JourSemaine = 2 ;

	return (vl_ValRet);
}	/* fin XZSM_07LireHorodate */

// int	XZSM_07LireHorodate(
		// int		va_TypeHorloge,
		// int *		pa_JourSemaine,
		// double * 	pa_HorodateSec )
// {
	// static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_07LireHorodate ";
	// struct tm *		pl_InfoTemps ;
	// int	vl_ValRet	= XDC_OK;
// #ifdef _TIBCO_EMS
	// double                vl_Heure;
// #else
	// time_t                vl_Heure;
// #endif


// /* selon le type d'horloge demandee */
// switch(va_TypeHorloge)
// {
  // /* horloge systeme */
  // case XZSMC_TYPE_SYSTEME :

       // /* utilisation de la fonctoin rtworks : tutgetwalltime */
// #ifdef _TIBCO_EMS
       // vl_Heure = TutGetWallTime() ;
       // *pa_HorodateSec = vl_Heure;
// #else
       // *pa_HorodateSec = TutGetWallTime() ;
// #endif
       // break ;
       
  // /* Horloge RTWorks */
  // case XZSMC_TYPE_RTWORKS :

       // /* si le mode est XZSMC_NSUPV */
       // if (XZSMV_Mode == XZSMC_NSUPV)
       // {
	  // /* l'horodate est l'heure systeme + u decalage */
	  // *pa_HorodateSec = TutGetWallTime() + XZSMV_DeltaHorodate ;
       // }
       // /* sinon si le mode est XZSMC_RTDAQ */
       // else if (XZSMV_Mode == XZSMC_RTDAQ)
       // {
	  // /* si l'heure courante est nulle */
	  // if ((*pa_HorodateSec = TutGetCurrentTime()) == 0.0)
	  // {
	     // /* recupere l'heure systeme */
	     // *pa_HorodateSec = TutGetWallTime() ;
	  // }
       // }
       // /* sinon recupere l'heure systeme */
       // else 	
	// *pa_HorodateSec = TutGetWallTime() ;

       // break ;

   // default :
       // vl_ValRet = XZSMC_NOK ;

// }  /* fin switch */


	// /* recupere le jour de la semaine */
// #ifdef _TIBCO_EMS
	// vl_Heure = TutGetWallTime() ;
	// *pa_HorodateSec = vl_Heure;
// #else
	// *pa_HorodateSec = TutGetWallTime() ;
// #endif

	// vl_Heure = (time_t) (*pa_HorodateSec);
	// pl_InfoTemps    = localtime(&vl_Heure) ;

	// /**pa_JourSemaine = pl_InfoTemps->tm_wday ;*/
	// *pa_JourSemaine = 2 ;


	// return (vl_ValRet);


// }	/* fin XZSM_07LireHorodate */




/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* APPEL :
*/

int	XZSM_08EmettreHorodate(
		double	va_HorodateSec)

/*
* ALGORITHME :
*
* Arguments
* HorodateSec : Horodate en secondes a envoyer.
*
* Fonction
* La fonction envoie le message XDM_TSH_HORO au 
* datagroup XDG_NSUPHORL_NomMachine.
* Le message contient l'horodate au format  %f.
* Le nom de la machine est recupere avec la  primitive
*  XZSC_07.
* Si les arguments sont invalides (horodate nulle ou 
* < 1/1/1970) la fonction retourne XZSMC_ARG_INV.
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XDC_OK		ok.
*
* Mecanismes
* RTworks :       RTS2
* ******************************************************/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_08EmettreHorodate ";
	XZSCT_NomSite           pl_NomSite;
	XZSCT_Datagroup  	vl_NomSiteHorl ;
	int 			vl_ValRet;

	
	XZST_03EcritureTrace(XZSTC_FONCTION,
          "XZSM_08EmettreHorodate(Horodate=%f)",va_HorodateSec) ;

/* Test si horodate > 1/1/1970 */
if (va_HorodateSec > 0.0)   
{
	/* Recuperation du site de la machine */
        if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) == XDC_OK)
        {
      		/* construction du nom du datagroup NSUPHORL */
		XDG_EncodeDG2 (vl_NomSiteHorl,  
				XDG_NSUPHORL, pl_NomSite);

      		/* envoi du message TSH_HORO 
		*	au datagroup NSUPHORL */ 
      		if (!TipcSrvMsgWrite(vl_NomSiteHorl
		   	 ,XDM_IdentMsg(XDM_TSH_HORO)
		   	 ,XDM_FLG_SRVMSGWRITE
			 ,T_IPC_FT_REAL8,va_HorodateSec
			 ,NULL))
      		{
	 		XZST_03EcritureTrace(XZSTC_WARNING
			    ,"XZSM_08EmettreHorodate:TipcSrvMsgWrite				(%s,%u,XDM_FLG_SRVMSGWRITE,,%f)"
			    ,vl_NomSiteHorl
			    ,XDM_IdentMsg(XDM_TSH_HORO)
			    ,va_HorodateSec) ;
	 
			return(XZSMC_NOK) ;
      		}
   	}
	else
		return (XZSMC_NOK);
}
else
/* l'argument est invalide */
{
	XZST_03EcritureTrace(XZSTC_WARNING,
		"XZSM_08EmettreHorodate:Horodate invalide") ;
   	return(XZSMC_ARG_INV) ;
}

return (XDC_OK);

}	/* fin XZSM_08EmettreHorodate */





/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* ModeEmis :	XSSMC_RTDAQ ou XZSMC_NSUPV ou XZSMC_SYSTE
*
* Fonction
* Appel primitive XZSC_07 pour connaitre le nom 
* de la machine.
* La fonction s'abonne au datagroup 
* XDG_NSUPHORL_NomMachine.
* De plus, le mode d'emission est memorise dans la 
* variable XZSMV_Mode.
* Appel primitive XZSE_09 pour connaitre l'etat du 
* calculateur.
* Si calculateur actif
*	- Preparation du message XDM_TSH_MODE avec la 
*	  variable XZSMV_Mode.
*	- Envoie du message XDM_TSH_MODE au 
*	  datagroup"_time".
* Fsi
* Elle arme le callback suivant sur reception du 
* message XDM_TSH_HORO :
*    XZSM_ActualiserHorodate(
*		(IN)(T_MSG_HEADER *) msg_header,
*		(OUT)(char *) msg_data)
*    Les arguments sont specifiques a un callback IPC. 
*    La fonction recupere l'horodate et effectue la 
*    mise a jour.
*    Si XZSMV_Mode = XZSMC_RTDAQ
*        - Utilisation de la fonction :
*	   TdaqSetCurrentTime
*    Sinon
*        - Appel primitive XZSE_09 pour connaitre 
*          l'etat du calculateur.
*	 - Si calculateur actif
*             - Preparation du message T_MT_TIME.
*             - Envoi du message T_MT_TIME au 
*		datagroup "_time" .   
*    	   Fsi
*        - Appel primitive XZ SM_10 avec l'horodate du 
*	   message.
*   Fsi
* Positionne l'indicateur XZSMV_TimeRecu a 1.
* Elle arme le callback suivant sur reception du 
* message XDM_TSH_SYNC :
*    XZSM_Synchronisation((IN)(T_MSG_HEADER *)msg_header,
*   			 (OUT)(char *)msg_data)
* Les arguments sont specifiques a un callback IPC. 
* La fonction recupere le nom de la machine et le nom de 
* la tache dans le message.
*       - Preparation du message XDM_TSH_MODE avec la 
*	  variable XZSMV_Mode.
*       - Envoie du message XDM_TSH_MODE au datagroup 
*	  XDG_NomMachine_NomTache.
*       - Si indicateur XZSMV_TimeRecu est positionne 
*	  a 1.
*           - Appel primitive XZ SM_07 avec 
*	      XZSMC_TYPE_RTWORKS.
*           - Preparation du message T_MT_TIME.
*           - Envoi du message T_MT_TIME au datagroup 
*	      XDG_NomMachine_NomTache.
*         Fsi 
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS1 , RTS2 et RTS3.
*******************************************************/
void			XZSM_ActualiserHorodate(
T_IPC_CONN				       va_Cnx,
T_IPC_CONN_PROCESS_CB_DATA		       va_Data,
T_CB_ARG				       va_Prive
					       )
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_ActualiserHorodate ";
	double			vl_Horodate ;
	int			vl_EtatMachine ;
	T_IPC_MT                vl_Msg ;

/* si la recuperation de l'horodate s'effectue sans probleme */
if (TipcMsgRead(va_Data->msg
	       ,T_IPC_FT_REAL8,(T_REAL8 *)&vl_Horodate
	       ,NULL
	       ) 
   )
{
   /* si le mode de diffusion de l'heure est RTDAQ */
   if (XZSMV_Mode == XZSMC_RTDAQ)
   {
      /* utilisation de la fonction TdaqSetCurrentTime */
   }
   /* sinon quel est l'etat de mon calculateur */
   else
   {
      XZSE_09EtatMachine(&vl_EtatMachine) ;
      
      /* si le calculateur est actif */
      if (vl_EtatMachine == XZSEC_ETAT_NACTIF)
      {
	 /* creation du message */
	 if ((vl_Msg = XDM_IdentMsg ( T_MT_TIME )) == NULL )
	 {
	    XZST_03EcritureTrace(XZSTC_WARNING
				,"XZSM_ActualiserHorodate:Erreur IdentMsg(T_MT_TIME)"
				) ;
	 }
	 /* Emission du message time */
	 if (!TipcSrvMsgWrite("_time"
			     ,vl_Msg
		   	     ,XDM_FLG_SRVMSGWRITE
			     ,T_IPC_FT_REAL8,vl_Horodate
			     ,NULL
			     )
	    )
	 {
	    XZST_03EcritureTrace(XZSTC_WARNING
				,"XZSM_ActualiserHorodate:Erreur TipcSrvMsgWrite(_time,XDM_FLG_SRVMSGWRITE,,%f)"
				,vl_Horodate
				) ;
	 }
	 else
	    XZST_03EcritureTrace(XZSTC_INFO
				,"XZSM_ActualiserHorodate:TipcSrvMsgWrite(_time,XDM_FLG_SRVMSGWRITE,,%f) = OK"
				) ;
      }
      /* actualise l'horodate */
      XZSM_10ActualiserDeltaHorodate(vl_Horodate) ;
   }
   /* Heure recue */
   XZSMV_TimeRecu = 1 ;
}
}
void			XZSM_Synchronisation(
T_IPC_CONN				    va_Cnx,
T_IPC_CONN_PROCESS_CB_DATA		    va_Data,
T_CB_ARG				    va_Prive
					    )
{
#ifndef _TIBCO_EMS
	char *			pl_NomTache ;
	char *			pl_NomMachine ;
#else
	XDY_NomTache pl_NomTache;
	XDY_NomMachine pl_NomMachine;
#endif
	XZSCT_Datagroup		vl_Dg ;
	double			vl_Horodate ;
	int			vl_Jour_De_La_Semaine ;

/* si la recuperation du nom machine et du nom de la tache s'effectue sans probleme */
if (TipcMsgRead(va_Data->msg
	       ,T_IPC_FT_STR,&pl_NomTache
	       ,T_IPC_FT_STR,&pl_NomMachine
	       ,NULL
	       )
   )
{
   /* construction du datagroup cible */
   XDG_EncodeDG2 (vl_Dg, pl_NomMachine,pl_NomTache) ;

   /* envoie du message XDM_TSH_MODE */
   if (!TipcSrvMsgWrite(vl_Dg
		       ,XDM_IdentMsg(XDM_TSH_MODE)
		       ,XDM_FLG_SRVMSGWRITE
		       ,T_IPC_FT_INT4,XZSMV_Mode
		       ,NULL
		       )
      )
   {
      XZST_03EcritureTrace(XZSTC_WARNING
			  ,"XZSM_Synchronisation:TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%u)"
			  ,vl_Dg
			  ,XDM_IdentMsg(XDM_TSH_MODE)
			  ,XZSMV_Mode
			  ) ;
   }
   /* si l'heur a ete recu auparavant */
   if (XZSMV_TimeRecu)
   {
      /* connaisance de l'heure */
      if (XZSM_07LireHorodate(XZSMC_TYPE_RTWORKS,&vl_Jour_De_La_Semaine,&vl_Horodate) == XZSMC_OK)
      {
	 /* emission de l'horodate */
	 if (!TipcSrvMsgWrite(vl_Dg
			     ,XDM_IdentMsg(T_MT_TIME)
		   	     ,XDM_FLG_SRVMSGWRITE
			     ,T_IPC_FT_REAL8,vl_Horodate
			     ,NULL
			     )
	    )
	 {
	    XZST_03EcritureTrace(XZSTC_WARNING
				,"TipcSrvMsgWrite(%s,%u,XDM_FLG_SRVMSGWRITE,,%f)"
				,vl_Dg
			        ,XDM_IdentMsg(T_MT_TIME)
				,vl_Horodate
				) ;
	 }
      }
      else
      {
	 XZST_03EcritureTrace(XZSTC_WARNING ,"XZSM_Synchronisation:XZSM_07") ;
      }
   }
}
}





/*X*/
/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
------------------------------------------------------
* SERVICE RENDU : 
*  < Descriptif de la fonction >
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSM_09ModeHorodate(
	int 	Mode_Operation,
	int *	Mode_Emission)

/*
* ARGUMENTS EN ENTREE :
*   Mode_Operation :  Type de l'operation demandee
*			(Lecture ou ecriture )
*   Mode_Emission :  Valeur du Mode d'emission
*
*
* ARGUMENTS EN SORTIE :
*   Mode _Emission : Mode d'emission si demande en lecture
*
*
* CODE RETOUR : 
*   XDC_OK :   Succes 
*   XZSMC_ARG_INV : Argument invalide
*
* CONDITION D'UTILISATION
*   Aucune 
*
* FONCTION 
*   Si mode operation est en eciture, alors enregistrement
*	du mode d'emission dans la variable XZSMV_Mode
*   sinon si lecture on retourne la valeur de cette variable
*
------------------------------------------------------*/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_09ModeHorodate ";

	/* Test suivant le mode de l'operation */
	switch (Mode_Operation)   
	{
		/* Ecriture  */
		case XZSMC_ECRITURE:
		
			/* Memorisation du mode d'emission */
			XZSMV_Mode = *Mode_Emission;

			break;
	
		/*  Lecture */
		case XZSMC_LECTURE: 

			/* Retour valeur mode emission */
			*Mode_Emission = XZSMV_Mode;

			break;

		default :
			return (XZSMC_ARG_INV);

	} 

	return (XDC_OK);

}	/* Fin  XZSM_09ModeHorodate */



/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* HorodateSec : Horodate en secondes.
*
* Fonction
* - Appel primitive XZSM_07 avec XZSMC_TYPE_SYSTEME
*   (HorodateSysteme).
* - Calcul du DeltaHorodate = HorodateSec - 
*   HorodateSysteme.
* - Memorisation du DeltaHorodate dans la variable
*   XZSMV_DeltaHorodate.
*
* Codes retour
* XZSMC_INI_INV		Initialisation non effectuee.
* XDC_OK		ok.
*
* Mecanismes
* Diffusion de l'horodate a travers le reseau par le
* datagroup "_time", et disponibilite de l'horodate 
* sur toutes les taches.
*
*  -------------------------------------------
* Conditions Utilisations MIGRAZUR
*	Pour MIGRAZUR, on n'utilise pas un Delta pour
*	le calcul de l'heure system. En fait seuls les
* 	taches TSHORL ont l'heure systeme et surtout ont
*	modifiees cette heure sur leur calculateur.
*	Donc dans la fonction XZSM_10ActualiserSyste, seule
*	l'option concernant le mode XZSMC_SYSTE est utilise.
*   -------------------------------------------
*
*******************************************************/

int	XZSM_10ActualiserDeltaHorodate( double	va_Horodate )

{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_10ActualiserDeltaHorodate ";
	int		vl_Jour_De_La_Semaine ;
	double		vl_Horodate ;
	time_t 		vl_HeureSystem;


	/* Affichage trace */
	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSM_10ActualiserDeltaHorodate(%f)",va_Horodate) ;

/*  Test suivant le mode d'emission */
if ( 	(XZSMV_Mode == XZSMC_RTDAQ) || 
	(XZSMV_Mode == XZSMC_NSUPV) )
{

	/* demande de l'horodate systeme */
	if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,
		&vl_Jour_De_La_Semaine,&vl_Horodate) == XZSMC_OK)
	{
   		/* deduit le delta */
   		XZSMV_DeltaHorodate = va_Horodate - vl_Horodate ;
	}

	else 
		XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSM_10ActualiserDeltaHorodate:XZST_03") ;

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"XZSM_10ActualiserDeltaHorodate DELTA = (%f)",
		XZSMV_DeltaHorodate) ;

}
else if (XZSMV_Mode == XZSMC_SYSTE) /** MIGRAZUR */
{
	/* Mise a jour de l'heure systeme */
	vl_HeureSystem = (time_t )  va_Horodate;
#ifdef _HPUX_SOURCE
	if ( stime( &vl_HeureSystem) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
            	    "MAJ heure systeme a echouee : Erreur %d ", errno );

		return(XDC_NOK);
	}
#else
        /*on passe par une commande date -u pour pallier au pb de sticky bit*/
        /*date -u MMDDhhmm*/
        time_t    vl_Temps = (time_t)va_Horodate;
        struct tm *             pl_InfoTemps = NULL;
        char vl_cmd[30];
        
        // on supose qu'il faut pas tenir compte SAE_191
/*        char *tz
        tz = getenv("TZ");
        setenv("TZ", "", 1);
        tzset();
        pl_InfoTemps = localtime(&vl_Temps) ;
        if (tz)
            setenv("TZ", tz, 1);
        else
            unsetenv("TZ");
        tzset();
*/
//        pl_InfoTemps = localtime(&vl_Temps) ;
//        pl_InfoTemps = gmtime(&vl_Temps) ;

/* Modifs RGR */
  /* les nb de secondes passees sont en local time */
  /* on recupere dans la struct tm generee le tm_isdst pour savoir si il faut appliquer le decalage heure d'ete */
  pl_InfoTemps = localtime(&vl_Temps);
  vl_Temps = vl_Temps - 3600 - 3600 * pl_InfoTemps->tm_isdst;

  /* on remet a jour la struct tm */
  pl_InfoTemps = localtime(&vl_Temps);

  pl_InfoTemps->tm_year = pl_InfoTemps->tm_year + 1900;
  sprintf(vl_cmd,"tscmd.x \"date -u %02u%02u%02u%02u%04u.%02u\""
                ,(pl_InfoTemps->tm_mon +1)
                ,pl_InfoTemps->tm_mday
                ,pl_InfoTemps->tm_hour
                ,pl_InfoTemps->tm_min
                ,pl_InfoTemps->tm_year
                ,pl_InfoTemps->tm_sec);
        system (vl_cmd);
         XZST_03EcritureTrace(XZSTC_DEBUG1,"CMD %s",vl_cmd);

#endif

}

	/* Retour OK */
	return(XDC_OK);


} 	/* Fin XZSM_10ActualiserDeltaHorodate */





/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* HorodateSec :	Horodate en secondes
* Horodate    : Horodate au format horodate.
*
* Fonction
* Transformation de la date sous la forme 
* JJ/MM/AAAA HH:MN:SS.
* Fonctions utilisees : TutTimeCvtLookup et 
* TutTimeCvtNumToStr ou les fonctions Time UNIX.
*
* Codes retour
* XDC_OK	ok.
* 
* Mecanismes
*******************************************************/

int	XZSM_11ConversionHorodate(
		double	         va_HorodateSec,
		XZSMT_Horodate  *pa_Horodate)
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_11ConversionHorodate ";
	time_t	vl_Temps = (time_t)va_HorodateSec ;
	struct tm *		pl_InfoTemps = NULL;
	char *tz;

	XZST_03EcritureTrace(XZSTC_INTERFACE," XZSM_11ConversionHorodate : IN \
	-- Nb Secondes %f \n",va_HorodateSec);

// opÃƒÂ©ration a faire car elle n'est plus traitee par la base (en sortie sybase)
// Nous souhaitons recuuperer une chaine de caractere comme si nous etions sure un fuseau horaire TZ=UTC

/*	tz = getenv("TZ");
	setenv("TZ", "", 1);
	tzset();

	pl_InfoTemps = localtime(&vl_Temps) ;           
*/
	/* ajout du siecle */
/*	pl_InfoTemps->tm_year = pl_InfoTemps->tm_year + 1900;


	if (tz)
	    setenv("TZ", tz, 1);
	else
	    unsetenv("TZ");
	tzset();
*/
//        pl_InfoTemps = localtime(&vl_Temps) ;
//        pl_InfoTemps = gmtime(&vl_Temps) ;

/* Modifs RGR */
/* On recupere l'indicateur heure d'ete tm_isdst et on applique pour avoir du TZ */
   pl_InfoTemps = localtime(&vl_Temps);
   vl_Temps = vl_Temps - 3600 - 3600 * pl_InfoTemps->tm_isdst;

/* On remet a jour la struct tm */
   pl_InfoTemps = localtime(&vl_Temps);


   pl_InfoTemps->tm_year = pl_InfoTemps->tm_year + 1900;

	sprintf((char *)pa_Horodate,"%02u/%02u/%04u %02u:%02u:%02u"
		,pl_InfoTemps->tm_mday
		,(pl_InfoTemps->tm_mon +1)
		,pl_InfoTemps->tm_year
		,pl_InfoTemps->tm_hour
		,pl_InfoTemps->tm_min
		,pl_InfoTemps->tm_sec );

/* construction de la chaine */
/*strftime((char *)pa_Horodate,20,
		"%d/%m/%Y %T",localtime(&vl_Temps)) ;*/

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		" XZSM_11ConversionHorodate : OUT \
	-- Valeur date %s \n",pa_Horodate);

	return(XDC_OK);


}	/* Fin XZSM_11ConversionHorodate */



/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* HorodateSec : Horodate en secondes
* HorodateInv : Horodate au format inverse.
*
* Fonction
* Transformation de la date sous la forme 
* AAAAMMJJHHMNSS.
* Fonctions utilisees : TutTimeCvtLookup,
* TutTimeCvtNumToStr ou les fonctions Time UNIX, 
* puis fonction String UNIX.
*
* Codes retour
* XDC_OK	ok.
* Mecanismes
*******************************************************/

int			XZSM_12ConversionHorodateInv(
double						    va_HorodateSec,
XZSMT_HorodateInv *				    pa_HorodateInv )
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_12ConversionHorodateInv ";
        time_t		vl_Temps = (time_t)va_HorodateSec ;
	struct tm *		pl_InfoTemps ;

XZST_03EcritureTrace(XZSTC_FONCTION,"XZSM_12ConversionHorodateInv(Horo=%f,Cible=%08X)",va_HorodateSec,pa_HorodateInv) ;

/*-------------------------------------------------------------------*/
/*                       NOTE DU DEVELOPPEUR			     */
/*-------------------------------------------------------------------*/
/* Ici pas question de passer par la fonction strftime car le fait   */
/* de coller les formats %W %d correspond a des mots cles de sccs    */
/* qui seront substitueer lors d'u delta			     */
/*-------------------------------------------------------------------*/
// on supose qu'il faut pas tenir compte SAE_191
/*char *tz;
tz = getenv("TZ");
setenv("TZ", "", 1);
tzset();
pl_InfoTemps = localtime(&vl_Temps) ;
if (tz)
    setenv("TZ", tz, 1);
else
    unsetenv("TZ");
tzset();
*/

//        pl_InfoTemps = localtime(&vl_Temps) ;
//        pl_InfoTemps = gmtime(&vl_Temps) ;

/* Modifs RGR */
/* On recupere l'info heure d'ete tm_isdst et on applique pour avoir du TZ */
pl_InfoTemps = localtime(&vl_Temps);
vl_Temps = vl_Temps - 3600 - 3600 * pl_InfoTemps->tm_isdst;

/* On remet a jour la struct tm */
pl_InfoTemps = localtime(&vl_Temps);

/* ajout du siecle */
pl_InfoTemps->tm_year = pl_InfoTemps->tm_year + 1900;

sprintf((char *)pa_HorodateInv,"%04u%02u%02u%02u%02u%02u",pl_InfoTemps->tm_year
						 ,pl_InfoTemps->tm_mon
						 ,pl_InfoTemps->tm_mday
						 ,pl_InfoTemps->tm_hour
						 ,pl_InfoTemps->tm_min
						 ,pl_InfoTemps->tm_sec
						 ) ;
return(XDC_OK);
}
/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* Horodate    : Horodate au format horodate.
* HorodateSec : Horodate en secondes
*
* Fonction
* Transformation de la date au format 
* jj/mm/aaaa hh:mn:ss en secondes.
* Fonctions utilisees : les fonctions Time UNIX et
* fonction String UNIX.
*
* Codes retour
* XDC_OK	 ok.
* 
* Mecanismes
*******************************************************/

int	XZSM_13ConversionHorodateSec(
		XZSMT_Horodate	pa_Horodate,
		double *	pa_HorodateSec)
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_13ConversionHorodateSec ";
	struct tm		vl_Enr ;
	struct tm		*pl_temps ;
	int 			vl_ValRet = XDC_OK;
	char *tz;
	time_t timestamp;

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSM_13ConversionHorodateSec : IN \
		--Horodate %s \n",pa_Horodate);

	/*A Verification si la Date est une Chaine Vide */
	/* decode la chaine de caractere */
	if (sscanf(pa_Horodate
	  ,"%02u/%02u/%04u %02u:%02u:%02u"
	  ,&vl_Enr.tm_mday
	  ,&vl_Enr.tm_mon
	  ,&vl_Enr.tm_year
	  ,&vl_Enr.tm_hour
	  ,&vl_Enr.tm_min
	  ,&vl_Enr.tm_sec) == 6)
	{
   		/* config */
   		vl_Enr.tm_year = vl_Enr.tm_year - 1900;
   		vl_Enr.tm_isdst = 0 ;

   		/* si la conversion pose probleme */
		/*? On decemente le mois de 1, car la valeur
		*	numerique des mois est comprise entre 
		*	0 et 11 */
		vl_Enr.tm_mon = vl_Enr.tm_mon -1;

// correctif SAE_190
// celui ci doit rester car il recoit une date literale TZ 
// et on  doit continue a la donner en TZ pour Sybase
//		tz = getenv("TZ");
//		setenv("TZ", "", 1);
//		tzset();

		if ((*pa_HorodateSec = mktime(&vl_Enr)) == (time_t)-1)
   		{
      			XZST_03EcritureTrace(XZSTC_WARNING,
				"XZSM_13ConversionHorodateSec:mktime") ;
      			vl_ValRet = XZSMC_NOK ;
   		}
		/* appel de localtime sur la date pour recuperer l'indicateur heure d'ete tm_isdst */
		timestamp=(time_t) (*pa_HorodateSec);
		pl_temps=localtime( &timestamp);
		/**pa_HorodateSec=*pa_HorodateSec-3600-3600*pl_temps->tm_isdst;	*/
		/* *pa_HorodateSec=*pa_HorodateSec+3600*pl_temps->tm_isdst;*/
		/* Correction regression passage sous GitLab */
		/* Correction ete KO  */
		*pa_HorodateSec=*pa_HorodateSec+3600 ; /*+3600*pl_temps->tm_isdst;*/
// correctif SAE_190
//		if (tz)
//		    setenv("TZ", tz, 1);
//		else
//		    unsetenv("TZ");
//		tzset();
	}
	else 
	{
   		XZST_03EcritureTrace(XZSTC_WARNING
	       ,"XZSM_13ConversionHorodateSec:l'horodate %s est erronee"
		       ,pa_Horodate) ;
   		vl_ValRet = XZSMC_OK ;
	}

	XZST_03EcritureTrace(XZSTC_INTERFACE,
		"XZSM_13ConversionHorodateSec : OUT \
		-- Code Retour %d -- Nb Sec %lf \n",
		vl_ValRet,*pa_HorodateSec);

	return(vl_ValRet);

}	/* Fin XZSM_13ConversionHorodateSec */




/*X------------------------------------------------------
* SERVICE RENDU : 
*  Permet de retourner un nb de secondes au format
*		 DELAI jj:hh:mn:ss
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int 	XZSM_14ConversionHorodateSecDelai(
		double	        va_HorodateSec,
		XZSMT_Delai *	pa_Delai)

/*
* ARGUMENTS EN ENTREE :
*   va_HorodateSec : Nb de secondes a convertir
*
* ARGUMENTS EN SORTIE :
*   pa_Delai : chaine au format DELAI
*
*
* CODE RETOUR : 
*   XDC_OK
*
* CONDITION D'UTILISATION
*
* FONCTION 
*
------------------------------------------------------*/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_14ConversionHorodateSecDelai ";
	time_t	vl_Temps = (time_t)va_HorodateSec ;
	struct tm *		pl_InfoTemps ;

	/* Recuperation du Temps */
// on supose qu'il faut pas tenir compte SAE_191
/*
char *tz;
tz = getenv("TZ");
setenv("TZ", "", 1);
tzset();
pl_InfoTemps = localtime(&vl_Temps) ;
if (tz)
    setenv("TZ", tz, 1);
else
    unsetenv("TZ");
tzset();
*/    
//        pl_InfoTemps = localtime(&vl_Temps) ;
//        pl_InfoTemps = gmtime(&vl_Temps) ;

/* Modifs RGR */
/* On recupere l'info heure d'ete tm_isdst et on applique pour avoir du TZ */
 pl_InfoTemps = localtime(&vl_Temps);
 vl_Temps = vl_Temps - 3600 - 3600 * pl_InfoTemps->tm_isdst;

/* On met a jour la struct tm */
pl_InfoTemps = localtime(&vl_Temps);


	sprintf((char *)pa_Delai,"%02u:%02u:%02u:%02u"
		,pl_InfoTemps->tm_mday
		,pl_InfoTemps->tm_hour
		,pl_InfoTemps->tm_min
		,pl_InfoTemps->tm_sec );

	return (XDC_OK);

}	/* fin XZSM_14ConversionHorodateSecDelai */ 




/* Fichier : @(#)xzsm.c	1.23        Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Cette fonction recupere le nombre de secondes a une
*	heure precise 
*
******************************************************
* SEQUENCE D'APPEL :
*/

int XZSM_16ConvHeureNbSec(
	XZSMT_Date  	va_Date,
	XZSMT_Heure	va_Heure,
	double 		* pa_NbSec)

/*
* PARAMETRES EN ENTREE : aucun
*	va_Date	:	 Date au Format JJ/MM/AAAA
*	va_Heure: 	Heure au Format HH:MM:SS
*
* PARAMETRES EN SORTIE :
*       - pa_NbSec 	Nombre de Secondes
*
* VALEUR(S) DE RETOUR :
*       - XDC_OK : si l'heure a put etre recuperee
*       - XDC_NOK : sinon
*
* CONDITION D'ERREUR : aucune
*
* CONDITIONS LIMITES : aucune
*
******************************************************/
{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_16ConvHeureNbSec ";
	int             vl_ValRet	= XDC_OK;
	XZSMT_Horodate	vl_HorodateZero	= "";
	XZSMT_Horodate	vl_Horodate	= "";
	double		vl_HorodateSec	= 0.0;
	int		vl_JourSemaine	= 0;
	double		vl_NbSec	= 0.0;
	XZSMT_Date      vl_Date		= "";


	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSM_16ConvHeureNbSec : IN \
	-- Date %s -- Heure %s \n",va_Date,va_Heure);

	/*A Teste si demande la date est precise. */
	if (strlen(va_Date) == 0)
	{
		/*B Recuperation de la Date du Jour */
		vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,
				&vl_JourSemaine,&vl_HorodateSec);
		vl_ValRet = XZSM_11ConversionHorodate(
				vl_HorodateSec, &vl_HorodateZero);
		/*B Recuperation de la Date */
		sscanf(vl_HorodateZero,"%s",vl_Date);
	
	}

	/*B Construction de l'Horodate a Zero Heure **/
	sprintf(vl_Horodate,"%s %s",vl_Date,va_Heure);

	/*B Conversion de l'Horodate en Nombre de Secondes */
	vl_ValRet =  XZSM_13ConversionHorodateSec(vl_Horodate,&vl_NbSec);
	if (vl_ValRet != XDC_OK)
	{
		
		XZST_03EcritureTrace(XZSTC_WARNING, 
			"Pb recuperation du Nb Secondes \n");
		vl_ValRet = XDC_NOK;
	}

	/*A Affectation du Nombre de Secondes */
	*pa_NbSec = vl_NbSec;

	XZST_03EcritureTrace(XZSTC_INTERFACE,"XZSM_16ConvHeureNbSec : OUT \
	-- Code Retour %d -- NB Sec %lf \n",vl_ValRet,*pa_NbSec);

		return(vl_ValRet);

}	/* XZSM_16ConvHeureNbSec */



/* Fichier : @(#)xzsm.c	1.23      Release : 1.23        Date : 09/21/95
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSM 
******************************************************
* ALGORITHME :
*
* Arguments
* HorodateSec : Horodate en secondes.
*
* Fonction
* - Appel primitive XZSM_07 avec XZSMC_TYPE_SYSTEME
*   (HorodateSysteme).
* - Calcul du DeltaHorodate = HorodateSec - 
*   HorodateSysteme.
* - Memorisation du DeltaHorodate dans la variable
*   XZSMV_DeltaHorodate.
*
* Codes retour
* XZSMC_INI_INV		Initialisation non effectuee.
* XDC_OK		ok.
*
* Mecanismes
* Diffusion de l'horodate a travers le reseau par le
* datagroup "_time", et disponibilite de l'horodate 
* sur toutes les taches.
*
*  -------------------------------------------
* Conditions Utilisations MIGRAZUR
*	Pour MIGRAZUR, on n'utilise pas un Delta pour
*	le calcul de l'heure system. En fait seuls les
* 	taches TSHORL ont l'heure systeme et surtout ont
*	modifiees cette heure sur leur calculateur.
*	Donc dans la fonction XZSM_10ActualiserSyste, seule
*	l'option concernant le mode XZSMC_SYSTE est utilise.
*   -------------------------------------------
*
*******************************************************/

int	XZSM_10ActualiserDeltaHorodateU( double	va_Horodate )

{
	static char *version="@(#)xzsm.c	1.23 09/21/95 XZSM_10ActualiserDeltaHorodate ";
	int		vl_Jour_De_La_Semaine ;
	double		vl_Horodate ;
	time_t 		vl_HeureSystem;


	/* Affichage trace */
	XZST_03EcritureTrace(XZSTC_FONCTION,
			"XZSM_10ActualiserDeltaHorodate(%f)",va_Horodate) ;

/*  Test suivant le mode d'emission */
if ( 	(XZSMV_Mode == XZSMC_RTDAQ) || 
	(XZSMV_Mode == XZSMC_NSUPV) )
{

	/* demande de l'horodate systeme */
	if (XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,
		&vl_Jour_De_La_Semaine,&vl_Horodate) == XZSMC_OK)
	{
   		/* deduit le delta */
   		XZSMV_DeltaHorodate = va_Horodate - vl_Horodate ;
	}

	else 
		XZST_03EcritureTrace(XZSTC_WARNING,
			"XZSM_10ActualiserDeltaHorodate:XZST_03") ;

	XZST_03EcritureTrace(XZSTC_FONCTION,
		"XZSM_10ActualiserDeltaHorodate DELTA = (%f)",
		XZSMV_DeltaHorodate) ;

}
else if (XZSMV_Mode == XZSMC_SYSTE) /** MIGRAZUR */
{
	/* Mise a jour de l'heure systeme */
	vl_HeureSystem = (time_t )  va_Horodate;
#ifdef _HPUX_SOURCE
	if ( stime( &vl_HeureSystem) )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
            	    "MAJ heure systeme a echouee : Erreur %d ", errno );

		return(XDC_NOK);
	}
#else
        /*on passe par une commande date pour pallier au pb de sticky bit*/
        /*date MMDDhhmm*/
        time_t    vl_Temps = (time_t)va_Horodate;
        struct tm *             pl_InfoTemps = NULL;
        char vl_cmd[30];

        // on supose qu'il faut pas tenir compte SAE_191
/*        char *tz;
        tz = getenv("TZ");
        setenv("TZ", "", 1);
        tzset();
        pl_InfoTemps = localtime(&vl_Temps) ;
        if (tz)
            setenv("TZ", tz, 1);
        else
            unsetenv("TZ");
        tzset();
*/
//        pl_InfoTemps = localtime(&vl_Temps) ;
//        pl_InfoTemps = gmtime(&vl_Temps) ;


/* Modifs RGR */
/* On recupere l'indicateur heure d'ete et on applique pour avoir du TZ */
pl_InfoTemps = localtime(&vl_Temps);
vl_Temps = vl_Temps - 3600 - 3600 * pl_InfoTemps->tm_isdst;

/* On remet a jour la struct tm */
pl_InfoTemps = localtime(&vl_Temps);


        pl_InfoTemps->tm_year = pl_InfoTemps->tm_year + 1900;
        sprintf(vl_cmd,"tscmd.x \"date %02u%02u%02u%02u%04u.%02u\""
                ,(pl_InfoTemps->tm_mon +1)
                ,pl_InfoTemps->tm_mday
                ,pl_InfoTemps->tm_hour
                ,pl_InfoTemps->tm_min
                ,pl_InfoTemps->tm_year
                ,pl_InfoTemps->tm_sec);
        system (vl_cmd);
         XZST_03EcritureTrace(XZSTC_DEBUG1,"CMD %s",vl_cmd);

#endif

}

	/* Retour OK */
	return(XDC_OK);


} 	/* Fin XZSM_10ActualiserDeltaHorodate */
