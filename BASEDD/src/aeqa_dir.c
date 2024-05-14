/*E*/
/*Fichier : $Id: aeqa_dir.c,v 1.3 1995/08/18 11:11:48 gesconf Exp $      Release : $Revision: 1.3 $        Date : $Date: 1995/08/18 11:11:48 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE EQA * FICHIER aeqa_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*	Module Directeur de la tache TAEQA
*
*	TAEQA est une tache qui attend les messages RTWks (Sur DGs de TA,..)
*	et qui declanche le calcul des equations d'alerte.
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	25 jan 1995	: creation		V 1.1
* T.Milleville	12 Jul 1995	: Modification boucle traitement RTworks
*				V1.2
* T.Milleville	19 Aou 1995	: Ajout constante XDC_EXIT_RTCLIENT V1.3
------------------------------------------------------*/

/* fichiers inclus */

#include <xzsem.h>
#include "aeqa_dir.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: aeqa_dir.c,v 1.3 1995/08/18 11:11:48 gesconf Exp $ : aeqa_dir" ;

/* declaration de fonctions internes */
void aeqa_sortir( );

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
	static char *version = "$Id: aeqa_dir.c,v 1.3 1995/08/18 11:11:48 gesconf Exp $ : main" ;
		
	int            	      	vl_ValRet = XDC_OK;
   	XDY_NomSite        	pl_NomSite;
	XDY_Datagroup    	vl_datagroup;
	int                     vl_IndPre       = 0;
	T_IPC_MSG               pl_Msg;
	int                     vl_Attente      = 0;


	
	XZST_03EcritureTrace( XZSTC_INFO, "%s Demarrage *********************",version);
	
	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/*A
	** Connexion au RTserver et initialisation des variables internes (Supervision)
	*/
	if( XZSC_01InitCnxSup(  argv[ 1 ],argv[ 0 ],
				XZSCC_INTERDIRE,XZSCC_AUTORISER,XZSCC_INTERDIRE,aeqa_sortir,
				"/dev/null" ) != XDC_OK )
	{
		XZST_03EcritureTrace( XZSTC_WARNING, "%s : Echec connexion RTserver.\n",version);
		exit( XDC_NOK );
	}
	/*A
	** Recuperation du site de la machine 
	*/
	if ( (vl_ValRet = XZSC_04NomSite(pl_NomSite)) != XDC_OK)
	{
		XZST_03EcritureTrace( XZSTC_FATAL, "%s : Recuperation du Nom du site impossible .",version);
		exit(XDC_NOK);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : Recuperation du Nom du site %s.",version,pl_NomSite);
	}
	/*A
	** Connexion a la Base de donnees (SQL Serveur).
	*/
	if ( (vl_ValRet = XZAG02_Connexion_Base(pl_NomSite)) != XDC_OK)
	{
		/*B 
		** Ecriture Trace 
		*/
		XZST_03EcritureTrace( XZSTC_FATAL, "%s : Connexion au serveur SQL de %s impossible .",version,pl_NomSite);
		exit(1);
	} 
	else
	{
		XZST_03EcritureTrace( XZSTC_DEBUG1, "%s : Connexion au serveur SQL de %s reussie.",version,pl_NomSite);
	}
	
	/*A
	** Abonnement au data group. 
	*/
   	XDG_EncodeDG2(vl_datagroup,XDG_AEQA,pl_NomSite);
   	XZST_03EcritureTrace(XZSTC_DEBUG1,"Nom du data Group a joindre : %s",vl_datagroup);
   	
   	if (!XDG_JoindreDG(vl_datagroup))
   	{
                XZST_03EcritureTrace(XZSTC_FATAL,"%s Echec XDG_JoindreDG(%s)",version,vl_datagroup);               
		XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s",version);
		aeqa_sortir();
	}
	
	/*A
	** Initialisation des callbacks. 
	*/
	if (TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_AEQA),aeqa_calc,(T_CB_ARG)NULL) == NULL) 
	{	
                XZST_03EcritureTrace( XZSTC_FATAL,"%s : Callback sur XDM_AEQA non cree.",version );
		aeqa_sortir();
	}
		
	/*A
	** Attente bloquante sur les messages du RTserver.
	*/


	/*A Attente bloquante des messages du RTServer */
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
			else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
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

	/*for (;;)
	{
		if (!TipcSrvMainLoop(T_TIMEOUT_FOREVER) != T_ERR_CONN_EOF)
		{
                        XZST_03EcritureTrace(XZSTC_FATAL ,"%s : TipcSrvMainLoop a echoue  ",version);
			break;    
		}			   
	}*/

	XZST_03EcritureTrace( XZSTC_INFO, "%s Arret sur %s *********************",version,pl_NomSite);
}  /* Fin main */

/*X*/
/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant de sortir de la tache TAEQA
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void aeqa_sortir( )

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
*   fermeture du fichier de trace (XZST_02)
*
------------------------------------------------------*/
{
	static char *version = "$Id: aeqa_dir.c,v 1.3 1995/08/18 11:11:48 gesconf Exp $ : aeqa_sortir" ;
	int             vl_ValRet = XDC_OK;

	
	XZST_03EcritureTrace(XZSTC_FONCTION, "IN %s",version);

	/*A
	** Fermeture du fichier trace 
	*/
	if ( (vl_ValRet = XZST_02FermetureTrace() ) != XDC_OK )
	{
                XZST_03EcritureTrace(XZSTC_WARNING,"%s : XZST_02FermetureTrace a echoue",version);
	}
	XZST_03EcritureTrace( XZSTC_INFO, "TAEQA : Arret  *********************");
	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : %s : Sortie",version);
	/*A
	** Sortie 
	*/
	exit(0);
}  /* Fin aeqa_sortir */
