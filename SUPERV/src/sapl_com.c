/*E*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* SOUS-SYSTEME SUPERV
******************************************************
* MODULE SAPL_COM * FICHIER sapl_com.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module appartient a la tache tsaplg
* Il propose diverses routines externes pour arreter (stop)
* ou demarrer (start) les taches applicatives du systeme MIGRAZUR.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 10 Oct 1994	: Creation
* T.Milleville  15 Fev 1995	: Suppression du cas particulier de 
*				tcgcd.x    V1.3
* T.Milleville  09 Avr 1995 	: Modification du code pour le lancement
*	d'une tache. Si elle est Temporaire, on cree d'abord le 
*	datagroup NomMachine_NomTache puis seulement ensuite
*	on eesaie de la lancer V1.5
* T.Milleville  10 Jul 1995 	: Modification de la fonction stopTache.
*	Desormais on peut arreter les taches tsetat.x et tstim.x V1.6
* T.Milleville  25 Jul 1995 	: Ajout commentaires V1.7
* T.Milleville  26 Jul 1995 	: Suppression flag de non relance
*	sur arret de tache en FATAL 
*	Si FATAL alors numero d'ordre = -2  V1.8
* T.Milleville  6 Oct 1995 	:  Ajout d'une nouvelle fonction
*	sp_startTache_Inter + indication du fait que la tache est NOK
*	si le nb client = 0 V1.9
* T.Milleville  10  Oct 1995 	:  Modification pour lancement de
*	d'une tache apres verification du PID et non plus le nombre
*	de clients V1.9
*
*******************************************************/



/* fichiers inclus */
#include <errno.h>
#include <unistd.h>

#include "xdc.h"
#include "xdg.h"
#include "xdm.h"
#include "xdl.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzst.h"

#include "sapl_com.h"
#include "sapl_don.h"
#include "sapl_sur.h"




/* definitions de types locaux */

/* definition de macro locales */

/* definition de constantes locales */

#define 	C_TacheTimer 	"tstim.x"


/* declaration de variables locales */
static char *version = "@(#)sapl_com.c	1.11 11/06/95 : SAPL_COM" ;


/* declaration de fonctions internes */
void	sp_stopTacheASupp();

int sp_startTache_Inter(
	XZSCT_NomTache        va_maTache,
	XZSCT_NomMachine      va_NomMachine);



/* declaration de fonctions externes */



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Arreter une tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_stopTache(	
	XZSCT_NomTache 	va_NomTache,
	int		va_NumOrdre)

/*
* PARAMETRES EN ENTREE : - Nom de la tache a arreter
*			 - Numero d'ordre (non utilise)
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* La tache que l'on doit arreter ne nous est pas connus
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_stopTache" ;
  XZSCT_NomMachine	vl_NomMachine	= "";
  XZSCT_NomTache	vl_NomTache	= "";
  XZSCT_Datagroup	vl_datagroup	= "";
  SPT_SurvTach		*pl_surv_tach 	= NULL;


  XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : sp_stopTache : va_NomTache='%s' va_NumOrdre=%d",
			va_NomTache,va_NumOrdre);

  /*A recupere nom de tache locale */
  XZSC_06NomTache(vl_NomTache);

  /*A On ne s'arrete pas soi meme et
  * On n'arete pas une tache  temporaire 
  *  et pas si Num Ordre = -1  */
  /* Si NumORDRE = -2 alors arret FATAL , on permet la relance */
  if ( (strcmp(vl_NomTache,va_NomTache) ) && 
       ((XZSA_11TacheTemporaire(va_NomTache)!= XDC_OK) 
	|| (va_NumOrdre == -1) ||  (va_NumOrdre == -2)) )
      {
  	/*A recupere nom de machine locale */
  	XZSC_07NomMachine(vl_NomMachine);

  	/*A On verifie que la tache a arreter est connue de sapl.x */
  	if ( ( pl_surv_tach = sp_rechercherTache(va_NomTache ) ) == NULL )
  	{ 	
		/*B tache inconnue */
		XZST_03EcritureTrace(	XZSTC_WARNING,
				"Tache '%s' inconnue, ne peut etre arretee",
				va_NomTache);
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_stopTache");
		return;
  	}
	
  	/*A met l'indicateur relance a OFF  arret FATAL */
	if ( va_NumOrdre != -2)
	{
		/* Arret FATAL .d on ne relance pas automatiquement 
		*		la tache */
  		pl_surv_tach->Relance = SPC_RELANCE_OFF ;
		XZST_03EcritureTrace(XZSTC_DEBUG3," Flag relance tache %s = %d \n",
				pl_surv_tach->NomTache, pl_surv_tach->Relance);
	}

  	/*A Si la tache a un nombre de clients different de 0 */
  	if ( pl_surv_tach->NbClient > 0 )
	{
		/*B  envoi message XDM_TSP_EXIT au datagroup 
					XDG_NomMachine_NomTache */
		XDG_EncodeDG2(vl_datagroup,vl_NomMachine,va_NomTache); 

		if (!TipcSrvMsgWrite(	vl_datagroup ,
				XDM_IdentMsg(XDM_TSP_EXIT) ,
				XDM_FLG_SRVMSGWRITE ,
				NULL ) )
		{
			XZST_03EcritureTrace(XZSTC_WARNING, 
				"sp_stopTache : Erreur TipcSrvMsgWrite" ) ;
			XZST_03EcritureTrace(XZSTC_FONCTION,
						"OUT : sp_stopTache");
			return ;
		}
	
		/*B Mise a zero du pid */
		pl_surv_tach->Pid = 0;

		/*B  archive commande */
		XZST_10ArchiverMsgSyst(XDL_SUP02_CMDATAC,
					va_NomTache,vl_NomMachine,NULL);
	}
  }
  else
  {
	XZST_03EcritureTrace(	XZSTC_WARNING,
				"Refus d'arreter la tache '%s'",
				va_NomTache);
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_stopTache");

	return;
  }

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_stopTache");


}	/* Fin sp_stopTache **/



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Demarrer une tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

int sp_startTache(	
	XZSCT_NomTache 	va_NomTache,
	int		va_NumOrdre )

/*
* PARAMETRES EN ENTREE : - Nom de la tache a demarrer
*			 - Numero d'ordre (non utilise)
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : XDC_OK : tache demarree
*			XDC_NOK : tache non demarree ou
*			deja demarree
*
* CONDITION D'ERREUR :
*
* La tache que l'on doit demarrer ne nous est pas connus
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_startTache" ;
  XZSCT_NomMachine	vl_NomMachine	="";
  XZSCT_Datagroup	vl_datagroup	="";
  SPT_SurvTach		*pl_surv_tach 	= NULL;
  int			vl_Cr 	= 0;
  static char		*vl_argvt[3]; 	/* tableau de pointeurs de strings */
  XZSCT_NomTache	vl_maTache	= "";
  int 			vl_IndicTmp	= XDC_FAUX;
  int 			vl_ValRet	= XDC_OK;


  XZST_03EcritureTrace(	XZSTC_FONCTION,
			"IN : sp_startTache : va_NomTache='%s' va_NumOrdre=%d",
			va_NomTache,va_NumOrdre);

	/*A recupere nom de machine locale */
	XZSC_07NomMachine(vl_NomMachine);

	/*A Teste si la Tache est Temporaire ou Permanente 
	*	sauf pout tsaplg.x et tsetat.x  	*/
	if ( (strcmp(va_NomTache,vg_tacheSeta)) && 
			(strcmp(va_NomTache,vg_tacheSapl)) )
	{
		/*A Teste si la tache est Temporaire */
		if ( (vl_ValRet = XZSA_11TacheTemporaire(va_NomTache))==XDC_OK)
		{
			/*B Memorisation Tache Temporaire */
			vl_IndicTmp = XDC_VRAI;
		}
		else
		{
			if (vl_ValRet != XZSAC_TYPE_TPER)
			{
				/*B Pb */
				XZST_03EcritureTrace( XZSTC_DEBUG1,
					"Pb Recuperation Tache Temporaire \n");

				return (XDC_NOK);
			}
		}
	}

	/*A Si la tache est temporaire et Num_Ordre == -1
	*  ou tache permanente */
	if ( ((vl_IndicTmp == XDC_VRAI) && (va_NumOrdre == -1) ) 
		|| (vl_IndicTmp == XDC_FAUX) )
	{
  		/*B La tache est elle connue de sapl.x */
  		if ( ( pl_surv_tach = sp_rechercherTache(va_NomTache ) ) 
								== NULL )
		{
			/*B Teste si Tache Temporaire */
			if  (vl_IndicTmp == XDC_FAUX)
			{
				/*B tache inconnue */
			XZST_03EcritureTrace(	XZSTC_WARNING,
				"Tache '%s' inconnue, ne peut etre demarree",
				va_NomTache);
 			XZST_03EcritureTrace(XZSTC_FONCTION,
					"OUT : sp_startTache XDC_NOK");
			
				return(XDC_NOK);
			}
		}
	
  		/*A Si la tache a un nombre de clients differents de 0 */
  		/* c'est qu'elle est deja lancee			*/
/* Modification V1.9  -- Test Pid != 0 */
/*  		if ( ((pl_surv_tach != NULL) &&  (pl_surv_tach->NbClient != 0) 
			&& (pl_surv_tach->Pid !=0))  || (!strcmp(va_NomTache,vg_tacheSapl)) ) */

  		if ( ((pl_surv_tach != NULL) && (pl_surv_tach->Pid !=0))  
				|| (!strcmp(va_NomTache,vg_tacheSapl)) ) 
		{
			XZST_03EcritureTrace(	XZSTC_WARNING,
				"Tache '%s' deja lancee ?! (pl_surv_tach-NbClient=%d , pl_surv_tach-Pid=%d)",
				va_NomTache,pl_surv_tach->NbClient,pl_surv_tach->Pid);

 			XZST_03EcritureTrace(XZSTC_FONCTION,
					"OUT : sp_startTache XDC_NOK");

			return(XDC_NOK) ;
		}
  		else
  		{ 	
			/*A tache non lancee */
			vl_Cr = sp_startTache_Inter(va_NomTache,  vl_NomMachine );

			if (vl_Cr > 0)
			{
				/*B Ok,  on recupere le pid */
				if (pl_surv_tach != NULL)
				{
					pl_surv_tach->Pid = vl_Cr ;
				}
			}
			else
			{
				return(XDC_NOK);
			}
		} /* fin du else*/
	} /* Fin di if */
  	
	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_startTache XDC_OK");

	/* Attente pour prise en compte du Watch datagroup */
	sleep(1.0);

  	return(XDC_OK);

}	/* Fin sp_startTache */

/*X*/
/*------------------------------------------------------
* SERVICE RENDU :
*  Peremt de creer un process fils
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int sp_startTache_Inter(
	XZSCT_NomTache        va_NomTache,
	XZSCT_NomMachine      va_NomMachine)

/*
* ARGUMENTS EN ENTREE :
*       va_maTache      : Nom de la tache fils
*       va_NomMachine   : Nom de la machine
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR :
*   Aucun

ONDITION D'UTILISATION
*   Aucune
*
* FONCTION
*   Appel de la fonction Fork d'Unix
*
------------------------------------------------------*/
{
	static char *version="@(#)sapl_com.c	1.11 11/06/95 : sp_startTache_Inter " ;
	int                   vl_Cr   = 0;
	static char           *vl_argvt[3];   /* tableau de pointeurs de strings */

	XZST_03EcritureTrace(   XZSTC_FONCTION,
		" sp_startTache_Inter : IN -- Tache %s -- machine %s \n",
				va_NomTache, va_NomMachine);

	/*A Creation de la commande de lancement de la tache */
		/*B nom de la commande          $0 */
	vl_argvt[0] = va_NomTache ;
		/*B nom de l'application                $1 */
	vl_argvt[1] = vg_NomAppli ;
		/*B argument 2  de la commande  $2 */
	vl_argvt[2] = NULL ;

	/*B lance la tache par le principe du fork unix */
	/*vl_Cr = vfork();*/
	/* TIBCO SS66*/
	vl_Cr = fork();
	if (vl_Cr == 0 )
	{
		/*B process fils */
		execvp( vl_argvt[0], vl_argvt );

		exit(0);
	}
	else
	{	
		/*B process pere */
		if (vl_Cr > 0)
		{
			/*B archive commande */
			XZST_10ArchiverMsgSyst( XDL_SUP01_CMDLTAC, va_NomTache,va_NomMachine,NULL);
		}
		else
		{
			 /*B  echec du fork */
			 XZST_03EcritureTrace(   XZSTC_WARNING,
					 "Echec du fork pour '%s'", va_NomTache);
		}
	}

	XZST_03EcritureTrace(   XZSTC_FONCTION,
		 " sp_startTache_Inter : OUT code retour %d \n",vl_Cr);

	/* Valeur de Retour */
	 return(vl_Cr) ;

}       /* Fin sp_startTache_Inter */



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 	Arret d'un sous-syteme
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_stopSSysteme(	
	XZSCT_NomSSysteme 	va_NomSSysteme,
	int			va_NumOrdre )

/*
* PARAMETRES EN ENTREE : - Nom du sous-systeme a arreter
*			 - Numero d'ordre (non utilise)
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
  static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_stopSSysteme" ;
  XZSCT_NomMachine	vl_NomMachine 	= "";
  int			vl_int 	= 0;


  XZST_03EcritureTrace(	XZSTC_FONCTION,
		"IN : sp_stopSSysteme : va_NomSSysteme='%s' va_NumOrdre=%d",
			va_NomSSysteme,va_NumOrdre);

  	/*A recupere nom de machine locale */
  	XZSC_07NomMachine(vl_NomMachine);

  	/*A archive commande */
  	XZST_10ArchiverMsgSyst(XDL_SUP04_CMDASSY,
				va_NomSSysteme,vl_NomMachine,NULL);

  	/*A arrete les taches du sous-systeme */
  	XZSA_04ListeTacheMachineSSyst (vl_NomMachine,va_NomSSysteme,
						sp_stopTache,&vl_int,NULL);

  	/*A Arrete les taches du sous-systeme qui ne sont plus 
					dans le fichier administration */
  	sp_stopTacheASupp(va_NomSSysteme);


  	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_stopSSysteme");

}	/* Fin sp_StopSSysteme */



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 	Lancement d'un sous-syteme
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_startSSysteme(	
	XZSCT_NomSSysteme 	va_NomSSysteme,
	int			va_NumOrdre )

/*
* PARAMETRES EN ENTREE : - Nom du sous-systeme a lancer
*			 - Numero d'ordre (non utilise)
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
  static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_startSSysteme" ;
  XZSCT_NomMachine	vl_NomMachine 	= "";
  int			vl_int 		= 0;



  XZST_03EcritureTrace(	XZSTC_FONCTION,
		"IN : sp_startSSysteme : va_NomSSysteme='%s' va_NumOrdre=%d",
			va_NomSSysteme,va_NumOrdre);

  	/*A recupere nom de machine locale */
  	XZSC_07NomMachine(vl_NomMachine);

  	/*A archive commande */
	XZST_10ArchiverMsgSyst(XDL_SUP05_CMDLSSY,va_NomSSysteme,
					vl_NomMachine,NULL);

  	/*A lance les taches du sous-systeme */
  	XZSA_04ListeTacheMachineSSyst(vl_NomMachine,va_NomSSysteme,
				(void (*)())sp_startTache,&vl_int,NULL);

  	/*A Arrete les taches du sous-systeme qui ne sont 
				plus dans le fichier administration */
  	sp_stopTacheASupp(va_NomSSysteme);

  	XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_startSSysteme");

}	/* Fin sp_startSSysteme */



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 	Arret d'une application
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_stopAppli() 

/*
* PARAMETRES EN ENTREE : aucun
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
  static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_stopAppli" ;
  XZSCT_NomMachine	vl_NomMachine 	= "";
  int			vl_int 		= 0;


  XZST_03EcritureTrace(XZSTC_FONCTION,"IN : sp_stopAppli");

  /*A recupere nom de machine locale */
  XZSC_07NomMachine(vl_NomMachine);

  /*A archive commande */
  XZST_10ArchiverMsgSyst(XDL_SUP06_CMDAAPP,vl_NomMachine,NULL);

  /*A arrete les sous systemes tournant sur la machine */
  XZSA_02ListeSSysteme(vl_NomMachine,sp_stopSSysteme,&vl_int,NULL);

  XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_stopAppli");

}	/* Fin sp_StopAppli */



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 	Lancement d'une application
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_startAppli() 

/*
* PARAMETRES EN ENTREE : aucun
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES :
*
******************************************************/
{
  static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_startAppli" ;
  XZSCT_NomMachine	vl_NomMachine 	= "";
  int			vl_Cr 		= 0;
  int			vl_int 		= 0;


  XZST_03EcritureTrace (XZSTC_FONCTION, "IN : sp_startAppli");

  /*A recupere nom de machine locale */
  XZSC_07NomMachine(vl_NomMachine);

  /*A archive commande */
  XZST_10ArchiverMsgSyst(XDL_SUP07_CMDLAPP,vl_NomMachine,NULL);

  /*A lance les taches applicatives devant tourner sur ma machine */
  XZSA_02ListeSSysteme(vl_NomMachine,sp_startSSysteme,&vl_int,NULL);

  XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : sp_startAppli");

}	/* Fin sp_startAppli */



/*X*/
/* Fichier : @(#)sapl_com.c	1.11	Release : 1.11        Date : 11/06/95
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_COM
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Supprime du tableau des taches, les taches 
*	n'apparaissant plus dans le fichier d'administration
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_stopTacheASupp(	XZSCT_NomSSysteme	va_NomSSysteme)

/*
* PARAMETRES EN ENTREE : - Nom du sous systeme pour lequel
*			   il faut supprimer les taches
*			   n'apparaissant plus dans le fichier
*			   d'administration
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
  	static char *version = "@(#)sapl_com.c	1.11 11/06/95 : sp_stopTacheASupp" ;
	int	vl_ind		= 0;


XZST_03EcritureTrace (XZSTC_FONCTION, "IN : sp_stopTacheASupp");

/*A Parcour du tableau des taches */
for (vl_ind=0 ; vl_ind<vg_sp_nb_surv_tach ; vl_ind++)
{
	/*A Si la tache appartient au sous systeme 
				et qu'elle doit etre supprimee */
	if ( (strcmp(pg_sp_surv_tach[vl_ind].SSysTacheASupp,
						va_NomSSysteme) == 0) &&
	     (pg_sp_surv_tach[vl_ind].TacheASupp == XDC_VRAI) )
	{
		/*A Si la tache est lancee */
		if (pg_sp_surv_tach[vl_ind].NbClient != 0)
		{
			/* Arret de la tache */
			sp_stopTache(pg_sp_surv_tach[vl_ind].NomTache,0);
		}

		strcpy(pg_sp_surv_tach[vl_ind].NomTache,"");
		pg_sp_surv_tach[vl_ind].Relance = SPC_RELANCE_OFF;
		pg_sp_surv_tach[vl_ind].TacheASupp = XDC_FAUX;
	}
}

XZST_03EcritureTrace (XZSTC_FONCTION, "OUT : sp_stopTacheASupp");

}	/* Fin sp_stopTacheASupp */


