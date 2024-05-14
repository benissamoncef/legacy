/*E*/  
/*Fichier : $Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $	      Release : $Revision: 1.16 $        Date : $Date: 2011/01/13 10:02:14 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  SUPERV
------------------------------------------------------
* MODULE TSDIS * FICHIER tsdis_dir.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Permet de tester cycliquement si un disque Miroir 
*	est arrete 
*
------------------------------------------------------
* HISTORIQUE :
*
* T.Milleville	12 Jan 1995	: Creation
* T.Milleville	06 Mar 1995	: Verification des pertes memoires V1.5
* T.Milleville	10 Mar 1995	: Modification pour SCCS V1.6
* T.Milleville	12 Mai 1995	: Ajout initialisation des Timers V1.7
* T.Milleville	12 Juin 1995	: Modification de la fermeture du fixhier
*	lors de la commande de verification dataserver V1.8
* T.Milleville	30 Juin 1995	: Modification de la boucle de traitement des msg RTworks V1.9
* T.Milleville	20 Jul 1995	: Ajout commentaires V1.10
* T.Milleville	25 Jul 1995	: Ajout commentaires et de la constante XDC_EXIT_RTCLIENT V1.11
* T.Milleville	21 Sep 1995	: Redirection stdout et sdterr dans /dev/null V1.13
* T.Milleville	27 Sep 1995	: Suppression attente lors d'erreur RTWorks V1.14
* P.Niepceron	25 Jan 1995	: Modif des grep en egrep pour comprendre le francais et l'anglais v1.15
* JPL		13/01/11	: Migration architecture HP ia64 (DEM 971) : exit sur echec de l'initialisation  1.16
------------------------------------------------------*/

/* fichiers inclus */

#include "tsdis_dir.h"

#include "xdc.h"
#include "xdl.h"
#include "xdm.h"
#include "xdg.h"
#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzsm.h"
#include "xzss.h"
#include "xzse.h"


/* definitions de constantes */



#define vl_PV_Disk	"/etc/pvdisplay "
#define vl_VG_Disk	"/etc/vgdisplay "
#define C_GREP_DISPO	"| egrep \"non disponible|unavailable\""	/* modif 1.15 */
/*#define C_GREP_DISPO	"| grep \"unavailable \""*/
#define C_GREP_NOM	"| egrep \"Nom VG|Name \""
/*#define C_GREP_NOM	"| grep \"Name\""*/


/*-- GESTION DES DISQUES MIROIRS ---- */
	/*Permet de recuperer les drivers de Disk sur les cartes
	*		scsi externes **/
#define C_Disk_Phy	"/etc/ioscan -f -C disk | grep \"scsi3\""


	/* Toutes les 30 minutes */
#define C_PER_TESTDISQUE	(30 * 60)	

	/* Cle de programmation Timer */
#define C_CLE_TESTDISQUE	"Cle_Test_Disques"


#define vl_SYNC_Disk	"/etc/vgsync  "

#define C_DISK_MODEL	"/dev/dsk/cXd0s2"



/*---------------------------------------------*/


/* --- GERSTION DE LA TACHE SQL SERVER ----- */
	/* Nom de la Tache DataServer */
#define	C_TACHE_DATASRV	"/produits/sybase/bin/dataserver"

	/* Toutes les minutes */
#define C_PER_DATASRV	(60)	

	/* Cle de programmation Timer */
#define C_CLE_DATASRV	"Cle_Test_DataSrv"


/*---------------------------------------------*/



/* definitions de types locaux */
typedef  struct {
	char 	NomDisque[20];
	int	EtatDisponible; } tg_InfoDisk;

typedef struct {
	int NumMsg;      /* Numero message RTW */
	void (*FoncCB)(); /* Fonction callback */
		} tm_callback;

typedef char tg_NomVG[20];



/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_dir" ;


tm_callback pm_TabCB[] = {
	{ XDM_TST_TESTDISK, (void (*)())tsdis_test_pv_cb }, 
	{ XDM_TSE_SYNCDISK, (void (*)())tsdis_sync_cb } ,
	{ XDM_TST_DATASRV, (void (*)())tsdis_datasrv_cb } };


	/* Pointeur de memorisation des Volumes Groupes */
tg_NomVG	* ps_VG	= NULL;

	/* Nombre de Volumes groupes */
int 	vg_Nb_VG	= 0;

	/* Pointeur de memorisation des Disques Physiques */
tg_InfoDisk  * ps_DISK	= NULL;

	/* Nombre de disques physiques */
int	vg_Nb_DISK	 = 0;


	/* Nom du Site Local */
XZSCT_NomSite	vg_NomSite	= "";



/* declaration de fonctions internes */
int tsdis_init();
int tsdis_sync();
int tsdis_datasrv();





/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction principale de la tache tsdis.x 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

main(int argc, char *argv[])

/*
* ARGUMENTS EN ENTREE :
*   argc	: Nb arguments
*   argv        : Valeurs des arguments
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
*   Connexion au Noyau de Supervision, puis a la base de donnees
*	Abonnement au datagroup, armement des callbacks 
*	Decelenchement des Timers pour tester les disques miroirs
*	et la presence du Dataserver
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : main " ;
	int 		vl_ValRet	= XDC_OK;
	int             vl_nCB          = 0;
	T_IPC_MT       	vl_mt           = 0;
	XZSCT_Datagroup	pl_DataGroup	= "";
	XZSCT_NomMachine	vl_NomMachine	= "";
	int                     vl_IndPre       = 0;
	T_IPC_MSG               pl_Msg;
	int			vl_Attente	= 0;



	XZST_03EcritureTrace(XZSTC_FONCTION," Main tsdis_dir : IN \n");
	
	/*A connexion au noyau de supervision */
	vl_ValRet = XZSC_01InitCnxSup(argv[1]
	 	,argv[0]
		,XZSCC_INTERDIRE /* mise a jour horodate*/
		,XZSCC_AUTORISER /* systeme trace */
		,XZSCC_INTERDIRE /* synchro tsetat */
		,(void (*)())NULL /* fontion de sortie */
		,"/dev/null"     /* redirection sortie standard */
		);

	if (vl_ValRet != XDC_OK)
	{
	  	XZST_03EcritureTrace(XZSTC_FATAL,
	  		"erreur XZSC_01InitCnxSup cr = %d",vl_ValRet);
	  	exit(1);
	}


	/*A Reservation des ressources d'administration */
	XZSA_20InitAdmin();

	/*A Initialisation services timer */
      	XZSM_00InitServices ();

	/*A Recuperation du Site de la Machine */
	XZSC_04NomSite(vg_NomSite);

	/*A Joint le Datagroup NSUPDISK_NomSite */
	XDG_EncodeDG2(pl_DataGroup, XDG_NSUPDISK, vg_NomSite);
	if (!XDG_JoindreDG(pl_DataGroup))
	{
		/* erreur abonnement */
	    	XZST_03EcritureTrace(XZSTC_WARNING,
		  " erreur abonnement au DG %s", pl_DataGroup);
	}


	/*A Initialisation des callbacks de la tache */
	for (vl_nCB = 0; 
		vl_nCB < (sizeof(pm_TabCB)/sizeof(pm_TabCB[0])); vl_nCB++)
	{
		vl_mt = XDM_IdentMsg(pm_TabCB[vl_nCB].NumMsg);
		if(!TipcSrvProcessCbCreate(vl_mt,
			pm_TabCB[vl_nCB].FoncCB, (T_CB_ARG)NULL))
	    	{
	  		XZST_03EcritureTrace(XZSTC_WARNING,
			   "main : Erreur armement du callback %d",vl_nCB);
		}
	}

	/*A Recuperation des Informations sur les disques */
	tsdis_init();

	/*A Recuperation du Nom de la Machine */
	XZSC_07NomMachine(vl_NomMachine);

	/*A Construction du Datagroup pour la reception du message 
	*	de tests de disques miroirs */
	XDG_EncodeDG2(pl_DataGroup,vl_NomMachine,argv[0]);

	if (XZSM_01DemanderProgrammation(0, C_PER_TESTDISQUE,
		pl_DataGroup, XDM_TST_TESTDISK, 
		C_CLE_TESTDISQUE, 0) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,
	      		"main : Erreur demande programmation Test Disque\n");
	}

	/*A Construction du Datagroup pour la reception du message 
	*	de test  de la Tache SQL Server */
	XDG_EncodeDG2(pl_DataGroup,vl_NomMachine,argv[0]);

	if (XZSM_01DemanderProgrammation(0, C_PER_DATASRV,
		pl_DataGroup, XDM_TST_DATASRV, 
				C_CLE_DATASRV, 0) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING ,
	      		"main : Erreur demande programmation Data Srv\n");
	}

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
#if 0
			sleep(vl_Attente);
#endif
		}
	}

}	/* Fin main */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de recuperer les noms des Volumes Groupes et
*	des disques 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int tsdis_init()

/*
* ARGUMENTS EN ENTREE :
*   Aucun
*
*
* ARGUMENTS EN SORTIE :
*   Aucune
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*
* CONDITION D'UTILISATION
*   Tache avec privilege Super-User
*
* FONCTION 
*   Recupere les Noms des Volumes Groupes et des disques
*	physiques
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_init " ;
	char	 vl_LigneCmd[XDC_LINE_CMD_SIZE]	= "";
	FILE	* pl_cr	= NULL;
	char    vl_Buf1[80]      = "";
	char    vl_Buf2[80]      = "";
	char    vl_NumDisk[2] 	= "";
	int	vl_ValRet	= XDC_OK;
	int	i 	= 0;
	tg_NomVG	vl_VG	= "";



	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_init: IN \n");
	

	/*A Allocation de la memoire pour les VG */		
	ps_VG = malloc ( sizeof(tg_NomVG));

	/*A Construction de la Chaine de Commanes */
	sprintf(vl_LigneCmd,"%s %s",vl_VG_Disk,C_GREP_NOM);

	/*A Recuperation des Volumes Groupes */
	if ( (pl_cr = popen(vl_LigneCmd, "r")) != NULL )
	{
		while (fscanf( pl_cr, "%s %s %s\n",vl_Buf1,vl_Buf2,vl_VG) != EOF )
		{
			/*A Alors Erreur , Pas de VG  */
			XZST_03EcritureTrace(XZSTC_INFO,
				"Volume Group %s \n",vl_VG);
			XZST_03EcritureTrace(XZSTC_WARNING,"Volume Group %s \n",vl_VG);

			/*A Affectation a la Structure du VG */
			ps_VG = realloc (ps_VG,
				((size_t) vg_Nb_VG+1) * sizeof(tg_NomVG));
			strcpy(ps_VG[vg_Nb_VG],vl_VG);
			vg_Nb_VG++;
		}
	}

	/*A Fermeture popen */
	pclose(pl_cr);

	/*A  Allocation de la memoire pour les disques physiques */		
	ps_DISK = malloc ( sizeof(tg_InfoDisk));

	/*A Construction de la Chaine de Commanes */
	strcpy(vl_LigneCmd,"");
	sprintf(vl_LigneCmd,"%s",C_Disk_Phy);

	/*A Recuperation des Volumes Groupes */
	if ( (pl_cr = popen(vl_LigneCmd, "r")) != NULL )
	{
		while (fscanf( pl_cr, "%s %s %[ /_().a-zA-Z0-9]s\n",vl_Buf1,vl_NumDisk,vl_Buf2) != EOF )
		{
			XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Lecture du disque physique suivant \n");

			/*A Alors Erreur , Pas de VG  */
			XZST_03EcritureTrace(XZSTC_WARNING,
				"Numero Disk %s \n",vl_NumDisk);

			/*A Affectation a la Structure du InfoDisk */
			ps_DISK = realloc (ps_DISK,
				((size_t) vg_Nb_DISK+1) * sizeof(tg_InfoDisk));

			strcpy(ps_DISK[vg_Nb_DISK].NomDisque,C_DISK_MODEL);
			ps_DISK[vg_Nb_DISK].NomDisque[10] = vl_NumDisk[0];
			ps_DISK[vg_Nb_DISK].EtatDisponible = XDC_OK;
			vg_Nb_DISK++;

			XZST_03EcritureTrace(XZSTC_WARNING,
			"Nom Disque %s \n",ps_DISK[vg_Nb_DISK-1].NomDisque);
		}


		XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Fin lecture des disques physiques, fermeture  \n");
		errno = 0;
		/*A Fermeture popen */
		if (pclose(pl_cr) != 0)
		{
			XZST_03EcritureTrace (XZSTC_WARNING,
				" pb au pclose %d \n ", errno);
		}
	}


	XZST_03EcritureTrace(XZSTC_DEBUG3,
			"Fin lecture des disques physiques  \n");

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_init: OUT \
		-- Code Retour %d \n",vl_ValRet);

	return (vl_ValRet);
}	/* Fin tsdis_init */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de tester l'etat des disques Miroirs
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void tsdis_test_pv_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments d'une callback
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
*   Declenche sur reception du msg XDM_TST_TESTDISK 
*
* FONCTION 
*   Teste l'etat des disques physiques.Si au moins un des deux 
*	disques est indisponibles, alors enregistrement dans le
*	journal d'administration, si les 2 alors arret de 
*	l'application. Lorsque le disque est de nouveau disponible
*	alors on lance la synchronisation.
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_test_pv" ;
	char	 vl_LigneCmd[XDC_LINE_CMD_SIZE]	= "";
	int	vl_Nb_Indispo	= 0;
	int	i	= 0;
	FILE	* pl_cr	= NULL;
	char    vl_Buf[80]      = "";
	int	vl_ValRet	= XDC_OK;


	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_test_pv_cb: IN \n");

/*A Verification si au moins un disque indisponible */
for (i=0;i< vg_Nb_DISK;i++)
{
	/*A Construction de la Chaine de Commanes */
	sprintf(vl_LigneCmd,"%s %s %s",vl_PV_Disk,ps_DISK[i].NomDisque,C_GREP_DISPO);


	XZST_03EcritureTrace(XZSTC_WARNING," Cmd %s \n",
		vl_LigneCmd);
	

	if ( (pl_cr = popen(vl_LigneCmd, "r")) != NULL )
	{
		if (fscanf( pl_cr, "%s\n", vl_Buf ) != EOF )
		{
			/*A Alors Erreur , disque indisponible */
			XZST_03EcritureTrace(XZSTC_WARNING,
				"Disque %s indisponible !!\n",
				ps_DISK[i].NomDisque);

			/*B Memorisation dans le Journal D'administration */
			XZST_10ArchiverMsgSyst(XDL_SUP59_DSKNOK,ps_DISK[i].NomDisque,vg_NomSite,NULL);		
			/*B Memorisation Disque Indisponible */
			ps_DISK[i].EtatDisponible = XDC_NOK;

			/*B On incremente le Nb de disques indisponibles */
			vl_Nb_Indispo++;	
		}
		else
		{
			/*B verification de l'etat precedent 
			*	du Disque */
			if (ps_DISK[i].EtatDisponible == XDC_NOK)
			{
				/*B Alors disque de nouveau OK */
				ps_DISK[i].EtatDisponible = XDC_OK;

				/*B Memorisation dans le Journal D'administration */
				XZST_10ArchiverMsgSyst(XDL_SUP61_DSKOK,ps_DISK[i].NomDisque,vg_NomSite,NULL);

				/*B Lancement de la Synchonisation */
				tsdis_sync();
			}
		}
	}

	/*A fermeture popen */
	pclose(pl_cr);
}

/*A Teste si tous les disques indisponibles */
if ( (vl_Nb_Indispo == vg_Nb_DISK) && (vg_Nb_DISK != 0) )
{
	/*B Memorisation dans Journal */
	XZST_10ArchiverMsgSyst(XDL_SUP60_ALLDSKNOK,vg_NomSite,NULL);

	XZST_03EcritureTrace(XZSTC_WARNING," Tous les disques du Serveur \
			sont indisponibles !!! \n");
}

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_test_pv_cb : OUT \n");

}	/* Fin tsdis_test_pv_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de declencher la synchronisation Manuelle
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void tsdis_sync_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments de Callback
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
*   Decelenche par une demende manuelle de l'outil d'administration
*	sur reception du msg XDM_TSE_SYNCDISK
*
* FONCTION 
*   Appel la fonction de Synchronisation des disques 
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_sync_cb " ;

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_sync_cb : IN \n");

	/*A Synchronisation des disques miroirs */
	tsdis_sync();


	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_sync_cb : OUT \n");
}	/* Fin tsdis_sync_cb */




/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de synchroniser les disques miroirs
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int tsdis_sync()

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
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Execute la commande systeme de synchronisation des disques
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_sync " ;
	int	vl_ValRet	= XDC_OK;
	char	 vl_LigneCmd[XDC_LINE_CMD_SIZE]	= "";
	FILE	* pl_cr	= NULL;
	int	vl_Comp	= 0;

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_sync: IN \n");


/*A Synchronisation des divers Volumes GRoupes */
for (vl_Comp=0; vl_Comp<vg_Nb_VG; vl_Comp++)
{
	/*A Construction de la Chaine de Commanes */
	sprintf(vl_LigneCmd,"%s %s",vl_SYNC_Disk,ps_VG[vl_Comp]);

	/*A Execution de la commande */
	if ( (pl_cr = popen(vl_LigneCmd, "r")) != NULL )
	{
		/*A Synchronisation terminee */
		XZST_10ArchiverMsgSyst(XDL_SUP62_SYNCOK,vg_NomSite,NULL);
	}

	/*B fermeture popen */
	pclose(pl_cr);
}

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_sync: OUT \
		-- Code retour %d \n", vl_ValRet);

	return (vl_ValRet);


}	/* Fin tsdis_sync */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de ceclencher le basculement des serveurs d'un Site
*	si on detecte que le SQL Serveur associe n'est 
*	plus present
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int tsdis_datasrv()

/*
* ARGUMENTS EN ENTREE :
*   < Valeur et descriptif des parametres en entree >
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   OK : ok
*
* CONDITION D'UTILISATION
*   Aucun
*
* FONCTION 
*   Teste si SQL sreveur est present. Sinon on memorise dans le 
*	Journal D'administration et on declenche le Basculement
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_datasrv " ;
	int	vl_Pid	= 0;
	XZSCT_NomSite	vl_NomSite	= "";
	int		vl_ValRet	= XDC_OK;



	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_datasrv: IN \n");

	/*A Verification de l'existance de la Tache */
	if ( (vl_Pid = XZSS_20PidTache(C_TACHE_DATASRV)) == 0)
	{
		/*B Enregistrement dans Journal D'administartion */
		XZST_10ArchiverMsgSyst(XDL_SUP63_SRVNOK,vg_NomSite,NULL);		
		/*B Tache Non Presente donc Basculement */
		vl_ValRet = XZSE_05BasculementSC( vg_NomSite );
		if ( vl_ValRet == XDC_NOK )
			XZST_03EcritureTrace( XZSTC_WARNING,
				"Pb Basculement Serveur du Site %s \n",
				vg_NomSite);
	}

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_datasrv: OUT i- code retour %d\n",
			vl_ValRet);

	return (vl_ValRet);

}	/* Fin tsdis_datasrv */



/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de declencher la surveillance de la tache SQL Server
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void tsdis_datasrv_cb(
	T_IPC_CONN va_Conn,
	T_IPC_CONN_PROCESS_CB_DATA pa_Data,
	T_CB_ARG pa_Arg)

/*
* ARGUMENTS EN ENTREE :
*   Arguments de Callback
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
*   Decelenche sur reception du msg XDM_TST_DATASRV
*
* FONCTION 
*   Appel la fonction de surveillance de la tache SQL Server*
*
------------------------------------------------------*/
{
	static char *version="$Id: tsdis_dir.c,v 1.16 2011/01/13 10:02:14 gesconf Exp $ : tsdis_datasrv_cb " ;

	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_datasrv_cb : IN \n");

	/*A Surveillance tache SQL Server */
	tsdis_datasrv();


	XZST_03EcritureTrace(XZSTC_FONCTION," tsdis_datasrv_cb : OUT \n");

}	/* Fin tsdis_sync_cb */

