/*E*/
/*Fichier : @(#)acli_dir.c	1.42      Release : 1.42        Date : 05/29/12
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE CLI * FICHIER acli_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	Module Directeur de la tache TACLI
*
*	TACLI est une tache qui attend les messages RTWks (Sur DGs de TA,..)
*	et qui declanche des Open clients . Elle permet aux equipements de repondre
*	a la base (par exemple).
*
***************
*	void acli_sortir() permet de sortir proprement de TACLI
***************
*	void acli_init_Callback() initialise les callbacks sur les messages RTWks
***************
*	void acli_Joint_DGs(XDY_NomSite) permet de s abonner aux DGs
*	necessaires a TACLI
***************
*  	int acli_init_timer : Fonction permettant d initialiser les timers 
*  de la tache TACLI.
***************
* 	int acli_init_Globales( ) : Permet d initialiser les globales de TACLI
*	a son lancement.
***************
------------------------------------------------------
* HISTORIQUE :
*
* mercier	04 Oct 1994	: Creation
**********************
* mercier	02 Nov 1994	: Ajout 		V 1.2
*	d abonnement au data group XDG_A_TIMER_RAFRAI_IHM
**********************
* mercier	03 Nov 1994	: Ajout 		V 1.3
*	d abonnement au data group XDG_A_IHM_nomsite
**********************
* gaborit	29 dec 1994	: Ajout 		V 1.4
*	d abonnement au data group XDG_AA_NomSite
**********************
* gaborit	06 jan 1995	: Ajout 		V 1.5
*	d abonnement au data group XDG_AM_NomSite
**********************
* mercier       06 Jan 1995     : Modif : manque else   V 1.6
**********************
* gaborit	11 jan 1995	: Ajout 		V 1.7
*	d abonnement au data group XDG_AH_INITLT et XDG_AH_RADT_DAI
* gaborit	14 jan 1995	: Ajout inrets et bouchon		V 1.8								
**********************
* mercier	06 fev 1995	: Decoupe artificielle du code		V 1.9
* en fonction du nom de la tache.
**********************
* torregrossa	21 fev 1995	: Ajout declenchement sur XDM_AT_INI_EQT
*                                 et sur XDM_AT_INI_IHM	V 1.10
* gaborit	24 mai 1995	: Modif timers a heure fixe (si heure depassee
				  on repousse au lendemain (1.11)
* gaborit       20 jun 1995	: ajout du test vg_equipements pour joindre datagroup XDG_AT (1.12)
* T.Milleville  11 Jul 1995	: Ajout appel au module d'interface XZSM_00InitServices
*	Ceci pour permettre une memorisation du Timer de tacli.x V1.13
* T.Milleville  11 Jul 1995	: Modification argument XZSM_O1 pour tenir compte de
*	la priorite V1.14
* T.Milleville  11 Jul 1995	: Modification argument XZSM_O1 pour tenir compte de
*	des delais de Timer V1.15
* T.Milleville  12 Jul 1995	: Modification de la boucle de recuperation
*	des msg RTworks V1.16
* torregrossa	10 Aou 1995	: Decalage des purges de bases (delta 30mn) V 1.17
* torregrossa	11 Aou 1995	: Suppression du Decalage des purges historique V 1.18
* gaborit       13 sep 1995	: modif init timer ihm (1.19)
* niepceron     26 Aou 1996	: modif pour les pc simplifies (1.20)
* torregrossa	11 Aou 1995	: Ajout des decalage des purges d'exploitations pour DC et DS (RADT V1.21)
* niepceron     26 Aou 1996	: Ajout de l'acquisition des donnees pesage (eqt10) (1.22)
* niepceron     14 Oct 1996	: passage des travaux en cours au pcs uniquement pour tacli.x (radt) v1.23 1.24
* niepceron     12 Nov 1996	: Ajout de l'archivage des donnees Meteo (dem/1234) v1.25
* niepceron     04 Fev 1997	: Ajout TDP (dem/) v1.26
* Guilhou	04 nov 1997	: ajout XDG_AS_EVT_STRADA (dem/strada) 1.27
* Torregrossa	29 Jan 1998	: Ajout declenchement XDM_AT_PAL (1545) 1.28
* niepceron     05 Mai 1998	: Ajout de axtimer_FMC_locales_cb (1.29)
* niepceron	01 Jul 1998	: Separation EXP/HIS (1.30)
* niepceron     19 Oct 1998	: Suppression Satir dem/1700 1.31
* JMG		16/11/04	: pilotage des PAL en automatique 1.32
* niepceron	12 Jul 2005	: pilotage PAL uniquement pour tacli et non tapur 1.33
* JMG		11/10/05	: ajout PMVA BAF BAD 1.35
* JMG		14/11/05	: pilotage des NAV en auto 1.36
* PNI		26/10/06	: pilotage des PMV,PMVA,BAF,BAD,TUNNEL en auto 1.37 DEM584
* PNI		17/09/07	: suppression traitement pour FMC Locales v1.38
* AAZ       18/09/07    : GTC r�nov�e DEM 663 V1.39
* JMG           02/10/09        : secto DEM887  v1.40
* JMG		21/03/11 : 	jaout callback sur XDM_AA_alerte_localisee v1.41
* LCL		28/03/12 	: ajout callback et datagroup XDM_AA_COMMU_FMC (DEM/1015) v1.42
* VR		12/12/11	: Ajout PRV v1.42 (DEM/1016)
* JMG		26/04/16 : ajout SAGA 1.44
* LCL		28/02/18	: Ajout controleurs de feux CTRL_FEUX 1.45 DEM1284
* JMG		02/09/19	: gestio TACLI_XX 1.46
* LCL		16/09/19	: AJout alerte COCPKIT LOT27 DEM1351 1.47 
* ???		????????	: planning purge
* GR            30/07/2020      : TDP_FCD DEM-SAE152 1.48
* ABE		24/09/2020	: Modification pour appel acli_xzao960_cb DEM-SAE152 1.49
* JMG		05/10/2020	: PRA pour synos DEM-SAE93 1.50
* ABE		17/12/2020	: Ajout Callback IMU DEM-SAE155 1.51
* PNI		01/04/2021	: Correctif de l'appel de abt keepalibe SAE-259 V1.52
* ABE		06/04/2021	: Ajout Callback RAU SONO DEM-SAE244 V1.53
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include "acli_dir.h"
#include "xzems.h"
#include <xzsem.h>

/* declaration de variables locales */

static char *	version = "@(#)acli_dir.c	1.42 05/29/12 : acli_dir" ;

int	vg_PRA		= XDC_NOK;
static int	vg_Anim_IHM		= XDC_NOK;
static int	vg_Anim_IHM_bloc	= XDC_NOK;
static int	vg_Travaux		= XDC_NOK;
static int	vg_Alarmes		= XDC_NOK;
static int	vg_Alertes		= XDC_NOK;
static int	vg_Mesures		= XDC_NOK;
static int	vg_Mesureshis		= XDC_NOK;
static int	vg_Meteo		= XDC_NOK;
static int	vg_Meteohis		= XDC_NOK;
static int	vg_Equipmt		= XDC_NOK;
static int	vg_courbe		= XDC_NOK;
static int	vg_Purge_Histo		= XDC_NOK;
static int	vg_Purge_Exploit	= XDC_NOK;
static int	vg_Purge_Annuaire	= XDC_NOK;
static int	vg_Anim_Strada		= XDC_NOK;
static int      vg_courbehis		= XDC_NOK;


static int  	vm_TypeMachine=0;

/* declaration de fonctions internes */

static int  	acli_Joint_DGs(XDY_NomSite);
static int  	acli_init_Callback();
static int  	acli_init_Globales();
static int 	acli_init_timer( );

/* definition de fonctions externes */

/*E
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
*   Connexion au RTServer, mise en place des callbacks
*   
*
*     Attente des messages RTserver
*
*
------------------------------------------------------*/
int	argc;
char	**argv;
{
	static char *version = "@(#)acli_dir.c	1.42 05/29/12 : main" ;
	time_t	 		vl_Date;
	
	struct tm		*tl_Tm;
	
	int            	      	vl_ValRet = XDC_OK;
   	XDY_NomSite        	pl_NomSite;
   	/*XZSCT_NomMachine      pl_NomMachine;
	int                  	pl_TypeMachine;*/
	XZSCT_Datagroup         vl_NomTacheHorl;
	T_IPC_MSG              	pl_Msg ;
	int 			vl_ModeEmis;
	int 			vl_Inter;
	int 			vl_IndPre = 0;
	int 			vl_Attente = 0;
	T_INT4			vl_SockRT	= 0;
	char 			*vl_ch;
	XDY_NomTache		vl_tache;
	int			vl_InterrogationModePRA = 0;


	XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : %s Demarrage *********************",version);
	XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_WARNING|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION|XZSTC_DEBUG1,XZSTC_NECRAN);
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	** Connexion au RTserver et initialisation des variables internes (Supervision)
	*/
	if( XZSC_01InitCnxSup(  argv[ 1 ],argv[ 0 ],
				XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,acli_sortir,
				"/dev/null" ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{	
		XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : Ouverture de la connexion avec le RTserver.");
	}
	XZST_03EcritureTrace(XZSTC_INTERFACE,"IN : main TACLI: %s",version);

	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TACLI : Recuperation du Nom du site impossible .");
		exit(1);
	} 
	else
	{	
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Recuperation du Nom du site %s.",pl_NomSite);
		strcpy(vg_NomSite,pl_NomSite);
	}
	
	XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_WARNING|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION|XZSTC_DEBUG1,XZSTC_NECRAN);
	/*A
	** Recuperation du type de la machine 
	*/
	if ( (vl_ValRet =XZSC_05TypeMachine(&vm_TypeMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TACLI : Recuperation du type machine impossible .");
		exit(1);
	} 
	else
	{	
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Recuperation du type machine %d.",vm_TypeMachine);
	}

        /* TACLI_XX*/
        if ((vl_ch=strchr(argv[ 0 ], '_')) != NULL) {
                strncpy(pl_NomSite, vl_ch+1,2);
		strcpy(vg_NomSite,pl_NomSite);
                XZST_03EcritureTrace( XZSTC_WARNING, "SITE FORCE : %s",pl_NomSite);
		vg_PRA=XDC_OK;
		strncpy(vl_tache,argv[ 0 ],5);
		strcat(vl_tache,".x");
        }
	else
	{
		strcpy(vl_tache, argv[ 0 ]);
                XZST_03EcritureTrace( XZSTC_WARNING, "tacli en mode nominal  : |%s|",vl_tache);

		/* On va interroger la base apr�s que la connexion soit ouverte pour savoir si tacli fonctionne en mode PRA */
		vl_InterrogationModePRA = 1;
	}


	/*A
	** Initialisation des variables globales permettant en fonction du nom de la tache
	** de differencier les abonnements.
	*/
	if ( (vl_ValRet =acli_init_Globales(vl_tache)) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "TACLI : Probleme d initialisation des variables globales de TACLI");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TACLI : Initialisation des variables globales a reussi");
	}
	
	/*A
	** Connexion a la Base de donnees (SQL Serveur).
	*/
	if ( vm_TypeMachine == XZSCC_TYPEM_HIS )
	{
		if ( (vl_ValRet = XZAG02_Connexion_Base("HIS")) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_FATAL, "TACLI : Connexion au serveur SQL de HIS impossible .");
			exit(1);
		} 
		else
		{
			XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Connexion au serveur SQL de HIS reussie.");
		}
	}
	else
	{
		if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
		{
			XZST_03EcritureTrace( XZSTC_FATAL, "TACLI : Connexion au serveur SQL de %s impossible .",pl_NomSite);
			exit(1);
		} 
		else
		{
			XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Connexion au serveur SQL de %s reussie.",pl_NomSite);
		}
	}

	if (1==vl_InterrogationModePRA)
	{
		/* tacli.x en mode PRA */
		if ( (vl_ValRet = XZAO9999_ModePRA(pl_NomSite, &vg_PRA)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING, "TACLI : Probleme - Impossible de d�terminer le mode PRA. Mode nominal par defaut");
			vg_PRA = XDC_NOK;
		}
	}

	/*A
	** Abonnement aux data groups. 
	*/
	if (acli_Joint_DGs(pl_NomSite)!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TACLI :  impossible de joindre les Data Groups." );		
		exit(1);
	} 
	else
        {	
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Connexion aux Data Groups reussie");
	} 
	
	/*A
	** Initialisation des callbacks. 
	*/

	if (acli_init_Callback()!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TACLI :  impossible d enregistrer les Callbacks." );		
		exit(1);
	} 
	else
        {	
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Enregistrement des Callbacks reussi");
	}             
	
	/*A
	** Initialisation des timers declenchant des Callbacks cycliques. 
	*/
	if (acli_init_timer()!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TACLI :  impossible de programmer les timers sur callback." );		
		exit(1);
	} 
	else
        {
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Enregistrement des timers reussi");
	}    

	if ( vm_TypeMachine == XZSCC_TYPEM_HIS)
	{
		XZSC_31AbtKeepAlive(XDC_NOM_SITE_CI);

		XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : Abonnement KeepAlive");
	}

	/*A
	** Attente bloquante sur les messages du RTserver.
	*/
	while(1)
	{
//		XZST_03EcritureTrace (XZSTC_DEBUG1, "Attente infinie d'un message");

		errno=0;
		/* attente infinie d'un message */
		pl_Msg = TipcSrvMsgNext (T_TIMEOUT_FOREVER);


		/* si message recu */
		if(isMessageNotNULL(pl_Msg))
		{
			/* execution du call-back associe au message et destruction du message */
			TipcSrvMsgProcess(pl_Msg);
			TipcMsgDestroy(pl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
		}
		else
		/* erreur RTWORKS */
		{
#ifndef _TIBCO_EMS
			/*B Attente aleatoire pour permettre une reconnexion */
			vl_Attente = rand()%15;
			/* Test verification Socket */
			if (!TipcSrvCheck (T_IO_CHECK_READ, (double)vl_Attente))
			{	
				XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur TipcSrvCheck %d", TutErrNumGet());
				if (!TipcSrvGetSocket(&vl_SockRT))
				{
					XZST_03EcritureTrace ( XZSTC_WARNING, "Erreur TipcSrvGetSocket %d \n", TutErrNumGet());
				}
				else
				{
					/* Fermeture connexion avec RTserver */
					shutdown(vl_SockRT,2);
				}
			}

			/* Test de Presence RTserver */
			if (vl_IndPre == 0)
			{	
				if (errno && TutErrNumGet())
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
							TutErrNumGet(), errno);
			}
			else if (vl_IndPre >= 20)
			{
				XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tacli %d \n",vl_IndPre);
				/*A Arret de la tache */
				exit(0);
			}

			/*B INdication Non Presence RTserver */
			if (errno && TutErrNumGet()) /* AJOUT LCL */
				vl_IndPre++;
#endif
		}	
	}


}  /* Fin main */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de sortir de la tache TACLI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void acli_sortir( )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	XDC_NOK
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   puis annulation de la programmation timer(XZSM_02)
*   puis fermeture du fichier de trace (XZST_02)
*
------------------------------------------------------*/
{
	static char *version = "@(#)acli_dir.c	1.42 05/29/12 : acli_sortir" ;
	int             vl_ValRet = XDC_OK;
	XDY_NomSite     pl_NomSite;
	XDY_Datagroup   vl_datagroup;

	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN %s",version);
	/*A
        ** Recuperation du site de la machine.
        */
	strcpy(pl_NomSite, vg_NomSite);
        if (vg_Anim_IHM == XDC_OK)
        {
        	/*A
       	 	** Si la tache contient timer d annimation IHM :
       	 	** Annulation du TIMER de rafraichissement de l IHM dont la cle est 
       	 	** ACLIDC_TIMERCLE_XZAH10 et le Datagroup est XDG_A_TIMER_RAFRAI_IHM
        	*/
        
		/*
        	** Constitution du nom Data Group a partir du nom du site
        	*/       
        	XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAFRAI_IHM,pl_NomSite);
        	/*A
        	** Arret du timer de rafraichissement IHM
        	*/
        	if ((vl_ValRet = XZSM_02AnnulerProgrammation(ACLIDC_TIMERCLE_XZAH10,vl_datagroup, 0 ))!= XDC_OK)    
        	{
        	       XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : acli_sortir : Probleme annulation du TIMER Rafraichissement IHM !!\n");
        	}
        }
        if (vg_Purge_Annuaire == XDC_OK)
	{
		/*A
		** Si la tache contient timer de purge du calendrier
		** Desarme le timer de purge du calendrier des astreintes
		*/
		XDG_EncodeDG2(vl_datagroup,XDG_AN,pl_NomSite);
		if ((vl_ValRet = XZSM_02AnnulerProgrammation(XZSMC_CLEPROG_NUL,vl_datagroup, 0 ))!= XDC_OK)    
        	{
               		XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : acli_sortir : Probleme annulation de purge du calendrier des astreintes !!\n");
        	}
        }
        if (vg_Purge_Exploit == XDC_OK)
        {
        	/*A
		** Si la tache contient timer de purge de la base d exploitation,
		** Desarme le timer de purge de la base d exploitation.
		*/
		XDG_EncodeDG2(vl_datagroup,XDG_AG,pl_NomSite);
		if ((vl_ValRet = XZSM_02AnnulerProgrammation(XZSMC_CLEPROG_NUL,vl_datagroup, 0 ))!= XDC_OK)    
        	{
               		XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : acli_sortir : Probleme annulation du TIMER de purge base Exploitation !!\n");
        	}
        }
        if (vg_Purge_Histo == XDC_OK)
        {
		/*A
		** Si la tache contient timer de purge de la base Historique,
		** Si on est sur le CI, on desarme le timer de purge de la base Historique.
		*/
		if(!strcmp(pl_NomSite,XDC_NOM_SITE_CI))
		{
			/*
        		** Constitution du nom Data Group a partir du nom du site
        		*/       
        		XDG_EncodeDG2(vl_datagroup,XDG_AG,pl_NomSite);
        
        		if((vl_ValRet = XZSM_02AnnulerProgrammation(XZSMC_CLEPROG_NUL,
                        				vl_datagroup,
                        			 	0 ))!= XDC_OK)
        		{
              	 		XZST_03EcritureTrace( XZSTC_WARNING, "TACLI : acli_sortir : Probleme annulation du TIMER Purge de la base Historique!!\n");
       			}
		}
	}
	/*A
	** Fermeture du fichier trace 
	*/
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"TACLI : XZST_02FermetureTrace a echoue");
	}
	XZST_03EcritureTrace( XZSTC_INFO, "TACLI : Arret  *********************");
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_sortir () : Sortie");
	/*A
	** Sortie 
	*/
	exit(0);


}  /* Fin acli_sortir */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d initialiser les Callbacks de la tache TACLI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int acli_init_Callback( )

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	on appele acli_sortir pour arreter la tache.
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *version 		= "@(#)acli_dir.c	1.42 05/29/12 : acli_init_Callback" ;
	int 		vl_ValRet 	= XDC_OK;
	XDY_NomSite     pl_NomSite;
	XDY_Datagroup   vl_datagroup;
	int		vl_Resultat=0;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN :acli_init_Callback %s",version);
	
	/*A 
	** Si la tache doit contenir les armements sur les equipements :
	*/
	XZST_03EcritureTrace( XZSTC_WARNING,"Debut armements sur les equipements" );

	if ( vg_Equipmt == XDC_OK)
	{
		/*A  
		** Armement Callback sur Reception XDM_AT_TUBE 
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_TUBE),atcl22,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_TUBE non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_echangeur 
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_echangeur),atcl23,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_echangeur non cree.",version );
			acli_sortir();
		}
	
        /* AAZ 18/09/2007 ajout domaine SIG, ECL, VEN, ENR, INC */
        /*A  
        ** Armement Callback sur Reception XDM_AT_SIG 
        */
        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_SIG),atcl40,(T_CB_ARG)NULL) == NULL) 
        {   
            XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_SIG non cree.",version );
            acli_sortir();
        }
    
        /*A  
        ** Armement Callback sur Reception XDM_AT_ECL  
        */
        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_ECL),atcl41,(T_CB_ARG)NULL) == NULL) 
        {   
            XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_ECL non cree.",version );
            acli_sortir();
        }
    
        /*A  
        ** Armement Callback sur Reception XDM_AT_VEN 
        */
        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_VEN),atcl42,(T_CB_ARG)NULL) == NULL) 
        {   
            XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_VEN non cree.",version );
            acli_sortir();
        }
    
        /*A  
        ** Armement Callback sur Reception XDM_AT_ENR 
        */
        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_ENR),atcl43,(T_CB_ARG)NULL) == NULL) 
        {   
            XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_ENR non cree.",version );
            acli_sortir();
        }
    
        /*A  
        ** Armement Callback sur Reception XDM_AT_INC 
        */
        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_INC),atcl44,(T_CB_ARG)NULL) == NULL) 
        {   
            XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_INC non cree.",version );
            acli_sortir();
        }
		
        /*A  
        ** Armement Callback sur Reception XDM_AT_EXP
        */
        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_EXP),atcl45,(T_CB_ARG)NULL) == NULL) 
        {   
            XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_EXP non cree.",version );
            acli_sortir();
        }
		

        /* Fin AAZ */
	
		/*A  
		** Armement Callback sur Reception XDM_AT_NAV 
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_NAV),atcl24,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_NAV non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_BAF
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_BAF),atcl30,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_BAF non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_BAD
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_BAD),atcl31,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_BAD non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_PRV
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_PRV),atcl51,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_PRV non cree.",version );
			acli_sortir();
		}
		else
		{
					XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_PRV cree !",version);
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_PMV
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_PMV),atcl25,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_PMV non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_PMVA
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_PMVA),atcl250,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_PMVA non cree.",version );
			acli_sortir();
		}
		else
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_PMVA cree !",version);
		}
		/*A
		** Armement Callback sur Reception XDM_AT_EQT_SAGA
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_EQT_SAGA), atcl251,(T_CB_ARG)NULL) == NULL)
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_EQT_SAGA non cree.",version );
			acli_sortir();
		}
		else
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_EQT_SAGA cree !",version);
		}
		/*A
		** Armement Callback sur Reception XDM_AT_CFE
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_CFE), atcl60,(T_CB_ARG)NULL) == NULL)
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_CFE non cree.",version );
			acli_sortir();
		}
		else
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_CFE cree !",version);
		}
		/*A   

		** Armement Callback sur Reception XDM_AT_IMU 

		*/ 

		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_IMU),atcl70,(T_CB_ARG)NULL) == NULL)  

		{ 
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_IMU non cree.",version );
			acli_sortir(); 
		}
		else
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_IMU cree !",version);
		}
		/*A 
		** Armement Callback sur Reception XDM_AT_SONO 
		*/ 

		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_SONO),atcl80,(T_CB_ARG)NULL) == NULL)  

		{ 
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_SONO non cree.",version );
			acli_sortir(); 
		}
		else
		{
			XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_SONO cree !",version);
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_PAL 
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_PAL),atcl28,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_PAL non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_INI_EQT
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_INI_EQT),atcl26,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_INI_EQT non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AT_INI_IHM
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AT_INI_IHM),atcl27,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AT_INI_IHM non cree.",version );
			acli_sortir();
		}

		strcpy(pl_NomSite,vg_NomSite);
		XZST_03EcritureTrace(XZSTC_WARNING,"Nom Site = %s",pl_NomSite);
		/* On teste si on est bien sur le CI */
		if( !strcmp(pl_NomSite,XDC_NOM_SITE_CI))
		  {
		    /* Armement Callback sur Reception EMSG_TDP_FCD*/
		    if (TipcSrvProcessCbCreate (XDM_IdentMsg(EMSG_TDP_FCD), acli_xzao960_cb, (T_CB_ARG)NULL) == NULL)		
		      {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : Callback sur EMSG_TDP_FCD non cree.", version );
			acli_sortir();
		      }
		    else
		    {
			XZST_03EcritureTrace(XZSTC_WARNING,"%s :  Callback sur EMSG_TDP_FCD cree",version);
		    }
		  }
		
		XZST_03EcritureTrace( XZSTC_WARNING,"armements sur les equipements OK" );
	}
	/*A
	** Si la tache doit contenir les abonnements aux messages sur les alertes :
	*/	
	XZST_03EcritureTrace( XZSTC_WARNING,"abonnements aux messages sur les alertes" );
	
	if ( vg_Alertes == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_AA_alerte
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AA_alerte),aacl01,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AA_alerte non cree.",version );
			acli_sortir();
		}
                /*IPHONE*/
                if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AA_alerte_localisee),aacl01_localisee,(T_CB_ARG)NULL) == NULL)
                {
                        XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AA_alerte_localisee non cree.",version );
                        acli_sortir();
                }
		/*A  
		** Armement Callback sur Reception XDM_ETDP_SEUIL
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ETDP_SEUIL),aacl02,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AA_alerte non cree.",version );
			acli_sortir();
		}
		/*A
		** Armement Callback sur Reception XDM_AA_COMMU_FMC
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AA_COMMU_FMC),ahcl980,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AA_COMMU_FMC non cree.",version );
			acli_sortir();
		}
                /*COCKPIT*/
		/*A
		** Armement Callback sur Reception XDM_AA_alerte_cockpit
		*/
                if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AA_alerte_cockpit),aacl01_cockpit,(T_CB_ARG)NULL) == NULL)
                {
                        XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AA_alerte_cockpit non cree.",version );
                        acli_sortir();
                }
		else 
		{
			 XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AA_alerte_cockpit cree.",version );
		}
		
		XZST_03EcritureTrace( XZSTC_WARNING,"abonnements aux messages sur les alertes OK" );
	}
	/*A
	** Si la tache doit contenir les abonnements aux messages sur les alarmes :
	*/
	XZST_03EcritureTrace( XZSTC_WARNING,"abonnements aux messages sur les alarmes" );
	
	if ( vg_Alarmes == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_AM_alarme
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AM_alarme),amcl01,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AM_alarme non cree.",version );
			acli_sortir();
		}
		XZST_03EcritureTrace( XZSTC_WARNING,"abonnements aux messages sur les alarmes OK" );
	}
	/*A
	** Si la tache doit contenir les abonnements afin d animer les IHM :
	*/
	XZST_03EcritureTrace( XZSTC_WARNING,"abonnements afin d animer les IHM" );
	
	if ( vg_Anim_IHM == XDC_OK )
	{		
		/*A  
		** Armement Callback sur Reception XDM_A_INIT_IHM
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_A_INIT_IHM),ahcl11,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_A_INIT_IHM non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AH_INITLT
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AH_INITLT),ahcl26,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AH_INITLT non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AH_RADT_DAI
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AH_RADT_DAI),ahcl21,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_AH_RADT_DAI non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_A_TIMER_RAFRAI_IHM
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_A_TIMER_RAFRAI_IHM),axtimer_Ihm_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_A_TIMER_RAFRAI_IHM non cree.",version );
			acli_sortir();
		}
		XZST_03EcritureTrace( XZSTC_WARNING,"abonnements afin d animer les IHM OK" );
	}
	/*A
	** Si la tache doit contenir les abonnements afin d animer les IHM :
	*/
	if ( vg_Anim_IHM_bloc == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_A_TIMER_RAFRAI_IHM
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_A_TIMER_RAFRAI_IHM),axtimer_Ihm_bloc_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_A_TIMER_RAFRAI_IHM non cree.",version );
			acli_sortir();
		}
	}
	/*A
	** Si la tache doit passer les travaux en cours :
	*/
	if ( vg_Travaux == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_A_TIMER_TRV_COURS
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_A_TIMER_TRV_COURS),axtimer_trv_cours_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_A_TIMER_TRV_COURS non cree.",version );
			acli_sortir();
		}
	}
	/*A
	** Si la tache doit effectuer la purge de la base d exploitation :
	*/
	if ( vg_Purge_Exploit == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_AG_PURGE pour purge de la base d exploitation
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AG_PURGE),axtimer_XZAG05_Purge_Base_Exploitation_cb,(T_CB_ARG)NULL) == NULL) 
		{	
              	  	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AG_PURGE non cree.",version );
			acli_sortir();
		}
	}
	if ( vg_courbe == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_AS_FdeT pour envoie des donnees traffic 6 minutes.
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_FdeT),ascl06,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_FdeT non cree.",version );
			acli_sortir();
		}
	
		/*A  
		** Armement Callback sur Reception XDM_AS_INRETS pour envoie des donnees inrets.
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_INRETS),ascl02,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_INRETS non cree.",version );
			acli_sortir();
		}
	
		/*A  
		** Armement Callback sur Reception XDM_AS_BOUCHON pour envoie des donnees bouchon.
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_BOUCHON),ascl03,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_BOUCHON non cree.",version );
			acli_sortir();
		}
		/*A  
		** Armement Callback sur Reception XDM_AS_TDP pour envoie des donnees tdp 6 minutes.
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_TDP),ascl07,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_TDP non cree.",version );
			acli_sortir();
		}
	
	}
	if ( vg_courbehis == XDC_OK )
	{
		/*A  
		** Armement Callback sur Reception XDM_AS_FdeT pour envoie des donnees traffic 6 minutes.
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_FdeT),ascl06,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_FdeT non cree.",version );
			acli_sortir();
		}
		
		/*A  
		** Armement Callback sur Reception XDM_AS_BOUCHON pour envoie des donnees bouchon.
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_BOUCHON),ascl03,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_BOUCHON non cree.",version );
			acli_sortir();
		}
	
	}
	/*A
	** Armement des CB specifiques au Site CI.
	*/
	strcpy(pl_NomSite,vg_NomSite);
	/*A
	** On teste si on est bien sur le CI sinon on n arme pas le callback de purge 
	** base histo et calendrier des astreintes.
	*/
	if(!strcmp(pl_NomSite,XDC_NOM_SITE_CI))
	{
		/*A
		** Si la tache doit contenir le callback de purge de la base histo (Site CI) :
		*/
		if ( vg_Purge_Annuaire == XDC_OK )
		{
			/*A  
			** Armement Callback sur Reception XDM_AN_PURGE pour purge du calendrier 
			** des astreintes si sur site CI.
			*/
			if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AN_PURGE),axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb,(T_CB_ARG)NULL) == NULL) 
			{	
	                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AN_PURGE non cree.",version );
				acli_sortir();
			}
		}
		if ( vg_Purge_Histo == XDC_OK )
		{	
			/*A  
			** Armement Callback sur Reception XDM_AG_PURGE pour purge 
			** de la base Historique des astreintes si sur site CI.
			*/
			if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AG_PURGEHISTO),axtimer_XZAG06_Purge_Base_Historique_cb,(T_CB_ARG)NULL) == NULL) 
			{	
	                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AG_PURGEHISTO non cree." ,version);
				acli_sortir();
			}
		}
                if ( vg_Anim_Strada == XDC_OK )
                {
                        /*A
                        ** Armement Callback sur Reception XDM_AS_EVT_STRADA pour strada
                        */
                        if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AS_EVT_STRADA),ahcl99,(T_CB_ARG)NULL) == NULL)
                        {
                                XZST_03EcritureTrace( XZSTC_WARNING,"%s : CB sur XDM_AS_EVT_STRADA non cree." ,version);
                                acli_sortir();
                        }  
                }

		if ( vg_Meteohis == XDC_OK )
		{	
			/*A
			** Abonnement aux donn�es de mesures Meteo.
			*/

			if ( (vl_ValRet = XZEA34_Abt_Mesures_Meteo (	
							acli_XZEA_XZAS53_FU,
							&vl_Resultat)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA34_Abt_Mesures_Meteo a echoue.",version);
			} 
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es de mesure meteo r�ussi",version);
			}
		}
		if ( vg_Mesureshis == XDC_OK ) 
		{	
			/*A
			** Abonnement aux donn�es de mesures Meteo.
			*/

			if ( (vl_ValRet = XZEA37_Abt_TdP_1heu (	
							acli_XZEA_XZAD03_FU,
							&vl_Resultat)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA37_Abt_TdP_1heu a echoue.",version);
			} 
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es TDP 1h r�ussi",version);
			}
		}
	}
	else
	{
		if ( vg_Mesures == XDC_OK )
		{	
			/*A
			** Abonnement aux donn�es de mesures Meteo.
			*/

			if ( (vl_ValRet = XZEA36_Abt_TdP_6mn (	
							acli_XZEA_XZAD02_FU,
							pl_NomSite,
							&vl_Resultat)) != XDC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA36_Abt_TdP_6mn a echoue.",version);
			} 
			else
			{
				XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es TDP 6mn r�ussi",version);
			}
		}
		XZST_03EcritureTrace( XZSTC_INFO, "axtimer_XZAN34_Epurer_Calendrier_Astreinte_cb : NON arme sur le site  %s .",pl_NomSite);
	}
	if ( vg_Mesures == XDC_OK )
	{
		/*A
		** Abonnement aux donn�es de mesures 6mins sur voie
		*/
		XDG_EncodeDG2(vl_datagroup,XDG_AM_INIT_MES6MnsVoie,pl_NomSite);
		if ( (vl_ValRet = XZEA03_Abt_Mesures_6mn_Voie (XZEAC_DEBUT_ABONNEMENT,
							vl_datagroup,
							acli_XZEA03_XZAS10_FU,
							pl_NomSite,
							&vl_Resultat)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s: XZEA03_Abt_Mesures_6mn_Voie a echoue.",version);
		} 
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es de mesure 6mns voie r�ussi",version);
		}
		/*A
		** Abonnement aux donn�es de mesures 6mins sur la chaussee
		*/
		XDG_EncodeDG2(vl_datagroup,XDG_AM_INIT_MES6MnsCh,pl_NomSite);
		
		if ( (vl_ValRet = XZEA04_Abt_Mesures_6mn_Chaussee (XZEAC_DEBUT_ABONNEMENT,
							vl_datagroup,
							acli_XZEA04_XZAS10_FU,
							pl_NomSite,
							&vl_Resultat)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s: XZEA04_Abt_Mesures_6mn_Chaussee a echoue.",version);
		} 
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es de mesure 6mns chaussee r�ussi",version);
		}
		/*A
		** Abonnement aux donn�es de mesures poids.
		*/
		XDG_EncodeDG2(vl_datagroup,XDG_AM_INIT_MESPOIDS,pl_NomSite);
		if ( (vl_ValRet = XZEA07_Abt_Mes_Poids (	XZEAC_DEBUT_ABONNEMENT,
							vl_datagroup,
							acli_XZEA07_XZAS11_FU,
							pl_NomSite,
							&vl_Resultat)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA07_Abt_Mes_Poids a echoue.",version);
		} 
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es de mesure poids r�ussi",version);
		}
		/*A
		** Abonnement aux donn�es de niveau de service.
		*/
		XDG_EncodeDG2(vl_datagroup,XDG_AM_INIT_NivServ,pl_NomSite);
		if ( (vl_ValRet = XZEA05_Abt_Niveaux_Service (	XZEAC_DEBUT_ABONNEMENT,
							vl_datagroup,
							acli_XZEA05_XZAS12_FU,
							pl_NomSite,
							&vl_Resultat)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA05_Abt_Niveaux_Service a echoue.",version);
		} 
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es de niveau de services r�ussi",version);
		}
		/*A
		** Abonnement aux donn�es de pesage.
		*/
		if ( (vl_ValRet = XZEA33_Abt_Mesures_Pesage (	
							acli_XZEA_XZAS50_FU,
							acli_XZEA_XZAS51_FU,
							acli_XZEA_XZAS52_FU,
							pl_NomSite,
							&vl_Resultat)) != XDC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZEA33_Abt_Pesage a echoue.",version);
		} 
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,"%s : abonnement aux donn�es de pesages r�ussi",version);
		}
		


		/*A  
		** Armement Callback sur Reception XDM_EPAL_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPAL_AUTO),acli_xzac66_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPAL_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPAL_AUTO_FIN),acli_xzac67_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPAL_AUTO_FIN non cree.",version );
			acli_sortir();
		}


		/*A  
		** Armement Callback sur Reception XDM_ENAV_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ENAV_AUTO),acli_xzac03_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ENAV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ENAV_AUTO_FIN),acli_xzac62_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPAL_AUTO_FIN non cree.",version );
			acli_sortir();
		}

		/*A  
		** Armement Callback sur Reception XDM_EBAF_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EBAF_AUTO),acli_xzac92_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ENAV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EBAF_AUTO_FIN),acli_xzac98_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EBAF_AUTO_FIN non cree.",version );
			acli_sortir();
		}


		/*A  
		** Armement Callback sur Reception XDM_EPMV_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPMV_AUTO),acli_xzac01_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ENAV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPMV_AUTO_FIN),acli_xzac61_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPMV_AUTO_FIN non cree.",version );
			acli_sortir();
		}

		/*A
		** Armement Callback sur Reception XDM_EPRV_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPRV_AUTO),acli_xzac802_cb,(T_CB_ARG)NULL) == NULL)
		{
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPRV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPRV_AUTO_FIN),acli_xzac303_cb,(T_CB_ARG)NULL) == NULL)
		{
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPRV_AUTO_FIN non cree.",version );
			acli_sortir();
		}


		/*A  
		** Armement Callback sur Reception XDM_EPMVA_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPMVA_AUTO),acli_xzac82_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ENAV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EPMVA_AUTO_FIN),acli_xzac84_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EPMVA_AUTO_FIN non cree.",version );
			acli_sortir();
		}


		/*A  
		** Armement Callback sur Reception XDM_EBAD_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EBAD_AUTO),acli_xzac93_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ENAV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_EBAD_AUTO_FIN),acli_xzac99_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_EBAD_AUTO_FIN non cree.",version );
			acli_sortir();
		}


		/*A  
		** Armement Callback sur Reception XDM_ETUNNEL_AUTO
		*/
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ETUNNEL_AUTO),acli_xzac06_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ENAV_AUTO non cree.",version );
			acli_sortir();
		}
		if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_ETUNNEL_AUTO_FIN),acli_xzac63_cb,(T_CB_ARG)NULL) == NULL) 
		{	
                	XZST_03EcritureTrace( XZSTC_WARNING,"%s : Callback sur XDM_ETUNNEL_AUTO_FIN non cree.",version );
			acli_sortir();
		}

	}
	/*A  
	** Si tout est ok, retourne XDC_OK sinon on a fait appel a acli_sortir()
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT %s",version);
	return(XDC_OK);
	
}  /* Fin acli_init_Callback */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  acli_init_timer : Fonction permettant d initialiser les timers 
*  de la tache TACLI.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int acli_init_timer( )

/*
* ARGUMENTS EN ENTREE :  Aucun
* ARGUMENTS EN SORTIE :  Aucun
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	on appele acli_sortir pour arreter la tache.
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *		version 	= "@(#)acli_dir.c	1.42 05/29/12 : acli_init_timer" ;
	
	int 			vl_ValRet 	= XDC_OK;
	XDY_Horodate		vl_Nb_Secondes	= 0.0;	/* le timer est a declenchement cyclique */
	int			vl_JourSemaine=0;
	XDY_Horodate		vl_HorodateSec=0;
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
        XZSMT_Date      	vl_Date;
        XZSMT_Heure     	vl_Heure;
        
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN %s",version);

	/*A
	**	Initialisation des services necessaires au Timer 
	**/
	XZSM_00InitServices();


	/*A
        ** Recuperation du site de la machine
        */
	strcpy(pl_NomSite, vg_NomSite);

	if ( vg_Anim_IHM == XDC_OK )
	{
		/*A
        	** Constitution du nom Data Group a partir du nom du site
        	*/
        
        	XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAFRAI_IHM,pl_NomSite);
	
		/*A 
		** La CB de refraichissement IHM se declenchera 
		** dans ACLIDC_TIMERDELAI_XZAH10 secondes apres programmation
		*/
		if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_07LireHorodate a echoue");
			acli_sortir();
		}
		vl_Nb_Secondes = vl_HorodateSec ;	
		/*A  
		** Declenchement d'une programmation sur
    		** un datagroup  XDG_A_TIMER_RAFRAI_IHM_NomSite avec le message
   		** XDM_A_TIMER_RAFRAI_IHM ayant comme cle de ACLIDC_TIMERCLE_XZAH10  ********
   		*/
   	
        	if ((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				ACLIDC_TIMERDELAI_XZAH10,
                				vl_datagroup,
                				XDM_A_TIMER_RAFRAI_IHM,
                				ACLIDC_TIMERCLE_XZAH10,
                				0 ))!= XDC_OK)
   		{
     			XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER pour rafraichissement IHM \n\n ");
   			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
			acli_sortir();
   		}
   		else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER pour rafraichissement IHM reussi\n\n ");
	
		/*A
        	** rafraichissement de l'etat des voies RADT et DAI
        	** Constitution du nom Data Group a partir du nom du site
        	*/
        
       	 	XDG_EncodeDG2(vl_datagroup,XDG_AH_RADT_DAI,pl_NomSite);
	
		if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_07LireHorodate a echoue");
			acli_sortir();
		}
		vl_Nb_Secondes = vl_HorodateSec ;	
   	
        	if ((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
						ACLIDC_TIMERDELAI_AHCL21,
                				vl_datagroup,
                				XDM_AH_RADT_DAI,
                				ACLIDC_TIMERCLE_AHCL21,
                				0 ))!= XDC_OK)
   		{
     			XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER pour DAI RADT  ");
   			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
			acli_sortir();
   		}
   		else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER pour DAI_RADT reussi");
	}
	if ( vg_Anim_IHM_bloc == XDC_OK )
	{
		/*A
        	** Constitution du nom Data Group a partir du nom du site
        	*/
        
        	XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAF_IHM_BLOC, pl_NomSite);
	
		/*A 
		** La CB de refraichissement IHM se declenchera 
		** dans ACLIDC_TIMERDELAI_XZAH10 secondes apres programmation
		*/
		if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_07LireHorodate a echoue");
			acli_sortir();
		}
		vl_Nb_Secondes = vl_HorodateSec ;	
		/*A  
		** Declenchement d'une programmation sur
    		** un datagroup  XDG_A_TIMER_RAF_IHM_BLOC_NomSite avec le message
   		** XDM_A_TIMER_RAFRAI_IHM ayant comme cle de ACLIDC_TIMERCLE_XZAH10_BLOC  ********
   		*/
   	
        	if ((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				ACLIDC_TIMERDELAI_XZAH10,
                				vl_datagroup,
                				XDM_A_TIMER_RAFRAI_IHM,
                				ACLIDC_TIMERCLE_XZAH10_BLOC,
                				0 ))!= XDC_OK)
   		{
     			XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER pour rafraichissement IHM par bloc\n\n ");
   			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
			acli_sortir();
   		}
   		else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER pour rafraichissement IHM par bloc reussi\n\n ");
	}
	if ( vg_Travaux  == XDC_OK )
	{
		/*A
        	** Constitution du nom Data Group a partir du nom du site
        	*/
        
        	XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_TRV_COURS, pl_NomSite);
	
		/*A 
		** La CB de refraichissement IHM se declenchera 
		** dans ACLIDC_TIMERDELAI_TRV secondes apres programmation
		*/
		if ((vl_ValRet = XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_HorodateSec))!=XZSMC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_07LireHorodate a echoue");
			acli_sortir();
		}
		vl_Nb_Secondes = vl_HorodateSec;	
		/*A  
		** Declenchement d'une programmation sur
    		** un datagroup  XDG_A_TIMER_RAF_IHM_BLOC_NomSite avec le message
   		** XDM_A_TIMER_TRV_COURS ayant comme cle de ACLIDC_TIMERCLE_TRV  ********
   		*/
   	
        	if ((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_Nb_Secondes,
                				AXZAE102C_TIMERDELAI_XZAE102,
                				vl_datagroup,
                				XDM_A_TIMER_TRV_COURS,
                				ACLIDC_TIMERCLE_TRV,
                				0 ))!= XDC_OK)
   		{
     			XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER pour passage des travaux prevus a en cours \n\n ");
   			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
			acli_sortir();
   		}
   		else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER pour passage des travaux prevus a en cours \n\n ");
	
	}
	if ( vg_Purge_Exploit == XDC_OK ) 
	{
		/*A  
		** Declenchement d'une programmation sur
    		** un datagroup  XDG_AG_NomSite avec le message
   		** XDM_AG_PURGE ayant comme cle de ACLIDC_TIMERCLE_XZAG05  ********
   		*/
   		strcpy(vl_Date,"");
		strcpy(vl_Heure,ACLIDC_HEURE_DECLENCHEMENT_XZAG05);
		if ((vl_ValRet = XZSM_16ConvHeureNbSec(vl_Date,vl_Heure,&vl_HorodateSec))!=XZSMC_OK)
		{
			XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_16ConvHeureNbSec a echoue");
			acli_sortir();
		}

		/*A
		** Decaler la purge de ACLIDC_DELTA_PURGE_INTER_SITE minutes pour Mandelieu
		** et de 2 * ACLIDC_DELTA_PURGE_INTER_SITE minutes pour le CI
		** la purge sur DN est le premier site � etre purge
		*/
   		if (!strcmp(pl_NomSite,XDC_NOM_SITE_VC))
			vl_HorodateSec = vl_HorodateSec ;
   		else if (!strcmp(pl_NomSite,XDC_NOM_SITE_DP))
			vl_HorodateSec = vl_HorodateSec + 1.5 * ACLIDC_DELTA_PURGE_INTER_SITE;
   		else if (!strcmp(pl_NomSite,XDC_NOM_SITE_CI))
			vl_HorodateSec = vl_HorodateSec + 3 * ACLIDC_DELTA_PURGE_INTER_SITE;

		XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_Nb_Secondes);
		/* si l'heure recherchee est depassee, on repousse au lendemain */
		if (vl_Nb_Secondes>vl_HorodateSec)
			vl_HorodateSec=vl_HorodateSec+86400;

   		XDG_EncodeDG2(vl_datagroup,XDG_AG,pl_NomSite);
        	if ((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_HorodateSec,
                				0,
                				vl_datagroup,
                				XDM_AG_PURGE,
                				ACLIDC_TIMERCLE_XZAG05,
                				0 ))!= XDC_OK)
   		{
     			XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER de purge de la base d exploitation \n\n");
   			XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
			acli_sortir();
   		}
   		else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER de purge de la base d exploitation reussi\n\n ");
   	}
   	/*A
   	** Armement du timer pour le message XDM_AN_PURGE et XDM_AG_PURGEHISTO
   	** si on se trouve sur le CI.
   	*/
   	if (!strcmp(pl_NomSite,XDC_NOM_SITE_CI))
   	{
		if ( vg_Purge_Annuaire == XDC_OK )
   		{
   			/*B
        		** Constitution du nom Data Group a partir du nom du site
        		*/
        
        		XDG_EncodeDG2(vl_datagroup,XDG_AN,pl_NomSite);
	
			/*B 
			** La CB de purge du calendrier des astreintes se declenchera a ACLIDC_HEURE_DECLENCHEMENT_XZAN34 H du matin.
			*/
			strcpy(vl_Date,"");
			strcpy(vl_Heure,ACLIDC_HEURE_DECLENCHEMENT_XZAN34);
			if ((vl_ValRet = XZSM_16ConvHeureNbSec(vl_Date,vl_Heure,&vl_HorodateSec))!=XZSMC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_16ConvHeureNbSec a echoue");
				acli_sortir();
			}
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_Nb_Secondes);
			/* si l'heure recherchee est depassee, on repousse au lendemain */
			if (vl_Nb_Secondes>vl_HorodateSec)
				vl_HorodateSec=vl_HorodateSec+86400;
			/*B  
			** Declenchement d'une programmation sur
    			** un datagroup  XDG_AN_NomSite avec le message
   			** XDM_AN_PURGE ayant comme cle de ACLIDC_TIMERCLE_XZAN34 ********
   			*/
   		
       			if((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_HorodateSec,
                				0,
                				vl_datagroup,
                				XDM_AN_PURGE,
                				ACLIDC_TIMERCLE_XZAN34,
                				0 ))!= XDC_OK)
      		
   			{
     				XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER pour Purge du calendrier des astreintes \n\n ");
   				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
				acli_sortir();
   			}
   			else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER pour Purge du calendrier des astreintes\n\n ");
   		}
   		if ( vg_Purge_Histo == XDC_OK )
   		{
   			/*B
			** Constitution du nom Data Group a partir du nom du site
			*/
			
			XDG_EncodeDG2(vl_datagroup,XDG_AG,pl_NomSite);
			
			/*B 
			** La CB de purge de la base Histo se declenchera a ACLIDC_HEURE_DECLENCHEMENT_XZAG06 H du matin.
			*/
			strcpy(vl_Date,"");
			strcpy(vl_Heure,ACLIDC_HEURE_DECLENCHEMENT_XZAG06);
			if ((vl_ValRet = XZSM_16ConvHeureNbSec(vl_Date,vl_Heure,&vl_HorodateSec))!=XZSMC_OK)
			{
				XZST_03EcritureTrace(XZSTC_WARNING,"acli_init_timer : XZSM_16ConvHeureNbSec a echoue");
				acli_sortir();
			}

	
			XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_JourSemaine,&vl_Nb_Secondes);
			/* si l'heure recherchee est depassee, on repousse au lendemain */
			if (vl_Nb_Secondes>vl_HorodateSec)
				vl_HorodateSec=vl_HorodateSec+86400;
			/*B  
			** Declenchement d'une programmation sur
    			** un datagroup  XDG_AG_NomSite avec le message
   			** XDM_AG_PURGEHISTO ayant comme cle de ACLIDC_TIMERCLE_XZAG06 ********
   			*/
   		
       			if((vl_ValRet = XZSM_01DemanderProgrammation(
                				vl_HorodateSec,
                				0,
                				vl_datagroup,
                				XDM_AG_PURGEHISTO,
                				ACLIDC_TIMERCLE_XZAG06,
                				0 ))!= XDC_OK)
      		
   			{
     				XZST_03EcritureTrace(XZSTC_WARNING," Probleme de programmation TIMER pour Purge de la base HISTO \n\n ");
   				XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_init_timer : Sortie avec %d",vl_ValRet);
				acli_sortir();
   			}
   			else XZST_03EcritureTrace(XZSTC_INFO,"TACLI : Programmation TIMER pour Purge de la base HISTO \n\n ");
   		}
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT %s : Sortie avec %d",version,vl_ValRet);
	return(vl_ValRet);
	
}  /* Fin acli_init_timer */

/*X*/
/*
  ------------------------------------------------------
  * SERVICE RENDU : 
  *  Fonction permettant d abonner la tache TACLI aux DataGroups 
  * necessaires.
  ------------------------------------------------------
  * SEQUENCE D'APPEL :
  */

static int acli_Joint_DGs(XDY_NomSite pa_NomSite)

/*
 * ARGUMENTS EN ENTREE :
 *   	pa_NomSite : contient le nom du site sur lequel TACLI est lance
 *	On constitue les noms des dataGroups a joindre par cet argument.
 *
 *
 * ARGUMENTS EN SORTIE :
 *   Aucun
 *
 *
 * CODE RETOUR : 
 *  XDC_OK si on a reussi a joindre tous les datagroups
 *	sinon on arrete la tache en appelant acli_sortir( )
 *
 * CONDITION D'UTILISATION
 *   Aucune
 *
 * FONCTION 
 *   	Joint les DataGroups utiles a la tache TACLI.
 *
 ------------------------------------------------------*/
{
  static char *version = "@(#)acli_dir.c	1.42 05/29/12 : acli_Joint_DGs" ;
  int   vl_ValRet = XDC_OK;
  XDY_Datagroup    vl_datagroup;
	
  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : %s sur site %s",version,pa_NomSite);
	
  if ( vg_Equipmt == XDC_OK)
    {
      /*A
      ** Joint le datagroup XDG_AT_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AT,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_EPAL_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_EPAL_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_ENAV_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_ENAV_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_EPMV_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_EPMV_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_EBAF_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_EBAF_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_EBAD_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_EBAD_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_EPMVA_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_EPMVA_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_ETUNNEL_AUTO_NomMachine 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_ETUNNEL_AUTO,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_WARNING,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  if ( vg_Anim_IHM == XDC_OK )
    {
      /*A
      ** Joint le datagroup XDG_A_IHM_nomsite
      */
      XDG_EncodeDG2(vl_datagroup,XDG_A_IHM,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
		
      /*A
      ** Joint le datagroup XDG_A_TIMER_RAFRAI_IHM pour le timer de rafraichissement IHM.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAFRAI_IHM,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_AH_INITLT 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AH_INITLT,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}

      /*A
      ** Joint le datagroup XDG_AH_RADT_DAI 
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AH_RADT_DAI,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_INFO,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  if ( vg_Anim_IHM_bloc == XDC_OK )
    {
      /*A
      ** Joint le datagroup XDG_A_TIMER_RAF_IHM_BLOC pour le timer de rafraichissement IHM.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_RAF_IHM_BLOC,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  if ( vg_Travaux == XDC_OK )
    {
      /*A
      ** Joint le datagroup XDG_A_TIMER_TRV_COURS pour le timer de rafraichissement IHM.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_A_TIMER_TRV_COURS,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  if ( vg_Alertes == XDC_OK )
    {
      /*A
      ** Joint le datagroup XDG_AA pour la creation d'alerte.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AA,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
      /*A
      ** Joint le datagroup XDG_ETDP_SEUIL pour la creation d'alerte.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_ETDP_SEUIL,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
	

      if (!strcmp(pa_NomSite,XDC_NOM_SITE_CI)) {
	/*A
	** Joint le datagroup XDG_AS_COMMU_FMC pour la creation d'alerte.
	*/
	strcpy(vl_datagroup, XDG_AS_COMMU_FMC);

	XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
	if (!XDG_JoindreDG(vl_datagroup))
	  {
	    XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	    XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	    acli_sortir();
	    return(XDC_NOK);
	  }
      }
    }
  if ( vg_Alarmes == XDC_OK )
    {
      /*A
      ** Joint le datagroup XDG_AM pour la creation d'alarme.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AM,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  if ( vg_courbe == XDC_OK )
    { 
      /*A
      ** Joint le datagroup XDG_AS_COURBE pour recevoir les demandes de rafraichissement courbes Traffic.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  if ( vg_courbehis == XDC_OK )
    { 
      /*A
      ** Joint le datagroup XDG_AS_COURBE pour recevoir les demandes de rafraichissement courbes Traffic.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AS_COURBE,"HIS");
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }
  /*A
  ** Si on est sur le CI, on joint le datagroup XDG_AN pour le timer de purge du calendrier des astreintes.
  */
  if (!strcmp(pa_NomSite,XDC_NOM_SITE_CI))
    {
      if ( vg_Purge_Annuaire == XDC_OK )
	{
	  XDG_EncodeDG2(vl_datagroup,XDG_AN,pa_NomSite);
	  XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
	  if (!XDG_JoindreDG(vl_datagroup))
	    {
	      XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	      acli_sortir();
	      return(XDC_NOK);
	    }
	}
      if ( vg_Anim_Strada == XDC_OK )
	{
	  if (!XDG_JoindreDG(XDG_AS_EVT_STRADA))
	    {
	      XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",XDG_AS_EVT_STRADA);
	      XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	      acli_sortir();
	      return(XDC_NOK);
	    }
	}

    }
  if (( vg_Purge_Exploit == XDC_OK ) || ( vg_Purge_Histo == XDC_OK ))
    {
      /*A
      ** Joint le datagroup XDG_AG pour le timer de purge Base d exploitation 
      ** et si on est sur le CI pour le timer de purge Base historique.
      */
      XDG_EncodeDG2(vl_datagroup,XDG_AG,pa_NomSite);
      XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
      if (!XDG_JoindreDG(vl_datagroup))
	{
	  XZST_03EcritureTrace(XZSTC_FATAL,"Echec XDG_JoindreDG(%s)",vl_datagroup);               
	  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s",pa_NomSite);
	  acli_sortir();
	  return(XDC_NOK);
	}
    }

  if (!strcmp(pa_NomSite, XDC_NOM_SITE_CI) && vg_Equipmt == XDC_OK)
    {
      /*A
      ** Joint le datagroup XDG_ETDP_FCD pour la creation d'alarme.
      */

      if (!XDG_JoindreDG(XDG_ETDP_FCD))
      {
        XZST_03EcritureTrace(XZSTC_FATAL, "Echec XDG_JoindreDG(%s)", vl_datagroup);
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : acli_Joint_DGs sur site %s", pa_NomSite);
        acli_sortir();
        return (XDC_NOK);
      }
    }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT %s",version);
  return(XDC_OK);
	
}  /* Fin acli_Joint_DGs */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d initialiser les variables globales de la tache TACLI
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

static int acli_init_Globales(char * va_nom_tache )

/*
* ARGUMENTS EN ENTREE : va_nom_tache contient le nom de tache courante,
* 	permet de configurer les abonnements en fonction de la tache.
* ARGUMENTS EN SORTIE :Aucun
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	on appele acli_sortir pour arreter la tache.
* CONDITION D'UTILISATION : Aucune
* FONCTION 
------------------------------------------------------*/
{
	static char *version 	= "@(#)acli_dir.c	1.42 05/29/12 : acli_init_Globales" ;
	int vl_ValRet 		= XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN %s avec nom tache %s",version,va_nom_tache);
	
	if (!strcmp(va_nom_tache, ACLIDC_TACLI))
	{
	    if ( vm_TypeMachine != XZSCC_TYPEM_PCS )
	       {
	       if ( vm_TypeMachine == XZSCC_TYPEM_HIS )
	       {
	       vg_Anim_IHM             = XDC_NOK;
	       vg_Alertes              = XDC_NOK;
	       vg_Travaux              = XDC_NOK;
	       vg_Anim_Strada          = XDC_NOK;
	       vg_Meteo 	       = XDC_NOK;
	       vg_Meteohis 	       = XDC_OK;
	       vg_courbe               = XDC_NOK;
	       vg_courbehis		= XDC_OK;
	       vg_Mesureshis		= XDC_OK;
	       }
	       else
	       {
		vg_Anim_IHM		= XDC_OK;
		vg_Anim_Strada		= XDC_OK;
		vg_Alertes		= XDC_OK;
		vg_Travaux              = XDC_NOK;
		vg_Meteo 		= XDC_OK;
		vg_Meteohis		 = XDC_NOK;
		vg_courbehis             = XDC_NOK;
	        vg_Mesureshis		= XDC_NOK;
		}
	       }
	    else 
	       {
		vg_Anim_IHM		= XDC_NOK;
		vg_Anim_Strada		= XDC_NOK;
		vg_Alertes		= XDC_NOK;
		vg_Travaux		= XDC_OK;
		vg_Meteo 		= XDC_NOK;
		vg_courbehis             = XDC_NOK;
	        vg_Mesureshis		= XDC_NOK;

	       }
	       if ( vm_TypeMachine != XZSCC_TYPEM_HIS )
	       {
		vg_Purge_Histo		= XDC_NOK;
		vg_Purge_Exploit	= XDC_NOK;
		vg_Purge_Annuaire	= XDC_NOK;
		vg_Alarmes		= XDC_OK;
		vg_Mesures		= XDC_OK;
		vg_Equipmt		= XDC_OK;
		vg_courbe               = XDC_OK;
		}
		
		/*A
		** Rafraichissement global de l IHM demande a l initialisation de la tache TACLI
		*/
		XZAH10G_Init = XDC_VRAI;
	}
	if (!strcmp(va_nom_tache, ACLIDC_TACPC))
	{
		vg_Anim_IHM_bloc	= XDC_OK;
		vg_Meteo 		= XDC_NOK;
		vg_Travaux		= XDC_NOK;
		
		/*A
		** Rafraichissement global de l IHM demande a l initialisation de la tache TACLI
		*/
		XZAH10G_Init = XZAH_TRAIT_BLOC;
	}
	if (!strcmp(va_nom_tache, ACLIDC_TAPUR))
	{
		vg_Meteo 		= XDC_NOK;
		vg_Anim_IHM		= XDC_NOK;
		vg_Anim_Strada		= XDC_NOK;
		vg_Alarmes		= XDC_NOK;
		vg_Alertes		= XDC_NOK;
		vg_Mesures		= XDC_NOK;
		vg_Equipmt		= XDC_NOK;
		vg_courbe               = XDC_NOK;
		vg_Purge_Exploit	= XDC_OK;
		vg_Purge_Annuaire	= XDC_OK;
		vg_Travaux		= XDC_NOK;
		if ( vm_TypeMachine == XZSCC_TYPEM_HIS )
                {
			vg_Purge_Histo          = XDC_OK;
			vg_Purge_Annuaire       = XDC_NOK;
			vg_Purge_Exploit        = XDC_NOK;
		}
		else
		{
			vg_Purge_Histo          = XDC_NOK;
			vg_Purge_Annuaire       = XDC_OK;
			vg_Purge_Exploit        = XDC_OK;
		}
	}
	
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT %s",version);
	return(vl_ValRet);
}
