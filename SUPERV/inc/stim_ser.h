/*E*/
/* Fichier : $Id: stim_ser.h,v 1.5 2011/01/12 16:30:56 gesconf Exp $        Release : $Revision: 1.5 $        Date : $Date: 2011/01/12 16:30:56 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_SER * FICHIER stim_ser.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient tous les services accessibles aux
* autres modules de la tache:
* - structure de donnees
* - macro d'envoie d'un message
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  27 Sep 1994	: Creation
* T.Milleville  5 Jul 1995	: Modification structure SMT_ENRG_TIMER  V1.3
* JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : declarer 'sm_Inserer_Liste', 'sm_Supprimer_Liste'  1.5
*******************************************************/

#ifndef sm_services
#define sm_services


/* fichiers inclus */
#include "xzsc.h"
#include "xzsm.h"



/* definitions de constantes exportees */



/* definitions de types exportes */
typedef struct
{
	double		Horodate ;
	int		Delai,
			IdMsg ;
	XZSCT_Datagroup	Dg ;
	XZSMT_CleProg	Cle ;
	int		Priorite ;
	void *		Suivant ;
} SMT_ENRG_TIMER ;




/* definition de macro exportees */

extern void sm_Inserer_Liste (SMT_ENRG_TIMER *pa_Enr);

extern void sm_Supprimer_Liste (char * pa_Cle,
                                char * pa_Dg,
                                int va_IdMsg);


/*#define sm_envoyer(t)	TipcSrvMsgWrite((t)->Dg				\
				       ,XDM_IdentMsg((t)->IdMsg)	\
				       ,XDM_FLG_SRVMSGWRITE		\
				       ,T_IPC_FT_STR,(t)->Cle		\
				       ,NULL) ;				\
			TipcSrvFlush()*/




/* declarations de donnees globales */
#ifdef STIM_DONNEES
SMT_ENRG_TIMER		*pg_ListeTempo ;
double			vg_Instant_T ;
XZSCT_NomMachine	vg_NomMachine ;
#else
extern	SMT_ENRG_TIMER		*pg_ListeTempo ;
extern 	double			vg_Instant_T ;
extern 	XZSCT_NomMachine	vg_NomMachine ;
#endif




/* delaration de fonctions externes */

#endif




