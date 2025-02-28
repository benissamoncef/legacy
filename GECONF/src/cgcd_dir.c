/*E*/
/*Fichier : $Id: cgcd_dir.c,v 1.14 2016/07/13 18:34:57 devgfi Exp $      Release : $Revision: 1.14 $        Date : $Date: 2016/07/13 18:34:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE MDIR * FICHIER cgcd_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	cgcd_dir est le module directeur de la tache TCGCD.
*	Cette tache interprete les demandes issues de GESIHM
*  afin d assurer la coherence sur les modifications de la configuration
*  (autoroute, equipement,...) qui lui sont livres. De plus cette tache
*  fournit les moyens necessaires a la diffusion de ces modifications
*  qui auront ete verifiees synthaxiquement et testees dans une base 
*  de test.
*
*  Le module contient :
*****************************
*	main : realise les init: connexion au rtserveur via Supervision
*  sans option time et avec fonction de sortie.
*  Declaration des Callbacks RTWks.
*  Abonnement au Data Group XDG_GESCONFIG
*  Attente bloquante sur reception de messages RTWorks.
*  Toutes les demandes de gestions de configuration sont archivees 
*  dans le journal d administration par la primitive XZST10.
*****************************
*  cgcd_init_Callback : Fonction permettant d initialiser
*  les Callbacks de la tache TCGCD.
*****************************
*  cgcd_Joint_DGs : Fonction permettant d abonner 
*  la tache TCGCD aux DataGroups necessaires.
*****************************
*	void cgcd_sortir() permet de sortir proprement de TCGCD
*****************************
*  	cuti_Recup_Horodate : Fonction permettant de recuperer l horodate courante
*  sous forme de chaine : utilisee pour initialiser
*  les Callbacks de la tache TCGCD.
* 	cuti_BilanConf_Trace  permet d ecrire dans le fichier bilan config 
*  le message passe en argument.
*****************************
*  	cuti_Chgmt_Base_Courante permet de changer de base courante.
*  Pour le passage de la Base courante a CFT, il n y a pas de verification.
*  Pour passer la Base courante a CFG, il faut que le tableau de checklist soit
*  Valider : pour tous les types de Verif, le flag doit etre positionne a XDC_OK.
*  On envoie un message a l IHM de l administrateur pour l informer de toute demande.
*****************************
------------------------------------------------------
* HISTORIQUE :
*
* MERCIER O.	11 Oct 1994	: Creation
************
* MERCIER O.	12 Oct 1994	: Ajouts	V1.1
*	main
*	cgcd_init_Callback
*	cgcd_Joint_DGs
************
* MERCIER O. 	13 Oct 1994	: Ajout 	V1.2
* 	cgcd_sortir()
************
* VOLCIC F. 	08 Nov 1994	: Changement Base Config (CFG) 	V1.3
* 	cgcd_sortir()
************
* VOLCIC F. 	05 Dec 1994	: Rien 		V1.4
************
* MERCIER O. 	05 Dec 1994	: Ajout 	V1.5
* 		ajout du callback de livraison.
*		Ajout de cliv_Init_Tab_Checklist permettant de 
*  gerer le tab de checklist.
*  Mis a jour du CallBack cvce_majbd_cb sur reception de XDM_CGD_DBD
* 	
* T.Milleville 	03 Mar 1995 	: Suppression des appels a XZST_10 V1.8
* T.Milleville 	12 Jul 1995 	: Modification boucle traitement
*	des msg RTworks V1.9
* PhNIEPCERON   30 Aou 1995 	: Suppression de XZSA_20InitAdmin 
		(auotmatic ds XZSC_01InitCnxSup ) v1.10
* PhNIEPCERON   30 Aou 1995 	: Suppression de XZSA_20InitAdmin 
		(auotmatic ds XZSC_01InitCnxSup ) v1.11
* JPL		24/05/2016	: Gestion nom lecteur DAT, lu en fichier de config. (DEM 1165)  1.12
* JPL		31/05/2016	: Utilisation fonction commune pour lire le fichier de config.  1.13
* JPL		13/07/16	: Suppression de warnings a la compilation  1.14
*************
* KOU  25/09/2023	: Adaptation code pour rabbitmq DEM-483
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "cgcd_dir.h"
#include "cgcd_vce.h"
#include "xzems.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declarations de donnees globales */

char CGCDG_Base_Courante[4] = (XDC_BASE_CFT);

FILE	*CGCDG_Fich_BilanConfig = NULL;
FILE	*CGCDG_Fich_CHECKLIST	= NULL;

XDY_Nom		CGCDV_NOM_PERIPH_SVG = CVCEC_PERIPH_SVG_DEFAUT;


/* declaration de variables locales */

static char *version = "$Id: cgcd_dir.c,v 1.14 2016/07/13 18:34:57 devgfi Exp $ : cgcd_dir" ;

/* declaration de fonctions internes */
int cgcd_init_Callback ();
int cgcd_Joint_DGs (XDY_NomSite);

/* definition de fonctions externes */

/*E
------------------------------------------------------
* SERVICE RENDU :
* Programme principal
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int main( argc, argv )

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
	static char *version = "$Id: cgcd_dir.c,v 1.14 2016/07/13 18:34:57 devgfi Exp $ : main" ;
	time_t	 		vl_Date;
	
	struct tm		*tl_Tm;
	
	
	int			vl_JourSemaine;	/* variable necessaire pour lire l horodate
						** mais qui n est pas exploitee
						*/
	double			vl_NbSeconde;	/* va contenir l horodate renvoyee par SUPERV*/
	XZSMT_Horodate		pl_Horodate;	/* va contenir l horodate en caractere*/
	
	int            	      	vl_ValRet = XDC_OK;
   	XDY_NomSite        	pl_NomSite="";
   	
	
	XZSCT_Datagroup         vl_NomTacheHorl;
	T_IPC_MSG              	vl_Msg ;
	int 			vl_ModeEmis=0;
	int 			vl_Inter=0;
	XDY_NomMachine  	vl_NomMachine="";
	char  			pl_NomFile[255]="";          
	int                     vl_IndPre       = 0;
	T_IPC_MSG               pl_Msg;
	int                     vl_Attente      = 0;

					/* 
					** Nom Complet du fichier Bilan Config
					** dont le fd est passe en globale dans la tache.
					*/
					
	char  pl_LibMsg[255];		/* buffer de message pour le fichier bilan config */
	
	
	strcpy(CGCDG_Base_Courante,XDC_BASE_CFT);  	/* On travail sur la base de Test */
							/* par defaut */
	
	XZST_03EcritureTrace( XZSTC_INFO, "TCGCD : Demarrage *********************");
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	** Connexion au RTserver et initialisation des variables
	*/
	if( XZSC_01InitCnxSup(  argv[ 1 ],argv[ 0 ],
				XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,cgcd_sortir,
				"/dev/null" ) != XDC_OK )
	{
		/*B
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Ouverture de la connexion avec le RTserver.");
	}
	/*
	**  Initialisation de l administration par SUPERV.
	*/
	/*  if(XZSA_20InitAdmin()!= XDC_OK ): inutile cf historique	*/

	
	/*A
	**  Recupere l etat de verification des fichiers livres en cours de verification.
	*/
	if(cliv_Init_Tab_Checklist()!= XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TCGCD : Ne peut recuperer le tableau de Checklist.\n" );
		exit( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Recuperation du Tableau de checklist.");
	}

	/*A
	** On logue le demarrage dans le journal d administration.
	*/
	
	/*XZST_10ArchiverMsgSyst(XZSTC_INFO ," Demarrage de TCGCD.x");*/
	/*A
	** Initialisation de la Checklist a vrai par defaut
	*/
	CGCDV_CHECKLIST = XDC_OK;
	/*A
	** Recuperation du nom de la machine 
	*/
	if ( (vl_ValRet = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TCGCD : Recuperation du Nom de la machine impossible .");
		exit(1);
	} 
	else
	{
		strcpy(CGCDV_NOMMACHINE.machine,vl_NomMachine);
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Recuperation du Nom de la machine : %s.",vl_NomMachine);
	}
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TCGCD : Recuperation du Nom du site impossible .");
		exit(1);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Recuperation du Nom du site %s.",pl_NomSite);
	}

	/*A
	**  Connexion a la base de donnees
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_FATAL, "TCGCD : Connexion au serveur SQL de %s impossible .",pl_NomSite);
		exit(1);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Connexion au serveur SQL de %s reussie.",pl_NomSite);
	}

	
	/*A
	** Abonnement aux data groups. 
	*/
	if (cgcd_Joint_DGs(pl_NomSite)!= XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_FATAL, "TCGCD :  impossible de joindre les Data Groups." );		
		exit(1);
	} 
        {
		/*B 
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Connexion aux Data Groups reussie");
	} 
	/*A
	** Initialisation des Callbacks sur receptions de messages
	*/
	
	if (cgcd_init_Callback()!= XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_FATAL, "TCGCD :  impossible d enregistrer les Callbacks." );		
		exit(1);
	} 
	else
        {
		/*B 
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCGCD : Enregistrement des Callbacks reussi");
	} 
	            
	/*A 
	** Recuperation nom complet du fichier Bilan config. 
	*/
	sprintf(pl_NomFile,"%s/%s",XDC_BILAN_CONFIG_PATH,XDF_BILAN_CONFIG);
	
	/*A
	** Ouverture du fichier de bilan config
	*/
	
	if (!(CGCDG_Fich_BilanConfig=fopen(pl_NomFile,"w+")))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : Erreur d ouverture du fichier %s ",pl_NomFile);
		cgcd_sortir( );
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : FIN de TCGCD");
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"Ouverture du fichier %s ",pl_NomFile);
	}
	
	/*A
	** Constitution de l entete du fichier Bilan config.
	*/
	if ((vl_ValRet =cuti_Recup_Horodate(pl_Horodate))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cuti_Recup_Horodate a echoue");
		cgcd_sortir();
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : FIN de TCGCD");
	}
	
	/*if ((vl_ValRet =XZSM_07LireHorodate(CGCDC_TYPE_SYSTEM,&vl_JourSemaine,&vl_NbSeconde))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : XZSM_07LireHorodate a echoue");
		cgcd_sortir();
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : FIN de TCGCD");
	}
		
	if ((vl_ValRet =XZSM_11ConversionHorodate(vl_NbSeconde,&pl_Horodate))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : XZSM_11ConversionHorodate a echoue");
		cgcd_sortir();
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : FIN de TCGCD");
	}*/
	
	sprintf(pl_LibMsg, "%s %s",CGCDC_ENTETE_BILAN_CONFIG,pl_Horodate);
	
	
	if (!fprintf(CGCDG_Fich_BilanConfig,"%s\n",pl_LibMsg))
      	{
         	XZST_03EcritureTrace(XZSTC_FATAL,"TCGCD : erreur ecriture dans le fichier %s du msg %s",pl_NomFile,pl_LibMsg);
      	}
      	else
      	{
         	/*B  flush de l ecriture */
         	fflush(CGCDG_Fich_BilanConfig);
      	}

	/*A
	** Lecture en configuration du nom du lecteur de sauvegarde
	*/
	{
		char		*vl_nom_F_Devices = XDC_PATH_DEQ "/" XDF_CONF_PERIPH_SYSTEME;
		XDY_Nom		vl_nom_lecteur = "";

		if ( (XDF01_ValeurNom (vl_nom_F_Devices, "CVCEC_DEVICE_SAUVEGARDE", vl_nom_lecteur, NULL) == XDC_OK) &&
		      strcmp (vl_nom_lecteur, "") != 0 )
		{
			strcpy (CGCDV_NOM_PERIPH_SVG, vl_nom_lecteur);
			XZST_03EcritureTrace(XZSTC_WARNING, "TCGCD : Lecteur = %s", CGCDV_NOM_PERIPH_SVG);
		} else {
			XZST_03EcritureTrace(XZSTC_WARNING, "TCGCD : Aucun lecteur defini, defaut = %s", CGCDV_NOM_PERIPH_SVG);
		}
	}


/**************************************************************************/
/**************************************************************************/
/**************************************************************************/
/**************** VERRUE EN ATTENDANT L'ACCES A L'APPLI JAVA **************/
/**************************************************************************/
/**************************************************************************/
XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_WARNING|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION|XZSTC_DEBUG1|XZSTC_FATAL,XZSTC_NECRAN);

XZST_03EcritureTrace (XZSTC_WARNING, "VERRUE ATTENTE DISPO APPLI JAVA CNA : ENVOI FORCE DES MESSAGES");

XDY_Datagroup vl_dg;
	strcpy(vl_dg, "GESCONFIG" );
	TipcSrvMsgWrite(vl_dg,
		TipcMtLookupByNum(XDM_CGD_CANAL),
		FALSE,
		T_IPC_FT_STR,"SDCA2D",
		NULL);
	TipcSrvFlush();

	TipcSrvMsgWrite(vl_dg,
		TipcMtLookupByNum(XDM_CGD_CHGB),
		FALSE,
		T_IPC_FT_STR,"CFT",
		NULL);
	TipcSrvFlush();

//	TipcSrvMsgWrite(vl_dg,
//		TipcMtLookupByNum(XDM_CGD_VRF),
//		FALSE,
//		T_IPC_FT_INT4,atoi(argv[2]),
//		NULL);
//	TipcSrvFlush();



/**************************** FIN VERRUE **********************************/
/**************************************************************************/
/**************************************************************************/

	/*A
	** Attente bloquante sur les messages du RTserver.
	*/

	while(1)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Attente infinie d'un message \n");

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
			/* Test de Presence RTserver */
			if (vl_IndPre == 0)
			{
				if (errno)
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks num: %d -- errno %d \n",
							TutErrNumGet(), errno);

			 }
			 else if (vl_IndPre >= 20)
			 {
			 	XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tsdis %d \n",vl_IndPre);
			  	/*A Arret de la tache */
				exit(0);
			 }

			/*B INdication Non Presence RTserver */
			if (errno)
			{
				vl_IndPre++;
				errno=0;
			}

			/*B Attente aleatoire pour permettre une reconnexion */
			vl_Attente = rand()%15;
			sleep(vl_Attente);
		}
	}

	/*for (;;)
	{
		if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER) != T_ERR_CONN_EOF)
		{
                        XZST_03EcritureTrace(XZSTC_FATAL ,"TCGCD : TipcSrvMainLoop a echoue  ");
			break;    
		}			   
	}*/

	XZST_03EcritureTrace( XZSTC_INFO, "TCGCD : Arret sur %s *********************",pl_NomSite);
}  /* Fin main */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cgcd_init_Callback : Fonction permettant d initialiser
*  les Callbacks de la tache TCGCD.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cgcd_init_Callback( )

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
*	on retourne XDC_NOK apres avoir trace l erreur.
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: cgcd_dir.c,v 1.14 2016/07/13 18:34:57 devgfi Exp $ : cgcd_init_Callback" ;
	int vl_ValRet 		= XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : cgcd_init_Callback est appelee");
	
	/*A  
	** Armement Callback sur Reception XDM_CGD_LIV 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_LIV),cliv_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_LIV non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CGD_DIF 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_DIF),cdif_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_DIF non cree." );
		return(XDC_NOK);
	}
	
	/*A  
	** Armement Callback sur Reception XDM_CGD_MBD 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_MBD),cvce_VerifModif_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_MBD non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CGD_VRF : cvce_VerifModif_cb
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_VRF),cvce_VerifModif_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_VRF non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CGD_DBD
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_DBD),cvce_majbd_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** Si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_DBD non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CGD_CANAL
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_CANAL),cuti_CanalComInit_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** Si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_CANAL non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CGD_CHECK
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_CHECK),cuti_InitChecklist_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** Si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_CANAL non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CGD_CHGB
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CGD_CHGB),cuti_ChgmtBaseCour_cb,(T_CB_ARG)NULL) == NULL) 
	{	
		/*B 
		** Si Erreur detectee : Ecriture Trace 
		*/
                XZST_03EcritureTrace( XZSTC_WARNING,"TCGCD : cgcd_init_Callback Callback sur XDM_CGD_CANAL non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Si tout est ok, retourne XDC_OK sinon on a fait appel a cgcd_sortir()
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgcd_init_Callback : Sortie");
	return(XDC_OK);
	
}  /* Fin cgcd_init_Callback */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  cgcd_Joint_DGs : Fonction permettant d abonner 
*  la tache TCGCD aux DataGroups necessaires.
*  Fonction interne au module directeur.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int cgcd_Joint_DGs(XDY_NomSite pa_NomSite)

/*
* ARGUMENTS EN ENTREE :
*   	pa_NomSite : contient le nom du site sur lequel TCGCD est lance
*	On constitue les noms des dataGroups a joindre par cet argument.
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*  XDC_OK si on a reussi a joindre tous les datagroups
*	sinon on renvoie XDC_NOK.
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   	Joint les DataGroups utiles a la tache TCGCD.
*
------------------------------------------------------*/
{
	static char *version = "$Id: cgcd_dir.c,v 1.14 2016/07/13 18:34:57 devgfi Exp $ : cgcd_Joint_DGs" ;
	
	int   		vl_ValRet = XDC_OK;
	XDY_Datagroup 	vl_datagroup;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cgcd_Joint_DGs sur site %s",pa_NomSite);
	
	/*A
	** Reconstitution du DG a joindre 
	*/
   	strcpy(vl_datagroup,XDG_GESCONFIG);
   	
   	XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	/*A
   	** On s abonne au DG
   	*/
   	if (!XDG_JoindreDG(vl_datagroup))
   	{	
   		/*B
   		** Si une erreur est survenue on renvoie XDC_NOK apres avoir trace l erreur.
   		*/
                XZST_03EcritureTrace(XZSTC_FATAL,"TCGCD : cgcd_Joint_DGs : Echec XDG_JoindreDG(%s)",vl_datagroup);               
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgcd_Joint_DGs sur site %s",pa_NomSite);		
		return(XDC_NOK);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgcd_Joint_DGs a reussi a joindre %s",vl_datagroup);
	return(XDC_OK);
	
}  /* Fin cgcd_init_Callback */



/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de sortir proprement de la tache TCGCD
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void cgcd_sortir( )

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
	static char *version = "$Id: cgcd_dir.c,v 1.14 2016/07/13 18:34:57 devgfi Exp $ : cgcd_sortir" ;
	int            	      	vl_ValRet = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : cgcd_sortir () est appelee");
	/*A
	** Annulation de la programmation Timer 
	*/
	/*if ( (vl_ValRet = XZSM_02AnnulerProgrammation(vg_CleProg,vg_LecTime,
			XDM_TSH_SYST) ) != XDC_OK )
	{*/
		/*B 
		** Affichage trace 
		*/
        /*        XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : XZSM_02AnnulerProgrammation a echoue");
	}*/


	/*A
	** Fermeture du fichier trace 
	*/
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
		
                /*B 
                ** Affichage trace 
                */
                XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : XZST_02FermetureTrace a echoue");
	}
	
	/*A
	** Fermeture du fichier bilan config 
	*/
	
	if ((vl_ValRet = fclose(CGCDG_Fich_BilanConfig))==EOF)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCGCD : cgcd_sortir : \nErreur survenu lors fermeture du fichier de bilan config");
	}
	/*A
	** Sortie 
	*/
	/*XZST_10ArchiverMsgSyst(XZSTC_INFO ," Arret de TCGCD.x");*/
	XZST_03EcritureTrace( XZSTC_INFO, "TCGCD : Arret  *********************");
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : cgcd_sortir () : Sortie");
	exit(0);

}  /* Fin cgcd_sortir */
