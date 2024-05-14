/*E*/
/* Fichier : $Id: etim_don.h,v 1.4 1995/06/22 16:25:20 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1995/06/22 16:25:20 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME : EQUEXT
------------------------------------------------------
* MODULE TETIM * FICHIER etim_don.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Ce module gere les structures de donnees du timer.
*  La structure de donnees du timer est une liste a chainage avant.
*  Dans cette liste, les elements sont classes par horodate croissant (en secondes)
*  Le delai donne la frequence des reveils cycliques.
*  La variable globale pg_ListeTimer contient le pointeur de debut de liste.
*  
*
------------------------------------------------------
* HISTORIQUE :
*
* Mismer.D	version 1.1	31 Aug 1994	: Creation
* Mismer.D	version 1.2	23 Jan 1995	: 
* Fontaine.C	version 1.3	23 Mai 1995	: Reprise historique
* Mismer.D	version 1.5	22 Jun 1995	: Modif nom liste timers
*
------------------------------------------------------ */

#ifndef etim_don
#define etim_don

/* fichiers inclus */

#include <xdc.h>
#include <xdy.h>
#include <rtworks/common.h>
#include <rtworks/ipc.h>

#include <xzst.h>
#include <xzsm.h>
#include <xzex.h>
#include <ex_msok.h>
#include <xzsem.h>

/* definitions de constantes exportees */

/* definitions de types exportes */

typedef struct
{
	XDY_Horodate	HorodateSec;
	int		Delai;
	int		NumSocket;
	XZEXT_CLE_PROG	CleProg;
	void *		ptrEnrSuiv;

} ETIM_ENRG_TIMER ;

/* definition de macro exportees */

#define	ETIM_REVEIL_CYCLIQUE(PERIODE,CLEPROG,SOCKET,MSG)	\
sprintf ( MSG, "%s %lf %d %s %s",\
                XZEXC_DEMANDE_REVEIL,\
                (double)0,\
                PERIODE,\
                CLEPROG,\
                XZEXC_FIN_MSG_SOCK );\
sem_wait(&vg_semaphore); \
if ( write ( SOCKET, MSG, strlen (MSG) ) <= 0 )\
   XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );\
sem_post(&vg_semaphore); 

#define	ETIM_PROG_REVEIL(HORODATE,CLEPROG,SOCKET,MSG)	\
sprintf ( MSG, "%s %lf %d %s %s",\
                XZEXC_DEMANDE_REVEIL,\
                (double)HORODATE,\
                0,\
                CLEPROG,\
                XZEXC_FIN_MSG_SOCK );\
sem_wait(&vg_semaphore); \
if ( write ( SOCKET, MSG, strlen (MSG) ) <= 0 )\
   XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );\
sem_post(&vg_semaphore); 

#define	ETIM_SYNCHRO_DATE(HEURE,CLEPROG,SOCKET,MSG)	\
sprintf ( MSG, "%s %s %s %s",\
                XZEXC_SYNCHRO_DATE,\
                HEURE,\
                CLEPROG,\
                XZEXC_FIN_MSG_SOCK );\
sem_wait(&vg_semaphore); \
if ( write ( SOCKET, MSG, strlen (MSG) ) <= 0 )\
   XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );\
sem_post(&vg_semaphore); 


#define	ETIM_ANNUL_REVEIL(PERIODE,CLEPROG,SOCKET,MSG)	\
sprintf ( MSG, "%s %lf %d %s %s",\
                XZEXC_ANNULATION_REVEIL,\
                (double)0,\
                PERIODE,\
                CLEPROG,\
                XZEXC_FIN_MSG_SOCK );\
sem_wait(&vg_semaphore); \
if ( write ( SOCKET, MSG, strlen (MSG) ) <= 0 )\
   XZST_03EcritureTrace ( XZSTC_WARNING, "Impossible d'emettre un message sur la socket Timer." );\
sem_post(&vg_semaphore); 


/* declarations de donnees globales */

extern	ETIM_ENRG_TIMER *	pg_ListeTimer;
extern 	double			vg_Instant_T;
extern  XZEXT_MASK *		vg_SockMask;

/* delaration de fonctions externes */

#endif
