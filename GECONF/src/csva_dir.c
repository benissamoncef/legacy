/*E*/
/*Fichier : $Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $      Release : $Revision: 1.11 $        Date : $Date: 2016/05/31 18:40:57 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  GECONF
------------------------------------------------------
* MODULE SVADIR * FICHIER csva_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module Directeur de la tache TCSVA qui permet de sauvegarder
* restaurer les bases du CI.
* (DCG-002/0.1)
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	05 Jan 1995	: Creation
* T.Milleville	03 Mar 1995	: Suppression des appels a XZST_10 V1.2
* T.Milleville	12 Jul 1995	: Modification boucle traitement 
*	des msg RTworks V1.3
* PhNIEPCERON   30 Aou 1995 	: Suppression de XZSA_20InitAdmin 
		(auotmatic ds XZSC_01InitCnxSup ) v1.4
* PhNIEPCERON   15 Avr 1996	: Abt a XDG_CBDARCHIVE_site (au lieu de XDG_CBDARCHIVE) v1.6 (1115)
* PhNIEPCERON   15 Avr 1996     : Abt a XDG_CBDARCHIVE aussi v1.7 1.8(1115)
* JPL		24/05/2016	: Ajout fonction utilitaire cgcd_Lire_Valeur_Symbole (DEM 1165)  1.10
* JPL		31/05/2016	: Fonction utilitaire mise en commun dans xdf.c  1.11
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "csva_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declarations de donnees globales */

FILE	*CSVAG_Fich_BilanArchivResto = NULL;

/* declaration de variables locales */

static char *version = "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_dir" ;

/* declaration de fonctions internes */

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
	static char *version = "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : main" ;
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
	
		
	XZST_03EcritureTrace( XZSTC_INFO, "TCSVA : Demarrage *********************");
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	** Connexion au RTserver et initialisation des variables
	*/
	if( XZSC_01InitCnxSup(  argv[ 1 ],argv[ 0 ],
				XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,csva_sortir,
				"/dev/null" ) != XDC_OK )
	{
		/*B
		** Affichage Trace 
		*/
		XZST_03EcritureTrace( XZSTC_WARNING, "TCSVA : Ne peut ouvrir la connexion avec le RTserver.\n" );
		exit( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCSVA : Ouverture de la connexion avec le RTserver.");
	}
	/*
	**  Initialisation de l administration par SUPERV.
	*/
	/*  if(XZSA_20InitAdmin()!= XDC_OK ): inutile cf historique	*/
		
	/*A
	** On logue le demarrage dans le journal d administration.
	*/
	
	/*XZST_10ArchiverMsgSyst(XZSTC_INFO ," Demarrage de TCSVA.x");*/
	/*A
	** Recuperation du nom de la machine 
	*/
	if ( (vl_ValRet = XZSC_07NomMachine(vl_NomMachine)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TCSVA : Recuperation du Nom de la machine impossible .");
		exit(1);
	} 
	else
	{
		strcpy(CSVAV_NOMMACHINE.machine,vl_NomMachine);
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCSVA : Recuperation du Nom de la machine : %s.",vl_NomMachine);
	}
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TCSVA : Recuperation du Nom du site impossible .");
		exit(1);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCSVA : Recuperation du Nom du site %s.",pl_NomSite);
	}
	/*A
	**  Connexion a la base de donnees
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_FATAL, "TCSVA : Connexion au serveur SQL de %s impossible .",pl_NomSite);
		exit(1);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCSVA : Connexion au serveur SQL de %s reussie.",pl_NomSite);
	}
	
	/*A
	** Abonnement aux data groups. 
	*/
	if (csva_Joint_DGs(pl_NomSite)!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TCSVA :  impossible de joindre les Data Groups." );		
		exit(1);
	} 
	else
        {
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCSVA : Connexion aux Data Groups reussie");
	} 
	/*A
	** Initialisation des Callbacks sur receptions de messages
	*/
	
	if (csva_init_Callback()!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TCSVA :  impossible d enregistrer les Callbacks." );		
		exit(1);
	} 
	else
        {
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TCSVA : Enregistrement des Callbacks reussi");
	} 
	/*A
	** Initialisation des timers declenchant des Callbacks cycliques. 
	*/
	if (csva_init_timer()!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "TACLI :  impossible de programmer les timers sur callback." );		
		exit(1);
	} 
        else
        {
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TACLI : Enregistrement des timers reussi");
	}  
	
	/*A
	** Attente bloquante sur les messages du RTserver.
	*/
	while(1)
	{
		XZST_03EcritureTrace (XZSTC_WARNING, "Attente infinie d'un message \n");

		/* attente infinie d'un message */
		pl_Msg = TipcSrvMsgNext (T_TIMEOUT_FOREVER);

		/* si message recu */
		if(pl_Msg != NULL)
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
				vl_IndPre++;

			/*B Attente aleatoire pour permettre une reconnexion */
			vl_Attente = rand()%15;
			sleep(vl_Attente);
		}
	}



/*	for (;;)
	{
		if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER) != T_ERR_CONN_EOF)
		{
                        XZST_03EcritureTrace(XZSTC_FATAL ,"TCSVA : TipcSrvMainLoop a echoue  ");
			break;    
		}			   
	}*/

	XZST_03EcritureTrace( XZSTC_INFO, "TCSVA : Arret sur %s *********************",pl_NomSite);
}  /* Fin main */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  csva_init_Callback : Fonction permettant d initialiser
*  les Callbacks de la tache TCSVA.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int csva_init_Callback( )

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
	static char *version 	= "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_init_Callback" ;
	int vl_ValRet 		= XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : csva_init_Callback est appelee");
	
	/*A  
	** Armement Callback sur Reception XDM_CSV_SBD 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CSV_SBD),csbd_sauvebd_cb,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_WARNING,"TCSVA : csva_init_Callback Callback sur XDM_CSV_SBD non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CSV_ARC 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CSV_ARC),csva_sauvebd_cb,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_WARNING,"TCSVA : csva_init_Callback Callback sur XDM_CSV_ARC non cree." );
		return(XDC_NOK);
	}
	
	/*A  
	** Armement Callback sur Reception XDM_CSV_INF 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CSV_INF),cinf_sauvinf_cb,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_WARNING,"TCSVA : csva_init_Callback Callback sur XDM_CSV_INF non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CSV_RES : 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CSV_RES),crsa_restarc_cb,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_WARNING,"TCSVA : csva_init_Callback Callback sur XDM_CSV_RES non cree." );
		return(XDC_NOK);
	}
	/*A  
	** Armement Callback sur Reception XDM_CSV_CANAL : 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_CSV_CANAL),csva_CanalComInit_cb,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_WARNING,"TCSVA : csva_init_Callback Callback sur XDM_CSV_CANAL non cree." );
		return(XDC_NOK);
	}
	
	/*A  
	** Si tout est ok, retourne XDC_OK sinon on a fait appel a csva_sortir()
	*/
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_init_Callback : Sortie");
	return(XDC_OK);
	
}  /* Fin csva_init_Callback */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  csva_Joint_DGs : Fonction permettant d abonner 
*  la tache TCSVA aux DataGroups necessaires.
*  Fonction interne au module directeur.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int csva_Joint_DGs(XDY_NomSite pa_NomSite)

/*
* ARGUMENTS EN ENTREE :
*   	pa_NomSite : contient le nom du site sur lequel TCSVA est lance
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
*   	Joint les DataGroups utiles a la tache TCSVA.
*
------------------------------------------------------*/
{
	static char *version = "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_Joint_DGs" ;
	
	int   		vl_ValRet = XDC_OK;
	XDY_Datagroup 	vl_datagroup;
	XDY_NomSite             pl_NomSite;

	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : csva_Joint_DGs sur site %s",pa_NomSite);
        /*A
		** Recuperation du site de la machine
	*/
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "csbd_ProgramTimer_Hebdo : Recuperation du Nom du site impossible .");
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csbd_ProgramTimer_Hebdo : Sortie");
		csva_sortir();
		return(XDC_NOK);
	}

	/*A
	** Reconstitution du DG a joindre 
	*/
   	XDG_EncodeDG2(vl_datagroup,XDG_CBDARCHIVE,pl_NomSite);
   	XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
	

   	/*A
   	** On s abonne au DG
   	*/
   	if (!XDG_JoindreDG(vl_datagroup))
   	{	
                XZST_03EcritureTrace(XZSTC_FATAL,"TCSVA : csva_Joint_DGs : Echec XDG_JoindreDG(%s)",vl_datagroup);               
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_Joint_DGs sur site %s",pa_NomSite);		
		return(XDC_NOK);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_Joint_DGs a reussi a joindre %s",vl_datagroup);
	
	strcpy(vl_datagroup,XDG_CBDARCHIVE);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);


   	/*A
   	** On s abonne au DG
   	*/
   	if (!XDG_JoindreDG(vl_datagroup))
   	{	
                XZST_03EcritureTrace(XZSTC_FATAL,"TCSVA : csva_Joint_DGs : Echec XDG_JoindreDG(%s)",vl_datagroup);               
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_Joint_DGs sur site %s",pa_NomSite);		
		return(XDC_NOK);
	}
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_Joint_DGs a reussi a joindre %s",vl_datagroup);
	return(XDC_OK);
	
}  /* Fin csva_init_Callback */
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  csva_init_timer : Fonction permettant d initialiser les timers 
*  de la tache TACLI.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int csva_init_timer( )

/*
* ARGUMENTS EN ENTREE :  Aucun
* ARGUMENTS EN SORTIE :  Aucun
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	on appele csva_sortir pour arreter la tache.
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *		version 	= "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_init_timer" ;
	
	int 			vl_ValRet 	= XDC_OK;
	XDY_Horodate		vl_Nb_Secondes	= 0.0;	/* le timer est a declenchement cyclique */
	int			vl_JourSemaine=0;
	XDY_Horodate		vl_HorodateSec=0;
	XDY_Datagroup   	vl_datagroup;
        XDY_NomSite     	pl_NomSite;
        XZSMT_Date      	vl_Date;
        XZSMT_Heure     	vl_Heure;
        
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : csva_init_timer est appelee");
	
	/*A
	** Programmation du timer mensuel pour prevenir qu il faut sauvegarder la base
	*/
	if ( (vl_ValRet = cinf_ProgramTimer_Mois()) != XDC_OK)
	{
                XZST_03EcritureTrace(XZSTC_WARNING, "csva_init_timer : csbd_ProgramTimer_Mois a echoue.");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_init_timer : Sortie");
                csva_sortir();
                return( XDC_NOK);
        }
        
	/*A
	** Programmation du timer hebdomadaire effectuant automatiquement les sauvegardes des bases
	** Bases Histo et CFG.
	*/
	if ( (vl_ValRet = csbd_ProgramTimer_Hebdo ()) != XDC_OK)
	{
                XZST_03EcritureTrace(XZSTC_WARNING, "csva_init_timer : csbd_ProgramTimer_Hebdo a echoue.");
                XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_init_timer : Sortie");
                csva_sortir();
                return( XDC_NOK);
        }
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_init_timer : Sortie avec %d",vl_ValRet);
	return(vl_ValRet);
	
}  /* Fin csva_init_timer */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de sortir proprement de la tache TCSVA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void csva_sortir( )

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
	static char *version = "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_sortir" ;
	int            	      	vl_ValRet = XDC_OK;
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : csva_sortir () est appelee");
	/*A
	** Annulation de la programmation Timer 
	*/
	/*if ( (vl_ValRet = XZSM_02AnnulerProgrammation(vg_CleProg,vg_LecTime,
			XDM_TSH_SYST) ) != XDC_OK )
	{	
           	XZST_03EcritureTrace(XZSTC_WARNING,"TCSVA : XZSM_02AnnulerProgrammation a echoue");
	}*/


	/*A
	** Fermeture du fichier trace 
	*/
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"TCSVA : XZST_02FermetureTrace a echoue");
	}
	
	/*A
	** Fermeture du fichier bilan config 
	*/
	
	if ((vl_ValRet = fclose(CSVAG_Fich_BilanArchivResto))==EOF)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCSVA : csva_sortir : \nErreur survenu lors fermeture du fichier de bilan config");
	}
	/*A
	** Sortie 
	*/
	/*XZST_10ArchiverMsgSyst(XZSTC_INFO ," Arret de TCSVA.x");*/
	XZST_03EcritureTrace( XZSTC_INFO, "TCSVA : Arret  *********************");
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_sortir () : Sortie");
	exit(0);


}  /* Fin csva_sortir */
/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  csva_Recup_Horodate : Fonction permettant de recuperer l horodate courante
*  sous forme de chaine : utilisee pour initialiser
*  les Callbacks de la tache TCSVA.
------------------------------------------------------
* SEQUENCE D'APPEL :
*/
int csva_Recup_Horodate(XZSMT_Horodate pa_Horodate)
/*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*	pa_Horodate	recoit l horodate en caractere.
* CODE RETOUR : 
*  	XDC_OK si tout s est bien passe sinon
*	on retourne XDC_NOK apres avoir trace l erreur.
*
* CONDITION D'UTILISATION
*   Connexion au RTserver doit etre initialisee.
*
* FONCTION 
*   
*
------------------------------------------------------*/
{
	static char *version 	= "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_Recup_Horodate" ;
	int vl_ValRet 		= XDC_OK;
	
	int			vl_JourSemaine;	/* variable necessaire pour lire l horodate
						** mais qui n est pas exploitee
						*/
						
	double			vl_NbSeconde;	/* va contenir l horodate renvoyee par SUPERV*/
	XZSMT_Horodate		pl_Horodate;	/* va contenir l horodate en caractere*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN  : csva_Recup_Horodate est appelee");
	/*A
	** Lecture de l Horodate
	*/
	if ((vl_ValRet =XZSM_07LireHorodate(CSVAC_TYPE_SYSTEM,&vl_JourSemaine,&vl_NbSeconde))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCSVA : XZSM_07LireHorodate a echoue");
		return(XDC_NOK);
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCSVA : FIN de TCSVA");
	}
	/*A
	** Conversion en caractere de l Horodate
	*/
	if ((vl_ValRet =XZSM_11ConversionHorodate(vl_NbSeconde,&pl_Horodate))!=XZSMC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"TCSVA : XZSM_11ConversionHorodate a echoue");
		return(XDC_NOK);
		XZST_03EcritureTrace(XZSTC_FATAL ,"TCSVA : FIN de TCSVA");
	}
	
	strcpy(pa_Horodate,pl_Horodate);
	
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_Recup_Horodate sortie");
	return(XDC_OK);
}

/*X*/
/*----------------------------------------------------
* SERVICE RENDU : 
*	csva_CanalComInit_cb est declenchee sur reception du message RTWks XDM_CSV_CANAL.
*  Permet de positionne la Globale CSVAV_NOMMACHINE vers la quelle on envoie les messgae 
*  d administration (via xzia08).
*	
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void csva_CanalComInit_cb (	T_IPC_CONN 			va_Cnx,
        			T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        			T_CB_ARG                        pa_ARG) 
/*
* ARGUMENTS EN ENTREE :
*
*   	Arguments d'une callback RTServer.
*
* ARGUMENTS EN SORTIE :   aucun
* CODE RETOUR :  aucun
* CONDITION D'UTILISATION
*
*
* FONCTION 
*	Fonction declenchee sur reception de message XDM_CSV_CANAL.	
* 	
------------------------------------------------------*/
{
	static char *version = "$Id: csva_dir.c,v 1.11 2016/05/31 18:40:57 devgfi Exp $ : csva_CanalComInit_cb" ;
	
	/* Variables locales pour messages RTWks */
	
	int			vl_retcode    = XDC_OK;		/* code de retour 	*/
	char         	*	vl_NomMachine = "";		/* type RTwks : str	*/
	
	XZST_03EcritureTrace(XZSTC_FONCTION,"IN : csva_CanalComInit_cb : Debut d execution");
	
	/*A
	** Recuperation du contenu du message XDM_CSV_CANAL
	*/ 
	if (TipcMsgRead(pa_Data->msg,
			T_IPC_FT_STR, 
			&vl_NomMachine, 
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_DEBUG1,"TCSVA : csva_CanalComInit_cb a recu NomMachine = %s",vl_NomMachine);	
	}
	else
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,"TCSVA : csva_CanalComInit_cb : TipcMsgRead a echoue");	
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : csva_CanalComInit_cb Sortie");
		return;	 
	}
	strcpy(CSVAV_NOMMACHINE.machine,vl_NomMachine);
	XZST_03EcritureTrace(XZSTC_DEBUG1,"TCSVA : csva_CanalComInit_cb a positionne le canal Applix de communication (XZIA08) a %s",CSVAV_NOMMACHINE.machine);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : csva_CanalComInit_cb : Fin d execution");
	return;			
}
