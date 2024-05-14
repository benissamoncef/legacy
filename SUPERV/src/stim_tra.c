/* Fichier : stim_tra.c
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_TRA * FICHIER stim_tra.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les fonctions d'entree de la tache.
* Demande de programmation, demande d'annulation de 
* programmation, reveil sur signal UNIX.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  27 Sep 1994	: Creation
* T.Milleville  5 Mai 1995	: Ajout teste d'existance du Timer 
*	dans la callback d'insertion d'un Timer V1.6
* T.Milleville  5 Jul 1995	: Ajout recuperation priorite dans le
*	msg TST_PROG V1.7
* T.Milleville  10 Jul 1995	: Ajout traces + liberation memoire
*	V1.8
* T.Milleville  25 Jul 1995 	: Ajout commentaires V1.10
* T.Milleville  6 Aou  1995 	: Ajout traces V1.11
* T.Milleville  22 Sep 1995 	: Modification datagroup pour envoi 
*	suppression Timer NSUPTIME a la place TIMEINIT V1.13
* T.Milleville  27 Sep 1995 	: Annulation de la modification precedente
*	On enoie le message de suppression des Timers dans le datagroup 
*	associe au declenchement du Timer V1.14
*		V1.14
* T.Milleville  02 Nov 1995 	: Annulation du Time memorise si 
*	le delai = 0 avant l'envoi du msg de declenchement V1.15
* JPL		16/12/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.16
* JMG	07/03/20 : EMS 1.17
* LCL		20/12/21	: MOVIS P2 ems SAE-312
* JPL		06/10/22	: Suppression donnees de version; IDEM PRECEDENTE  1.19
* JPL		10/10/22 : Reprise parcours complet liste des Tempos si modifiee; liberation memoire  1.20
* JPL		16/01/23 : Ajout fonction de trace (dump) des donnees internes (timer, liste Tempos)  1.21
* JPL		09/02/23 : Gestion des acces concurrents aux principales fonctions (SAE-458)  1.22
*******************************************************/


/* fichiers inclus */
#include <sys/time.h>

#include <xzsem.h>
#include "stim_tra.h"
#include "stim_ser.h"


/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "stim_tra.c 1.22" ;


/* declaration de fonctions internes */
void sm_envoyer( SMT_ENRG_TIMER		*pl_Timer );

/* definition de fonctions externes */



/*
------------------------------------------------------
* SERVICE RENDU : 
*  Fonction permettant d'envoyer un message XDM_TST_TSUP
*	afin de supprimer le Timer memorise
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int sm_Sup_Memorise(
	char *	pa_Cle,
	char *  pa_Dg,
	int     va_IdMsg )

/*
* ARGUMENTS EN ENTREE :
*  pa_Cle : Cle du Timer que l'on veut supprimer
*  pa_Dg  : Datagroup du Timer que l'on veut supprimer
*  va_IdMsg : Identificateur du message que l'on veut supprimer
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK : 		ok
*   XZSMC_RTW_INV : 	appel(s) a RT-WORKS infructueux
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Envoi du message XDM_TST_TSUP sur le datagroup XDG_NTIMEINIT.
*
------------------------------------------------------*/
{
	XZSCT_Datagroup pl_NomDg	= "\0";
	int 		vl_ValRet	= XDC_OK;

	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Sup_Memorise : IN -- Cle %s -- Dg %s -- Idmsg %d",
	                        pa_Cle,pa_Dg, va_IdMsg );

	/*A Envoi du message de Suppression du Timer memorise */
	if (!TipcSrvMsgWrite(pa_Dg
			,XDM_IdentMsg(XDM_TST_TSUP)
			,XDM_FLG_SRVMSGWRITE
			,T_IPC_FT_STR,pa_Cle
			,T_IPC_FT_STR,pa_Dg
			,T_IPC_FT_INT4,va_IdMsg
			,NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING, "erreur envoi message XDM_TST_TSUP");

		vl_ValRet = XZSMC_RTW_INV;
	}

	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Sup_Memorise : OUT -- Code retour %d", vl_ValRet );

	return (vl_ValRet);

}	/* Fin sm_Sup_Memorise */




/*
*------------------------------------------------------
* SERVICE RENDU : 
*
*  Permet d'envoyer un msg avec priorite
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sm_envoyer( SMT_ENRG_TIMER		*pl_Timer )

/*
* ARGUMENTS EN ENTREE :
*   pl_Timer : pointeur sur valeur TIMER
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
* CODE RETOUR : 
*   Aucun
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Reuprere la valeur du Datagroup, IdMsg et priorite et envoie le 
*	msg
*
------------------------------------------------------*/
{
	T_IPC_MSG vl_Msg;

	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_envoyer : IN -- Timer dg %s -- cle %s -- priorite %d -- Idmsg %d",
	                        pl_Timer->Dg,pl_Timer->Cle,pl_Timer->Priorite,pl_Timer->IdMsg);

      if (!TipcSrvMsgWrite (pl_Timer->Dg,
                            XDM_IdentMsg (pl_Timer->IdMsg),
                            XDM_FLG_SRVMSGWRITE,
                            T_IPC_FT_STR, pl_Timer->Cle,
                            NULL))
	 {
		XZST_03EcritureTrace(XZSTC_WARNING, "Pb creation msg %d",pl_Timer->IdMsg);
	 }

	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_envoyer : OUT");

}	/* Fin sm_envoyer */




/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* Traitement des temporisations en cours. Cette fonction
* peut etre appelee par la gestin d'un timer ou par 
* le signal SIGALRM.
*
*	principe : 
*
*	- on determine en nb de secondes, l'heure actuelle
*	- toutes les demandes de tempo dont l'horodate est
*         egale ou anterieure sont declenchees
*	- le timer est a chaque appel reprogramme
*
******************************************************
* SEQUENCE D'APPEL :
*/

void		sm_Reveiller_Signal(int va_signal)
{
	struct itimerval	vl_Prog ;
	SMT_ENRG_TIMER *	pl_Tempo 		= NULL;
	SMT_ENRG_TIMER		vl_Timer ;
	int			vl_Instant_T,vl_Horodate,vl_jourSemaine = 0;

	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Reveiller_Signal : IN");

	/* Recuperation de l'heure */
	XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_jourSemaine,&vg_Instant_T) ;
	vl_Instant_T = (int)vg_Instant_T ;

	/* pour toutes les tempos existantes */
	for (pl_Tempo = pg_ListeTempo ; pl_Tempo != NULL ; )
	{
    		vl_Horodate = (int)pl_Tempo->Horodate ;

    		/* si la tempo courante est arrive a echeance */
    		if (vl_Horodate <= vl_Instant_T)
    		{
			XZST_03EcritureTrace(XZSTC_DEBUG1,
				"emission du msg %s vers %s (Id=%u) prio %d"
	 			,pl_Tempo->Cle
	 			,pl_Tempo->Dg
	 			,pl_Tempo->IdMsg
				,pl_Tempo->Priorite) ;
#if 1	/* Modif V1.15 */
			/*A Demande de suppression du Timer memorisee **/
			if (pl_Tempo->Delai  == 0)
			{
				sm_Sup_Memorise(pl_Tempo->Cle,
						pl_Tempo->Dg,
						pl_Tempo->IdMsg );
			}
#endif

       			/* emission du message demande */
       			sm_envoyer(pl_Tempo) ;

       			/* copie enregistrement */
       			memcpy(&vl_Timer,pl_Tempo,sizeof(vl_Timer)) ;

       			/* suppression de la demande de reveil */
       			sm_Supprimer_Liste(pl_Tempo->Cle
			 			,pl_Tempo->Dg
			 			,pl_Tempo->IdMsg) ;

       			/* si la tempo est cyclique */
       			if (vl_Timer.Delai != 0)
       			{
				/* Calcul de la prochaine echeance pour declencher le TIMER */
				while ( (int)vl_Timer.Horodate <= vl_Instant_T)
				{
	  				/* calcul la prochaine echeance */
	  				vl_Timer.Horodate = vl_Timer.Horodate + vl_Timer.Delai ;
				}

	  			/* insere de nouveau dans la liste des tempos a venir */
	  			sm_Inserer_Liste(&vl_Timer) ;
       			}
#if 0	/* Modif V1.15  */
			/* Demande de suppression du Timer memorisee */
			else
			{
				sm_Sup_Memorise(vl_Timer.Cle,
						vl_Timer.Dg,
						vl_Timer.IdMsg );
			}
#endif
			pl_Tempo = pg_ListeTempo ;
    		}
		else
		{
			pl_Tempo = pl_Tempo->Suivant ;
		}
	}

	/* progammation du prochain reveil */
	vl_Prog.it_value.tv_sec     = (pg_ListeTempo == NULL ? 0 : pg_ListeTempo->Horodate - vl_Instant_T) ;
	vl_Prog.it_value.tv_usec    = 0;
	vl_Prog.it_interval.tv_sec  = 0 ;
	vl_Prog.it_interval.tv_usec = 0 ;

	if (pg_ListeTempo == NULL)
	{
    		XZST_03EcritureTrace(XZSTC_INFO, "Plus de demande de temporisation pour le moment") ;
	}
	else
	{
   		XZST_03EcritureTrace(XZSTC_INFO,"Dans %us et %ums, emission du msg %s vers %s (Id=%u)"
	 		,vl_Prog.it_value.tv_sec,vl_Prog.it_value.tv_usec
	 		,pg_ListeTempo->Cle
	 		,pg_ListeTempo->Dg
	 		,pg_ListeTempo->IdMsg) ;
	}

	/*B armement du reveil */
	setitimer(ITIMER_REAL,&vl_Prog,(struct itimerval *)NULL) ;

	XZST_03EcritureTrace(XZSTC_FONCTION, "sm_Reveiller_Signal : OUT");

}	/* Fin sm_Reveiller_Signal */




/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Declenche sur reception du msg XDM_TST_PROG
* Point d'entree du service "programmation d'un reveil"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void	sm_Reveiller_Cb(
		T_IPC_CONN		      	va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
		T_CB_ARG		       pa_Arg )

/*
* PARAMETRES EN ENTREE :
*
*	va_cnx :	connection avec RTServer
*	pa_Data :	acces aux donnees du msg
*	pa_Arg :	Prive
*
******************************************************/
{
#ifndef _TIBCO_EMS
	T_REAL8		vl_Horodate 	= 0.0;
	T_INT4		vl_Delai,
				vl_IdMsg 	= 0;
	T_INT2		vl_Priorite	= 0;
	T_STR		vl_Dg 		= "";
	T_STR		vl_Cle 		= "";
#else
	XDY_Horodate	vl_Horodate 	= XZSMC_HORODATE_NUL;
	XDY_Entier		vl_Delai,
					vl_IdMsg 	= 0;
	XDY_Mot			vl_Priorite	= 0;
	XDY_Datagroup	vl_Dg 		= "";
	XDY_Texte		vl_Cle 		= "";
#endif
	double		vl_Instant_T 	= 0.0;
	SMT_ENRG_TIMER  vl_Timer ;
	int             vl_jourSemaine	= 0;
	int	vl_ValRet		= XDC_OK;
	SMT_ENRG_TIMER *pl_Enr 		= NULL;


	sem_wait(&vg_semaphore);

XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Reveiller_Cb IN : Demande de programmantion");

	/* Lecture du message */
	if (!TipcMsgRead(pa_Data->msg,	T_IPC_FT_REAL8,	&vl_Horodate,
				T_IPC_FT_INT4,	&vl_Delai,
				T_IPC_FT_STR,	&vl_Dg,
				T_IPC_FT_INT4,	&vl_IdMsg,
				T_IPC_FT_STR,	&vl_Cle,
				T_IPC_FT_INT2,	&vl_Priorite,
				NULL))
	{
   		XZST_03EcritureTrace(XZSTC_WARNING,"sm_Reveiller_Cb - pb lecture message");
   		XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Reveiller_Cb OUT");
		sem_post(&vg_semaphore);
   		return;
	}
	XZST_03EcritureTrace(XZSTC_MESSAGE,"Reception message TST_PROG Delai %d -- DG %s -- IDMsg %d -- Cle %s -- Priorite %d",
		vl_Delai, vl_Dg, vl_IdMsg, vl_Cle, vl_Priorite);

	/*B suppression de la demande de reveil */
	sm_Supprimer_Liste(vl_Cle, vl_Dg, vl_IdMsg);

	/*A Teste pour savoir si le Timer a deja ete declenche */
	for (pl_Enr = pg_ListeTempo ; pl_Enr != NULL ; 	pl_Enr = pl_Enr->Suivant)
	{
		/*B Teste si identifieur, Datagroup et Cle existent */
		if ( (pl_Enr->IdMsg == vl_IdMsg) &&
			(!strcmp(pl_Enr->Dg, vl_Dg)) &&
			(!strcmp(pl_Enr->Cle,vl_Cle)) )
		{
			vl_ValRet = XDC_NOK;
			break;
		}
	}

/*A Teste si Timer doit etre enregistre */
if (vl_ValRet == XDC_OK)
{
	/* si la demande est un delai */
	if (vl_Horodate == XZSMC_HORODATE_NUL)
	{
   		/* quel est l'horodate actuelle ? */
   		XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_jourSemaine,&vl_Instant_T) ;

   		/* calcul l'horodate du delai */
   		vl_Horodate = vl_Instant_T;
	}

	vl_Timer.Horodate = vl_Horodate ;
	vl_Timer.Delai    = vl_Delai ;
	vl_Timer.IdMsg    = vl_IdMsg ;
	vl_Timer.Suivant  = (SMT_ENRG_TIMER *)NULL ;
	strcpy(vl_Timer.Dg,vl_Dg) ;
	strcpy(vl_Timer.Cle,vl_Cle) ;
	vl_Timer.Priorite = vl_Priorite ;

	/* insertion et armement */
	sm_Inserer_Liste(&vl_Timer) ;

	/* traitement */
	sm_Reveiller_Signal(SIGALRM) ;
}
	XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Reveiller_Cb OUT");

	sem_post(&vg_semaphore);
	return;
}





/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_TRA
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Declenche sur reception du msg XDM_TST_ANNU
* point d'entree du service "Annulation de demande de 
* reveil"
*
******************************************************
* SEQUENCE D'APPEL :
*/

void	sm_Annuler_Cb(
		T_IPC_CONN			va_Cnx,
		T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
		T_CB_ARG			pa_Arg )
/*
* PARAMETRES EN ENTREE :
*
*	va_cnx :	connection avec RTServer
*	pa_Data :	acces aux donnees du msg
*	pa_Arg :	Prive
*
******************************************************/
{
#ifndef _TIBCO_EMS
	T_STR		vl_Dg 		= "";
	T_STR		vl_Cle 		= "";
	T_INT4		vl_IdMsg 	= 0;
#else
	XDY_Datagroup	vl_Dg 		= "";
	XDY_Texte		vl_Cle 		= "";
	XDY_Entier		vl_IdMsg 	= 0;
#endif


sem_wait(&vg_semaphore);

XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Annuler_Cb IN : Demande d'annulation de programmantion");

/* Lecture du message */
if (!TipcMsgRead(pa_Data->msg,	T_IPC_FT_STR,	&vl_Cle,
				T_IPC_FT_STR,	&vl_Dg,
				T_IPC_FT_INT4,	&vl_IdMsg,
				NULL))
{
   XZST_03EcritureTrace(XZSTC_WARNING,"sm_Annuler_Cb - pb lecture message");
   XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Annuler_Cb OUT");
   sem_post(&vg_semaphore);
   return;
}

	XZST_03EcritureTrace(XZSTC_MESSAGE,"sm_Annuler_Cb : Reception msg XDM_TST_ANNU cle %s -- datagrp %s -- Idmsg %d",
		vl_Cle, vl_Dg, vl_IdMsg);

/* Annulation de la temporisation */
sm_Supprimer_Liste(vl_Cle,vl_Dg,vl_IdMsg) ;

/* rearmer une eventuelle nouvelle tempo */
sm_Reveiller_Signal(SIGALRM) ;

XZST_03EcritureTrace(XZSTC_FONCTION,"sm_Annuler_Cb OUT");

sem_post(&vg_semaphore);
return;

}




void sm_dump (int va_param)
{
	struct itimerval	vl_timer;
	sigset_t		vl_sigmask;
	SMT_ENRG_TIMER *	pl_Tempo;

	getitimer (ITIMER_REAL, &vl_timer);
	XZST_03EcritureTrace(XZSTC_WARNING,"Timer %ld sec + %ld micros, intervalle %ld sec + %ld micros",
	                                vl_timer.it_value.tv_sec, vl_timer.it_value.tv_usec,
	                                vl_timer.it_interval.tv_sec, vl_timer.it_interval.tv_usec);

	sigprocmask (SIG_BLOCK, (sigset_t *) NULL, &vl_sigmask);
	XZST_03EcritureTrace(XZSTC_WARNING,"Signal SIGALRM bloque ? : %d", sigismember (&vl_sigmask, SIGALRM));

	if (pg_ListeTempo == NULL) {
		XZST_03EcritureTrace(XZSTC_WARNING,"Liste Tempo VIDE");
	} else {
		for ( pl_Tempo = pg_ListeTempo; pl_Tempo != NULL;  pl_Tempo = pl_Tempo->Suivant) {
			XZST_03EcritureTrace(XZSTC_WARNING,"   Timer/delai = %lf/%d, Dg %s, cle %s, IdMsg %d",
			                pl_Tempo->Horodate, pl_Tempo->Delai, pl_Tempo->Dg, pl_Tempo->Cle, pl_Tempo->IdMsg);
		}
	}
}
