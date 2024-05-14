/*E*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI * FICHIER sapl_ini.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module appartient a la tache tsaplg
* Il initialise les donnees de la tache TSAPLG
*
******************************************************
* HISTORIQUE :
*
* T.Milleville  10 Oct 1994	: Creation
* T.Milleville  09 Avr 1995 	: Modification du traitement 
*	de l'abonnement des taches surveillees. On surveille
*	a l'init que les taches permanentes V1.3
* T.Milleville  09 Avr 1995 	: Ajout de la callback 
*		sp_starttmp_cb V1.9
* T.Milleville  25 Jul 1995 	: Ajout de commentaires V1.4
* T.Milleville  24 Aou 1995 	: Modification initialisation
*	flag de relance V1.5
* T.Milleville  2 Oct 1995 	: Ajout d'une constante pour preciser
*	le nombre de taches initialialement lancees C_NB_INIT_TACHE V1.7
* T.Milleville  24 Oct 1995 	: Modification sp_initsurv_dg dans laquelle
*	on envoie l'etat des sous-systemes et machines V1.8
* P.Niepceron	13 fev 1996	: Modif de la prise en compte des fic adm (sp_recupTacheASupp,sp_init,sp_initsurv_dgsp_initsurv_ss) v1.10
* JMG	12/12/10 : portage linux dem/934 1.11
* JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.13
* LCL		25/09/2023	: Adaptation code pour rabbitmq DEM-483
*******************************************************/


/* fichiers inclus */
#include "xdc.h"
#include "xdg.h"
#include "xdm.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"
#include "xzems.h"

#include "sapl_ini.h"
#include "sapl_don.h"
#include "sapl_com.h"
#include "sapl_dir.h"
#include "sapl_sur.h"


/* definitions de constantes */

#define	C_NB_INIT_TACHE 	2


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sapl_ini" ;

static XZSCT_NomMachine	vm_sp_NomMachine;

	/* sauvegarde du nombre de taches de la machine */
static int   vm_sp_sauv_nb_surv_tach = 0 ;	
	/* sauvegarde du nombre de sous-systemes de la machine */
static int   vm_sp_sauv_nb_surv_ssys = 0 ;	

	/* sauvegarde du pointeur sur tableau alloue dynamiquement */
static SPT_SurvSsys *pm_sp_sauv_surv_ssys	= NULL; 
	/* sauvegarde du pointeur sur tableau alloue dynamiquement */
static SPT_SurvTach *pm_sp_sauv_surv_tach 	= NULL; 

/* declaration de fonctions internes */
void sp_recupTacheASupp();
void sp_initTacheSSyst();



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	met en route la surveillance du dg de la
*	tache qui est lancee
*
******************************************************
* SEQUENCE D'APPEL :
*/
static void sp_initsurv_dg ( XZSCT_NomTache	va_NomTache ,
			     int		va_NumOrdre ,
			     int 		va_TypeTache)

/*
* PARAMETRES EN ENTREE : - Nom de la tache a surveiller
*			 - numero d'ordre (non utilise)
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucun
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_initsurv_dg" ;
  XZSCT_Datagroup	vl_datagroup	= "";
  SPT_SurvTach		*pl_surv_tach	= NULL;
  int			i		= 0;
  int			vl_trouve	= FALSE;
  int			vl_trouve_Sauve	= FALSE;
  T_BOOL		vl_dgDejaSurv	= FALSE;


  XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : sp_initsurv_dg : va_NomTache='%s' va_NumOrdre=%d \
			type tache %d \n",
			va_NomTache,va_NumOrdre,va_TypeTache);

	/*A Teste si la tache est Presente dans les 
	*			2 fichiers d'administration et 
	*	qu'elle est Permanente */ 
	if ( (va_TypeTache == XZSAC_TYPE_TPER)
	   || ((va_TypeTache == XZSAC_TYPE_TTMP) && (va_NumOrdre == -1)) )
	{
  		/*B Construction du nom du datagroup NomMachine_NomTache */
  		XDG_EncodeDG2( vl_datagroup, vm_sp_NomMachine, va_NomTache ) ;

  		/*B Surveille t-on deja ce dataGroup */
		/*TIBCO SS66*/
  		/*TipcSrvDgGetWatch( vl_datagroup , &vl_dgDejaSurv );*/
		TipcMonSubjectSubscribeGetWatch( vl_datagroup , &vl_dgDejaSurv );


  		/* Si le dataGroup n'est pas encore surveille */
  		if (vl_dgDejaSurv == FALSE)
  		{
			XZST_03EcritureTrace(	XZSTC_INFO,
				" Datagroup non surveille %s -- Nombre de taches surveillees %d \n"
				,vl_datagroup, vg_sp_nb_surv_tach);

			/*B Surveillance du dataGroup si pas encore surveille */
  			XDG_SurveillerDG(vl_datagroup);
			 
			/*B Reallocation du pointeur de taches surveillees */
			pg_sp_surv_tach  = realloc(pg_sp_surv_tach,
					((size_t) vg_sp_nb_surv_tach+1) * sizeof(SPT_SurvTach) );

			/* On enregistre les options de la tache */
			strcpy(pg_sp_surv_tach[vg_sp_nb_surv_tach].NomTache,va_NomTache);
			pg_sp_surv_tach[vg_sp_nb_surv_tach].EtatTache = XZSEC_ETAT_NOK;
			pg_sp_surv_tach[vg_sp_nb_surv_tach].NbClient = 0 ;

			if  (va_TypeTache == XZSAC_TYPE_TPER)
			{
				pg_sp_surv_tach[vg_sp_nb_surv_tach].Relance = SPC_RELANCE_ON;
			}
			else if (va_TypeTache == XZSAC_TYPE_TTMP)
			{
				pg_sp_surv_tach[vg_sp_nb_surv_tach].Relance = SPC_RELANCE_OFF;
			}

			pg_sp_surv_tach[vg_sp_nb_surv_tach].Pid = 0 ;
			pg_sp_surv_tach[vg_sp_nb_surv_tach].TacheASupp = XDC_FAUX;
			strcpy(pg_sp_surv_tach[vg_sp_nb_surv_tach].SSysTacheASupp,"");

			i=0;
			vl_trouve = FALSE;
			if (pm_sp_sauv_surv_tach != NULL) {
			while ( ( i < vm_sp_sauv_nb_surv_tach ) && ( ! vl_trouve ) )
			{
				if ( strcmp (va_NomTache , pm_sp_sauv_surv_tach[i].NomTache ) == 0 )
				{
					vl_trouve = TRUE ;
				}	
				else
				{
					i++;
				}
			}
			}
			if ( vl_trouve == TRUE ) 
			   {
			   pm_sp_sauv_surv_tach[i].TacheASupp=XDC_FAUX;
			   }
			   
			/*B Incrementation du Nb de taches surveillees */
			vg_sp_nb_surv_tach++;
			XZST_03EcritureTrace(XZSTC_FONCTION,"vg_sp_nb_surv_tach passe a %d",vg_sp_nb_surv_tach);
  		}
		/* Si Datagroup deja surveille, alors recherche de l'etat des taches
		*	deja presentes */
		else 
		{
			XZST_03EcritureTrace(	XZSTC_INFO,
					" Datagroup deja surveille %s -- Nombre de taches sureillees %d \n"
					,vl_datagroup, vg_sp_nb_surv_tach);

			/*A recherche si existence dans le tableau des taches existantes */
			i=0;
			vl_trouve = FALSE;
			XZST_03EcritureTrace(   XZSTC_INFO,"vg_sp_nb_surv_tach %d",vg_sp_nb_surv_tach);
			while ( ( i < vg_sp_nb_surv_tach ) && ( ! vl_trouve ) )
			{
				 XZST_03EcritureTrace(   XZSTC_INFO,"%s", pg_sp_surv_tach[i].NomTache);
				if ( strcmp (va_NomTache , pg_sp_surv_tach[i].NomTache ) == 0 )
				{
					vl_trouve = TRUE ;
				}	
				else
				{
					i++;
				}
			}
			if (( vl_trouve != TRUE ) && (va_TypeTache == XZSAC_TYPE_TPER) ) /* si nouvelle tache permanente alors on relance */
			   {
			   XZST_03EcritureTrace(   XZSTC_INFO,"avant sp_startTache");
			   (void) sp_startTache(va_NomTache,0);
			   }

/* Modif V1.9 Envoi de l'etat du sous-systeme associe */
#if 0
			if (vl_trouve == TRUE)
			{
				/* determine le pointeur sur le nouvel enregistrement */
				pl_surv_tach = &(pg_sp_surv_tach[i] )  ;

				/* Generation du Sous Systeme */
				 sp_genetat_ssys (pl_surv_tach);
				 sp_genetat_mach();
			}
#endif
				

		}
#if 0
  		/* determine le pointeur sur le nouvel enregistrement */
  		pl_surv_tach = &(pg_sp_surv_tach[i] )  ;


		XZST_03EcritureTrace(XZSTC_DEBUG2,"Nb tache surveillees %d \
			Nb Taches sauvees %d \n",
			vg_sp_nb_surv_tach,vm_sp_sauv_nb_surv_tach);

  		/*A recherche si existence dans le tableau sauvegarde */
  		while ( ( i < vg_sp_nb_surv_tach ) && ( ! vl_trouve_Sauve ) )
  		{ 
	XZST_03EcritureTrace(XZSTC_DEBUG3,"Existance taches surveilles 	tache %s -- sauve tache %s \n"
				,va_NomTache, pm_sp_sauv_surv_tach[i].NomTache);

			if ( strcmp (va_NomTache , pm_sp_sauv_surv_tach[i].NomTache ) == 0 )
			{
				vl_trouve_Sauve = TRUE ;
			}	
			else
			{
				i++ ;
			}
   		}
#endif
#if 0
  		/* determine le pointeur sur le nouvel enregistrement */
  		pl_surv_tach = &(pg_sp_surv_tach[i] )  ;

  		/*A met a jour le nouvel enregistrement */
  		strcpy(pl_surv_tach->NomTache,va_NomTache);
  		pl_surv_tach->EtatTache = XZSEC_ETAT_NOK;
  		pl_surv_tach->NbClient = 0 ;
  		
		if  (va_TypeTache == XZSAC_TYPE_TPER)
		{
  			/*pl_surv_tach->Relance = SPC_RELANCE_ON;*/
  			pl_surv_tach->Relance = SPC_RELANCE_OFF;
		}
		else if (va_TypeTache == XZSAC_TYPE_TTMP)
		{
  			pl_surv_tach->Relance = SPC_RELANCE_OFF;
		}
  		pl_surv_tach->Pid = 0 ;
  		pl_surv_tach->TacheASupp = XDC_FAUX;
  		strcpy(pl_surv_tach->SSysTacheASupp,"");

  		/*B recopie des anciennes informations s'il y en avait */
		if ( vl_trouve_Sauve )
  		{ 	/* valeurs sauvegardees */
			pl_surv_tach->EtatTache = 
					pm_sp_sauv_surv_tach[i].EtatTache ;
	
			pl_surv_tach->NbClient = 
					pm_sp_sauv_surv_tach[i].NbClient ;
	
			pl_surv_tach->Relance = 
					pm_sp_sauv_surv_tach[i].Relance ;
	
			pl_surv_tach->Pid = 
					pm_sp_sauv_surv_tach[i].Pid ;
	
			pl_surv_tach->TacheASupp = 
				pm_sp_sauv_surv_tach[i].TacheASupp ;
			strcpy(pl_surv_tach->SSysTacheASupp,
				pm_sp_sauv_surv_tach[i].SSysTacheASupp);

			/* Marquage de cette tache comme recuperee */
			strcpy(pm_sp_sauv_surv_tach[i].NomTache,"");
  		}
		else
		{
			/*B Incrementation du Nb de taches surveillees */
			vg_sp_nb_surv_tach++;

			/*B Reallocation du pointeur de taches surveillees */
			pg_sp_surv_tach  = realloc(pg_sp_surv_tach, 
				(vg_sp_nb_surv_tach+1) * sizeof(SPT_SurvTach) );
		}
#endif
	}

  XZST_03EcritureTrace(	XZSTC_FONCTION,"OUT : sp_initsurv_dg");

} /* sp_initsurv_dg */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Ajoute au tableau des sous systemes 
*	le sous systeme passe en parametre
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void sp_initsurv_ss ( XZSCT_NomSSysteme	va_NomSSysteme ,
			     int		va_NumOrdre ,
			     void  		* pa_Args)

/*
* PARAMETRES EN ENTREE : - nom du sous systeme a surveiller
*			 - numero d'ordre (non utilise)
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
B
*
******************************************************/
{
  static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_initsurv_ss" ;
  SPT_SurvSsys		*pl_surv_ssys	= NULL;
  int			i		= 0;
  int			vl_trouve	= FALSE;
  int			vl_int		= 0;


  	XZST_03EcritureTrace(	XZSTC_FONCTION,
		"IN : sp_initsurv_ss : va_NomSSysteme='%s' va_NumOrdre=%d",
			va_NomSSysteme,va_NumOrdre);

  /* determine le pointeur sur le nouvel enregistrement */
  pl_surv_ssys = &(pg_sp_surv_ssys[va_NumOrdre] )  ;

  /* met a jour le nouvel enregistrement */
  strcpy( pl_surv_ssys->NomSSysteme , va_NomSSysteme ) ;
  pl_surv_ssys->Etat = XZSEC_SANS_ETAT ;

  /* recherche si existence dans le tableau sauvegarde */
   if (pm_sp_sauv_surv_ssys!=NULL) {
  while ( ( i < vm_sp_sauv_nb_surv_ssys ) && ( ! vl_trouve ) )
  {
	if ( strcmp (va_NomSSysteme , 
			pm_sp_sauv_surv_ssys[i].NomSSysteme ) == 0 )
	{
		vl_trouve = TRUE ;
	}
	else
	{
		i++ ;
	}
  }
  }
  
  /* recopie des anciennes informations s'il y en avait */
  if ( vl_trouve )
  { 	/* valeurs sauvegardees */
	pl_surv_ssys->Etat = pm_sp_sauv_surv_ssys[i].Etat ;
  }


  /* Met a jour le sous systeme de chaque tache */
  XZSA_04ListeTacheMachineSSyst(vm_sp_NomMachine,va_NomSSysteme,
		sp_initTacheSSyst,&vl_int,(void *)va_NomSSysteme);

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_initsurv_ss");

}  /* sp_initsurv_ss */




/* declaration de fonctions externes */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Demmande pour l'initialisation des donnees de la tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_init_cb (
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_init_cb" ;

  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : sp_init_cb");

  sp_init();

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_init_cb");

} /* sp_init_cb */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Initialise le tableau contenant la liste des
*	taches de la machine
*	Initialise le tableau contenant la liste des
*	sous systemes de la machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_init ( )

/*
* PARAMETRES EN ENTREE : aucun
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Echec d'un malloc
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_init" ;
  int			vl_int 	= 0;
  int			vl_ind 	= 0;
  int			vl_Ind 	= XDC_VRAI;
  XZSCT_Datagroup       vl_datagroup;


  XZST_03EcritureTrace(XZSTC_FONCTION,"sp_init : sp_init \n");

  if (vg_sp_nb_surv_tach != 0 ) 	/* sauvegarde de la memoire actuelle */
  {
	vm_sp_sauv_nb_surv_tach = vg_sp_nb_surv_tach ;  /* nombre */
	vm_sp_sauv_nb_surv_ssys = vg_sp_nb_surv_ssys ;  /* nombre */
	pm_sp_sauv_surv_ssys = pg_sp_surv_ssys ;  /* pointeur tableau */
	pm_sp_sauv_surv_tach = pg_sp_surv_tach ;  /* pointeur tableau */
	vg_sp_nb_surv_tach=0;
  	pg_sp_surv_tach=NULL;
  	/* Suppression surveillance de toutes les anciennes taches */
	for (vl_ind=0;vl_ind<vm_sp_sauv_nb_surv_tach;vl_ind++)
	{
	XDG_EncodeDG2( vl_datagroup, vm_sp_NomMachine, pm_sp_sauv_surv_tach[vl_ind].NomTache ) ;
	XDG_AnnulerSurvDG(vl_datagroup );
	pm_sp_sauv_surv_tach[vl_ind].TacheASupp=XDC_VRAI;
	}
   }

  /* determine le nom de ma machinee*/
  XZSC_07NomMachine(vm_sp_NomMachine);

  /* determine le nombre de taches */
  XZSA_03ListeTacheMachine(vm_sp_NomMachine,NULL,&vg_sp_nb_surv_tach,&vl_Ind);

  XZST_03EcritureTrace(XZSTC_DEBUG2," Nb de taches Surveillees %d \n",
					vg_sp_nb_surv_tach);

  /* determine le nombre de sous-systemes */
  XZSA_02ListeSSysteme(vm_sp_NomMachine,NULL,&vg_sp_nb_surv_ssys,NULL);

  XZST_03EcritureTrace(XZSTC_INTERFACE," Nb de Sous-systemes  Surveilles %d \n",
				vg_sp_nb_surv_ssys);

  /* allocation memoire du tableau de surveillance des sous-systemes */
  if ( (pg_sp_surv_ssys = 
	malloc ( vg_sp_nb_surv_ssys * sizeof(SPT_SurvSsys)) ) == NULL )
  {
	XZST_03EcritureTrace(XZSTC_FATAL,"sapl.x/sp_init()/malloc()");
	XZST_03EcritureTrace(	XZSTC_FATAL,
			"allocation memoire (surv ssys) non satisfaite");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_init SPC_EXIT_NOK");
	sp_sortir();
  }
  /* allocation memoire du tableau de surveillance des taches */
/*JM on enleve les commentaires*/
#ifdef _TIBCO_EMS
  if ( (pg_sp_surv_tach = 
		malloc( (vg_sp_nb_surv_tach+1) * sizeof(SPT_SurvTach)) ) == NULL)
  {
	XZST_03EcritureTrace(XZSTC_FATAL,"sapl.x/sp_init()/malloc()");
	XZST_03EcritureTrace(	XZSTC_FATAL,
			"allocation memoire (surv taches) non satisfaite");
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_init SPC_EXIT_NOK");
	sp_sortir();
  }

	for (vl_ind=0;vl_ind<vg_sp_nb_surv_tach+1;vl_ind++)
	{
			strcpy(pg_sp_surv_tach[vl_ind].NomTache,"");
  }
#endif
  
  /* initialisation de la liste des datagroups a surveiller */
  XZSA_03ListeTacheMachine(vm_sp_NomMachine,sp_initsurv_dg,&vl_int,&vl_Ind);

  /* initialisation de la liste des sous-systemes a surveiller */
  XZSA_02ListeSSysteme(vm_sp_NomMachine,sp_initsurv_ss,&vl_int,NULL);

  /* Recuperation des taches n'existant plus dans le fichier d'administration */
#ifndef _TIBCO_EMS
 sp_recupTacheASupp();
#endif

  /*  liberation memoire */
  if ( pm_sp_sauv_surv_ssys != NULL )
  {
	free(pm_sp_sauv_surv_ssys);
	pm_sp_sauv_surv_ssys=NULL;
  }

  if ( pm_sp_sauv_surv_tach != NULL )
  {
	free(pm_sp_sauv_surv_tach);
	pm_sp_sauv_surv_tach=NULL;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION," sp_init  : out sp_init \n");

} /* sp_init */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Recupere dans l'ancien tableau des taches celles
*	qui n'apparaissent plus dans le fichier administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_recupTacheASupp()

/*
* PARAMETRES EN ENTREE : aucun
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
*
* - Echec du realloc
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_recupTacheASupp" ;
	int		vl_ind		= 0;
	int		vl_indNew	=0;
	SPT_SurvTach	*pl_SurvTachNew	= NULL;
  	XZSCT_NomSSysteme     vl_NomSSysteme  = "";
	XZSCT_Datagroup         vl_datagroup;

XZST_03EcritureTrace(XZSTC_FONCTION,"IN : sp_recupTacheASupp");

/* Parcour de l'ancien tableau */
for (vl_ind=0;vl_ind<vm_sp_sauv_nb_surv_tach;vl_ind++)
{
	/* S'il reste une tache et qu'elle doit etre maintenue active */
#if 0
	if ( (pm_sp_sauv_surv_tach[vl_ind].NomTache[0] != '\0') &&
	     (pm_sp_sauv_surv_tach[vl_ind].Relance == SPC_RELANCE_ON) )
		{
		/* Augmentation de la taille du tableau */
		vl_indNew = vg_sp_nb_surv_tach;
		vg_sp_nb_surv_tach = vg_sp_nb_surv_tach + 1;
		pl_SurvTachNew = realloc((void *)pg_sp_surv_tach,
				vg_sp_nb_surv_tach*sizeof(SPT_SurvTach));
		if (pl_SurvTachNew == NULL)
		{
			XZST_03EcritureTrace(XZSTC_FATAL,
				"sapl.x/sp_recupTacheASupp()/realloc()");
			XZST_03EcritureTrace(	XZSTC_FATAL,
			"allocation memoire (surv taches) non satisfaite");
			XZST_03EcritureTrace(XZSTC_FONCTION,
				"OUT : sp_recupTacheASupp SPC_EXIT_NOK");
			sp_sortir();
		}
		else
		{
			/* Mise a jour du pointeur global */
			pg_sp_surv_tach = pl_SurvTachNew;

			/* Recuperation des infos de la tache */
			strcpy(pg_sp_surv_tach[vl_indNew].NomTache,
					pm_sp_sauv_surv_tach[vl_ind].NomTache);
			pg_sp_surv_tach[vl_indNew].EtatTache = 
					pm_sp_sauv_surv_tach[vl_ind].EtatTache;
			pg_sp_surv_tach[vl_indNew].NbClient = 
					pm_sp_sauv_surv_tach[vl_ind].NbClient;
			pg_sp_surv_tach[vl_indNew].Relance = 
					pm_sp_sauv_surv_tach[vl_ind].Relance;
			pg_sp_surv_tach[vl_indNew].Pid = 
					pm_sp_sauv_surv_tach[vl_ind].Pid;
			pg_sp_surv_tach[vl_indNew].TacheASupp = XDC_VRAI ;
			strcpy(pg_sp_surv_tach[vl_indNew].SSysTacheASupp,
				pm_sp_sauv_surv_tach[vl_ind].SSysTacheASupp);

		}
	}


       if  ((pm_sp_sauv_surv_tach[vl_ind].NomTache[0] != '\0') &&
            (strcmp(pm_sp_sauv_surv_tach[vl_ind].NomTache,"tsaplg.x")) )
		{
		XZSE_12EnvoieEtatTache(vm_sp_NomMachine,pm_sp_sauv_surv_tach[vl_ind].NomTache,XZSEC_SANS_ETAT,10);
		if ( (XZSA_08TacheNomSSysteme(pm_sp_sauv_surv_tach[vl_ind].NomTache,vl_NomSSysteme))
                                                                != XDC_OK )
  		{
        		XZST_03EcritureTrace(   XZSTC_WARNING,
                        "Sous systeme de la tache '%s' non trouve par XZSA",
                                pm_sp_sauv_surv_tach[vl_ind].NomTache );
  		}
  		else
  		{
			XZSE_11EnvoieEtatSSysteme( vm_sp_NomMachine,
                                vl_NomSSysteme,XZSEC_SANS_ETAT);
                }
                
		pm_sp_sauv_surv_tach[vl_ind].EtatTache = XZSEC_SANS_ETAT;
		sp_genetat_ssys (&pm_sp_sauv_surv_tach[vl_ind]);
		XDG_EncodeDG2(vl_datagroup,vm_sp_NomMachine,pm_sp_sauv_surv_tach[vl_ind].NomTache);
		if (!TipcSrvMsgWrite(   vl_datagroup ,
					 XDM_IdentMsg(XDM_TSP_EXIT) ,
					 XDM_FLG_SRVMSGWRITE ,
					 NULL ) )
                {
			XZST_03EcritureTrace(XZSTC_WARNING, "Erreur TipcSrvMsgWrite") ;
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_DiffTaches");
			return ;
                }
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,
			"Arret de la tache %s, qui n'est plus dans les fichiers d'administration",
			pm_sp_sauv_surv_tach[vl_ind].NomTache) ;
		}
		}

#endif
       if  ( pm_sp_sauv_surv_tach[vl_ind].TacheASupp == XDC_VRAI )
           {
		XZSE_12EnvoieEtatTache(vm_sp_NomMachine,pm_sp_sauv_surv_tach[vl_ind].NomTache,XZSEC_SANS_ETAT,10);
		if ( (XZSA_08TacheNomSSysteme(pm_sp_sauv_surv_tach[vl_ind].NomTache,vl_NomSSysteme))
                                                                != XDC_OK )
  		{
        		XZST_03EcritureTrace(   XZSTC_WARNING,
                        "Sous systeme de la tache '%s' non trouve par XZSA",
                                pm_sp_sauv_surv_tach[vl_ind].NomTache );
  		}
  		else
  		{
			XZSE_11EnvoieEtatSSysteme( vm_sp_NomMachine,
                                vl_NomSSysteme,XZSEC_SANS_ETAT);
                }
                
		pm_sp_sauv_surv_tach[vl_ind].EtatTache = XZSEC_SANS_ETAT;
		sp_genetat_ssys (&pm_sp_sauv_surv_tach[vl_ind]);
		XDG_EncodeDG2(vl_datagroup,vm_sp_NomMachine,pm_sp_sauv_surv_tach[vl_ind].NomTache);
		if (!TipcSrvMsgWrite(   vl_datagroup ,
					 XDM_IdentMsg(XDM_TSP_EXIT) ,
					 XDM_FLG_SRVMSGWRITE ,
					 NULL ) )
                {
			XZST_03EcritureTrace(XZSTC_WARNING, "Erreur TipcSrvMsgWrite") ;
			XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_DiffTaches");
			return ;
                }
		else
		{
			XZST_03EcritureTrace(XZSTC_INFO,
			"Arret de la tache %s, qui n'est plus dans les fichiers d'administration",
			pm_sp_sauv_surv_tach[vl_ind].NomTache) ;
		}
           }
	
}

	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_recupTacheASupp");

}	/* sp_recupTacheASupp */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Affecte a chaque tache du tableau le sous
*	systeme auquel elle appartient
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_initTacheSSyst(	XZSCT_NomTache	va_NomTache,
			int		va_numOrdre,
			void		*va_NomSSysteme)

/*
* PARAMETRES EN ENTREE : - Nom de la tache
*			 - Numero d'ordre (inutilise)
*			 - Nom du sous systeme de cette tache
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : 
*
* - La tache passee en parametre n'est pas dans notre tableau
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_initTacheSSyst" ;
	SPT_SurvTach	*pl_survTache = NULL;


	XZST_03EcritureTrace(	XZSTC_DEBUG1,
 "IN : sp_initTacheSSyst : va_NomTache='%s' va_numOrdre=%d va_NomSSysteme='%s'",
			va_NomTache,va_numOrdre,(char *)va_NomSSysteme);

/* Recherche de la tache dans le tableau */
pl_survTache = sp_rechercherTache(va_NomTache);

/* Si elle existe */
if (pl_survTache != NULL)
{
	strcpy(pl_survTache->SSysTacheASupp,(char *)va_NomSSysteme);
}
else
{
	XZST_03EcritureTrace(	XZSTC_DEBUG1,
"sp_initTacheSSyst : Impossible d'affecter sous systeme '%s' a la tache '%s'",
				(char *)va_NomSSysteme,va_NomTache);
}

	XZST_03EcritureTrace(XZSTC_DEBUG1,"OUT : sp_initTacheSSyst");

}	/* Fin sp_initTacheSSyst */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Initialise le tableau contenant les taches a
*	surveiller avec la tache passee en parametre
*	au main (seta.x) afin de pouvoir la lancer.
*	Et avec soi meme (sapl.x) afin que la gestion
*	des taches soit la meme pour toutes les taches
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_initTabSurvTache(	XZSCT_NomTache	va_NomTache)

/*
* PARAMETRES EN ENTREE : - Nom de la tache seta.x a mettre
*			   dans le tableau
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Echec du malloc
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_initTabSurvTache" ;
	XZSCT_NomTache	vl_maTache		= "";


	XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : sp_initTabSurvTache : va_NomTache='%s'",
			va_NomTache);

/* determine le nom de ma machinee*/
XZSC_07NomMachine(vm_sp_NomMachine);

#if 0

/* Le tableau va contenir 2 tache */
vg_sp_nb_surv_tach = C_NB_INIT_TACHE;

/* allocation memoire du tableau de surveillance des taches */
if ( (pg_sp_surv_tach = malloc(vg_sp_nb_surv_tach * sizeof(SPT_SurvTach)) ) == NULL)
{
	XZST_03EcritureTrace(XZSTC_FATAL,
			"sapl.x/sp_initTabSurvTache()/malloc()");
	XZST_03EcritureTrace(	XZSTC_FATAL,
		"allocation memoire (surv taches) non satisfaite");
	XZST_03EcritureTrace(XZSTC_FONCTION,
			"OUT : sp_initTabSurvTache SPC_EXIT_NOK");
	sp_sortir();
}

/* Affectation de la tache seta.x dans le tableau */
strcpy(pg_sp_surv_tach[0].NomTache,va_NomTache);
pg_sp_surv_tach[0].EtatTache = XZSEC_ETAT_NOK;
pg_sp_surv_tach[0].NbClient = 0;
pg_sp_surv_tach[0].Pid = 0;
pg_sp_surv_tach[0].Relance = SPC_RELANCE_OFF;
pg_sp_surv_tach[0].TacheASupp = XDC_FAUX;
strcpy(pg_sp_surv_tach[0].SSysTacheASupp,"");

/* Recuperation du nom de la tache sapl.x */
XZSC_06NomTache(vl_maTache);

/* Affectation de la tache sapl.x dans le tableau */
strcpy(pg_sp_surv_tach[1].NomTache,vl_maTache);
pg_sp_surv_tach[1].EtatTache = XZSEC_ETAT_NOK;
pg_sp_surv_tach[1].NbClient = 0;
pg_sp_surv_tach[1].Pid = 0;
pg_sp_surv_tach[1].Relance = SPC_RELANCE_OFF;
pg_sp_surv_tach[1].TacheASupp = XDC_FAUX;
strcpy(pg_sp_surv_tach[1].SSysTacheASupp,"");

#endif


/* Recuperation du nom de la tache sapl.x */
XZSC_06NomTache(vl_maTache);

/*Surveillance des Dg tsetat.x et tsaplg.x */
	/* Tache  tsaplg.x */
sp_initsurv_dg(vl_maTache,1,XZSAC_TYPE_TPER);

#if 0
pg_sp_surv_tach[1].NbClient = 1;
#endif


XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_initTabSurvTache");

}	/* sp_initTabSurvTache */



/*X*/
/* Fichier : $Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $        Release : $Revision: 1.14 $        Date : $Date: 2020/11/03 17:53:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_INI
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Demmande de lancement d'une tache Temporaire.
*	On ecupre le nom de la tache, puis on demande 
*	la creation du datagroup de suioveillance 
*	NomMachine_NomTache 
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_starttmp_cb (
	T_IPC_CONN			va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA	pa_Data,
	T_CB_ARG			pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
	static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sp_starttmp_cb" ;
#ifndef _TIBCO_EMS
	T_STR  		vl_NomTache	= "";
#else
	XDY_NomTache	vl_NomTache     = "";
#endif
	int             vl_NumOrdre	= -1;
	int		vl_TypeTache	= 0;
	int		vl_ValRet	= XDC_OK;
	int		vl_NbAbonnes	= 0;
	XZSCT_Datagroup	vl_NomDatagroup	= "";
	XZSCT_NomMachine	vl_NomMachine	="";


	XZST_03EcritureTrace(XZSTC_FONCTION,"sp_starttmp_cb : IN \n");

	/*A Recuperation du  Nom de la Tache */
	if (!TipcMsgRead(pa_Data -> msg,
		T_IPC_FT_STR, &vl_NomTache,NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,"Erreur reception msg \
			XDM_TSP_STARTTMP  \n");
	}

	XZST_03EcritureTrace(XZSTC_DEBUG2,"Recup XDM_STARTTMP nom tache %s \n",
		vl_NomTache);
	
	/*A recherche du type de la tache */
	vl_ValRet = XZSA_11TacheTemporaire(vl_NomTache);	
	if (vl_ValRet == XDC_OK)
	{
		/*B Tache Temoraire */
		vl_TypeTache = XZSAC_TYPE_TTMP;
	}
	else if  (vl_ValRet == XZSAC_TYPE_TPER)
	{
		/*B Tache Permanente */
		 vl_TypeTache = XZSAC_TYPE_TPER;
	}

	/*A Creation du Datagroup de Surveillance */
	sp_initsurv_dg(vl_NomTache,vl_NumOrdre,vl_TypeTache);

#if 0
	/*A Appel pour permettre l'initialisation */
	vl_NbAbonnes	= 1;
  	XZSC_07NomMachine(vl_NomMachine);
  	XDG_EncodeDG2( vl_NomDatagroup, vl_NomMachine, vl_NomTache ) ;
	sp_dgstatus(vl_NomDatagroup,vl_NbAbonnes);
#endif

	XZST_03EcritureTrace(XZSTC_FONCTION,"sp_starttmp_cb : OUT \n");

}	/* Fin sp_starttmp_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de surveiller le datagroup _server 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int sapl_init_surv_dbserver()

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
*   XDC_OK	! ok
*   XDC_NOK	! nok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Appel la fonction de surveillance d'un datagroup
*
------------------------------------------------------*/
{
	static char *version = "$Id: sapl_ini.c,v 1.14 2020/11/03 17:53:05 pc2dpdy Exp $ : sapl_init_surv_dbserver" ;
	int	vl_ValRet	= XDC_OK;
  	T_BOOL		vl_dgDejaSurv	= FALSE;


  	/*ASurveille t-on deja ce dataGroup */
	/*TIBCO SS66*/
  	/*TipcSrvDgGetWatch( "_server" , &vl_dgDejaSurv );*/
	TipcMonSubjectSubscribeGetWatch( "/_server" , &vl_dgDejaSurv );

  	/*B Si le dataGroup n'est pas encore surveille */
  	if (vl_dgDejaSurv == FALSE)
  	{
		/*B Surveillance du dataGroup si pas encore surveille */
  		XDG_SurveillerDG("_server");
  	}

	return(vl_ValRet);

}	/* Fin sapl_init_surv_dbserver */


