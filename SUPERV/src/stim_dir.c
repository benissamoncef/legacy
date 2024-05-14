/* Fichier : stim_dir.c
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_DIR * FICHIER stim_dir.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient la fonction de sortie de la tache,
* ainsi que le main de la tache.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  27 Sep 1994	: Creation
* T.Milleville  12 Mai 1995	: Ajout initialisation administartio
*	XZSA_20		V1.9 
* T.Milleville  25 Jul  1995	: Ajout commentaires V1.11
* T.Milleville  8 Aou  1995	: Ajout test erreur sur boucle RTworks
*			(T_ERR_TIMEOUT_REACHED)  V1.12
* T.Milleville  15 Sep  1995	: Redirection stdout et sdterr dans
*	dev/null V1.13
* T.Milleville  21 Sep  1995	: Ajout destruction connexion avec RTserver 
*	dans la fonction de sortie V1.14
*
* JPL	08/02/18 : Migration Linux 64 bits (DEM 1274) : declaration et appel des fonctions; type et valeur de retour  1.15
* JMG	07/03/20 : EMS 1.16
* JPL	11/10/22 : Rearmement signal; Suppression donnees de version  1.17
* JPL	11/10/22 : Controles d'echeance et reveil des taches clientes hors interruption (SAE-413)  1.18
* JPL	16/01/23 : Adaptation de la boucle principale a EMS  1.19
* JPL	16/01/23 : Support de trace des donnees internes sur signal utilisateur  1.20
* JPL	09/02/23 : Gestion des acces concurrents aux principales fonctions (SAE-458)  1.21
* KOU   25/09/2023	: Adaptation code pour rabbitmq DEM-483
*******************************************************/

/* fichiers inclus */
#include <rtworks/common.h>
#include <rtworks/ipc.h>
#include <xzems.h>

#include <xzsem.h>

#include "xzsa.h"

#include "stim_tra.h"

#define STIM_DONNEES
#include "stim_ser.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "stim_dir.c 1.21" ;


/* Indicateur de reception de signal d'echeance de temporisation */
static	XDY_Booleen	vm_Temporisation_Ecoulee = XDC_FAUX;

static	XDY_Booleen	vm_Demande1_Utilisateur = XDC_FAUX;

static	sigset_t	vm_maskAlarm;


/* declaration de fonctions internes */

/* definition de fonctions externes */



/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction marquant la reception d'un signal
 *   manuel de la part d'un intervenant externe
 *
 *  Executee sur reception du signal USR1.
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void sm_Notifier_Demande1_Utilisateur ( int va_signal )
{
	vm_Demande1_Utilisateur = XDC_VRAI;
}



/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction marquant la reception d'un signal
 *   d'echeance de temporisation
 *
 *  Executee sur declenchement du signal ALRM.
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void sm_Notifier_Echeance_Tempo ( int va_signal )
{
	sigprocmask (SIG_BLOCK, &vm_maskAlarm, (sigset_t *) NULL);
	vm_Temporisation_Ecoulee = XDC_VRAI;
	sigprocmask (SIG_UNBLOCK, &vm_maskAlarm, (sigset_t *) NULL);
}



/*------------------------------------------------------
 * SERVICE RENDU :
 *  Fonction verifiant si une notification d'echeance
 *  de temporisation a ete recue
 *
 *  Si c'est le cas, reveille les taches dont 
 *  l'heure programmee est atteinte ou depassee.
 *------------------------------------------------------
 * SEQUENCE D'APPEL :
 */

void sm_Controler_Temporisation ( )

/*
 * ARGUMENTS EN ENTREE :
 *   aucun
 *
 *
 * ARGUMENTS EN SORTIE :
 *   aucun
 *
 *
 * CODE RETOUR :
 *   aucun
 *
 * CONDITION D'UTILISATION
 *
 * FONCTION
 *   Inhibition du signal d'alarme
 *
------------------------------------------------------*/
{
	XDY_Booleen   vl_Tempo_Ecoulee;

	/* Consulter l'indicateur de notification de temporisation ecoulee */
	sigprocmask (SIG_BLOCK, &vm_maskAlarm, (sigset_t *) NULL);
	vl_Tempo_Ecoulee = vm_Temporisation_Ecoulee;
	vm_Temporisation_Ecoulee = XDC_FAUX;
	sigprocmask (SIG_UNBLOCK, &vm_maskAlarm, (sigset_t *) NULL);

	/* Verifier l'echeance des reveils si la temporisation est ecoulee */
	if ( vl_Tempo_Ecoulee == XDC_VRAI )
	{
		sem_wait(&vg_semaphore);
		sm_Reveiller_Signal (SIGALRM);
		sem_post(&vg_semaphore);
	}
}



/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_DIR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Fonction de sortie de la tache TSTIME. Liberation des
* ressources prises par les demandes de temporisations
*
*
******************************************************
* SEQUENCE D'APPEL :
*/

void	sm_Sortir(void)

/*
******************************************************/
{
	SMT_ENRG_TIMER 		*pl_Tempo	= NULL;
	SMT_ENRG_TIMER		*pl_Tempo_A_Liberer = NULL;
	struct itimerval	vl_Prog ;
	struct sigaction	vl_sig;

XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Sortir  : IN \n");

XZST_03EcritureTrace(XZSTC_INFO,"sm_Sortir IN : suppression des reveils");

	/* Inhibe le signal d'alarme */
	vl_sig.sa_handler = SIG_IGN;
	vl_sig.sa_flags = 0;
	sigemptyset( &vl_sig.sa_mask );
	sigaction ( SIGALRM, &vl_sig, NULL );

	/*A stoppe la tempo en cours */
	vl_Prog.it_interval.tv_sec  = 0 ;
	vl_Prog.it_interval.tv_usec = 0 ;
	vl_Prog.it_value.tv_sec  = 0 ;
	vl_Prog.it_value.tv_usec = 0 ;
	setitimer(ITIMER_REAL,&vl_Prog,(struct itimerval *)NULL) ;

	/*A libere les temporisations */
	for (pl_Tempo = pg_ListeTempo ; pl_Tempo != NULL ; free(pl_Tempo_A_Liberer))
	{
    		pl_Tempo_A_Liberer = pl_Tempo ;
    		pl_Tempo           = pl_Tempo->Suivant ;
	}

	XZST_03EcritureTrace(XZSTC_INFO,"sm_Sortir OUT : Sortie demandee");
	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Sortir  : OUT \n");

	/* On detruit la connexion completement */
	if (!TipcSrvDestroy(T_IPC_SRV_CONN_NONE))
	{
		XZST_03EcritureTrace(   XZSTC_WARNING, "Erreur deconnexion serveur");
	}
	exit(0);

}	/* sm_Sortir */




/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_DIR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Main de la tache tstim.x
*
******************************************************
* SEQUENCE D'APPEL :
*/

int main( 	int	argc,
	char *  argv[] )

/*
* PARAMETRES EN ENTREE : argv[0]= nom de l'application RTWorks
*
* PARAMETRES EN SORTIE : neant
*
* VALEUR(S) DE RETOUR : neant
*
* CONDITION D'ERREUR : non connection au RTServer
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	XZSCT_Datagroup		pl_Dg		= "\0";
	struct sigaction	vl_sig;
	T_IPC_MSG		vl_Msg;
	int			vl_IndPre	= 0;
	int			vl_errno	= 0;


	/*A Initialisation du pointeur de debut
	*	de liste chainee pour les tempos
	**/
	pg_ListeTempo = NULL ;
	errno=0;

	/*A
	*  initialisation semaphore
	*/
	sem_init(&vg_semaphore, 0, 1);

	/* Masque des signaux a bloquer durant l'execution de la fonction d'interruption */
	sigemptyset( &vm_maskAlarm );
	sigaddset( &vm_maskAlarm, SIGUSR1 );
	sigaddset( &vm_maskAlarm, SIGALRM );

vl_sig.sa_handler = sm_Notifier_Demande1_Utilisateur;
vl_sig.sa_flags = 0;
vl_sig.sa_mask = vm_maskAlarm;
sigaction( SIGUSR1, &vl_sig, NULL );

	/* Mise en place des traitements du signal d'alarme pour les Timers */
	vl_sig.sa_handler = sm_Notifier_Echeance_Tempo;
	vl_sig.sa_flags = 0;
	vl_sig.sa_mask = vm_maskAlarm;
	sigaction( SIGALRM, &vl_sig, NULL );

	/*A
	*	Connexion a l'application MIGRAZUR */
	if (XZSC_01InitCnxSup(argv[1]
		 ,argv[0]
		 ,XZSCC_INTERDIRE /* mise a jour horodate*/
		 ,XZSCC_AUTORISER /* systeme trace */
		 ,XZSCC_INTERDIRE /* synchro tsetat */
		 ,sm_Sortir       /* fontion de sortie */
		 ,"/dev/null"     /*        redirection sortie standard */
		 ) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_INFO,
				"STIM_DIR : Echec XZSC_01InitCnxSup");
		sm_Sortir();
	}

	/*A Reservation des ressources d'administration */
      	XZSA_20InitAdmin();

	/*A Quelle est le nom de ma machine */
	XZSC_07NomMachine(vg_NomMachine) ;

	/*A Calcul du nom du data group */
	XDG_EncodeDG2(pl_Dg,XDG_NSUPTIME,vg_NomMachine);

	/*A Jonction du data group de reception des messages */
	XDG_JoindreDG(pl_Dg);

	/*B joint les points d'entrees de mes services */
	TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TST_PROG),
		(T_IPC_CONN_PROCESS_CB_FUNC)sm_Reveiller_Cb,(T_CB_ARG)NULL) ;

	TipcSrvProcessCbCreate(XDM_IdentMsg(XDM_TST_ANNU),
		(T_IPC_CONN_PROCESS_CB_FUNC)sm_Annuler_Cb,(T_CB_ARG)NULL) ;

	/*A demande des reveil deja programmes */
	XDG_EncodeDG2(pl_Dg,XDG_NTIMEINIT, vg_NomMachine);
	if (!TipcSrvMsgWrite(pl_Dg,
     		XDM_IdentMsg(XDM_TST_TINIT),
     		XDM_FLG_SRVMSGWRITE,
     		NULL))
	{
   		XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur redemande des programmations en cours");
	}

	XZST_08ForceNivVisuTraceLocal(XZSTC_INFO|XZSTC_MESSAGE|XZSTC_INTERFACE|XZSTC_FONCTION|XZSTC_DEBUG1,XZSTC_NECRAN);

	/* Attente bloquante des messages EMS + signaux */
	for (;;)
	{
		//XZST_03EcritureTrace (XZSTC_WARNING, "Attente message");
		errno = 0;
		vl_Msg = TipcSrvMsgNext(T_TIMEOUT_FOREVER);

		// if (vl_Msg != NULL)
		if(isMessageNotNULL(vl_Msg))
		{
			TipcSrvMsgProcess(vl_Msg);
			TipcMsgDestroy(vl_Msg);

			/* Memorisation Test Presence RTserver */
			vl_IndPre = 0;
		}

		else
		{
#ifndef _TIBCO_EMS
			if ( (TutErrNumGet() != T_ERR_CONN_EOF) &&
				(TutErrNumGet() != T_ERR_TIMEOUT_REACHED) )
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
					XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage et arret tache tstim  %d \n",vl_IndPre);
					/* Arret de la tache */
					exit(0);
				}

				/*B INdication Non Presence RTserver */
				if (errno)
					vl_IndPre++;
			}
			vl_errno = errno;

			/* Test Etat d'erreur */
			if ( (vl_errno != 0)  &&  (vl_errno != EINTR) )
			{
				if (vl_IndPre == 0)
				{
					XZST_03EcritureTrace (XZSTC_WARNING, "Erreur RTWorks %d -- errno=%d",
					                         TutErrNumGet(), vl_errno);
				}
				else if (vl_IndPre >= XDC_EXIT_RTCLIENT)
				{
					XZST_03EcritureTrace (XZSTC_WARNING, "Bouclage en erreur, errno=%d", vl_errno);

					/* Arret de la tache */
					//sm_Sortir();
				}

				/* Indication d'Etat d'erreur */
				vl_IndPre++;
			}
			else
			{
				vl_IndPre = 0;
			}
#else

			errno = 0;
#endif
		}

		if (vm_Demande1_Utilisateur == XDC_VRAI)
		{
			vm_Demande1_Utilisateur = XDC_FAUX;
			sm_dump (0);
		}

		/* Verifier l'echeance d'une temporisation et traiter les demandes de reveil echues */
		sm_Controler_Temporisation ( );

	} /* fin for infinie */

	return (0);
}	/* fin du main */

/* fin du fichier */
