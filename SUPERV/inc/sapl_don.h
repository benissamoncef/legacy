/*E*/
/* Fichier : $Id: sapl_don.h,v 1.2 1995/01/20 17:13:45 milleville Exp $	Release : $Revision: 1.2 $        Date : $Date: 1995/01/20 17:13:45 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_DON * FICHIER sapl_don.c
******************************************************
* DESCRIPTION DU MODULE :
*
*  Ce module offre les structures de donnees communes a la tache TSAPLG
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 10 Oct 1994 	: Creation
*
*******************************************************/
#ifndef SAPL_DON
#define SAPL_DON


/* fichiers inclus */
#include "xzsc.h"


/* definitions des types globaux */

/* Surveillance d'un sous systeme */
typedef struct {
	XZSCT_NomSSysteme NomSSysteme;
	int               Etat;
} SPT_SurvSsys ;



/* Surveillance d'une tache */
typedef struct {
	XZSCT_NomTache  	NomTache;
	int			EtatTache;
        int             	NbClient;
	char			Relance;
        int             	Pid;
	int			TacheASupp;
	XZSCT_NomSSysteme 	SSysTacheASupp;
} SPT_SurvTach ;



/* definitions de constantes */

/* valeurs possibles du champ SPT_SurvTach.Relance */
#define SPC_RELANCE_ON		'1'
#define SPC_RELANCE_OFF		'0'

/* Valeurs possibles d'un return */
#define SPC_EXIT_OK            0
#define SPC_EXIT_NOK           1

	/* Flag indiquant la redondance */
#define 	REDONDANCE_INTERNE 	XDC_FAUX


/* caracteristiques des taches utilisateurs */

	/* Premiere lettre des taches utilisateur	*/
#define	SPC_PREM_LETTRE_TACHE_UTIL	't'	
	/* Si XDC_FAUX : On ne gere pas les taches	*/
	/* commencant par la lettre ci-dessus		*/
	/* Si XDC_VRAI : On gere les taches commencant	*/
	/* par la lettre ci-dessus			*/
#define	SPC_GERE_TACHE_UTIL		XDC_FAUX

	/*Definition de la tache tcgcd.x **/
#define	vg_tacheTcgcd	"tcgcd.x"


/* definit les variables globales */
#ifdef SAPL_DON_C	/* en interne */

	/* pointeur sur tableau alloue dynamiquement */
SPT_SurvSsys	*pg_sp_surv_ssys = NULL;	
	/* pointeur sur tableau alloue dynamiquement */
SPT_SurvTach	*pg_sp_surv_tach = NULL;	
	/* nombre de sous-systemes de la machine */
int		vg_sp_nb_surv_ssys = 0;	
	/* nombre de taches de la machine */
int		vg_sp_nb_surv_tach = 0;	
	/* Nom de la tache setat.x passee en parametre du main */
XZSCT_NomTache	vg_tacheSeta;		
	/* Nom de la tache saplg.x passee en parametre du main */
XZSCT_NomTache	vg_tacheSapl;		
	/* booleen indiquant si l'init est terminee */
int		vg_saplFinInit;		
	/* nom de l'application pour RTWorks */
char		vg_NomAppli[XDC_LINE_CMD_SIZE];	


#else			/* en externe */

extern SPT_SurvSsys	*pg_sp_surv_ssys ;
extern SPT_SurvTach	*pg_sp_surv_tach ;
extern int		vg_sp_nb_surv_ssys ;
extern int		vg_sp_nb_surv_tach ;
extern XZSCT_NomTache	vg_tacheSeta;
extern XZSCT_NomTache	vg_tacheSapl;
extern int		vg_saplFinInit;
extern char		vg_NomAppli[XDC_LINE_CMD_SIZE];


#endif


/* definit les routines externes */
extern SPT_SurvTach *sp_rechercherTache( XZSCT_NomTache  va_NomTache ) ;
extern SPT_SurvSsys *sp_rechercherSSysteme( XZSCT_NomSSysteme  va_NomSSysteme ) ;

#endif	/* SAPL_DON	*/

