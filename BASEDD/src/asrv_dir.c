/*E*/
/*Fichier : $Id: asrv_dir.c,v 1.11 2020/10/27 16:50:27 pc2dpdy Exp $      Release : $Revision: 1.11 $        Date : $Date: 2020/10/27 16:50:27 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE asrv * FICHIER asrv_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* main:  Fonction principale demarrant l'Open Serveur de type TASRV_CI
*
------------------------------------------------------
* HISTORIQUE :
*
* MERCIER O.	25 Aug 1994	: Creation
------------------------------------------------------
* MERCIER O.	29 Sep 1994	: Modification V1.6
*	Rajoue des flags  de version 
------------------------------------------------------
* MERCIER O.	29 Sep 1994	: Modification V1.7
*	Modification de la localisation du fichier de log :TASRV_**.log
* 	dans /produits/migrazur/appliSD/traces/
------------------------------------------------------
* MERCIER O.	28 Oct 1994	: Modification V1.8
*	Modification des commentaires.
* GABORIT B	20 Jan 1995 : ajout du lancement de la replication (1.9)
* JMG		11/02/04 : ajout d une trace WARNING au demarrage (1.10)
* JMG		02/09/19 : recup eventuelle du site dans le nom 1.11
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "asrv_dir.h"
#include "asrv_rpcom.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: asrv_dir.c,v 1.11 2020/10/27 16:50:27 pc2dpdy Exp $ : asrv_dir" ;

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
* 	Fonction principale demarrant l'Open Serveur de type TASRV_CI
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int main(int     argc,char    *argv[])

/*
* ARGUMENTS EN ENTREE :
*   < Nom du OPEN SERVEUR -s NOM du SQL_SERVEUR >
*
*
* ARGUMENTS EN SORTIE :
*   
*
* CODE RETOUR : 
*   
*
* CONDITION D'UTILISATION
*   
*
* FONCTION 
*  
*
------------------------------------------------------*/
{
	static char *	version 	= "$Id: asrv_dir.c,v 1.11 2020/10/27 16:50:27 pc2dpdy Exp $ : main" ;
	int 		vl_retcode 	= XDC_OK;
	CS_BOOL         vl_boolval;
	
	CS_CHAR         pl_logname[1024];  /* Nom du log file.           		 	*/
	CS_CHAR         pl_ctlogname[1024];/* Nom du file pour le tracing de la CT-lib 	*/
	XDY_NomSite	vl_NomSite;	   /* Nom Site sur le quel est lance la tache	*/
	char            pl_script[100];  /* contient la commande shell a lancer */
	
	XDY_NomTache	vl_tache;
	char *vl_ch;
	vl_boolval	= CS_TRUE;      /* Truncate log on startup      */
	
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	
	/*A
	**  Initialisation de la connexion avec le RT Serveur.
	*/
	
//	if((vl_retcode = XZSC_01InitCnxSup (argv[ 1 ], argv[ 0 ],XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,NULL,"/dev/null"))!= XDC_OK)
	if((vl_retcode = XZSC_01InitCnxSup ("migrazur", "tasrv.x",XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,NULL,"/dev/null"))!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TASRV : Ne peut ouvrir la connexion avec le RTserver.\nXZSC_01InitCnxSup a retourne %d",vl_retcode );
		exit( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TASRV : Ouverture de la connexion avec le RTserver.\n" );
	}

	//XZST_08ForceNivVisuTraceLocal(XZSTC_FATAL|XZSTC_WARNING|XZSTC_INFO|XZSTC_FONCTION|XZSTC_INTERFACE|XZSTC_MESSAGE|XZSTC_DEBUG1|XZSTC_DEBUG2|XZSTC_DEBUG3|XZSTC_SGBD,XZSTC_NECRAN);
XZST_03EcritureTrace(XZSTC_WARNING,"Ligne de commande argv[0]=[%s] argv[1]=[%s]", argv[0], argv[1]);

	XZST_03EcritureTrace(XZSTC_WARNING,"Demarrage de Open Serveur\n");
	/*A
	** Recupere le nom du site afin de constituer le nom de l'Open serveur : TASRV_CI pour le site CI.
	*/
	if( (vl_retcode =XZSC_04NomSite( vl_NomSite))!= XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "TASRV : XZSC_07NomSite a echoue : Pas recuperer le nom du site : code = %d",vl_retcode );
		exit( XDC_NOK );
	}
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "TASRV : Recuperation du nom Site a reussi" );
	}

	/* TASRV-XX*/
/*	if ((vl_ch=strchr(argv[ 0 ], '_')) != NULL) {
		strncpy(vl_NomSite, vl_ch+1,2);
		XZST_03EcritureTrace( XZSTC_WARNING, "SITE FORCE : %s",vl_NomSite);
	}
*/
	strcpy(Name,"TASRV_");
	strcat(Name,vl_NomSite);
        XZST_03EcritureTrace( XZSTC_WARNING, "SITE : %s",vl_NomSite);		
	/*A
	** Allocation et initialisation de la structure contexte,
	** Erreur Handler...
	*/
	if(asrv_setversion(&Context) == CS_FAIL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : asrv_setversion() a �chou�.  Sortie.");
		XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie.");
		exit(1);

	}
	
	/*A
	** Parse les arguments passes par la ligne de commande.                             
	*/
	
        /*if (asrv_proc_args(Context, argc, argv, Name, CS_MAX_CHAR,
                SqlServername, sizeof(SqlServername)) != CS_SUCCEED)
        {*/
	
               /*B
               ** Liberation de la structure de contexte si la ligne de commande n est pas bonne.
               */
        /*       (CS_VOID)cs_ctx_drop(Context);
               XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : asrv_proc_args() a �chou�.  Sortie.");
               XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie.");
               exit(CS_FAIL);
        }*/
	

	XZST_03EcritureTrace(XZSTC_SGBD," Le Open serveur est : %s",Name);
	
	XZST_03EcritureTrace(XZSTC_SGBD,"Install the Open Server error handler.");
	
        /*A
        ** Installation du handler pour les evenements erreurs pour l'OpSRV.
        */
        
        if (srv_props(Context, CS_SET, SRV_S_ERRHANDLE,
                (CS_VOID *)asrv_server_err_handler, sizeof(CS_VOID *),
                (CS_INT *)NULL) != CS_SUCCEED)
        {
                /*B
                ** Desallocation de la structure contexte deja allouee.
                */
                (CS_VOID)cs_ctx_drop(Context);
                
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV :srv_props(ERRHANDLE) a �chou�.  Sortie.");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie.");
                exit(CS_FAIL);
         }
         
	/*
        ** Configure the logfile to be truncated on start-up.
        */
        if(srv_props(Context, CS_SET, SRV_S_TRUNCATELOG, (CS_VOID *)&vl_boolval,
                sizeof(vl_boolval), (CS_INT *)NULL) == CS_FAIL)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV :srv_props(SRV_S_TRUNCATELOG) a �chou�.  Sortie.\n");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d .",CS_FAIL);
                exit(CS_FAIL);
        }
        
        /*
        ** Configure the log file to have the same name as the server.
        */
        (CS_VOID)sprintf(pl_logname, "%s%s.log","/produits/migrazur/appliSD/traces/", Name);
/*        (CS_VOID)sprintf(pl_logname, "%s.log", Name);*/
        if(srv_props(Context, CS_SET, SRV_S_LOGFILE, (CS_VOID *)pl_logname, CS_NULLTERM,
                (CS_INT *)NULL) == CS_FAIL)
        {
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV :srv_props(SRV_S_LOGFILE) a �chou�.  Sortie.\n");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d .  \n",CS_FAIL);
                exit(CS_FAIL);
        }
        
        /*A 
        ** Initialise l'Open Server.                                      
        */
        XZST_03EcritureTrace(XZSTC_SGBD,"Initialisation de l Open Server.");
        if (srv_init((SRV_CONFIG *)NULL, Name, CS_NULLTERM) == (SRV_SERVER *)NULL)
        {
                /*!
                ** Verifier que c est compatible avec la suite ;
                ** Sortie de la Ct lib.
                */
                (CS_VOID)ct_exit(Context, CS_FORCE_EXIT);
                
                /*B
                ** Desallocation de la structure contexte deja allouee.
                */
                (CS_VOID)cs_ctx_drop(Context);
                

		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV :srv_init a �chou�.  Sortie.\n");
		XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d \n",CS_FAIL);
                
                exit(CS_FAIL);
        }
	/*
	** Initialise la Client-Library.
	*/
	if(ct_init(Context, CS_VERSION_100) == CS_FAIL)
	{
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV :ct_init a �chou�.  Sortie.\n");
		XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d \n",CS_FAIL);
		exit(CS_FAIL);
	}

        /*A
        ** Si un niveau de contexte de traces CT-Lib est positionne, on l active.
        ** now.
        */
        if(Ctcflags != (CS_INT)0 || Conflags != (CS_INT)0)
        {
                /*
                ** Configure the CT-lib trace files to have the same name
                ** as the server.
                */
                (CS_VOID)sprintf(pl_ctlogname, "%s.ctlog", Name);
                
                vl_retcode=ct_debug(Context, (CS_CONNECTION *)NULL, CS_SET_DBG_FILE, CS_UNUSED, pl_ctlogname, (CS_INT)strlen(pl_ctlogname));
		if(vl_retcode != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : asrv_srv_handler : ct_debug(CS_SET_DBG_FILE) a �chou� sur le fichier %s",pl_ctlogname);
                        XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d \n",CS_FAIL);
                        exit(CS_FAIL);
                }
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : ct_debug a ouvert le fichier %s",pl_ctlogname);
        }
        
        if(Ctcflags != (CS_INT)0)
        {
                if(ct_debug(Context, (CS_CONNECTION *)NULL, CS_SET_FLAG, Ctcflags,
                        (CS_CHAR *)NULL, (CS_INT)0) != CS_SUCCEED)
                {
                        XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : asrv_srv_handler : ct_debug(Ctcflags) failed\n");
                        XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d \n",CS_FAIL);
                        exit(CS_FAIL);
                }
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Mise en place des flags de debug pour la CT lib");
        }
        
	/*!
        ** Installe le CT-Libary error handler.
        */
        
        if(ct_callback(Context, (CS_CONNECTION *)NULL, CS_SET, CS_CLIENTMSG_CB,(CS_VOID *)asrv_cterr) == CS_FAIL)
        {
                (CS_VOID)ct_exit(Context, CS_FORCE_EXIT);
                XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : ct_callback(CS_CLIENTMSG_CB) a echou�");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d \n",CS_FAIL);
                exit(CS_FAIL);
        }
       /*A
        ** Appel a asrv_start_handler pour initialiser la structure du serveur en remote.
        ** Other event handlers are defined in asrv_start_handler.
        */
        XZST_03EcritureTrace(XZSTC_SGBD,"Appel asrv_start_handler pour initialiser le serveur en remote");
        if (srv_handle((SRV_SERVER *)NULL, SRV_START, asrv_start_handler) == (SRV_EVENTHANDLE_FUNC)NULL)
        {
                /*B
                ** Desallocation de la structure contexte deja allouee.
                */
                (CS_VOID)cs_ctx_drop(Context);
                
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : asrv_srv_handle a �chou�.  Sortie.\n");
                XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie.  \n");
                exit(1);
        }
      	
	/*A
        ** srv_run doit retourner CS_SUCCEED si arrete normalement 
        ** via un evenement SRV_STOP.
        ** On rentre dans la boucle infinie de srv_run().
        */
        
        XZST_03EcritureTrace(XZSTC_SGBD,"srv_run retourne CS_SUCCEED si arrete nomalement via a SRV_STOP.\n");
        
        /*A lancement de la replication */
        
        strcpy(pl_script,"/produits/migrazur/appliSD/exec/xzag22.ksh ");
        strcat(pl_script,vl_NomSite);
        strcat(pl_script,"&");
        
        system(pl_script);
        
        if (srv_run((SRV_SERVER *)NULL) != CS_SUCCEED)
        {
                
		XZST_03EcritureTrace(XZSTC_FATAL,"TASRV : srv_run a �chou� \n");
		vl_retcode = 1;
	}
        /*A 
        ** Desallocation des structures de control avant arret de la tache.                        
        */
        (CS_VOID)cs_ctx_drop(Context);
        
        XZST_03EcritureTrace(XZSTC_SGBD,"TASRV : Sortie de la tache \n");
        XZST_03EcritureTrace(XZSTC_FONCTION,"TASRV : Sortie avec %d.  \n",vl_retcode);
        
        exit(vl_retcode);

}







