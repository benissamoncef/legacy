/*E*/
/*Fichier : $Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $      Release : $Revision: 1.10 $        Date : $Date: 2021/05/31 13:18:12 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_trt.c
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Module d'analyse et de traitement des demandes de reveil.
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	28 Oct 1994	:
* Mismer.D	version 1.3	09 Dec 1994	:
* Mismer.D	version 1.4	25 Jan 1995	:
* Fontaine.C	version 1.5	23 Mai 1995	: Reprise entete, historique
* Mismer.D	version 1.6	22 Jun 1995	: Modif nom liste timers
*
* JPL		07/03/11 : Migration HP ia64 (DEM 975) : Gestion conversions types de tailles differentes (=,+)  1.7
* XXX		03/11/20 : Ajout de traces  1.8
* JPL		05/05/21 : Ajout fonction d'observation de l'etat  1.9
* JPL		20/05/21 : Plus de masquage des alarmes lors du reveil des taches clientes (SAE-272)  1.10
------------------------------------------------------*/

/* fichiers inclus */
#include <xzsem.h>

#include "etim_trt.h"

/* definitions de constantes */

/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */

static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : etim_trt";

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction determinant la nature de la demande formulee a la tache TETIM
*  par les taches clientes.
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_analyse_dem ( int	va_NumSock )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Lecture du message sur la socket appelante
*   Examen du message ( programation d'un reveil , annulation d'un reveil , synchro date )
*
------------------------------------------------------*/
{
   static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : et_analyse_dem " ;
   int			vl_IndCnx = 0,
   			vl_LgMsg  = 0;
   XZEXT_MSG_SOCKET	pl_Msg;
   ETIM_ENRG_TIMER *	pl_Tempo;
   ETIM_ENRG_TIMER *	pl_TempoSuivante;
   
   
  
   /*A
    *  Lecture du message recu dans la socket appelante
    */
	 
   if ( ex_lire_msg ( va_NumSock, pl_Msg, &vl_LgMsg, &vg_SockMask, &vl_IndCnx ) != XDC_OK )
   {
      /*A Ecriture trace Erreur lecture message socket. */
      XZST_03EcritureTrace( XZSTC_WARNING, " Erreur lecture message socket n� %d ", va_NumSock );
   }   
   else
   {
      /*A Si c'est une deconnexion */
      if ( vl_IndCnx == XZEXC_DECONNEC )
      {   
         /*A Annulation de toute les demandes de ce client */
         es_sup_liste ( NULL, va_NumSock );
      }
      else
      {
         /*A Si le message est correct */
         if ( vl_LgMsg != 0 )
         {
  	     /*A Si il s'agit d'une demande de programation d'un reveil */
  	     if ( strstr ( pl_Msg, XZEXC_DEMANDE_REVEIL ) != NULL )
  	     {
  	        /*A Appel de la fonction et_prog_reveil */
  	        et_prog_reveil ( va_NumSock, &pl_Msg[strlen(XZEXC_DEMANDE_REVEIL)]);
  	     }
  	  
 	     /*A Si il s'agit d'une demande d'annulation d'un reveil */
  	     if ( strstr ( pl_Msg, XZEXC_ANNULATION_REVEIL ) != NULL )
  	     {
  	        /*A Appel de la fonction et_prog_reveil */
  	        et_annul_reveil ( va_NumSock, &pl_Msg[strlen(XZEXC_ANNULATION_REVEIL)] );
  	     }

 	     /*A Si il s'agit d'une demande de synchro date */
  	     if ( strstr ( pl_Msg, XZEXC_SYNCHRO_DATE ) != NULL )
  	     {
  	        /*A Appel de la fonction et_prog_reveil */
  	        et_reveil_synchro ( va_NumSock, &pl_Msg[strlen(XZEXC_SYNCHRO_DATE)] );
  	     }
          }
       }
    }
    
 
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction enregistant une demande de reveil d'une tache cliente
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_prog_reveil ( 	int	va_NumSock,
			char	*pa_Msg )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*   pa_Msg		: Message contenant les parametres du reveil
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Si horadate nulle : lire l'heure courante et ajouter le delai
*   Insertion de la demande de reveil dans la liste des demandes
*
------------------------------------------------------*/
{
static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : et_prog_reveil " ;
ETIM_ENRG_TIMER		vl_Timer;
double			vl_Instant_T;
int           		vl_jourSemaine;
	
	   
   /*A Lecture des parametres contenus dans le message */
   sscanf ( pa_Msg, "%lf %d %s", &vl_Timer.HorodateSec,
   				 &vl_Timer.Delai,
   				 vl_Timer.CleProg );
   vl_Timer.NumSocket = va_NumSock;

   XZST_03EcritureTrace( XZSTC_INFO, "Programation  H: %lf  delai: %d  Cle: %s ",
   vl_Timer.HorodateSec,vl_Timer.Delai,vl_Timer.CleProg );

   /*A si la demande est un delai */
   if (vl_Timer.HorodateSec == XZSMC_HORODATE_NUL)
   {
      /*A quel est l'horodate actuelle ? */
      XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Instant_T );

      /*A calcul l'horodate du delai */
      vl_Timer.HorodateSec = vl_Instant_T + vl_Timer.Delai;
   }
   
   vl_Timer.ptrEnrSuiv  = (ETIM_ENRG_TIMER *)NULL;
   

   /*A insertion de la tempo */
   ei_ins_liste( &vl_Timer );

#ifdef _TIBCO_EMS
et_reveiller_signal(SIGALRM);
#endif
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction reveille les taches clientes a l'heure programmee
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void		et_reveiller_signal ( int va_signal )

/*
* ARGUMENTS EN ENTREE :
*   va_signal
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Lecture de l'horodate actuelle
*   Pour chaque enregistrement de la liste si l'horodate est echu :
*      reveil de la tache concernee
*      suppression dans la liste
*      si reveil cycle alors programation du prochaine reveil
*
------------------------------------------------------*/
{
	static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : et_reveiller_signal " ;
	struct itimerval	vl_Prog;
	ETIM_ENRG_TIMER *	pl_Tempo;
	ETIM_ENRG_TIMER *	pl_TempoSuivante;
	ETIM_ENRG_TIMER		vl_Timer;
	int			vl_Instant_T,vl_Horodate,vl_jourSemaine;

   /*A Quel est l'instant T ? */
   XZSM_07LireHorodate(XZSMC_TYPE_SYSTEME,&vl_jourSemaine,&vg_Instant_T);
   vl_Instant_T = (int)vg_Instant_T;
   
   /*A pour toutes les tempos existantes */
   for ( pl_Tempo = pg_ListeTimer; pl_Tempo != NULL; )
   {
       vl_Horodate = (int)pl_Tempo->HorodateSec;
       pl_TempoSuivante = pl_Tempo->ptrEnrSuiv;

       /*A si la tempo courante est arrive a echeance */
       if (vl_Horodate <= vl_Instant_T)
       {
          XZST_03EcritureTrace(XZSTC_INFO,"echeance %d",vl_Horodate);

          /*A emission du message demande */
          et_envoyer( pl_Tempo );

          /*A copie enregistrement */
          memcpy ( &vl_Timer, pl_Tempo, sizeof(vl_Timer));

          /*A suppression de la demande de reveil */
          es_sup_liste ( pl_Tempo->CleProg, pl_Tempo->NumSocket );

          /*A si la tempo est cyclique */
          if (vl_Timer.Delai)
          {
	     /*A calcul la prochaine echeance */
	     vl_Timer.HorodateSec = vl_Horodate + vl_Timer.Delai;

	     /*A insere de nouveau dans la liste des tempos a venir */
	     ei_ins_liste ( &vl_Timer );
          }
          pl_Tempo = pg_ListeTimer; 

       }
       else
       {
          XZST_03EcritureTrace(XZSTC_INFO,"prog %d",vl_Horodate);
          pl_Tempo = pl_TempoSuivante;
       }
   }
   
   /*A progammation du prochain reveil */
   vl_Prog.it_value.tv_sec     = (pg_ListeTimer == NULL ? 0 : pg_ListeTimer->HorodateSec - vl_Instant_T);
   vl_Prog.it_value.tv_usec    = 0;
   vl_Prog.it_interval.tv_sec  = 0;
   vl_Prog.it_interval.tv_usec = 0;

   if ( pg_ListeTimer == NULL )
   {
       XZST_03EcritureTrace(XZSTC_INFO,"Plus de timer programme");
   }
   else
   	XZST_03EcritureTrace(XZSTC_INFO,"timer %d",vl_Prog.it_value.tv_sec);
  
   /*A armement du reveil */
   setitimer ( ITIMER_REAL, &vl_Prog, (struct itimerval *)NULL );
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction supprime les demandes de reveil
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_annul_reveil ( 	int	va_NumSock,
			char	*pa_Msg )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*   pa_Msg		: Message contenant les parametres du reveil
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Lecture des parametres de la tempo
*   Appel de la fonction es_sup_liste
*
------------------------------------------------------*/
{
static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : et_annul_reveil " ;
ETIM_ENRG_TIMER		vl_Timer;
double			vl_Instant_T;
	
	   
   /*A Lecture des parametres contenus dans le message */
   sscanf ( pa_Msg, "%lf %d %s", &vl_Timer.HorodateSec,
   				 &vl_Timer.Delai,
   				 vl_Timer.CleProg );
   vl_Timer.NumSocket = va_NumSock;
   XZST_03EcritureTrace( XZSTC_WARNING, "Annulation  H: %lf  delai: %d  Cle: %s ",
   			 vl_Timer.HorodateSec,vl_Timer.Delai,vl_Timer.CleProg );

   /*A Annulation de la temporisation */
   es_sup_liste ( vl_Timer.CleProg, vl_Timer.NumSocket );
   
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Fonction enregistant une demande de synchro date d'une tache cliente
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_reveil_synchro ( 	int	va_NumSock,
				char	*pa_Msg )

/*
* ARGUMENTS EN ENTREE :
*   va_NumSock		: Numero de la socket active
*   pa_Msg		: Message contenant les parametres du reveil
*
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Calcul de l'horodate de reveil
*   Insertion de la demande de reveil dans la liste des demandes
*
------------------------------------------------------*/
{
static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : et_reveil_synchro " ;
ETIM_ENRG_TIMER		vl_Timer;
double			vl_Instant_T;
int			vl_heure, vl_minute,vl_seconde;
int			vl_sec_prog, vl_sec_act;
int           		vl_jourSemaine;
XZSMT_Horodate		pl_Horodate;
char			pl_poub[32];
	
	   
   /*A Lecture des parametres contenus dans le message */
   sscanf ( pa_Msg, "%02d:%02d:%02d %s", &vl_heure, &vl_minute,&vl_seconde,
   				 	    vl_Timer.CleProg );
   vl_Timer.NumSocket = va_NumSock; 
   XZST_03EcritureTrace( XZSTC_WARNING, "Programation Synchro date  Heure: %02d/%02d/%02d  Cle: %s ",
   vl_heure, vl_minute,vl_seconde,vl_Timer.CleProg );
     				 	    
   /*A Convertir de l'heure de synchro en nombre de seconde depuis minuit */
   vl_sec_prog = vl_heure*3600 + vl_minute*60 + vl_seconde;

   /*A quel est l'horodate actuelle ? */
   XZSM_07LireHorodate ( XZSMC_TYPE_SYSTEME, &vl_jourSemaine, &vl_Instant_T );
   /*A Convertir l'horodate en JJ/MM/AAAA HH:MM:SS */
   XZSM_11ConversionHorodate ( vl_Instant_T, &pl_Horodate );
   /*A Recherche du nombre de seconde depuis minuit */
   sscanf ( pl_Horodate, "%s %02d:%02d:%02d", pl_poub, &vl_heure, &vl_minute,&vl_seconde );
   vl_sec_act = vl_heure*3600 + vl_minute*60 + vl_seconde;
   /*A Calcul l'horodate de reveil */
   if ( vl_sec_act >= vl_sec_prog )
   {
       vl_Timer.HorodateSec = vl_Instant_T - vl_sec_act + 24*3600 + vl_sec_prog;
   }
   else
   {
       vl_Timer.HorodateSec = vl_Instant_T - vl_sec_act + vl_sec_prog;
   }

   vl_Timer.Delai = 0;
   vl_Timer.ptrEnrSuiv  = (ETIM_ENRG_TIMER *)NULL;
   
   XZST_03EcritureTrace( XZSTC_WARNING, "Horodate Synchro date %lf  act:%ld  prog:%ld ",vl_Timer.HorodateSec,vl_sec_act,vl_sec_prog);
   /*A insertion de la tempo */
   ei_ins_liste( &vl_Timer );


}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction envoie un message de reveil a la tache concerne
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void et_envoyer ( ETIM_ENRG_TIMER *pa_Enr )

/*
* ARGUMENTS EN ENTREE :
*   pa_Enr		: Enregistrement arrivee a echeance
*
* ARGUMENTS EN SORTIE :
*   aucun
*
* CODE RETOUR : 
*   aucun
*
* CONDITION D'UTILISATION
*    
*
* FONCTION 
*   Construction du message de reveil avec la cle
*   Appel de la fonction ex_env_msg pour envoyer le message de reveil a la tache traitement
*
------------------------------------------------------*/
{
   static char *version = "$Id: etim_trt.c,v 1.10 2021/05/31 13:18:12 devgfi Exp $ : et_envoyer " ;
   XZEXT_MSG_SOCKET		pl_Msg;

   /*A Construction du message de reveil avec la cle */
   sprintf ( pl_Msg, "%s %lf %d %s %s", XZEXC_MSG_REVEIL,
   				     pa_Enr->HorodateSec,
   				     pa_Enr->Delai,
   				     pa_Enr->CleProg,
   				     XZEXC_FIN_MSG_SOCK );
   				     
   /*A Ecriture sur la socket du message de reveil a la tache traitement */
   sem_wait(&vg_semaphore); 
   if ( write ( pa_Enr->NumSocket, pl_Msg, strlen(pl_Msg) )  == XDC_NOK )
   {
      /*A ecriture trace */
      XZST_03EcritureTrace( XZSTC_WARNING, "Impossible d'�mettre un message sur une socket." );
   }
   sem_post(&vg_semaphore); 
   XZST_03EcritureTrace( XZSTC_FONCTION,"emission %s",pl_Msg);
}



void et_dump (int va_param)
{
	struct itimerval	vl_timer;
	sigset_t		vl_sigmask;
	ETIM_ENRG_TIMER *	pl_Tempo;

	getitimer (ITIMER_REAL, &vl_timer);
	XZST_03EcritureTrace(XZSTC_WARNING,"Timer %ld sec + %ld micros, intervalle %ld sec + %ld micros",
	                                vl_timer.it_value.tv_sec, vl_timer.it_value.tv_usec,
	                                vl_timer.it_interval.tv_sec, vl_timer.it_interval.tv_usec);

	sigprocmask (SIG_BLOCK, (sigset_t *) NULL, &vl_sigmask);
	XZST_03EcritureTrace(XZSTC_WARNING,"Signal SIGALRM bloque ? : %d", sigismember (&vl_sigmask, SIGALRM));

	if ( pg_ListeTimer == NULL) {
		XZST_03EcritureTrace(XZSTC_WARNING,"Liste Timers VIDE");
	} else {
		for ( pl_Tempo = pg_ListeTimer; pl_Tempo != NULL;  pl_Tempo = pl_Tempo->ptrEnrSuiv) {
			XZST_03EcritureTrace(XZSTC_WARNING,"   Timer/delai = %d/%d, socket %d, cle %s",
			                (int) pl_Tempo->HorodateSec, pl_Tempo->Delai, pl_Tempo->NumSocket, pl_Tempo->CleProg);
		}
	}
}
