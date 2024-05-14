/*
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TEWSP * FICHIER ewsp_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* C'est le module directeur de la tache TEWSP
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG		18/8/16	1.1 creation
* JMG		12/06/19 : DEM1338 1.2 ODI-C
* LCL   	06/09/19        LOT27 COCKPIT DEM1351 1.3
* ABE		26/02/20 	LOT27 COCKPIT DEM1351 1.4
* ABE		12/08/2020	Ajout taitement données 6min TDP FCD DEM-SAE152 1.5
* ABE		23/09/2020	Revert de la modification précedente (1.5) DEM-SAE152 1.6
* ABE		02/10/2020	Synchronisation toute les 7 min DEM-SAE152 1.7
* JMG		21/12/2021	Passerelle rt/ems temporaire pour tewst/tewsr DEM-SAE312
* LCL		14/01/2022	Ajout partie radio dans passerelle rt/ems DEM-SAE312
* ABE   	18/01/21    Ajout cb sur msg AVA DEM-SAE367
* ABE		04/11/2022	Suppression de l'armement du callback pour tewsr, la tache ecoute sur le bus EMS maitenant DEM-SAE380
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
* LCL		04/10/2023	Adaptation CNA DEM-483 : lecture site principal en base
* JPL		29/01/2024	Offset du reveil TDP FCD (apres que la push-gateway genere le fichier des TDP)
------------------------------------------------------*/

/* fichiers inclus */
#include "xzems.h"
#include <xzsem.h>
#include "xzsm.h"
#include "xzsc.h"
#include "ewsp_dir.h"
#include "xzao00.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "ewsp_dir.c" ;

int	vg_SockTIM=0;
int	vg_tewsx;
int	vg_deja_poste_tewst=0;
int	vg_deja_poste_tewsr=0;
XDY_Mot		vg_SitePrincipal = 0;
XDY_NomSite vg_NomSitePrincipal;

/* declaration de fonctions internes */
void ec_sortir();
void cb_tewst();
void cb_tewsr();

/* definition de fonctions externes */

void cb_tewst(va_Cnx, pa_Data, pa_Arg)
        T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{
T_INT4 va_codecmd;
T_STR va_NumTel;
T_INT2 va_NumPO;
T_STR va_nomSite;
T_STR vl_dg_dest;

TipcMsgRead(pa_Data->msg, T_IPC_FT_INT4, &va_codecmd, T_IPC_FT_STR,
                        &va_NumTel, T_IPC_FT_INT2, &va_NumPO, T_IPC_FT_STR, &va_nomSite,
			                        NULL) ;
TipcMsgGetDest(pa_Data->msg, &vl_dg_dest);
XZST_03EcritureTrace( XZSTC_WARNING, "cb_tewst : Destinataire [%s]",vl_dg_dest);

//if (!strcmp(vl_dg_dest,"SPCI2_tewsx.x"))
if (!vg_deja_poste_tewst)
{
  TipcSrvMsgWrite (  XDG_ECMD_AUTOTEL
         ,XDM_IdentMsg(XDM_ECMD_AUTOTEL2)
         ,XDM_FLG_SRVMSGWRITE
          ,T_IPC_FT_INT4,va_codecmd
           ,T_IPC_FT_STR, va_NumTel
            ,T_IPC_FT_INT2,va_NumPO
             ,T_IPC_FT_STR,va_nomSite
            ,NULL);
  vg_deja_poste_tewst=1;
}
else
  vg_deja_poste_tewst=0;
} 

void cb_tewsr(va_Cnx, pa_Data, pa_Arg)
        T_IPC_CONN va_Cnx;T_IPC_CONN_PROCESS_CB_DATA pa_Data;T_CB_ARG pa_Arg;
{
T_STR 		va_nomSite;
T_INT4 		va_typeAppel;
T_STR	 	va_numeroGroupeRadio;
T_STR		va_numeroCibleAppel;
T_INT2	 	va_NumPO;
T_STR 		vl_dg_dest;

TipcMsgRead (  pa_Data->msg,
                  T_IPC_FT_INT4, &va_typeAppel,
                  T_IPC_FT_STR,  &va_numeroGroupeRadio,
                  T_IPC_FT_STR,  &va_numeroCibleAppel,
                  T_IPC_FT_INT2, &va_NumPO,
                  T_IPC_FT_STR,  &va_nomSite,
                  NULL);

TipcMsgGetDest(pa_Data->msg, &vl_dg_dest);
XZST_03EcritureTrace( XZSTC_WARNING, "cb_tewsr : Destinataire [%s]",vl_dg_dest);

if (!vg_deja_poste_tewsr)
{
	TipcSrvMsgWrite (XDG_ECMD_AUTORAD
        ,XDM_IdentMsg(XDM_ECMD_AUTORAD)
        ,XDM_FLG_SRVMSGWRITE
        ,T_IPC_FT_INT4, va_typeAppel
        ,T_IPC_FT_STR,  va_numeroGroupeRadio
        ,T_IPC_FT_STR,  va_numeroCibleAppel
        ,T_IPC_FT_INT2, va_NumPO
        ,T_IPC_FT_STR,  va_nomSite
        ,NULL);
  vg_deja_poste_tewsr=1;
}
else
  vg_deja_poste_tewsr=0;
} 

/*X*/
/* 
------------------------------------------------------
* SERVICE RENDU : 
* Programme principal
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void main( argc, argv )

/*
* ARGUMENTS EN ENTREE :
*   argc : Nombre d'arguments
*   argv : liste des arguments
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*  
* Initialisations :connexion rtserver, connection avec les taches TESRV
* TETIM, init des structures en memoire, gestion des callbacks
* init des timer cycliques
* Boucle sur les sockets et gestion des messages recus 
* 
------------------------------------------------------*/

int	argc;
char	**argv;
{
	int       vl_IndPre       = 0;
	int       vl_Attente      = 0;
	int       vl_cr   = XDC_OK;
	T_IPC_MSG pl_Msg;

	int 	vl_ValRet	=XDC_OK,          	      	
			vl_SockRet	= 0,
			vl_SockSRV	= 0,
			vl_resultat;
   	XDY_Datagroup		pl_NomDG;
	int			vl_indice_callback, vl_Type = 0;
	int                     vl_JourSemaine;
	double                  vl_HorodateSec;
	int                  vl_HorodateTemp = 0;
	int                  vl_HorodateReveil = 0;
	XZSCT_NomTache	vl_NomTache="";
	

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	 * Connexion au RTserver et initialisation des variables
	 */
	if( XZSC_01InitCnxSup(   argv[1],
				argv[ 0 ],
				XZSCC_INTERDIRE,
				XZSCC_AUTORISER,
				XZSCC_INTERDIRE,
				ec_sortir,
				"/dev/null" ) != XDC_OK )
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_FATAL, "Ne peut ouvrir la connexion avec le RTserver." );
	   exit( XDC_NOK );
	}
	else
	{
	   /*B Affichage Trace */
	   XZST_03EcritureTrace( XZSTC_DEBUG1, "Ouverture de la connexion avec le RTserver." );
	}

	XZSC_07NomMachine(vg_NomMachine);
	vg_NumeroSite=XZSC_22NumeroSiteLocal();
	XZSC_06NomTache(vl_NomTache);
	vg_tewsx=0;

	/*A
	 * Recuperation du site de la machine 
	 */
	if ( (vl_ValRet = XZSC_04NomSite(vg_NomSite)) != XDC_OK)
	{
	   /* Ecriture Trace */
           XZST_03EcritureTrace( XZSTC_FATAL, "Recuperation du Nom du site impossible . " );
	} 

	XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1,XZSTC_NECRAN);

	/*A
	 * Connexion a la base de donnees
	 */
	if ( ( vl_ValRet = XZAG02_Connexion_Base( vg_NomSite ) ) != XDC_OK )
	{
	   XZST_03EcritureTrace( XZSTC_WARNING, "Impossible de se connecter a la base erreur : %d.", vl_ValRet );
	   return;
	}

	/* Les deux datagroups suivants ne sont utiles pour tewsp mais ne pas les enlever - utilise pour tewsx/gateway/pilotage radio/telephone */
	if (! XDG_JoindreDG(XDG_ECMD_AUTOTEL) ) 
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,"tewsx : Abonnement sur %s non effectue. \n " ,XDG_ECMD_AUTOTEL);
		return;
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_INFO,"tewsx : Abonnement sur %s effectue. \n " ,XDG_ECMD_AUTOTEL);
	}

	if (! XDG_JoindreDG(XDG_ECMD_AUTORAD) ) 
	{
		/* Ecriture Trace */
		XZST_03EcritureTrace( XZSTC_WARNING,"tewsx : Abonnement sur %s non effectue. \n " ,XDG_ECMD_AUTORAD);
		return;
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_INFO,"tewsx : Abonnement sur %s effectue. \n " ,XDG_ECMD_AUTORAD);
	}

	/* Lecture site principal en base */
	if (XZAO00_SitePrincipal(&vg_SitePrincipal,vg_NomSitePrincipal) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL,"%s : Site principal non defini en base RES_DIS.",version );
		return XDC_NOK;
	}


	if (!strcmp(vl_NomTache,"tewsx.x"))
		vg_tewsx=1;

	if (vg_tewsx==1)
	{
		/*
		* Armement du Callback pour XDM_ECMD_AUTOTEL2
		*/
/*		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_AUTOTEL2),
				(T_IPC_CONN_PROCESS_CB_FUNC) cb_tewst,
					(T_CB_ARG) NULL) == NULL)
		{
				XZST_03EcritureTrace(XZSTC_FATAL,"probleme lors de la creation du callback de reception de XDM_ECMD_AUTOTEL2");
					exit (1);
		}
*/
		/*
		* Armement du Callback pour XDM_ECMD_AUTORAD
		*/
		/* DEM-SAE 380 migration radio, la tache �coute maintenant sur le bus EMS plus besoin de convertir EMS - rtserver */
		/*if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ECMD_AUTORAD),
				(T_IPC_CONN_PROCESS_CB_FUNC) cb_tewsr,
				(T_CB_ARG) NULL) == NULL)
		{
				XZST_03EcritureTrace(XZSTC_FATAL,"probleme lors de la creation du callback de reception de XDM_ECMD_AUTORAD");
				exit(1);
		}*/
} 
	/*A
	 * Demande des timers cycliques
	 */
	
	if (vg_tewsx==0) 
	{
		/* Programation timer 1mn */
		XZSM_00InitServices();
		XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME , &vl_JourSemaine, &vl_HorodateSec);		
		if (! XDG_JoindreDG(XDG_TIMER_POLYVOX) ) {
			/* Ecriture Trace */
				XZST_03EcritureTrace( XZSTC_WARNING,
					"IN : ewsp_dir : Abonnement sur %s non effectuee. \n " ,XDG_TIMER_POLYVOX);
					return;
			}
		
		if (XZSM_01DemanderProgrammation(       vl_HorodateSec,
				EWSPC_REVEIL,
				XDG_TIMER_POLYVOX,
				XDM_AS_TIMER_POLYVOX,
				"polyvox_timer_60s",
				XZSMC_SEC_VRAI) !=  XDC_OK ) {
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : pb XZSM_01DemanderProgrammation");
			return;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : XZSM_01DemanderProgrammation : armement timer POLYVOX effectue");


		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_POLYVOX),
					ewsp_cyc_60,(T_CB_ARG)NULL) == NULL)  {
		XZST_03EcritureTrace( XZSTC_FATAL,
				"IN : Callback sur XDM_AS_TIMER_POLYVOX non cree.");
				return ;
			}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : TipcSrvProcessCbCreate : callback timer POLYVOX effectue");

		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ODI_C),
					ewsp_odi_c,(T_CB_ARG)NULL) == NULL)  {
		XZST_03EcritureTrace( XZSTC_FATAL,
				"IN : Callback sur XDM_ODI_C non cree.");
				return ;
			}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : TipcSrvProcessCbCreate : callback ODI-C effectue");
		
		/* Armemment time TDP_FCD 6 min */

		// Horodate courante entiere, en secondes
		vl_HorodateTemp=(int)vl_HorodateSec;

		// derniere horodate multiple de 6 minutes
		vl_HorodateReveil = vl_HorodateTemp - (vl_HorodateTemp % 360);

		// On declenchera au multiple de 6 minutes + offset en secondes
		// si c'est trop tot par rapport a la date courante alors 6 minutes plus tard
		vl_HorodateReveil += 125;
		if (vl_HorodateReveil < vl_HorodateTemp + 5) {
			vl_HorodateReveil += 360;
		}

		// Programmer le premier reveil et la periode
		vl_HorodateSec = (double) vl_HorodateReveil;
		XZST_03EcritureTrace( XZSTC_WARNING, "IN : debut du timer TDP dans %d sec", vl_HorodateReveil - vl_HorodateTemp);


		if (XZSM_01DemanderProgrammation(vl_HorodateSec,
				EWSP_TDP_FCD_REVEIL,
				XDG_TIMER_POLYVOX,
				XDM_AS_TIMER_TDP_FCD,		
				"TDP_FCD",
				XZSMC_SEC_VRAI) !=  XDC_OK ) {
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : pb XZSM_01DemanderProgrammation");
			return;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : XZSM_01DemanderProgrammation : armement timer TDP FCD effectue");


		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TIMER_TDP_FCD),
					ewsp_fcd,(T_CB_ARG)NULL) == NULL)  
		{
			XZST_03EcritureTrace( XZSTC_FATAL,"IN : Callback sur XDM_AS_TIMER_TDP_FCD non cree.");
			return ;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : TipcSrvProcessCbCreate : callback timer TDP FCD effectue");

		/* Callback pour AVA */
		if (! XDG_JoindreDG(XDG_API_GW_AVA) ) {
		/* Ecriture Trace */
			XZST_03EcritureTrace( XZSTC_WARNING,"IN : ewsp_dir : Abonnement sur %s non effectuee. \n " ,XDG_API_GW_AVA);
			return;
		}

		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_GTW_AVA_MODIFICATION),
									ewsp_ava_cb_modification,(T_CB_ARG)NULL) == NULL)  
		{
			XZST_03EcritureTrace( XZSTC_FATAL,"IN : Callback sur XDM_GTW_AVA_MODIFICATION non cree.");
			return ;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : TipcSrvProcessCbCreate : callback XDM_GTW_AVA_MODIFICATION effectue");

		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_GTW_AVA_CREATION),
									ewsp_ava_cb_creation,(T_CB_ARG)NULL) == NULL)  
		{
			XZST_03EcritureTrace( XZSTC_FATAL,"IN : Callback sur XDM_GTW_AVA_CREATION non cree.");
			return ;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : TipcSrvProcessCbCreate : callback XDM_GTW_AVA_CREATION effectue");

		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_GTW_AVA_RETOUR_ALERTE),
									ewsp_ava_cb_retour_alerte,(T_CB_ARG)NULL) == NULL)  
		{
			XZST_03EcritureTrace( XZSTC_FATAL,"IN : Callback sur XDM_GTW_AVA_RETOUR_ALERTE non cree.");
			return ;
		}
		else
			XZST_03EcritureTrace( XZSTC_WARNING, "IN : TipcSrvProcessCbCreate : callback XDM_GTW_AVA_RETOUR_ALERTE effectue");
    }
	/*A Attente des messages sur les sockets (RTserver, d'ecoute et de donnees */
	
	while (1)
	{
                /* Recuperation du msg RTworks suivant */
                pl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

                if(isMessageNotNULL(pl_Msg))
                {

                        TipcSrvMsgProcess(pl_Msg);
                        TipcMsgDestroy(pl_Msg);

                        /* Memorisation Test Presence RTserver */
                        vl_IndPre = 0;
                }
                /* erreur RTWORKS */
                else
                {
                        /* Test de Presence RTserver */
                        if (vl_IndPre == 0)
                        {
							if (errno)
                                XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
                                        TutErrNumGet(), errno);
                        }
                        else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
                        {
                                XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tewsp %d \n",vl_IndPre);
                        }

                        /*B INdication Non Presence RTserver */
						if (errno)
                        	vl_IndPre++;
  
		}
	}   /* fin while */
	



}  /* Fin main */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Effectue les operation de terminaison de la tache
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void    ec_sortir       (  )

/*
* ARGUMENTS EN ENTREE :
*   aucun
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR :
*   aucun
*
* CONDITION D'UTILISATION
*
* FONCTION
*   Liberation des ressources memoires
*   Envoyer le message aux taches protocoles
*   Attendre x secondes
*   Fermeture des sockets
*
------------------------------------------------------*/
{

/*A Deconnexion de la base de donnees */
XZAG03_Deconnexion_Base();

ewsp_ava_freemem();

exit(0);
}
