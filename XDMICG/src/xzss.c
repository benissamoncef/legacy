/*E*/
/* Fichier : xzss.c
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSS * FICHIER xzss.c
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSS
*        - Declaration des fonctions externes
*        - Declaration des variables externes
*
*   Reference DCG-021 chapitre 3.6
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville   01 Sep 1994      : Creation
*****************
*  O. MERCIER	29 Jan 	1994	: Modif de xzss_07Transfert	V 1.11
*****************
*  O. MERCIER	29 Jan 	1994	: Modif de xzss_07Transfert	V 1.12
* Oubli  d une , .
*
*  T.Milleville   13 Fev 1995      : Ajout Argument aux modules
*					d'interface XZSS_03 et XZSS_04
*				pour preciser le type d'impression V1.13
*
*  D. MISMER	1 Mar 	1995	: Rien	V 1.14 et V 1.15
*
*  T.Milleville   09 Mar 1995      : Modification de la foncion
*		lecture de la RS232 V1.16
*  T.Milleville   04 Avr 1995      : Modification de l'option 
*	pour les impressions V1.18  V1.19
*  T.Milleville   20 Jul 1995      : Modification code XZSS_04 .
*	Si erreur appel a XZSS_03   V1.20
*  T.Milleville   20 Jul 1995      : Ajout commentaires V1.21
*  T.Milleville   25 Jul 1995      : Ajout commentaires V1.22
*  P/Niepceron    25 Aou 1995      : Ajout de -onb dans les options d'impression V1.23
*  T.Milleville   11 Sep  1995      : Ajout commentaires V1.24
*  P/Niepceron    15 Sep 1995	   :Ajout de l'option -c pour l'impression v1.25
*  P/Niepceron    19 Sep 1995      :Ajout de l'option -c pour l'impression (2eme) v1.26
*  T.Milleville   12 Oct 1995      :Modification codage XZSS_20 V1.27
*  P/Niepceron    24 Jun 1998	   :Ajout trace errno sur l'impression 1.28
*  JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.29
*  JPL		19/10/21 : Ensemble de fonctions de Minuterie -Timer- XZSS_14, XZSS_15, XZSS_18; Temporisation XZSS_22 (SAE_339)  1.30
*  JPL		21/10/21 : XZSS_14ArmerTimeout : desactiver si duree nulle, erreur si negative (SAE_339)  1.31
*  JPL		17/05/22 : XZSS_22Attendre : argument entier en millisecondes  1.32
*  JPL		31/05/22 : Gestion des versions ; IDENTIQUE PRECEDENTE  1.33
* LCL		24/07/23 : patch rcp XZSS_07TransfertFichier DEM-471
*******************************************************/

/* fichiers inclus */
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <signal.h>
#include <poll.h>
#include <termios.h>

#include <sys/types.h>
#include <syscall.h>

#include <xdm.h>

#include "xzsc.h"
#include "xzst.h"

#include "xzss.h"

/* declaration des variables locales  */
static char *version = "xzss.c 1.33";

static struct pollfd	XZSSV_pollFds [1];

/* definition de fonctions externes */

/* declaration de fonctions internes */

/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* NomFichier : Nom du fichier a sauvegarder.
* PathFicS   : Chemin d'acces du fichier a sauvegarder.
*
* Fonction
* Si PathFicS est NULL ou NomFichier est NULL, la 
* fonction retourne XZSSC_ARG_INV.
* Le nom du fichier complet est la concatenation de 
* PathFicS + '/' + NomFichier. L'existence du fichier 
* est teste avec le mecanisme SYS1.
* Utilisation du mecanisme SYS2 pour se positionner 
* dans le repertoire PathFicS.
* Creation de la commande : " tar -u NomFichier "
* Execution de la commande et compte rendu par le 
* mecanisme SYS3.
* L'identifiant du peripherique est recupere par la
* primitive XZSC_08 et appel de la primitive XZSE_14 
* pour envoyer l'etat du peripherique.
*
* Codes retour
* XDC_OK		ok
* XZSSC_ARG_INV		Nom d'argument egal a NULL.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_DEV_INV		Device inconnu (compte-rendu 
*			de XZSC_08).
* XZSSC_ERR_SYS		Erreur execution shell. 
*
* Mecanismes
* SYS1 , SYS2 , SYS3.
******************************************************/

extern int XZSS_01SauveFichierSD ( char *NomFichier, char *PathFicS )
{
  /* Variables Locales */
  static char *fonction = "XZSS_01SauveFichierSD";
  int				vl_ValRet = XDC_OK,
  				cr;
  char				vl_nomAbsoluFic [XZSSC_PATH_ABS_SIZE] = "",
   				vl_ligneCommande [XZSSC_LINE_CMD_SIZE] = "",
  				vl_errorBuffer [XZSSC_ERRBUFSIZE] = "";
  XZSCT_NomPeriph		vl_nomDAT = "";
  int				vl_etatDAT = XZSEC_ETAT_OK;
  FILE*				vl_commande = NULL;

  /* recuperation de l'identifiant du peripherique */
  if ((cr = XZSC_08NomPeriphSR (vl_nomDAT)) != XDC_OK)
    {
      vl_ValRet = cr;
      vl_etatDAT = XZSEC_ETAT_NOK;
    }	      
  /* Verification des arguments */
  else if ((NomFichier == (char*) NULL) || (PathFicS == (char*) NULL))
    {
      vl_ValRet = XZSSC_ARG_INV;
    }
  else
    {
      /* construction du nom complet du fichier SD */
      sprintf (vl_nomAbsoluFic, "%s/%s", PathFicS, NomFichier);
      
      /* Verification de son existence */
      if (access (vl_nomAbsoluFic, F_OK) != 0)
	{
	  vl_ValRet = XZSSC_FIC_INV;
	}
      /* Aller se positionner dans le repertoir PathFicS */
      else if (chdir (PathFicS) != 0)
	{
	  vl_ValRet = XDC_NOK;
	}
      else
	{  
	  /* creation de la commande a executer */
	  sprintf (vl_ligneCommande, "tar -uf %s %s", vl_nomDAT, NomFichier);
	  
	  /* Execution de la commande */
	  vl_commande = popen (vl_ligneCommande, "r");
	  if (vl_commande != NULL)
	    {
	      /* Ecriture du retour dans les traces */
	      fgets (vl_errorBuffer, XZSSC_ERRBUFSIZE, vl_commande);
	      XZST_03EcritureTrace (XZSTC_INFO, "XZSS_01SauveFichierSD CompteRendu de %s:\n \t%s", 
				    vl_ligneCommande, 
				    (vl_errorBuffer == NULL) ? "OK" : vl_errorBuffer);
              pclose(vl_commande);
	    }
	  else
	    {
	      XZST_03EcritureTrace (XZSTC_WARNING, "XZSS_01SauveFichierSD %s non OK",vl_nomDAT); 
	      vl_ValRet = XZSSC_ERR_SYS;
	      vl_etatDAT = XZSEC_ETAT_NOK;
	    }
	}
    }
  
  /* Envoi de l'etat du peripherique */
  XZSE_14EnvoieEtatPeriph (vl_nomDAT, vl_etatDAT);
 
	/*  Envoi du Code de Retour */
	return(vl_ValRet);
  
}



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* NomFichier : Nom du fichier a restaurer.
* PathFicR   : Chemin d'acces du fichier a restaurer.
*
* Fonction
* Si PathFicR est NULL ou NomFichier est NULL, la 
* fonction retourne XZSSC_ARG_INV.
* Le nom du fichier complet est la concatenation de
* PathFicR + '/' + NomFichier. L'existence du fichier 
* est teste avec le mecanisme SYS1.
* Utilisation du mecanisme SYS2 pour se positionner 
* dans le repertoire PathFicR.
* Creation de la commande : " tar -x NomFichier "
* Execution de la commande et compte rendu par le 
* mecanisme SYS3.L'identifiant du peripherique est 
* recupere par la primitive XZSC_08 et appel de la 
* primitive XZSE_14 pour envoyer l'etat du peripherique.
*
* Codes retour
* XDC_OK		ok
* XZSSC_ARG_INV		Nom d'argument egal a NULL.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_DEV_INV		Device inconnu (compte-rendu 
*			de XZSC_08).
* XZSSC_ERR_SYS		Erreur execution shell. 
*
* Mecanismes
* SYS1 , SYS2 , SYS3.
******************************************************/

extern int XZSS_02RestaureFichierSD (char *NomFichier, char *PathFicR)
{   
  /* Variables Locales */
  static char *fonction = "XZSS_02RestaureFichierSD";
  int			vl_ValRet = XDC_OK,
  			cr;
  char			vl_nomAbsoluFic [XZSSC_PATH_ABS_SIZE] = "",
			vl_ligneCommande [XZSSC_LINE_CMD_SIZE] = "",
  			vl_errorBuffer [XZSSC_ERRBUFSIZE] = "";
  XZSCT_NomPeriph	vl_nomDAT = "";
  int			vl_etatDAT = XZSEC_ETAT_OK;
  FILE *		vl_commande = NULL;

  /* recuperation de l'identifiant du peripherique */
  if ((cr = XZSC_08NomPeriphSR (vl_nomDAT)) != XDC_OK)
    {
      vl_ValRet = cr;
      vl_etatDAT = XZSEC_ETAT_NOK;
    }	      
  else if ((NomFichier == (char*) NULL) || (PathFicR == (char*) NULL))
    {
      vl_ValRet = XZSSC_ARG_INV;
    }
  else
    {
      /* construction du nom complet du fichier SR */
      sprintf (vl_nomAbsoluFic, "%s/%s", PathFicR, NomFichier);
      
      /* Verification de son existence */
      if (access (vl_nomAbsoluFic, F_OK) != 0)
	{
	  vl_ValRet = XZSSC_FIC_INV;
	}
      /* Aller se positionner dans le repertoir PathFicR */
      else if (chdir (PathFicR) != 0)
	{
	  vl_ValRet = XDC_NOK;
	}
      else
	{
	  /* creation de la commande a executer */
	  sprintf (vl_ligneCommande, "tar -xf %s %s", vl_nomDAT, NomFichier);

	  /* Execution de la commande */
	  vl_commande = popen (vl_ligneCommande, "r");
	  if (vl_commande != NULL)
	    {
	      /* Ecriture du retour dans les traces */
	      fgets (vl_errorBuffer, XZSSC_ERRBUFSIZE, vl_commande);
	      XZST_03EcritureTrace (XZSTC_INFO, "XZSS_02RestaureFichierSD CompteRendu de %s:\n \t%s", 
				    vl_ligneCommande, 
				    (vl_errorBuffer == NULL) ? "OK" : vl_errorBuffer);
              pclose(vl_commande);
	    }
	  else
	    {
	      XZST_03EcritureTrace (XZSTC_WARNING, "XZSS_02RestaureFichierSD %s non OK",vl_nomDAT); 
	      vl_ValRet = XZSSC_ERR_SYS;
	      vl_etatDAT = XZSEC_ETAT_NOK;
	    }
	}
    }
  
  /* Envoi de l'etat du peripherique */
  XZSE_14EnvoieEtatPeriph (vl_nomDAT, vl_etatDAT);
  
  return (vl_ValRet);
}


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*  Cette fonction permet d'envoyer le fichier a imprimer
*	vers le spooler d'impression secondaire
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

int XZSS_03ImprimerFichierSec(
	int 	TypePeriph,
	char 	*NomFic,
	char 	*PathFic,
	int 	vl_TypeImpression)

/*
* ARGUMENTS EN ENTREE :
*   TypePeriph :  type de la machine d'impression
*   NomFic : Nom  du fichier a imprimer 
*   PathFic : chemin d'acces du fichier
*   vl_TypeImpression : Type de l'impression
*
* ARGUMENTS EN SORTIE :
*   Aucun
*
*
* CODE RETOUR : 
*   XDC_OK, XZSSC_ARG_INV, XZSSC_FIC_INV, XZSSC_DEV_INV,XZSSC_ERR_SYS
*
* CONDITION D'UTILISATION
*   Aucune
*
* FONCTION 
*	* Cette fonction envoie le fichier a imprimer vers
* le spooler d'impression.
* Si TypePeriph est incorrect ou NomFic est NULL ou
* PathFic est NULL, la fonction retourne XZSSC_ARG_INV.
* Le nom du fichier complet est la concatenation de
* PathFic + '/' + NomFic. L'existence du fichier est
* teste avec le mecanisme SYS1.
* Appel de la primitive XZSC_11 pour retrouver le nom
* de l'imprimante ou de la primitive XSZC_12 pour le
* nom du traceur.
* Creation de la commande :
* " lp -d nom_device nom_fichier_complet "
* Execution de la commande et compte rendu par le
* mecanisme SYS3.Appel de la primitive XZSE_14 pour
* envoyer l'etat du peripherique.
*
------------------------------------------------------*/
{

  /* Variables locales */
  static char *fonction = "XZSS_03ImprimerFichierSec";
  int			vl_ValRet = XDC_OK;
  char			vl_nomFicAbsolu [XZSSC_PATH_ABS_SIZE] = "",
			vl_ligneCommande [XZSSC_LINE_CMD_SIZE] = "",
  			vl_errorBuffer [XZSSC_ERRBUFSIZE] = "";
  XZSCT_NomPeriph	vl_scribeSecours = "";
  int			vl_etatScribe = XZSEC_ETAT_OK;
  FILE *		vl_commande = NULL;
  char			vl_OptionImp[20] 	= ""; 


	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSS_03ImprimerFichierSec : IN \
		-- Type Peri %d -- Fichier %s -- Path %s -- Type imp %d \n",
		TypePeriph,NomFic,PathFic,vl_TypeImpression);

  if ((NomFic == (char*) NULL) || (PathFic == (char*) NULL))
    {
      vl_ValRet = XZSSC_ARG_INV;
    }
  else 
    {
	/*A Determination du Type  d'impression */
	switch (vl_TypeImpression) {
		case XDC_IMP_ASCII : 
			break;

		case XDC_IMP_PCL : 
			strcpy(vl_OptionImp,"-o raw");
			break;

		case XDC_IMP_POSTSCRIPT : 
			strcpy(vl_OptionImp,"-o pos");
			break;
		default:
			break;
	}

      switch (TypePeriph)
	{
	case XZSSC_TYPE_IMPRIMA:
	  /* Recuperation du nom de l'imprimante de secours */
	  if ( (vl_ValRet = XZSC_15NomImprimanteSecours (vl_scribeSecours))					 != XDC_OK)
	    {
	      vl_ValRet = XZSSC_DEV_INV;
	      vl_etatScribe = XZSEC_ETAT_NOK;
	    }
	  break;
	  
	default:
	  vl_ValRet = XZSSC_ARG_INV;
	  break;
	}
      
      if (vl_ValRet == XDC_OK)
	{
	  /* construction du nom complet du fichier envoye au spooler */
	  sprintf (vl_nomFicAbsolu, "%s/%s", PathFic, NomFic);

	  /* Verification de son existence */
	  if (access (vl_nomFicAbsolu, F_OK) != 0)
	    {
	      vl_ValRet = XZSSC_FIC_INV;
	    }
	  else 
	    {
	      /* Creation de la commande d'impression */
	      sprintf (vl_ligneCommande, "lp -onb %s -c -d %s %s",vl_OptionImp, 
					vl_scribeSecours, vl_nomFicAbsolu);

	      /* Execution de la commande */
	      vl_commande = popen (vl_ligneCommande, "r");
	      if (vl_commande != NULL)
		{
		  /* Ecriture du retour dans les traces */
		  fgets (vl_errorBuffer, XZSSC_ERRBUFSIZE, vl_commande);
		  XZST_03EcritureTrace (XZSTC_INFO, 
		    "XZSS_03ImprimerFichierSec CompteRendu de %s:\n \t%s", 
			vl_ligneCommande, 
			(vl_errorBuffer == NULL) ? "OK" : vl_errorBuffer);

			/* Memorisation du Retour d'erreur */
		  if (!strcmp(vl_errorBuffer,""))
		  {
		  	vl_ValRet = XZSSC_ERR_SYS;
		  	vl_etatScribe = XZSEC_ETAT_NOK;
		  }

                  pclose(vl_commande);
		}
	      else
		{
		  XZST_03EcritureTrace (XZSTC_WARNING, 
				"XZSS_03ImprimerFichierSec non OK");
		  vl_ValRet = XZSSC_ERR_SYS;
		  vl_etatScribe = XZSEC_ETAT_NOK;
		}
	    }
	}
    }

  /* Envoi de l'etat du peripherique */
  XZSE_14EnvoieEtatPeriph (vl_scribeSecours, vl_etatScribe);


	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSS_03ImprimerFichierSec : OUT -- Compte Rendu %d \n",
		vl_ValRet);

  return (vl_ValRet);


}	/* Fin  XZSS_03ImprimerFichierSec */



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* TypePeriph 	: XZSSC_TYPE_IMPRIMA ou 
*		  XZSSC_TYPE_TRACEUR
* NomFic	: Nom du fichier a imprimer.
* PathFic	: Chemin d'acces du fichier.
*
* Fonction
* Cette fonction envoie le fichier a imprimer vers 
* le spooler d'impression.
* Si TypePeriph est incorrect ou NomFic est NULL ou
* PathFic est NULL, la fonction retourne XZSSC_ARG_INV.
* Le nom du fichier complet est la concatenation de
* PathFic + '/' + NomFic. L'existence du fichier est 
* teste avec le mecanisme SYS1.
* Appel de la primitive XZSC_11 pour retrouver le nom 
* de l'imprimante ou de la primitive XSZC_12 pour le 
* nom du traceur.
* Creation de la commande : 
* " lp -d nom_device nom_fichier_complet "
* Execution de la commande et compte rendu par le
* mecanisme SYS3.Appel de la primitive XZSE_14 pour
* envoyer l'etat du peripherique.
*
* Codes retour
* XDC_OK		ok
* XZSSC_ARG_INV		Argument TypePeriph incorrect.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_DEV_INV		Device inconnu (compte-rendu 
*			de XZSC_11 et XZSC_12).
* XZSSC_ERR_SYS		Erreur execution shell. 
*
* Mecanismes
* SYS1 , SYS3.
******************************************************/

extern int XZSS_04ImprimerFichier (
	int TypePeriph, 
	char *NomFic, 
	char *PathFic,
	int vl_TypeImpression)

{
  /* Variables locales */
  static char *fonction = "XZSS_04ImprimerFichier";
  int			vl_ValRet = XDC_OK;
  char			vl_nomFicAbsolu [XZSSC_PATH_ABS_SIZE] = "",
			vl_ligneCommande [XZSSC_LINE_CMD_SIZE] = "",
  			vl_errorBuffer [XZSSC_ERRBUFSIZE] = "";
  XZSCT_NomPeriph	vl_scribeLocal = "";
  int			vl_etatScribe = XZSEC_ETAT_OK;
  FILE *		vl_commande = NULL;
  char			vl_OptionImp[20] 	= ""; 
 



	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSS_04ImprimerFichier : IN \
		-- Type Peri %d -- Fichier %s -- Path %s -- type imp %d \n",
		TypePeriph,NomFic,PathFic,vl_TypeImpression);


  if ((NomFic == (char*) NULL) || (PathFic == (char*) NULL))
    {
      vl_ValRet = XZSSC_ARG_INV;
    }
  else 
    {
	/*A Determination du Type  d'impression */
	switch (vl_TypeImpression) {
		case XDC_IMP_ASCII : 
			break;

		case XDC_IMP_PCL : 
			strcpy(vl_OptionImp,"-o raw");
			break;

		case XDC_IMP_POSTSCRIPT : 
			strcpy(vl_OptionImp,"-o pos");
			break;
		default:
			break;
	}

      switch (TypePeriph)
	{
	case XZSSC_TYPE_IMPRIMA:
	  /* Recuperation du nom de l'imprimante local */
	  if ( (vl_ValRet = XZSC_11NomImprimante (vl_scribeLocal))
						!= XDC_OK)
	    {
	      vl_ValRet = XZSSC_DEV_INV;
	      vl_etatScribe = XZSEC_ETAT_NOK;
	    }
	  break;
	  
	default:
	  vl_ValRet = XZSSC_ARG_INV;
	  break;
	}
      
      if (vl_ValRet == XDC_OK)
	{
	  /* construction du nom complet du fichier envoye au spooler */
	  sprintf (vl_nomFicAbsolu, "%s/%s", PathFic, NomFic);
	  
	  /* Verification de son existence */
	  if (access (vl_nomFicAbsolu, F_OK) != 0)
	    {
	      vl_ValRet = XZSSC_FIC_INV;
	    }
	  else 
	    {
	      /* Creation de la commande d'impression */
	      sprintf (vl_ligneCommande, 
			"lp -c -onb %s -d %s %s", vl_OptionImp,
					vl_scribeLocal, vl_nomFicAbsolu);

	      /* Execution de la commande */
	      vl_commande = popen (vl_ligneCommande, "r");
	      if (vl_commande != NULL)
		{
		  /* Ecriture du retour dans les traces */
		  fgets (vl_errorBuffer, XZSSC_ERRBUFSIZE, vl_commande);
		  XZST_03EcritureTrace (XZSTC_INFO, 
		  "XZSS_04ImprimerFichier CompteRendu de %s -- %s\n \t%s\n", 
				vl_ligneCommande,vl_errorBuffer ,
			(vl_errorBuffer == NULL) ? "OK" : vl_errorBuffer);

		  /* Memorisation du Retour d'erreur */
		  if (!strcmp(vl_errorBuffer,""))
		  {
		  	vl_ValRet = XZSSC_ERR_SYS;
		  	vl_etatScribe = XZSEC_ETAT_NOK;
		  }

                  pclose(vl_commande);
		}
	      else
		{
		  XZST_03EcritureTrace (XZSTC_WARNING,"popen ko : %d",errno);
		  XZST_03EcritureTrace (XZSTC_WARNING, 
				"XZSS_04ImprimerFichier non OK");
		  vl_ValRet = XZSSC_ERR_SYS;
		  vl_etatScribe = XZSEC_ETAT_NOK;
		}
	    }
	}
    }
  /* Envoi de l'etat du peripherique */
  XZSE_14EnvoieEtatPeriph (vl_scribeLocal, vl_etatScribe);
 

	/*A Premierement essai d'impression sur l'imprimante
	*	 de Secours */
	if (vl_ValRet == XZSSC_ERR_SYS)
	{
		vl_ValRet = XZSS_03ImprimerFichierSec(
			TypePeriph,NomFic,PathFic,vl_TypeImpression);
	}

  
  
	XZST_03EcritureTrace (XZSTC_INTERFACE, 
		"XZSS_04ImprimerFichier : OUT -- Compte Rendu %d \n",
		vl_ValRet);

  return (vl_ValRet);

}	/* Fin XZSS_04ImprimerFichier */


/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction 
* Cette fonction inhibe les signaux UNIX suivants  :
*        SIGHUP
*        SIGINT
*        SIGPIPE
*        SIGTERM
*        SIGALRM
*        SIGUSR1
*        SIGUSR2
*        SIGPOLL
*        SIGTSTP
*        SIGTTIN
*        SIGTTOU
*        SIGVTALRM
*        SIGPROF
*
* Codes retour
* XDC_OK	ok
*
* Mecanismes
******************************************************/

extern int XZSS_05TraitSignaux()
{
  /* Variables Locales */
  static char *fonction = "XZSS_05TraitSignaux";
  int			vl_ValRet = XDC_OK;
  struct sigaction	vl_sig;

  vl_sig.sa_handler = SIG_IGN;
  vl_sig.sa_flags = 0;
  sigemptyset (&vl_sig.sa_mask);

  /* Inhibition des signaux UNIX */
  if ((sigaction (SIGHUP, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGINT, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGPIPE, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGTERM, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGALRM, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGUSR1, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGUSR2, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGPOLL, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGTSTP, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGTTIN, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGTTOU, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGVTALRM, &vl_sig, (struct sigaction *) NULL) < 0) ||
      (sigaction (SIGPROF, &vl_sig, (struct sigaction *) NULL) < 0) )
    {
      vl_ValRet = XDC_NOK;
    }
  
  return (vl_ValRet);
}


/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* NomFichier      : Nom du fichier a detruire.
*
* Fonction
* Si NomFichier est NULL, la fonction retourne 
* XZSSC_ARG_INV.
* Test de l'existence du fichier par le mecanisme SYS1.
* Destruction du fichier par l'instruction " unlink".
*
* Codes retour
* XDC_OK		ok
* XZSSC_ARG_INV		Argument incorrect.
* XZSSC_FIC_INV		Fichier inconnu.
*
* Mecanismes
* SYS1.
******************************************************/

extern int XZSS_06DetruireFichier (char* NomFichier)
{
  /* Variables Locales */
  static char *fonction = "XZSS_06DetruireFichier";
  int			vl_ValRet = XDC_OK;

  if (NomFichier == (char*) NULL)
    {
      vl_ValRet = XZSSC_ARG_INV;
    }
  /* Verification de son existence */
  else if (access (NomFichier, F_OK) != 0)
    {
      vl_ValRet = XZSSC_FIC_INV;
    }
  else if (unlink (NomFichier)) 
    {
      vl_ValRet = XDC_NOK;
    }
  return (vl_ValRet);
}



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* NomMachineOrig	: Nom de la machine origine.
* NomFicOrig 		: Nom du fichier a transferer.
* PathOrig 		: Chemin d'acces du fichier a 
*			  transferer.
* NomMachineDest	: Nom de la machine destinatrice.
* NomFicDest		: Nom du fichier transfere.
* PathDest		: Chemin d'acces du fichier 
*			  transfere.
*
* Fonction
* Si NomFichier est NULL ou PathOrig est NULL ou
* NomFicOrig est NULL ou PathOrig est NULL, la fonction
* retourne XZSSC_ARG_INV.
*
* Transfert du fichier : 
* "rcp    NomMachineOrig:nomFichier
*  	NomMachineDest:PathDest + '/' + NomFicDest"
* Execution de la commande et compte rendu par le 
* mecanisme SYS3.
*
* Codes retour
* XDC_OK		ok
* XZSSC_ARG_INV		Argument incorrect.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_ERR_SYS		Erreur execution shell.
*
* Mecanismes
* SYS1 , SYS3.
******************************************************/

extern int XZSS_07TransfertFichier (	XZSCT_NomMachine NomMachineOrig, 
				    					char* NomFicOrig, 
				    					char* PathOrig, 
				    					XZSCT_NomMachine NomMachineDest, 
				    					char* NomFicDest, 
				    					char* PathDest)
{
  /* Variables locales */
  static char *fonction = "XZSS_07TransfertFichier";
  int			vl_ValRet = XDC_OK;
  int			vl_indice = 0;
  char			vl_nomFicOrigAbsolu [XZSSC_PATH_ABS_SIZE] = "",
  			vl_nomFicDestAbsolu [XZSSC_PATH_ABS_SIZE] = "",
  			vl_errorBuffer [XZSSC_ERRBUFSIZE] = "",
  			vl_laCommande [XZSSC_LINE_CMD_SIZE] = "";
  FILE *		vl_commande = NULL;
  XDY_NomMachine  vl_NomMachine = "";


	XZST_03EcritureTrace(XZSTC_WARNING,"XZSS_07TransfertFichier : IN \
		-- Mach Ori %s -- Fic Ori %s -- Path Ori %s -- \
		Mach Dest %s -- Fic Dest %s -- Path Dest %s \n",
		NomMachineOrig, NomFicOrig, PathOrig,
		NomMachineDest, NomFicDest, PathDest);

  	XZSC_07NomMachine(vl_NomMachine);

  	if ((!strcmp(NomFicOrig,"")) || (!strcmp(PathOrig,"")) ||
		(!strcmp(NomFicDest,"")) || (!strcmp(PathDest,"")))
    {
		vl_ValRet = XZSSC_ARG_INV;
    }
  	/* si les paths et machines sont les memes: nothing to do */
  	else if ((!strcmp (NomFicOrig, NomFicDest)) && 
			 (!strcmp (PathOrig, PathDest)) && 
			 (!strcmp (NomMachineOrig, NomMachineDest)))
    {
		vl_ValRet = XDC_OK;
    }
  	/* Si le nom du SD actif n'a put etre determine (gmot.x) */
  	else if ((!strcmp(NomMachineOrig,"")) || (!strcmp(NomMachineDest,""))) 
    {
		vl_ValRet = XDC_OK;
    }
	else 
    {
      /* si les machines sont les memes, just cp 'hem */
      if (!strcmp (NomMachineOrig, NomMachineDest))
	{
	  /* Preparation de la commande cp */
	  sprintf (vl_laCommande, "cp %s/%s %s/%s", 
		   PathOrig, NomFicOrig,
		   PathDest, NomFicDest);
	}
      else
	{
	  	/* Preparation de la commande rcp */
	  	/* Si la machine de destination est identique a la machine locale on ne la renseigne pas dans la commande rcp */
	  	if (strcmp (vl_NomMachine, NomMachineDest) == 0)
		{
			sprintf (vl_laCommande, "rcp %s:%s/%s %s/%s", 
											NomMachineOrig, PathOrig, NomFicOrig,
															PathDest, NomFicDest);
		}
	  	/* Si la machine source est identique a la machine locale on ne la renseigne pas dans la commande rcp */
	  	else if (strcmp (vl_NomMachine, NomMachineOrig) == 0)
		{
	  		sprintf (vl_laCommande, "rcp %s/%s %s:%s/%s", 
															PathOrig, NomFicOrig,
											NomMachineDest, PathDest, NomFicDest);
		}
		else
		{
	  		sprintf (vl_laCommande, "rcp %s:%s/%s %s:%s/%s", 
											NomMachineOrig, PathOrig, NomFicOrig,
											NomMachineDest, PathDest, NomFicDest);
		}

		XZST_03EcritureTrace(XZSTC_WARNING,"XZSS_07TransfertFichier : vl_laCommande=%s \n", vl_laCommande);
	}
      
	  /*  Execution !	*/
      vl_commande = popen (vl_laCommande, "r");
      if (vl_commande != NULL )
	{
	  fgets (vl_errorBuffer, XZSSC_ERRBUFSIZE, vl_commande);
	  
	  if ( (!strcmp(vl_errorBuffer,""))||(strlen(vl_errorBuffer) <=1)||(vl_errorBuffer[0]=='\0')||(vl_errorBuffer[0]=='\n'))
	  {
	  	vl_ValRet = XDC_OK;

				XZST_03EcritureTrace (XZSTC_WARNING,
										"%s/%s de la machine %s transfere vers %s/%s sur %s",
										PathOrig, NomFicOrig,NomMachineOrig,
										PathDest, NomFicDest,NomMachineDest);
	  		}	  
	  		else
			{
				/*A Erreur sur le transfert */
				vl_ValRet = XDC_NOK;

				XZST_03EcritureTrace (XZSTC_WARNING,
										"Pb sur le transfert de Fichier : %s\n",vl_errorBuffer);
			}

	  		XZST_03EcritureTrace (XZSTC_INFO, 
									"Compte Rendu de la commande shell %s: %s.\n", 
									vl_laCommande, 
									(vl_errorBuffer == NULL) ? "OK" : vl_errorBuffer);

		}
		else
		{
			XZST_03EcritureTrace (XZSTC_WARNING, "XZSS_07TransfertFichier Erreur transfert fichier :commande shell %s \n", vl_laCommande); 
	  		vl_ValRet = XZSSC_ERR_SYS;
		}
		pclose (vl_commande);

    }

	XZST_03EcritureTrace(XZSTC_WARNING,"XZSS_07TransfertFichier : OUT -- Code Retour %d \n",vl_ValRet);

	return (vl_ValRet);

}	/* Fin XZSS_07TransfertFichier */



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* Device	: Nom du device a configurer et a 
*		  ouvrir
* Vitesse	: XZSSC_RS_9600
* NbitInfo	: XZSSC_RS_8_BITS_INFO
* NbitStop	: XZSSC_RS_2_STOP_BIT ou 
*		  XZSSC_RS_1_STOP_BIT
* Parite	: XZSSC_RS_SANS_PARITE ou 
*		  XZSSC_RS_PARITE_PAIRE ou 
*		  XZSSC_RS_PARITE_IMPAIRE
* Fd		: Descripteur du fichier.
*
* Fonction
* Ouverture du device par la fonction "open".
* Recuperation de la structure du device par la fonction 
* "ioctl avec TCGETA".
* Mise a jour de la structure.
* Configuration du device par la fonction  "ioctl avec 
* TCSETA".
* Retour du descripteur du fichier dans Fd.
*
* Codes retour
* XDC_OK 		ok
* XZSSC_ARG_INV		Argument incorrect.
* XZSSC_DEV_INV		Ouverture du device incorrecte.
* XZSSC_DEV_CFN		Configuration du device 
*			incorrecte.
*
* Mecanismes
* SYS7.
********************************************************/

extern int XZSS_08OuvertureRS232 (char* 	Device, 
				  unsigned int 	Vitesse, 
				  int 		FlagParam,
				  int* 		Fd) 
{
  /* Variables Locales */
  static char *fonction = "XZSS_08OuvertureRS232";
  int			vl_ValRet = XDC_OK;
  char 			vl_ligneCommande [XZSSC_LINE_CMD_SIZE];
  struct termios	vl_structureRS232 = {0}; 
  speed_t		vl_vitesse;

  /* Ouverture du port de communication */
   if ( (XZSSV_pollFds [0].fd = open (Device, O_RDWR | O_NONBLOCK)) == -1 )
    {
      vl_ValRet = XZSSC_DEV_INV;
    }
    
  /* Recuperation de la structure du device */
  else 
    {
      *Fd = XZSSV_pollFds [0].fd;
      XZSSV_pollFds [0].events = POLLIN;

      if (tcgetattr (*Fd, &vl_structureRS232) == -1)
	{
	  vl_ValRet = XDC_NOK;
	}
	
      /* Mise a jour de la structure */
      else switch (FlagParam)
	{
	case XZSSC_8BITS_PNON_1SB:
	  vl_structureRS232.c_cflag &= ~CS5 & ~CS6 & ~CS7;
	  vl_structureRS232.c_cflag &= ~PARENB & ~PARODD & ~CSTOPB;
	  vl_structureRS232.c_cflag |= CS8;
	  break;

	case XZSSC_8BITS_PPAIRE_1SB:
	  vl_structureRS232.c_cflag &= ~CS5 & ~CS6 & ~CS7 & ~CSTOPB;
	  vl_structureRS232.c_cflag |= CS8 | PARENB;
	  break;

	case XZSSC_8BITS_PIMPAIRE_1SB:
	  vl_structureRS232.c_cflag &= ~CS5 & ~CS6 & ~CS7 & ~CSTOPB;
	  vl_structureRS232.c_cflag |= CS8 | PARENB | PARODD;
	  break;

	case XZSSC_8BITS_PNON_2SB:
	  vl_structureRS232.c_cflag &= ~CS5 & ~CS6 & ~CS7;
	  vl_structureRS232.c_cflag |= CS8 | CSTOPB;
	  vl_structureRS232.c_cflag &= ~PARENB & ~PARODD;
	  break;

	case XZSSC_8BITS_PPAIRE_2SB:
	  vl_structureRS232.c_cflag &= ~CS5 & ~CS6 & ~CS7;
	  vl_structureRS232.c_cflag |= CS8 | CSTOPB;
	  vl_structureRS232.c_cflag |= PARENB;
	  break;

	case XZSSC_8BITS_PIMPAIRE_2SB:
	  vl_structureRS232.c_cflag &= ~CS5 & ~CS6 & ~CS7;
	  vl_structureRS232.c_cflag |= PARENB | PARODD;
	  vl_structureRS232.c_cflag |= CSTOPB | CS8;
	  break;
	  
	default:
	  return (XDC_NOK);
	}

      /* initialisation des parametres sur la ligne */
      if ( (cfsetospeed (&vl_structureRS232, Vitesse) == -1) || (tcsetattr (*Fd, TCSANOW, &vl_structureRS232) == -1) )
	{
	  vl_ValRet = XZSSC_DEV_CFN;
	}
    }      
  return (vl_ValRet);
  
}


/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* Fd			: Descripteur du fichier.
*
* Fonction
* Utilisation de la fonction "close".
*
* Codes retour
* XDC_OK		ok
* XZSSC_DEV_INV		Device invalide.
*
* Mecanismes
********************************************************/

extern int XZSS_09FermetureRS232 (int Fd)
{
  /* Variables Locales */
  int				vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_09FermetureRS232";

  if (close (Fd) < 0)
    {
      vl_ValRet = XZSSC_DEV_INV;
    }
  return (vl_ValRet);
}




/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* Fd		: Descripteur du fichier.
* NbCar		: Nombre d'octets a recevoir.
* Buffer	: Buffer de reception.
*
* Fonction
* Utilisation de la fonction "read".
*
* Codes retour
* XDC_OK		ok
* XZSSC_DEV_INV		Probleme lecture.
*
* Mecanismes
********************************************************/

extern int XZSS_10LectureRS232 (int 		Fd, 
				char* 		Buffer,
				int 		Taille) 
				
{
  /* Variables Locales */
  int				vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_10LectureRS232";
  int 				vl_chrono;
  int 				vl_NbCar;
  
  /*if ( poll (XZSSV_pollFds, 1, XZSSC_TIMEOUT) < 0)
    {
      vl_ValRet = XZSSC_DEV_INV;
      close (XZSSV_pollFds [0].fd);
    }
  else switch (XZSSV_pollFds [0].revents)
    {
    case 0: 
      return (vl_ValRet = XDC_NOK);
      break;
      
    case POLLIN:
      if ( (vl_NbCar = read (XZSSV_pollFds [0].fd, Buffer, Taille)) <= 0)
	{
	  return (vl_ValRet = XDC_NOK);
	}
      break;
      
    default:
      return (vl_ValRet = XDC_NOK);
      break;
    } */
  
  return (vl_ValRet);
}



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* Fd		: Descripteur du fichier.
* NbCar		: Nombre d'octets a envoyer.
* Buffer	: Buffer d'emission.
*
* Fonction
* Utilisation de la fonction "write".
*
* Codes retour
* XDC_OK		ok
* XZSSC_DEV_INV		Probleme d'ecriture.
*
* Mecanismes
********************************************************/

extern int XZSS_11EcritureRS232 (int 		Fd, 
				 char* 		Buffer,
				 int 		NbCar)
{
  /* Variables Locales */
  static char *fonction = "XZSS_11EcritureRS232";
  int				vl_ValRet = XDC_OK;

  /* Ecriture */
  if ( write (Fd, Buffer, NbCar) != NbCar)
    {
      vl_ValRet = XZSSC_DEV_INV;
    }

  return (vl_ValRet);
}


/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* FlagO		: XZSSC_FIC_RD ou XZSSC_FIC_WR ou
*		  XZSSC_FIC_CR ou XZSSC_FIC_AP ou 
*		  XZSSC_FIC_RDWR.
* NomFichier	: Nom du fichier complet avec chemin
*		  d'acces a ouvrir.
* Fd		: Descripteur du fichier.
*
* Fonction
* Utilisation de la fonction "fopen"selon les flags 
* d'ouverture et appel de la primitive XZST_03 en 
* cas d'echec.
* XZSSC_FIC_RD correspond l'option "r".
* XZSSC_FIC_WR correspond l'option "w".
* XZSSC_FIC_AP correspond l'option "a".
* XZSSC_FIC_CR correspond l'option "w".
* XZSSC_FIC_RDWR correspond l'option "r+".
*
* Codes retour
* XDC_OK		ok
* XZSSC_FIC_INV		Probleme d'ouverture selon le 
*			compte_rendu de la fonction 
*			fopen.
*
* Mecanismes
********************************************************/

extern int XZSS_12OuvrirFichier (
	int FlagO, 
	char* NomFichier, 
	int* Fd)

{

  /* Variables Locales */
  int				vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_12OuvrirFichier";

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_12OuvrirFichier IN : Flag : %d -- \
		Nom Fic %s \n",FlagO, NomFichier );

  /*B Ouverture du fichier */
  if ((*Fd = open (NomFichier, FlagO)) < 0)
  {
      vl_ValRet = XZSSC_FIC_INV;
  }

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_12OuvrirFichier OUT  : File : %d -- Code Retour %d \n",
		* Fd, vl_ValRet );

  return (vl_ValRet);

}	/* Fin XZSS_12OuvrirFichier */



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* Fd		: Descripteur du fichier.
* 
* Fonction
* Utilisation de la fonction "fclose".
* 
* Codes retour
* XDC_OK 		ok
* XZSSC_FIC_INV		Probleme de fermeture selon le 
*			compte_rendu de la fonction 
*			fclose.
* Mecanismes
********************************************************/

extern int XZSS_13FermerFichier (
	int Fd)

{
  /* Variables Locales */
  int				vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_13FermerFichier";

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_13FermerFichier IN : File : %d \n", Fd );

  if (close (Fd) < 0)
    {
      vl_ValRet = XZSSC_FIC_INV;
    }
	
	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_13FermerFichier OUT  : Code Retour : %d \n",vl_ValRet);

  	return (vl_ValRet);

}	/* Fin XZSS_13FermerFichier */



/* ====================================================================================================
**
**  Fonctions simples de gestion de Minuterie -Timer-, ne permettant pas de specifier la fonction
**   la fonction a executer lorsque le delai (timeout) expire ou l'horodate limite est atteinte.
**
**  A l'echeance, la minuterie n'est pas rearmee automatiquement.
**
==================================================================================================== */

static int	XZSSV_Timeout_Atteint = XDC_NON;


/*******************************************************
** SERVICE RENDU :
**  Fonction marquant la reception de la notification d'expiration de la minuterie.
**
**  Executee sur declenchement du signal ALARM.
**
********************************************************
*/

static void XZSS_Notifier_Timeout (int va_signal)
{
  XZSSV_Timeout_Atteint = XDC_OUI;
}


/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* va_NbSecondes		nombre de secondes
* 
* Fonction
* Installe un traitement de notification d'echeance de timer,
* puis arme une minuterie ayant la duree indiquee.
* 
* Codes retour
* XDC_OK 		ok
* XZSSC_NOK		not ok
*
* Mecanismes
********************************************************/

extern int XZSS_14ArmerTimeout (int va_NbSecondes)
{
  int			vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_14ArmerTimeout";
  struct sigaction	vl_sig;
  struct itimerval	vl_timer;

  if (va_NbSecondes < 0)
  {
    return (XDC_NOK);
  }

  /* Ignorer les eventuels signaux en attente */
  vl_sig.sa_handler = SIG_IGN;
  vl_sig.sa_flags = 0;
  sigaction (SIGALRM, &vl_sig, (struct sigaction *) NULL);

  /* Reinitialiser la marque de notification d'echeance du timeout */
  XZSSV_Timeout_Atteint = XDC_NON;

  if (va_NbSecondes > 0)
  {
    /* Activer la notification d'echeance de timer */
    vl_sig.sa_handler = XZSS_Notifier_Timeout;
    vl_sig.sa_flags = 0;
    sigemptyset (&vl_sig.sa_mask);
    sigaction (SIGALRM, &vl_sig, (struct sigaction *) NULL);
  }

  /* Armer une minuterie ayant la duree indiquee */
  vl_timer.it_value.tv_sec = va_NbSecondes;
  vl_timer.it_value.tv_usec = 0;
  vl_timer.it_interval.tv_sec = 0;
  vl_timer.it_interval.tv_usec = 0;
  setitimer (ITIMER_REAL, &vl_timer, (struct itimerval *) NULL);

  return (vl_ValRet);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction
* 
* Codes retour
* 
* XDC_VRAI:		timeout ecoule
* XDC_FAUX:		timeout non initialise ou en cours
*
* Mecanismes
*			pouet.
********************************************************/

extern int XZSS_15ConsulterTimeout ()
{
  int			vl_Timeout_Atteint;
  sigset_t		vl_maskAlarm;

  /* Desactiver la notification d'echeance de timer */
  sigemptyset (&vl_maskAlarm);
  sigaddset (&vl_maskAlarm, SIGALRM);
  sigprocmask (SIG_BLOCK, &vl_maskAlarm, (sigset_t *) NULL);

  vl_Timeout_Atteint = XZSSV_Timeout_Atteint;

  /* Reactiver la notification d'echeance de timer */
  sigprocmask (SIG_UNBLOCK, &vl_maskAlarm, (sigset_t *) NULL);

  return (vl_Timeout_Atteint);
}



/*X*/
/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction
*  Desactive l'eventuelle minuterie en cours.
* 
* Codes retour
* 
* XDC_OK
*
* Mecanismes
********************************************************/

extern int XZSS_18AnnulerTimeout ()
{
  struct sigaction	vl_sig;
  struct itimerval	vl_timer;

  /* Annuler la notification d'echeance de timer */
  vl_sig.sa_handler = SIG_IGN;
  vl_sig.sa_flags = 0;
  sigemptyset (&vl_sig.sa_mask);
  sigaction (SIGALRM, &vl_sig, (struct sigaction *) NULL);

  /* Reinitialiser la marque de notification d'echeance du timeout */
  XZSSV_Timeout_Atteint = XDC_NON;

  /* Desactiver le timer en cours */
  vl_timer.it_value.tv_sec = 0;
  vl_timer.it_value.tv_usec = 0;
  vl_timer.it_interval.tv_sec = 0;
  vl_timer.it_interval.tv_usec = 0;
  setitimer (ITIMER_REAL, &vl_timer, (struct itimerval *) NULL);

  return (XDC_OK);
}



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* FlagO		: XZSSC_FIC_RD ou XZSSC_FIC_WR ou
*		  XZSSC_FIC_CR ou XZSSC_FIC_AP ou 
*		  XZSSC_FIC_RDWR.
* NomFichier	: Nom du fichier complet avec chemin
*		  d'acces a ouvrir.
* Fd		: Descripteur du fichier.
*
* Fonction
* Utilisation de la fonction "fopen"selon les flags 
* d'ouverture et appel de la primitive XZST_03 en 
* cas d'echec.
* XZSSC_FIC_RD correspond l'option "r".
* XZSSC_FIC_WR correspond l'option "w".
* XZSSC_FIC_AP correspond l'option "a".
* XZSSC_FIC_CR correspond l'option "w".
* XZSSC_FIC_RDWR correspond l'option "r+".
*
* Codes retour
* XDC_OK		ok
* XZSSC_FIC_INV		Probleme d'ouverture selon le 
*			compte_rendu de la fonction 
*			fopen.
*
* Mecanismes
********************************************************/

extern int XZSS_16OuvrirFichierStream (
	char * FlagO, 
	char* NomFichier, 
	FILE ** Fd)

{

  /* Variables Locales */
  int				vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_16OuvrirFichierStream";


	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_16OuvrirFichierStream IN : Flag : %s -- \
		Nom Fic %s \n",FlagO, NomFichier );

  /* ouverture du fichier */
  errno =0;
  if ((*Fd = fopen (NomFichier, FlagO)) == NULL)
    {
	XZST_03EcritureTrace (XZSTC_WARNING ,
	  "Erreur Ouverture Fichier %d \n",errno);

      vl_ValRet = XZSSC_FIC_INV;
    }

	XZST_03EcritureTrace (XZSTC_INTERFACE,
	  "XZSS_16OuvrirFichierStream OUT  : File : %d -- Code Retour %d \n",
		 Fd, vl_ValRet );

  return (vl_ValRet);

}	/* Fin XZSS_16OuvrirFichierStream */



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
* Fd		: Descripteur du fichier.
* 
* Fonction
* Utilisation de la fonction "fclose".
* 
* Codes retour
* XDC_OK 		ok
* XZSSC_FIC_INV		Probleme de fermeture selon le 
*			compte_rendu de la fonction 
*			fclose.
* Mecanismes
********************************************************/

extern int XZSS_17FermerFichierStream (
	FILE * Fd)

{
  /* Variables Locales */
  int				vl_ValRet = XDC_OK;
  static char *fonction = "XZSS_17FermerFichierStream";

	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_17FermerFichierSream IN : File : %d \n", Fd );

  errno = 0;
  if (fclose (Fd) != 0)
    {
	XZST_03EcritureTrace (XZSTC_WARNING ,
	  "Erreur Fermeture  Fichier %d \n",errno);
      vl_ValRet = XZSSC_FIC_INV;
    }
	
	XZST_03EcritureTrace (XZSTC_INTERFACE,
		"XZSS_17FermerFichierStream OUT  : Code Retour : %d \n",
		vl_ValRet);

  	return (vl_ValRet);

}	/* Fin XZSS_17FermerFichierStream */


/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSSYTE
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	A partir du nom d'une tache, retrouve son PID
*
******************************************************
* SEQUENCE D'APPEL :
*/

int	XZSS_20PidTache(	XZSCT_NomTache	pa_nomTache)

/*
* PARAMETRES EN ENTREE : Nom de la tache dont on cherche
*			le PID
*
* PARAMETRES EN SORTIE : aucun
*
* VALEUR(S) DE RETOUR : PID de la tache si elle existe
*			0 sinon
*
* CONDITION D'ERREUR :
*
* - Le repertoire /proc n'a put etre consulte
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *fonction = "XZSS_20PidTache";

	char commande[XDC_LINE_CMD_SIZE]	= "";
	FILE *ps				= NULL;
	char line[XDC_LINE_CMD_SIZE] 		= "";

	XZST_03EcritureTrace(XZSTC_INTERFACE,
			"IN XZSS_20PidTache(tache %s)", pa_nomTache);

	/* Recuperation du PID de la tache */
	sprintf(commande,"ps -ef | awk \'{if (length($5) == 3) {if ($9 == \"%s\"){print $2}} else{if ($8 == \"%s\"){print $2}}}\'",pa_nomTache,pa_nomTache);

		XZST_03EcritureTrace(XZSTC_INFO,
			  "Recherche PID tache %s -- Cmd %s \n",
			  pa_nomTache,commande);

 	/* * Get processes information.  */
	if ((ps = popen (commande, "r")) != NULL) 
	{
		/* Recuperation de la ligne indiquant le PID */
		errno =  0;
		fgets(line,XDC_LINE_CMD_SIZE,ps);	
		if (errno)
		{
			/* ecriture trace pour connaitre la ligne lu */
			XZST_03EcritureTrace(XZSTC_WARNING,
			  "ERROR fgets PID %d ",errno);
		}

		/* ecriture trace pour connaitre la ligne lu */
		XZST_03EcritureTrace(XZSTC_INFO,
			  "PID %s trouve pour tache %s",line,pa_nomTache);

#if 1 
			/* Modif V1.27 */	
		/* Fermeture du Fichier */
		(void) pclose (ps);
#endif
	}
	else 
	{
		/*  Ecriture trace indiquant un probleme de lecture PID */
		XZST_03EcritureTrace(XZSTC_WARNING,
		  "lecture du PID par popen a echou� pour %s -- errno %d\n",pa_nomTache,errno);
	}

#if 0
	(void) pclose (ps);
#endif

	/* On n a pas trouve le pid de la tache !!! */
   	if (!strcmp(line,""))
	{
		strcpy(line,"0");
	}
					   
	XZST_03EcritureTrace(XZSTC_INTERFACE,
			"XZSS_20PidTache : OUT  Pid : %s \n",line);


#if 0
	/*A Liberation du Pointeur */
	free(ps);
#endif


	return(atoi(line));

}	/* Fin XZSS_20PidTache */



/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSS
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	A partir d'un nom de repertoire retourne un 
*	pointeur vers la liste des fichiers de ce 
*	repertoire.
*
******************************************************
* SEQUENCE D'APPEL :
*/

int	XZSS_21LectureRepertoire(	char	*pa_nomRepert,
					char	***pa_listeFic)

/*
* PARAMETRES EN ENTREE : nom du repertoire dont on veut
*			connaitre le contenue
*
* PARAMETRES EN SORTIE : le pointeur vers la liste des 
*			fichiers du repertoire
*
* VALEUR(S) DE RETOUR : XDC_NOK si la liste des fichiers
*			du repertoire n'a put etre obtenue
*			XDC_OK sinon
*
* CONDITION D'ERREUR :
*
* - Le repertoire n'est pas accessible
* - Echec d'un malloc
*
* CONDITIONS LIMITES : neant
*
******************************************************/
{
  static char *fonction = "XZSS_21LectureRepertoire";

typedef struct list 	
{ 
	char 		*nom; 
	struct list 	*suiv; 
} vl_list;

vl_list		*pl_listfic = (struct list *)NULL;
vl_list 	*pl_listaux = (struct list *)NULL;
DIR		*pl_repert;
struct dirent	*pl_unFic;
int		vl_ind,vl_nbFic;


/* Le repertoire est-il accessible */
if (access(pa_nomRepert,F_OK|R_OK))
	{
	XZST_03EcritureTrace(	XZSTC_WARNING,
		"XZSS_21LectureRepertoire : acces repertoire '%s' impossible",
		pa_nomRepert);
	return(XDC_NOK);
	}

/* Ouverture du repertoire */
pl_repert = opendir(pa_nomRepert);
if (pl_repert == NULL)
	{
	XZST_03EcritureTrace(	XZSTC_WARNING,
		"XZSS_21LectureRepertoire : ouverture repertoire '%s' impossible",
		pa_nomRepert);
	return(XDC_NOK);
	}

/* Lecture du repertoire */
vl_nbFic = 0;
while ( (pl_unFic = readdir(pl_repert)) != NULL )
	{
	/* Allocation de la taille de la structure locale */
	pl_listaux = (struct list *)malloc(sizeof(struct list));
	if (pl_listaux == NULL)
		{
		XZST_03EcritureTrace(	XZSTC_FATAL,
			"XZSS_21LectureRepertoire : malloc pl_listaux impossible");
		closedir(pl_repert);
		return(XDC_NOK);
		}

	/* Allocation de la taille du nom de fichier */
	pl_listaux->nom = (char *)malloc(strlen(pl_unFic->d_name)+1);
	if (pl_listaux->nom == NULL)
		{
		XZST_03EcritureTrace(	XZSTC_FATAL,
			"XZSS_21LectureRepertoire : malloc pl_listaux->nom impossible");
		closedir(pl_repert);
		return(XDC_NOK);
		}

	/* Recopie du nom du fichier */
	strcpy(pl_listaux->nom,pl_unFic->d_name);

	/* Chainage de la liste */
	if ( pl_listfic == (struct list *)NULL )
		{
		pl_listaux->suiv = (struct list *)NULL;
		}
	else
		{
		pl_listaux->suiv = pl_listfic;
		}
	pl_listfic = pl_listaux;

	/* Comptage du nombre de fichiers */
	vl_nbFic++;
	}

/* Fermeture du repertoire */
closedir(pl_repert);

/* Allocation de la liste a renvoyer */
*pa_listeFic = (char **)malloc(sizeof(void *) * (((size_t) vl_nbFic)+1));
if (*pa_listeFic == NULL)
	{
	XZST_03EcritureTrace(	XZSTC_FATAL,
		"XZSS_21LectureRepertoire : malloc *pa_listeFic impossible");
	return(XDC_NOK);
	}

/* Remplissage de la liste */
pl_listaux = pl_listfic;
for(vl_ind = vl_nbFic-1;vl_ind >= 0;vl_ind--)
	{
	(*pa_listeFic)[vl_ind] = pl_listaux->nom;
	if (pl_listaux->suiv != (struct list *)NULL) 
		pl_listaux = pl_listaux->suiv;
	}
(*pa_listeFic)[vl_nbFic] = (char *)NULL;

/* Liberation de la liste de structures locales */
free(pl_listfic);

return(XDC_OK);

} /* XZSS_21LectureRepertoire */




/*X*/
/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* ALGORITHME :
*
* Arguments
*
* Fonction
*  Suspend l'execution de la tache pendant une duree specifiee
* 
* Codes retour
* 
* XDC_OK
*
* Mecanismes
********************************************************/

extern int XZSS_22Attendre (int va_Nb_Millisec)
{
  struct timespec	vl_duree;
  struct timespec	vl_reste;
  int			vl_cr = 0;

  vl_duree.tv_sec = (time_t) (va_Nb_Millisec / 1000);
  vl_duree.tv_nsec = (long) ((va_Nb_Millisec % 1000) * 1000000);

  do {
    vl_cr = nanosleep (&vl_duree, &vl_reste);
    vl_duree = vl_reste;
  } while ( vl_cr != 0 );

  return (XDC_OK);
}


/* Fin du fichier */
