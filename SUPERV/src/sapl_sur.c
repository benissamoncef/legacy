/*E*/
/*  Fichier : $Id: sapl_sur.c,v 1.23 2020/11/03 17:53:14 pc2dpdy Exp $        Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 17:53:14 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR * FICHIER sapl_sur.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module appartient a la tache tsaplg
* Il effectue la surveillance des datagroups afin
* de reactualiser les donnees de la tache TSAPLG
*
******************************************************
* HISTORIQUE :
*
* T. MIlleville  10 Oct 1994 	: Creation
* T. MIlleville  23 Avr 1995 	: Ajout de traces V1.3
* T. MIlleville  15 Mai 1995 	: Ajout d'un test permettant si il 
*	y a incoherence entre les tests de surveillance de DG et 
*	la presence UNIX des taches d'arreter le process  V1.3
* T. Milleville  25 Jul 1995 	: Ajout de commentaires V1.4
* T. Milleville  8 Aou 1995 	: Modification du traitement si il y a
*	incoherence si Nb abonnes=0 et existence du process sous UNIX.
*	Alors dans ce cas on tue le process et on relance suivant le flag 
*	de relance V1.5
* T. Milleville  25 Aou 1995 	: Modification appel XZSE_12 V1.6
* T. Milleville  29 Sep 1995 	: Envoie des etats des SSYS, des Taches
*	des Machines et des Traces si l'etat est different du precedent V1.8
* T. Milleville  5 Oct 1995 	: Indication du fait que la tache 
*	est NOK si le nb de clients = 0 V1.9
* T. Milleville  16  Oct 1995 	: Ajout memorisation nB client = 0 dans la focntion
* sp_status si DG != 0 et tache inexsitante sous UNIX V1.9
* T. Milleville  18  Oct 1995 	: Ajout de traces V1.10
* T. Milleville  24   Oct 1995 	: Modification sp_genemach pour envoi de 
*	l'etat si different dui precedent V1.11
* T. Milleville  03 Nov  1995 	: Ajout dans la fonction de reconnexion entre RTServeur
*	l'envoi du msg de _disconnect avant le _connect V1.11
* T. Milleville  10 Nov  1995 	: Si le nb du datagroup _server est different de 3
*	alors on execute un test reseau sur sa propre machine pour permettre sur 
*	le retour du test de declencher un "_disconnect _connect" dans tsetat.x V1.11
* P.NIEPCERON	16 Fev 1996 : Ajout d'un sleep (conseil unitechnic) lors d'incoherence DG et unix v1.12
* P.NIEPCERON   06 Mar 1996 : Remplacement du sleep par une demande d'arret/relance de la tache: 
		ceci permet de depiler les msg de watch DG=0 avant de relancer la tache	v1.13
                             Supperession de la tache de lancement en differe v1.14
* P.NIEPCERON   26 Jun 1996 : Modif de la demande arret lors d'incoherence par arret_marche v1.15
* P.NIEPCERON   23 dec 1997	: _disconnect devient disconnect avec RTW3.5 (idem _connect)v1.16
* ??????	xx Jul  2007	: Passage RT Server 55 vers Smart Sockets 66  v1.17
* P.FERRERO	16 avr 2009	: Suppr. Warning l.975; Ajout de traces pour pb. Itanium v1.18
* JMG		26/06/10 : portage linux dem/934 1.19
* JPL		16/12/10 : Migration architecture HP ia64 (DEM 961) : suppression erreurs mineures  1.20
* JPL		12/01/11 : Migration architecture HP ia64 (DEM 971) : suppression warnings mineurs  1.21
* LCL	04/10/23	: Correction sp_watch_cb sur type de variable tibemsCollection pour adaptation CNA DEM-483
*******************************************************/


/* fichiers inclus */
#include "xdc.h"
#include "xdg.h"
#include "xdl.h"
#include "xdm.h"

#include "xzsa.h"
#include "xzsc.h"
#include "xzss.h"
#include "xzst.h"
#include "xzsp.h"
#include "xzems.h"

#include "sapl_sur.h"
#include "sapl_com.h"
#include "sapl_don.h"


/* definitions de constantes */
#define 	C_NOM_DGSERVER	"_server"
#define 	C_NB_RTSERVER	3


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char *version = "@(#)sapl_sur.c	1.16 02/09/98 : SAPL_SUR" ;

/*static int	SPV_MACHINE_ETAT	= XZSEC_ETAT_NACTIF; */
static int	SPV_MACHINE_ETAT	= XZSEC_SANS_ETAT; 
static T_CB	vm_callbackdiff	= NULL;

/* declaration de fonctions internes */
int	sp_verifTacheMorte();
int	sp_verifTacheVivante();
int	sp_TestSurvDG(XZSCT_Datagroup pl_NomDatagroup);



/*X*/
/*  Fichier : $Id: sapl_sur.c,v 1.23 2020/11/03 17:53:14 pc2dpdy Exp $        Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 17:53:14 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Mise a jour de l'etat du sous-systeme dont
*   depend la tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_genetat_ssys ( SPT_SurvTach	*pa_surv_tach )

/*
* PARAMETRES EN ENTREE : - pointeur vers la structure de
*			la tache a surveiller
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
*
* - Impossibilite de trouver le sous systeme de la tache
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *version = "@(#)sapl_sur.c	1.16 02/09/98 : sp_genetat_ssys" ;
  SPT_SurvSsys   	*pl_surv_ssys 	= NULL;
  XZSCT_NomSSysteme	vl_NomSSysteme 	= "";
  XZSCT_NomMachine	vl_NomMachine 	= "";
	/* TRUE ou FALSE selon que le sous	*
	* systeme est sans etat ou non		*/
  int			vl_SSystSansEtat	= XDC_FAUX;	
  	/* TRUE ou FALSE selon que le sous      *
	* systeme est a l'etat OK ou non	*/
  int			vl_SSystEtatOk	= XZSEC_ETAT_OK;		
  int			vl_ind	= 0;			/* indice de boucle */
  int 			vl_ValRet	= XDC_OK;



  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_genetat_ssys " ) ;

  /* Recupere le nom du sous systeme de la tache */
  if ( (XZSA_08TacheNomSSysteme(pa_surv_tach->NomTache,vl_NomSSysteme)) 
								!= XDC_OK )
  {
	XZST_03EcritureTrace(	XZSTC_WARNING,
			"Sous systeme de la tache '%s' non trouve par XZSA",
				pa_surv_tach->NomTache);
  }
  else
  {
	/* Recupere la structure du sous systeme */
	if ( (pl_surv_ssys = sp_rechercherSSysteme(vl_NomSSysteme) ) == NULL )
	{ 	/* sous-systeme inconnu de SPDONNEES  */
		XZST_03EcritureTrace(	XZSTC_WARNING,
				"Sous systeme '%s' non trouve par SPDONNEES",
					vl_NomSSysteme ) ;
	}
	else
	{
  		/*A recupere nom de machine locale */
		XZSC_07NomMachine(vl_NomMachine);

  XZST_03EcritureTrace(XZSTC_DEBUG2, " NB de taches surveillees IN : sp_genetat_ssys " ) ;
		/*A Determination de l'etat du sous-systeme */
		for (vl_ind=0;vl_ind<vg_sp_nb_surv_tach;vl_ind++)
		{
			/*A Si le nom du Sous-Systeme correspond */
			if ( (strcmp(pg_sp_surv_tach[vl_ind].SSysTacheASupp,
							vl_NomSSysteme) == 0) )
			{				
				/*A Teste si la tache est Temporaire */
				vl_ValRet = XZSA_11TacheTemporaire(
					pg_sp_surv_tach[vl_ind].NomTache);
				if (	(vl_ValRet == XZSAC_TYPE_TPER) &&
			(pg_sp_surv_tach[vl_ind].EtatTache == XZSEC_ETAT_NOK) )
				{
					/*A Memorisation de l'etat OK du SSyst
					*	a FAUX */
					/*vl_SSystEtatOk = XDC_FAUX;*/
					vl_SSystEtatOk = XZSEC_ETAT_NOK;
				}
				if (	(vl_ValRet == XZSAC_TYPE_TPER) &&
			(pg_sp_surv_tach[vl_ind].EtatTache == XZSEC_ETAT_OK) )
				{
					/*A Memorisation de l'etat OK du SSyst
					*	a FAUX */
					/*vl_SSystSansEtat = XDC_FAUX;*/
					vl_SSystSansEtat = XZSEC_ETAT_NOK;
				}

			}
		}
#if 0   /* Modif V1.8 */
		/* On determine la valeur de l'etat du sous-systeme. */
		if (vl_SSystEtatOk == XDC_VRAI)
		{
			pl_surv_ssys->Etat = XZSEC_ETAT_OK;
		}
		else 
		{
			pl_surv_ssys->Etat = XZSEC_ETAT_NOK ;
		}


		/* On determine la valeur de l'etat du sous-systeme. */
		if (vl_SSystSansEtat == XDC_VRAI)
		{
			pl_surv_ssys->Etat = XZSEC_SANS_ETAT;
		}
		else
		{
			if (vl_SSystEtatOk == TRUE)
			{
				pl_surv_ssys->Etat = XZSEC_ETAT_OK;
			}
			else 
			{
				pl_surv_ssys->Etat = XZSEC_ETAT_NOK ;
			}
		}	
#endif

#if 1   /* Modif V1.8 */
		XZST_03EcritureTrace(	XZSTC_DEBUG1,
			"Etat SSys precedent %d -- actuel %d \n",pl_surv_ssys->Etat,vl_SSystEtatOk);

		/*B Envoie de l'etat si l'etat est different du precedent V1.8 */
		if (pl_surv_ssys->Etat != vl_SSystEtatOk)
		{
			pl_surv_ssys->Etat = vl_SSystEtatOk;

			XZSE_11EnvoieEtatSSysteme( vl_NomMachine,
				vl_NomSSysteme,pl_surv_ssys->Etat);
		}
#endif
	}
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_genetat_ssys " ) ;

}  /* sp_genetat_ssys */



/*X*/
/*  Fichier : $Id: sapl_sur.c,v 1.23 2020/11/03 17:53:14 pc2dpdy Exp $        Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 17:53:14 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Mise a jour de l'etat de la machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_genetat_mach ()

/*
* PARAMETRES EN ENTREE : aucun
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR : neant
A
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  	static char *version = "@(#)sapl_sur.c	1.16 02/09/98 : sp_genetat_mach" ;
  	int  			vl_EtatMachine 	= XZSEC_SANS_ETAT;
  	int  			vl_EtatMachineLoc 	= XZSEC_SANS_ETAT;
  	int			vl_Ind		= 0;
  	XZSCT_NomMachine	vl_NomMachine 	= "";


  	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_genetat_mach" ) ;

  	/*A recupere nom de machine locale */
  	XZSC_07NomMachine(vl_NomMachine);

	/*A Recuperation Etat de la Machine Locale  */
	XZSE_09EtatMachine(&vl_EtatMachineLoc);

  	/*A pour chaque sous-systeme */
  	for(vl_Ind=0;vl_Ind<vg_sp_nb_surv_ssys;vl_Ind++) 	
  	{
		/*B Teste si l'etat de celui est OK ou pas */
    		if (pg_sp_surv_ssys[vl_Ind].Etat == XZSEC_ETAT_NOK)
    		{
			/*B Si etat non Ok alors memorisation du mode degrade **/
      			vl_EtatMachine = XZSEC_ETAT_DEGRADE;

			break;
    		}

		/*A Teste si la machine est Passive */
		if (vl_EtatMachineLoc == XZSEC_ETAT_NPASSIF)
		{
			/*B Memorisation de l'etat Passif de la Machine */
			vl_EtatMachine = XZSEC_ETAT_NPASSIF;
		}
		else
		{
			/*B Memorisation de l'etat Actif de la Machine */
			vl_EtatMachine = XZSEC_ETAT_NACTIF;
		}
  	}

/* Modif V1.10 */
	/* Envoi de l'etat si modification */
	if (SPV_MACHINE_ETAT != vl_EtatMachine)
	{
  		/*A envoi de l'etat de la machine  */
  		XZSE_13EnvoieEtatMachine(vl_NomMachine, vl_EtatMachine);
	}

	/*A Memorisation de l'etat du calculateur dans la variable globale */
	SPV_MACHINE_ETAT = vl_EtatMachine;


  	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_genetat_mach -- Etat machine : %d \n ", SPV_MACHINE_ETAT) ;

}  /* sp_genetat_mach */




/* declaration de fonctions externes */


#ifndef _TIBCO_EMS
/*X*/
/*  Fichier : $Id: sapl_sur.c,v 1.23 2020/11/03 17:53:14 pc2dpdy Exp $        Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 17:53:14 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*   Reception des messages de type T_MT_DG_STATUS,
*   indiquant le nombre d'abonnes a un datagroup.
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_dgstatus_cb (	
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
* CONDITION D'ERREUR :
*
* - La tache dont le nombre d'abonnes vient de changer
*   n'est pas connus dans le tableau de surveillance des
*   taches
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  	static char *version = "@(#)sapl_sur.c	1.16 02/09/98 : sp_dgstatus_cb" ;
  	T_STR			pl_NomDatagroup = "";
  	T_INT4		vl_NbAbonnes 	= 0;

        T_STR   subject_name;
        T_STR   *subscribe_client_names;
        T_INT4  num_subscribe_client_names;
        T_STR   start_client;
        T_STR   stop_client;
        int   i;

  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_dgstatus_cb" );

  /*TIBCO SS66*/
#if 0
  if (TipcMsgRead(pa_Data->msg,
		T_IPC_FT_STR,&pl_NomDatagroup,
		T_IPC_FT_INT4,&vl_NbAbonnes, NULL))
  {
	XZST_03EcritureTrace(	XZSTC_MESSAGE,
				"%d abonnes sur '%s'",
				vl_NbAbonnes,pl_NomDatagroup);

	XZST_03EcritureTrace(	XZSTC_WARNING,
				"Reception ok STATUS %d abonnes sur '%s'",
				vl_NbAbonnes,pl_NomDatagroup);

	/*B Appel fonction de traitement de lancement de taches */
	sp_dgstatus(pl_NomDatagroup,vl_NbAbonnes);

  }
  else
  {
	XZST_03EcritureTrace(   XZSTC_WARNING,"Erreur reception DG_STATUS \n");
  }

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_dgstatus_cb" );
#endif

  TipcMsgSetCurrent(pa_Data->msg, 0);
  if (!TipcMsgNextStr(pa_Data->msg, &subject_name)
      || !TipcMsgNextStrArray(pa_Data->msg, &subscribe_client_names,
                                &num_subscribe_client_names)
      || !TipcMsgNextStr(pa_Data->msg, &start_client)
      || !TipcMsgNextStr(pa_Data->msg, &stop_client))
  {
        XZST_03EcritureTrace(   XZSTC_WARNING,"Erreur reception DG_STATUS \n");
        XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_dgstatus_cb" );
        return;
  }

  XZST_03EcritureTrace( XZSTC_WARNING,
                "%d abonnes sur '%s'",
                num_subscribe_client_names,subject_name);

/* 
	PB SMART SOCKET : ITANIUM :  alors on affiche le nom des clients
*/

  for (i = 0;  i < num_subscribe_client_names;  i++)
  {
    XZST_03EcritureTrace( XZSTC_DEBUG1, "Nom Abonne : '%s'", subscribe_client_names[i]);
  }

  /*B Appel fonction de traitement de lancement de taches */
  /*sp_dgstatus(pl_NomDatagroup,vl_NbAbonnes);*/
  sp_dgstatus(subject_name,num_subscribe_client_names);


  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_dgstatus_cb" );


} /* sp_dgstatus_cb */


#endif

/*X*/
/*  Fichier : $Id: sapl_sur.c,v 1.23 2020/11/03 17:53:14 pc2dpdy Exp $        Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 17:53:14 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Verifie q'une tache est morte en utilisant le
*	noyau UNIX
*
******************************************************
* SEQUENCE D'APPEL :
*/

int	sp_verifTacheMorte(SPT_SurvTach	*pa_tache)

/*
* PARAMETRES EN ENTREE : pa_tache : pointeur sur la 
*			structure decrivant la tache
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : XDC_OK : tache morte
*			XDC_NOK : tache vivante
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  	static char *version = "@(#)sapl_sur.c	1.16 02/09/98 : sp_verifTacheMorte" ;
	int		vl_ValRet	= XDC_NOK;
	int		vl_statLoc	= 0;
	XZSCT_NomTache	vl_maTache	= "";


	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_verifTacheMorte" );

	/*A Recuperation du nom de ma tache */
	XZSC_06NomTache(vl_maTache);

	/*A S'il s'agit de soi meme */
	if (strcmp(pa_tache->NomTache,vl_maTache) == 0)
	{
		pa_tache->Pid = 0;
		vl_ValRet=XDC_NOK;
	}
	else
	{
		/* On verifie que la tache est morte pour cette application */
		/* avec XZSS_20 qui regarde le repertoire /proc	       */
		/*pa_tache->Pid = XZSS_20PidTache(pa_tache->NomTache,vg_NomAppli);*/
		pa_tache->Pid = XZSS_20PidTache(pa_tache->NomTache);
		if (pa_tache->Pid == 0)
		{
	       		/* OK la tache est inexistante */
			vl_ValRet=XDC_OK;
		}
		else
		{
	       		/* NOK la tache est vivante */
			vl_ValRet=XDC_NOK;
		}
	}

	XZST_03EcritureTrace(	XZSTC_INFO,
			"sp_verifTacheMorte : tache %s pid=%d",
			pa_tache->NomTache,pa_tache->Pid);

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_verifTacheMorte" );

	return(vl_ValRet);

}	/*Fin  sp_verifTacheMorte */



/*X*/
/*  Fichier : $Id: sapl_sur.c,v 1.23 2020/11/03 17:53:14 pc2dpdy Exp $        Release : $Revision: 1.23 $        Date : $Date: 2020/11/03 17:53:14 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE SAPL_SUR
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Verifie qu'une tache tourne toujours en
*	utilisant le noyau UNIX
*
******************************************************
* SEQUENCE D'APPEL :
*/

int	sp_verifTacheVivante(SPT_SurvTach	*pa_tache)

/*
* PARAMETRES EN ENTREE : pa_tache : pointeur sur la 
*			structure decrivant la tache
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : XDC_OK : tache existante
*			XDC_NOK : tache inexistante
*
* CONDITION D'ERREUR : neant
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  	static char *version = "@(#)sapl_sur.c	1.16 02/09/98 :sp_verifTacheVivante" ;
	int		vl_crWaitPid	= 0;
	int		vl_statLoc	= 0;
	int		vl_ValRet	= XDC_NOK;
	XZSCT_NomTache	vl_maTache	= "";


	XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_verifTacheVivante" );

	/*A Recuperation du nom de ma tache */
	XZSC_06NomTache(vl_maTache);

	/* S'il s'agit de soi meme */
	if (strcmp(pa_tache->NomTache,vl_maTache) == 0)
	{
		pa_tache->Pid = 0;
		vl_ValRet=XDC_OK;
	}
	else
	{
		/* On verifie que la tache est vivante pour cette application */
		/* avec XZSS_20 qui regarde le repertoire /proc		 */
		/*pa_tache->Pid = XZSS_20PidTache(pa_tache->NomTache,vg_NomAppli);*/
		pa_tache->Pid = XZSS_20PidTache(pa_tache->NomTache);
		if (pa_tache->Pid == 0)
		{
	        	/* NOK la tache est inexistante */
			vl_ValRet=XDC_NOK;
		}
		else
		{
	        	/* OK la tache est bien vivante */
			vl_ValRet=XDC_OK;
		}
	}

	XZST_03EcritureTrace(	XZSTC_INFO,
			"sp_verifTacheVivante : tache %s pid=%d",
			pa_tache->NomTache,pa_tache->Pid);

	XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_verifTacheVivante" );

	return(vl_ValRet);

}	/* Fin  sp_verifTacheVivante */

#ifdef _TIBCO_EMS
void sp_watch_cb(
        T_IPC_CONN                      va_Cnx,
        T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
        T_CB_ARG                        pa_Arg )
{
tibemsErrorContext errorContext;
#ifdef _RABBITMQ
int consumerInfoCollection=NULL;
XZST_03EcritureTrace( XZSTC_WARNING,"sp_watch_cb RABBIT");
#else
tibemsCollection consumerInfoCollection=NULL;
XZST_03EcritureTrace( XZSTC_WARNING,"sp_watch_cb EMS");
#endif
char vl_NomMachine[100];
char vl_str[100];
tibems_status    status;
int vl_ind;

	XZSC_07NomMachine(vl_NomMachine);

	for (vl_ind=0;vl_ind<vg_sp_nb_surv_tach;vl_ind++)
	{
		if (strlen(pg_sp_surv_tach[vl_ind].NomTache)>0) 
		{
			sprintf(vl_str,"%s_%s",vl_NomMachine,pg_sp_surv_tach[vl_ind].NomTache);

			consumerInfoCollection=XZEMS_03IsTopic(vl_str);
			if (consumerInfoCollection==NULL)
       		{
  				XZST_03EcritureTrace( XZSTC_WARNING,"sp_watch_cb : mise a 0 du status '%s'", vl_str);
				sp_dgstatus(vl_str,0);
       		}
			else
       		{
  				XZST_03EcritureTrace( XZSTC_WARNING,"sp_watch_cb : mise a 1 du status '%s'", vl_str);
				sp_dgstatus(vl_str,1);
				
				XDG_tibemsCollection_Destroy(consumerInfoCollection);
       		}

        	}
        }

}	
#endif


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer a la tache TSETAT, l'etat des machines et 
*	des sous-systemes 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

void sp_etat_cb(
	T_IPC_CONN                      va_Cnx,
	T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
	T_CB_ARG                        pa_Arg )

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
*   aucune
*
* FONCTION 
*   Pour chaque tache , envoie l'etat de celle-ci par la fonction
*	XZSE_12 . Pour chaque SSysteme envoie l'etat de celui-ci
*	par la fonction XZSE_11. Enfin envoie l'etat du calculateur
*	par la fonction XZSE_13
*
------------------------------------------------------*/
{
  	static char *version = "@(#)sapl_sur.c	1.16 02/09/98 :sp_etat_cb" ;
	int 			vl_ValRet	= XDC_OK;
	int			vl_Ind		= 0;
  	XZSCT_NomMachine	vl_NomMachine 	= "";



	XZST_03EcritureTrace( XZSTC_FONCTION, " sp_etat_cb : IN \n");


	/*A recupere nom de machine locale */
	XZSC_07NomMachine(vl_NomMachine);

#if 0
	/*A Pour chaque element du tableau , envoie l'etat de 
	*	la tache **/
	for (vl_Ind=0;vl_Ind<vg_sp_nb_surv_tach;vl_Ind++)
	{
		/*B Envoie etat de la Tache */
		XZSE_12EnvoieEtatTache(vl_NomMachine,
				pg_sp_surv_tach[vl_Ind].NomTache,
				pg_sp_surv_tach[vl_Ind].EtatTache, 10);
	}
#endif

	/* Pour chaque cwelement du tableau, envoie l'etat du 
	*		SSysteme */
	for(vl_Ind=0;vl_Ind<vg_sp_nb_surv_ssys;vl_Ind++)
	{ 
		/*A Envoie l'etat du SSysteme */
		XZSE_11EnvoieEtatSSysteme(
			vl_NomMachine, 
			pg_sp_surv_ssys[vl_Ind].NomSSysteme,
			pg_sp_surv_ssys[vl_Ind].Etat);
	}

	/* Envoie etat de la machine **/
	XZSE_13EnvoieEtatMachine(vl_NomMachine, SPV_MACHINE_ETAT);


	XZST_03EcritureTrace( XZSTC_FONCTION, " sp_etat_cb : OUT \n");

}	/* Fin sp_etat_cb */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer le msg de changement de connexion
*	entre les Serveurs
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int sapl_connect_rtserver()

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
*   XDC_OK : ok
*   XDC_NOK : nok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Envoie le msg _connect sur le datagroup _server
*
------------------------------------------------------*/
{
	static char *version="@(#)sapl_sur.c	1.16 02/09/98 : sapl_connect_rtserver " ;
	int	vl_ValRet	= XDC_OK;
	char             CommandeRTW[100]        = "\0";
	XZSCT_NomMachine        vl_NomMachine   = "";


	XZST_03EcritureTrace(XZSTC_FONCTION,"sapl_connect_rtserver :  IN \n");


#if 0	/* Modif V1.18 pour declencher un test reseau sur sa propre machine
	*	afin de verifier la deconnexion avec les autres Serveurs */

	/*B Nom de Machine */
	XZSC_07NomMachine(vl_NomMachine);

	if (XZSG_01SurveillerReseau(vl_NomMachine) != XDC_OK)
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			       "seta_arme_testres : echec de la programmation du test reseau");
	};
#endif


#if 0

	sprintf(CommandeRTW, "setopt server_auto_flush_size 1");
	TutCommandParseStr(CommandeRTW);

	XZST_03EcritureTrace(XZSTC_MESSAGE,
		"Envoi message de reconnexion RTServer  \n");
	/*B Envoie du message de Controle _disconnect
	* aux differents RTServers */
	if (!TipcSrvMsgWrite(T_IPC_SUBJECT_SERVER,
			XDM_IdentMsg(T_MT_CONTROL),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_STR,"disconnect",
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur TipcSrvMsgWrite T_MT_CONTROL \n");

		vl_ValRet = XDC_NOK;
	}
					
	TipcSrvFlush();

	/*B Envoie du message de Controle _connect
	* aux differents RTServers */
	if (!TipcSrvMsgWrite(T_IPC_SUBJECT_SERVER,
			XDM_IdentMsg(T_MT_CONTROL),
			XDM_FLG_SRVMSGWRITE,
			T_IPC_FT_STR,"connect",
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur TipcSrvMsgWrite T_MT_CONTROL \n");

		vl_ValRet = XDC_NOK;
	}
	TipcSrvFlush();
#endif

	XZST_03EcritureTrace(XZSTC_FONCTION,"sapl_connect_rtserver :  OUT \n");

	return (vl_ValRet);

}	/* Fin sapl_connect_rtserver */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet de verifier le lancement d'une tache et la diffusion
*	de l'etat 
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int sp_dgstatus(
	XZSCT_Datagroup 	va_NomDatagroup,
	int			va_NbAbonnes )

/*
* ARGUMENTS EN ENTREE :
*   va_NomDatagroup : Nom du Datagroup 
*   va_NbAbonnes	: nombre d'abonnes
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*   XDC_NOK	: nok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Verifie l'existence d'une tache, par rapport au nombre
*	d'abonnes. Ensuite suivant les cas, envoie l'etat de la tache
*	des sous-systemes et des machines
*
------------------------------------------------------*/
{
	static char *version="@(#)sapl_sur.c	1.16 02/09/98 : sp_dgstatus " ;
  	XZSCT_NomMachine	vl_NomMachine	= "";
  	char			*pl_char 	= NULL;
  	SPT_SurvTach		*pl_surv_tach 	= NULL;
  	XZSCT_NomTache	vl_maTache	= "";
		/* Indication si Tache Temporaire */
  	int 			vl_IndicTmp	= XDC_FAUX;
  	int			vl_ValRet	= XDC_OK;
	int			vl_Pid		= 0;
	int			i		= 0;


  XZST_03EcritureTrace(XZSTC_FONCTION, "IN : sp_dgstatu %s %d",va_NomDatagroup,
  			va_NbAbonnes);


	/*A Teste si le datagroup est _server , afin de verifier les
	*	connexions entre les serveurs */
	if (!strcmp(va_NomDatagroup,C_NOM_DGSERVER))
	{
		if (va_NbAbonnes < C_NB_RTSERVER)
		{
			/*B Envoie du msg de changement de connexion RTserver */
			sapl_connect_rtserver();
		}
	}
	/* determine le nom de la tache selon le nom du datagroup */
	else if ( ( pl_char = strrchr ( (char *)va_NomDatagroup , '_' ) ) == NULL )
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
				"erreur de formatage pl_NomDatagroup");
	}
	else
	{
		/* decale d'un caractere pour pointer sur 
		*	le premier caractere apres _  , pour recuperer
		*	le nom de la tache  */
		pl_char ++ ;


		/*A recherche dans le tableau du module SPDONNEES */
  		if ( ( pl_surv_tach = sp_rechercherTache( pl_char ) ) == NULL )
		{ 	
			/*A tache inconnue */
			XZST_03EcritureTrace(XZSTC_WARNING, 
					"tache inconnue '%s'", pl_char ) ;
		}
		else
		{	
			/*A tache connue */
			/*A memorise le nombre d'abonnes */
			pl_surv_tach->NbClient = va_NbAbonnes ;
			
  			/*A recupere nom de machine locale */
			XZSC_07NomMachine(vl_NomMachine);

			/*A Recuperation du fait que la tache est temporaire
			*	ou pas */
			if ( (vl_ValRet = XZSA_11TacheTemporaire(
					pl_surv_tach->NomTache)) == XDC_OK )
			{
				/*A Memorisation Tache Temporaire */
				vl_IndicTmp = XDC_VRAI;
			}
				
			if    ( pl_surv_tach->NbClient == 0 ) 
			{  
				/* aucun abonne 
				*	==> equivaut a l'arret de la tache */

				/* Verification unix que la 
				*		tache ne tourne plus */
			        if (sp_verifTacheMorte(pl_surv_tach) == XDC_OK)
				{
					/* archive evenement */
					XZST_10ArchiverMsgSyst(XDL_SUP09_DISPTAC						,pl_char,vl_NomMachine,NULL);

					/* Memorisation Pid = 0 */
					pl_surv_tach->Pid = 0;
					
			XZST_03EcritureTrace(XZSTC_WARNING,"Tache Morte \
				Relance %d \n",pl_surv_tach->Relance);


					if (pl_surv_tach->Relance 
							== SPC_RELANCE_ON)
					{
						/* Indication tache NOK */
						pl_surv_tach->EtatTache = XZSEC_ETAT_NOK;

						/* il faut relancer la tache */
						XZST_10ArchiverMsgSyst(
				XDL_SUP10_RELATAC,pl_char,vl_NomMachine,NULL);
	
						/* redemarre la tache */
						sp_startTache( pl_char, -1  ) ;
					}
					else
					{
						/*A Teste si Temporaire */
						if (vl_IndicTmp == XDC_FAUX)
						{
						   /*B Tache non Temporaire, donc 
						   *   etat tache --> ETAT_NOK  */
						   if ( pl_surv_tach->EtatTache != XZSEC_ETAT_NOK)
						   {
						   	pl_surv_tach->EtatTache =
									XZSEC_ETAT_NOK;

							/*B Envoie Etat de la Tache */
							XZSE_12EnvoieEtatTache ( vl_NomMachine, 
								pl_char, pl_surv_tach->EtatTache, 10 ) ;
						   }
						}
						else
						{
						   /*B Tache Temporaire, donc 
						   *   etat tache --> SANS_ETAT */
						   if ( pl_surv_tach->EtatTache != XZSEC_SANS_ETAT)
						   {
						   	pl_surv_tach->EtatTache =
									XZSEC_SANS_ETAT;
							/*B Envoie Etat de la Tache */
							XZSE_12EnvoieEtatTache ( vl_NomMachine, 
								pl_char, pl_surv_tach->EtatTache, 10 ) ;
						   }
						}
						/* genere l'etat du SSysteme 						          			  *   et de la machine */
						sp_genetat_ssys(pl_surv_tach);
						sp_genetat_mach ( ) ;
					}
				}
				else
				{
			           XZST_03EcritureTrace(XZSTC_WARNING, 
				"Incoherence RTworks DG = 0 et tache %s \
				existante sous unix", pl_surv_tach ) ;
			
				if (strncmp(pl_surv_tach->NomTache,"tele",4)==0)
					return (XDC_OK);

#ifndef _HPUX_SOURCE
/*				if (strncmp(pl_surv_tach->NomTache,"tseta",5)==0)
					return (XDC_OK);*/
                                        if (strncmp(pl_surv_tach->NomTache,"tstim",5)==0)
                                                 return (XDC_OK);
                                        if (strncmp(pl_surv_tach->NomTache,"tshor",5)==0)
                                                 return (XDC_OK);
#endif
			           XZST_03EcritureTrace(XZSTC_WARNING, 
				"Alors envoi kill %s %d", pl_surv_tach->NomTache,pl_surv_tach->Pid ) ;

					   
					/*B On tue le process existant */
					vl_ValRet = kill( pl_surv_tach->Pid,SIGKILL);
					if (vl_ValRet == 0)
					{
						XZST_03EcritureTrace(   XZSTC_INFO,
							"SIGKILL bien envoye a %s (PID=%d)",
							 pl_surv_tach->NomTache, pl_surv_tach->Pid);
					}
					else
					{
						XZST_03EcritureTrace(   XZSTC_WARNING,
							"Erreur lors de l'envoie de SIGKILL a %s (PID=%d)",
							 pl_surv_tach->NomTache, pl_surv_tach->Pid);
					}

					/* Memorise le PID a 0 */
					pl_surv_tach->Pid = 0 ;

					/*A Teste si Temporaire */
					if (vl_IndicTmp == XDC_FAUX)
					{
			                	XZST_03EcritureTrace(XZSTC_DEBUG1, 
							"Tache Permanente %s \n",pl_surv_tach->NomTache);

						/* Teste si etat different du precedent */
						if (pl_surv_tach->EtatTache != XZSEC_ETAT_NOK)
						{
							/*B Tache NOK */
							pl_surv_tach->EtatTache= XZSEC_ETAT_NOK;

							/*B Envoie etat de la tache */
							XZSE_12EnvoieEtatTache(vl_NomMachine,
								pl_char, pl_surv_tach->EtatTache, 10);
						}

						/* Test si tache doit etre relancee */
						if (pl_surv_tach->Relance == SPC_RELANCE_ON)
						{
			                		XZST_03EcritureTrace(XZSTC_WARNING, 
								"Tache Permanente on relance la tache \n");
							 XZST_03EcritureTrace(XZSTC_WARNING,"en fait non, on a la supervision (sic) le redemarrage de la tache"); /*1.13*/
							 XZSP_03CmdTache(  vl_NomMachine,pl_surv_tach->NomTache, XZSPC_ARRET_MARCHE );
							 XZSP_03CmdTache(  vl_NomMachine,pl_surv_tach->NomTache, XZSPC_MARCHE );
							 XZSP_03CmdTache(  vl_NomMachine,pl_surv_tach->NomTache, XZSPC_MARCHE );
							 /*sp_startTache( pl_surv_tach->NomTache, -1  );*/
						}
					}
					else
					{
						/* Teste si etat different du precedent */
						if (pl_surv_tach->EtatTache != XZSEC_SANS_ETAT)
						{
							/*B Tache Temporaire */
							pl_surv_tach->EtatTache= XZSEC_SANS_ETAT;

							/*B Envoie etat de la tache */
							XZSE_12EnvoieEtatTache(vl_NomMachine,
								pl_char, pl_surv_tach->EtatTache, 10);
						}
					}

					/*B genere l'etat du SSysteme 
					*	et de la machine */ 
					sp_genetat_ssys(pl_surv_tach) ;
					sp_genetat_mach ( ) ;
					
				}
			}
			/* Au moins 1 abonne */
#ifdef _HPUX_SOURCE
			else if ( pl_surv_tach->NbClient == 1 )
#else
			else if ( pl_surv_tach->NbClient >= 1 )
#endif
			{	
#ifndef _TIBCO_EMS
				/* si la tache tourne au point de vue unix*/
				if (sp_verifTacheVivante(pl_surv_tach) 
								== XDC_OK)
				{
#endif
					/* Teste si etat different du precedent */
					if (pl_surv_tach->EtatTache != XZSEC_ETAT_OK)
					{
						/* Emission de l'etat de la tache */
						pl_surv_tach->EtatTache = XZSEC_ETAT_OK;
						XZSE_12EnvoieEtatTache ( vl_NomMachine, 																pl_char, pl_surv_tach->EtatTache , 10 ) ;

#ifdef _TIBCO_EMS
					/*B genere l'etat du SSysteme 
					*	et de la machine */ 
					sp_genetat_ssys(pl_surv_tach) ;
					sp_genetat_mach ( ) ;
#endif

					}

					/*B Teste si Temporaire */
					if (vl_IndicTmp == XDC_FAUX)
					{
						/*B On force la relance  automatique */
						pl_surv_tach->Relance = 
								SPC_RELANCE_ON;
					}
#ifndef _TIBCO_EMS

					/*B genere l'etat du SSysteme 
					*	et de la machine */ 
					sp_genetat_ssys(pl_surv_tach) ;
					sp_genetat_mach ( ) ;
				}
				else
				{
			                XZST_03EcritureTrace(XZSTC_WARNING, 
				"Incoherence numero 2 RTworks DG != 0 et tache %s \
				inexistante sous unix", pl_surv_tach ) ;

					/* On positionne le nombre de clients a 0 */
					pl_surv_tach->NbClient = 0;

					
					/*A Teste si Temporaire */
					if (vl_IndicTmp == XDC_FAUX)
					{
						/* Teste si etat different du precedent */
						if (pl_surv_tach->EtatTache != XZSEC_ETAT_NOK)
						{
							/*B Tache Permanente */
							pl_surv_tach->EtatTache= XZSEC_ETAT_NOK;

							/* Emission de l'etat de la tache */
							XZSE_12EnvoieEtatTache ( vl_NomMachine, pl_char, pl_surv_tach->EtatTache, 10 ) ;
						}
					}
					else
					{
						if (pl_surv_tach->EtatTache != XZSEC_SANS_ETAT)
						{
							/*B Tache Temporaire */
							pl_surv_tach->EtatTache= XZSEC_SANS_ETAT;

							/* Emission de l'etat de la tache */
							XZSE_12EnvoieEtatTache ( vl_NomMachine, pl_char, pl_surv_tach->EtatTache, 10 ) ;
						}
					}

#ifdef _HPUX_SOURCE
#else
                                        if (pl_surv_tach->Relance
                                                        == SPC_RELANCE_ON)
                                        {
                                                /* Indication tache NOK */
                                                pl_surv_tach->EtatTache = XZSEC_ETAT_NOK;

                                                /* il faut relancer la tache */
                                                XZST_10ArchiverMsgSyst(
                                XDL_SUP10_RELATAC,pl_char,vl_NomMachine,NULL);

                                                /* redemarre la tache */
                                                sp_startTache( pl_char, -1  ) ;
                                        }
#endif

					/*B genere l'etat du SSysteme 
					*	et de la machine */ 
					sp_genetat_ssys(pl_surv_tach) ;
					sp_genetat_mach ( ) ;
				}
#endif
			}
#ifdef _HPUX_SOURCE
			else if ( pl_surv_tach->NbClient > 1)
			{
				/* Alors on tue les taches : SI PB SMART SOCKET ITANIUM : Ne plus tuer les taches */ 
				for (i=0;i< pl_surv_tach->NbClient;i++)
				{

				  if (sp_verifTacheVivante(pl_surv_tach) == XDC_OK)
				  {
			                XZST_03EcritureTrace(XZSTC_WARNING, 
						"Tue la tache %d \n",pl_surv_tach->Pid);

						/*B On tue la tache */
						vl_ValRet = kill( pl_surv_tach->Pid,SIGKILL);
				  }
				}
			}
#endif
		}
	}

  XZST_03EcritureTrace(XZSTC_FONCTION, "OUT : sp_dgstatus" );

	return (vl_ValRet);

}	/* Fin sp_dgstatus */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Permet d'envoyer un message sur un datagroup precis 
*	pour specifier au RTserver de verifier l'existence
*	de ce datagroup dans ces tables
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int sp_TestSurvDG(
	XZSCT_Datagroup 	pl_NomDatagroup)

/*
* ARGUMENTS EN ENTREE :
*   pl_NomDatagroup : Nom du datagroup
*
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK	: ok
*   XDC_NOK	: nok
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*   Envoie le msg XDM_TSP_MIGRAZUR sur le datagroup specifie
*	en argument.
*
------------------------------------------------------*/
{
	static char *version="@(#)sapl_sur.c	1.16 02/09/98 : sp_TestSurvDG " ;
	int	vl_ValRet	= XDC_OK;

	if (!TipcSrvMsgWrite(pl_NomDatagroup,
			XDM_IdentMsg(XDM_TSP_MIGRAZUR),
			XDM_FLG_SRVMSGWRITE,
			NULL))
	{
		XZST_03EcritureTrace(XZSTC_WARNING,
			"erreur TipcSrvMsgWrite XDM_TSP_MIGRAZUR \n");

		vl_ValRet = XDC_NOK;
	}
	TipcSrvFlush();

	return (vl_ValRet);

}	/* Fin sp_TestSurvDG */
