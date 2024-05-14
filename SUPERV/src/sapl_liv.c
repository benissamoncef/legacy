/* Fichier : @(#)sapl_livrer.c	2.6      Release : 2.6        Date : 06 Sep 1994
******************************************************
* STERIA *  PROJET SURF
******************************************************
* MODULE SPLIVRER * FICHIER sapl_livrer.c
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module appartient a la tache tsaplg.
* Il permet d'installer sur une machine une nouvelle 
* version d'une tache.
* Le fichier des versions des taches par machines est
* mis a jour.
*
******************************************************
* HISTORIQUE :
*
* B. ORCIBAL	11 Avr 1994	: Creation
* B. ORCIBAL	   Mai 1994	: Mise au point
*******************************************************/
/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
* NOM DU RELECTEUR : BRU
* DATE             : Fri Jun 10 15:15:34 MET DST 1994
* COMMENTAIRES     :
*		Les retours des fonctions lseek, write et close
*		ne sont pas testes.
*
*******************************************************/
/******************************************************
* HISTORIQUE :
*
* B. ORCIBAL	30 Aou 1994	: Corrections suite relecture
*******************************************************/


/* fichiers inclus */
#include "xdc.h"
#include "xdg.h"
#include "xdm.h"
#include "xdl.h"

#include "xzca.h"
#include "xzsa.h"
#include "xzsc.h"
#include "xzsp.h"
#include "xzss.h"
#include "xzst.h"
#include "xzsx.h"

#include "sapl_livrer.h"
#include "sapl_donnees.h"


/* definitions de constantes */
#define	SPLC_LONG_LIGNE_BL	200


/* definitions de types locaux */

/* definition de macro locales */

/* declaration de variables locales */
static char  *version = "@(#)sapl_livrer.c	2.6 06 Sep 1994 : SPLIVRER" ;
XZSCT_NomMachine	SPV_MACHINE_BL;


/* declaration de fonctions internes */




/* Fichier : @(#)sapl_livrer.c	2.6      Release : 2.6        Date : 06 Sep 1994
******************************************************
* STERIA *  PROJET SURF
******************************************************
* MODULE SPLIVRER
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Ecrit dans le fichier des versions, la nouvelle
*	version d'une tache
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void sp_verslog_ecriture ( XZSCT_NomMachine	va_NomMachine ,
				  XZCAT_Executable	va_Executable )

/*
* PARAMETRES EN ENTREE : - nom de la machine destinatrice
*			de la nouvelle version d'une tache
*			 - structure decrivant la nouvelle
*			tache
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucune
*
* CONDITION D'ERREUR :
*
* - Impossibilite d'ouvrir ou de creer le fichier des versions
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  int			vl_Fd;
  XZSXT_VersLog		vl_ENRGECR;
  XZSXT_VersLog		vl_ENRGLEC;
  int			vl_Enr_Ecrit = XDC_FAUX;
  int			vl_Machine_Trouve = XDC_FAUX;
  char			vl_cheminEtFic[XDC_PATH_ABS_SIZE];
  int			vl_cr;
  

  XZST_03EcritureTrace ( XZSTC_FONCTION , "IN : sp_verslog_ecriture : va_NomMachine='%s'" , va_NomMachine );

  /* Construction du nom du fichier des versions logicielles */
  sprintf(vl_cheminEtFic,"%s/%s",XDC_NSUP_PATHOPER,XDF_NSUP_VERSLOG);

  /* ouverture fichier versions en lecture/ecriture */
  if ( XZSS_12OuvrirFichier( XZSSC_FIC_RDWR , vl_cheminEtFic , &vl_Fd ) == XZSSC_FIC_INV )
	{
	/* ouverture fichier versions en creation */
	if ( XZSS_12OuvrirFichier( XZSSC_FIC_CR , vl_cheminEtFic , &vl_Fd ) == XZSSC_FIC_INV )
		{
		XZST_03EcritureTrace(XZSTC_WARNING,"Impossible creer fichier version");
		XZST_03EcritureTrace(XZSTC_FONCTION,"OUT : sp_verslog_ecriture");
		return;
		}
	}

  /* fabrication vl_ENRGECR */
  strcpy ( vl_ENRGECR.NomMachine , va_NomMachine );
  strcpy ( vl_ENRGECR.NomTache   , va_Executable.tache );
  strcpy ( vl_ENRGECR.NumVersion , va_Executable.version );
  strcpy ( vl_ENRGECR.Horodate   , va_Executable.date );

  /* Parcour du fichier */
  while (read(vl_Fd,(char *)&vl_ENRGLEC,sizeof(vl_ENRGLEC)) >= 0)
	{

	/* si meme machine et meme tache */
	if ( ( strcmp(vl_ENRGECR.NomMachine,vl_ENRGLEC.NomMachine) == 0 ) &&
	     ( strcmp(vl_ENRGECR.NomTache,vl_ENRGLEC.NomTache) == 0 )		)
		{
		/* on se repositionne au debut de l'enregistrement */
		vl_cr = lseek ( vl_Fd , - sizeof(vl_ENRGLEC) , SEEK_CUR );
		if (vl_cr == -1)
			{
			XZST_03EcritureTrace(XZSTC_WARNING,"Impossible lseek fichier version");
			break;
			}

		/* on ecrit le nouvel enregistrement */
		vl_cr = write ( vl_Fd , (char *)&vl_ENRGECR , sizeof(vl_ENRGECR) );
		if (vl_cr == -1)
			{
			XZST_03EcritureTrace(XZSTC_WARNING,"Impossible write fichier version");
			break;
			}

		/* indication enregistrement ecrit */
		vl_Enr_Ecrit = XDC_VRAI;

		break;
		}
	else
		/* si meme machine */
		if ( strcmp(vl_ENRGECR.NomMachine,vl_ENRGLEC.NomMachine) == 0 )
			{
			/* indication nom machine trouve */
			vl_Machine_Trouve = XDC_VRAI;
			}
		else
			/* si machine trouve */
			if ( vl_Machine_Trouve == XDC_VRAI )
				{
				/* on se repositionne au debut de l'enregistrement */
				vl_cr = lseek ( vl_Fd , - sizeof(vl_ENRGLEC) , SEEK_CUR );
				if (vl_cr == -1)
					{
					XZST_03EcritureTrace(XZSTC_WARNING,"Impossible lseek fichier version");
					break;
					}
			
				/* on ecrit le nouvel enregistrement */
				vl_cr = write ( vl_Fd , (char *)&vl_ENRGECR , sizeof(vl_ENRGECR) );
				if (vl_cr == -1)
					{
					XZST_03EcritureTrace(XZSTC_WARNING,"Impossible write fichier version");
					break;
					}

				/* indication enregistrement ecrit */
				vl_Enr_Ecrit = XDC_VRAI;

				/* mise a jour enregistrement a ecrire */
				strcpy ( vl_ENRGECR.NomMachine , vl_ENRGLEC.NomMachine );
				strcpy ( vl_ENRGECR.NomTache   , vl_ENRGLEC.NomTache );
				strcpy ( vl_ENRGECR.NumVersion , vl_ENRGLEC.NumVersion );
				strcpy ( vl_ENRGECR.Horodate   , vl_ENRGECR.Horodate );
				}
	}	/* fin while chaque enregistrement du fichier */

  /* si l'enregistrement n'a pas ete ecrit */
  if ( vl_Enr_Ecrit == XDC_FAUX )
	{
	/* on ecrit le nouvel enregistrement */
	vl_cr = write ( vl_Fd , (char *)&vl_ENRGECR , sizeof(vl_ENRGECR) );
	if (vl_cr == -1)
		{
		XZST_03EcritureTrace(XZSTC_WARNING,"Impossible write fichier version");
		}
	}

  /* fermeture du fichier */
  close(vl_Fd);

  XZST_03EcritureTrace ( XZSTC_FONCTION , "OUT : sp_verslog_ecriture " );

} /* sp_verslog_ecriture */




/* Fichier : @(#)sapl_livrer.c	2.6      Release : 2.6        Date : 06 Sep 1994
******************************************************
* STERIA *  PROJET SURF
******************************************************
* MODULE SPLIVRER
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Recopie de la nouvelle version d'une tache
*	sur la machine concernee
*	Met a jour le fichier des versions
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void sp_verslog_ta ( int			va_NumOrdre ,
			    XZCAT_Executable	va_Executable )

/*
* PARAMETRES EN ENTREE : - numero d'ordre (non utilise)
*			 - structure decrivant la nouvelle
*			version de la tache
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
  int			vl_etat_calc;
  XZSCT_NomMachine	vl_NomMachine;
  XZSCT_NomTache	vl_NomTache;


  XZST_03EcritureTrace ( XZSTC_FONCTION , "IN : sp_verslog_ta : va_numOrdre=%d" , va_NumOrdre );

  /* recuperation nom tache */
  strcpy ( vl_NomTache , va_Executable.tache );

  /* appel primitive pour connaitre etat calculateur */
  XZSE_09EtatMachine ( &vl_etat_calc );

  /* si nous sommes le calculateur Nominal Actif */
  if ( vl_etat_calc == XZSEC_ETAT_NACTIF )
	{
	/* arret de la tache */
	XZSP_03CmdTache(SPV_MACHINE_BL,vl_NomTache,XZSPC_ARRET);

	/* recuperation nom machine locale */
	XZSC_07NomMachine ( vl_NomMachine );

	/* transfert du nouvel executable */
	XZSS_07TransfertFichier(vl_NomMachine,vl_NomTache,XDC_NSUP_PATHBL,
				SPV_MACHINE_BL,vl_NomTache,XDC_NSUP_PATHEXEC);

	/* re-demarrage de la tache */
	XZSP_03CmdTache(SPV_MACHINE_BL,vl_NomTache,XZSPC_MARCHE);

	/* appel fonction de mise a jour du fichier des versions */
  	sp_verslog_ecriture ( SPV_MACHINE_BL , va_Executable );
	}

  XZST_03EcritureTrace ( XZSTC_FONCTION , "OUT : sp_verslog_ta " );

}  /* sp_verslog_ta */




/* Fichier : @(#)sapl_livrer.c	2.6      Release : 2.6        Date : 06 Sep 1994
******************************************************
* STERIA *  PROJET SURF
******************************************************
* MODULE SPLIVRER
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Appelle la fonction de mise a jour des taches
*	d'une machine
*
******************************************************
* SEQUENCE D'APPEL :
*/

static void sp_verslog_ma ( int			va_NumOrdre ,
			    XZSCT_NomMachine	va_NomMachine )

/*
* PARAMETRES EN ENTREE : - numero d'ordre (non utilise)
*			 - nom de la machine a mettre a jour
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

  XZST_03EcritureTrace ( XZSTC_FONCTION
			, "IN : sp_verslog_ma : va_NomOrdre=%d , va_NomMachine='%s'"
			, va_NumOrdre , va_NomMachine );

  strcpy ( SPV_MACHINE_BL , va_NomMachine );

  /* appel fonction utilisateur */
  XZCA_02traiterTachesBL ( va_NomMachine , sp_verslog_ta );

  XZST_03EcritureTrace ( XZSTC_FONCTION , "OUT : sp_verslog_ma " );

} /* sp_verslog_ma */




/* declaration de fonctions externes */




/* Fichier : @(#)sapl_livrer.c	2.6      Release : 2.6        Date : 06 Sep 1994
******************************************************
* STERIA *  PROJET SURF
******************************************************
* MODULE SPLIVRER
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	Appelle la fonction de mise a jour des machines
*	d'un bon de livraison
*
******************************************************
* SEQUENCE D'APPEL :
*/

void sp_verslog_cb (	 T_IPC_CONN                      va_Cnx,
                         T_IPC_CONN_PROCESS_CB_DATA      pa_Data,
                         T_CB_ARG                        pa_Arg )

/*
* PARAMETRES EN ENTREE : - Connexion avec RTserver
*			 - Donnees du message
*			 - Callback arg
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : aucunes
*
* CONDITION D'ERREUR : neant
*
* - Impossibilite d'initialiser le bon de livraison
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
int	vl_cr;


  XZST_03EcritureTrace ( XZSTC_FONCTION , "IN : sp_verslog_cb ");

  /* Prise des ressources pour traiter le bon de livraison */
  vl_cr = XZCA_03InitialiserBL();

  if (vl_cr == XDC_OK)
	{
  	/* Appel fonction utilisateur */
  	XZCA_01traiterMachinesBL ( sp_verslog_ma );

  	/* Liberation des ressources de traitement du bon de livraison */
  	XZCA_04LibererRessourcesBL ();

  	/* Indication prise en compte version logicielle */
  	XZST_10ArchiverMsgSyst(XDL_SUP17_PRISLOG,NULL);
	}
  else
	{
  	XZST_03EcritureTrace(XZSTC_WARNING,
			"sp_verslog_cb : erreur dans XZCA_03InitialiserBL = %d",
			vl_cr);
	}

  XZST_03EcritureTrace ( XZSTC_FONCTION , "OUT : sp_verslog_cb " );

} /* sp_verslog_cb */


