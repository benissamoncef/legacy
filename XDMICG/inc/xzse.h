/*E*/
/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSE * FICHIER xzse.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSE
*        - Declarations externes des fonctions
*        - Declarations externes des variables
*        - Declarations des constantes
*        - Declarations des types
*
*   Reference DCG-021 chapitre 3.3  
*
******************************************************
* HISTORIQUE :
*
*  T.MIlleville   21 Oct 1994  : Creation
*  T.MIlleville   25 Aou 1995  : Ajoput parametre XZSE_12 V1.3
***
*  P.Niepceron    21 Aou 1996  : Ajout de XZSE_Abt_Comm_Site + XZSET_EComm_Site v1.4
* JPL		30/11/10 : Migration architecture HP ia64 (DEM 961) : suppression warnings mineurs  1.5
*******************************************************/
#ifndef xzse
#define xzse

/* fichiers inclus */

#include "xzsc.h"
#include "xzst.h"
#include "xzea.h"

#include <errno.h> 

/*definitions de constantes exportees */
/* Codes d'erreur */
#define XZSEC_ARG_INV		0	/* argument invalide */
#define XZSEC_NOM_INV		1	/* nom machine invalide */
#define XZSEC_NOS_INV		2	/* nom sous-systeme invalide */
#define XZSEC_NOT_INV		3	/* nom tache invalide */
#define XZSEC_NOA_INV		4	/* nom site invalide */

#define XZSEC_MODE_INS		1	/* Station en mode instruction */
#define XZSEC_MODE_REC		2	/* Station en mode reconstruction */
#define XZSEC_MODE_NOR		3	/* Station en mode normal */
#define XZSEC_MODE_TST		4	/* Station en mode test */

#define XZSEC_ETAT_VEILLE	1	/* Calculateur en mode veille */
#define XZSEC_ETAT_NACTIF	2	/* Calcul en mode nominal actif */
#define XZSEC_ETAT_NPASSIF	3	/* Calcul en mode nominal passif */
#define XZSEC_ETAT_DEGRADE	4	/* Calculateur en mode degrade */

#define XZSEC_SANS_ETAT		0	/* Aucun etat */
#define XZSEC_ETAT_OK		1	/* Entite en mode correcte */
#define XZSEC_ETAT_NOK		2	/* Entite en mode problematique */

#define XZSEC_MODE_TOGGLE	5	/* Demande d'affectation du mode a l'oppose de sa valeur */


/* definitions de types exportes */

typedef struct {
	XDY_Booleen 	Etat;
	XDY_Nom		Site;
		}
		XZSET_EComm_Site;
		
		
/* declaration des donnees globales */

/* declaration des fonctions externes */



/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_01PassageModeExpIns(va_ModeIns)
*
* Arguments
* ModeIns	: XZSEC_MODE_INS ou XZSEC_MODE_NOR ou 
*		  XZSEC_MODE_TOGGLE
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOM_INV		Nom machine invalide 
*			(compte-rendu de XZSC_07)
* XZSEC_ARG_INV 	Argument ModeIns invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_01PassageModeExpIns(int va_ModeIns);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_03ModeIns(pa_ModeIns)
*
* Arguments
* ModeIns :	XZSEC_MODE_INS ou XZSEC_MODE_NOR
*
* Codes retour
* XZSCC_OK	 ok.
*
* Mecanismes
******************************************************/

extern int XZSE_03ModeIns(int *pa_ModeIns);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_05BasculementSC(va_NomSite)
*
* Arguments
* NomSite :	Nom du site.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOA_INV		Argument NomSite invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_05BasculementSC(XZSCT_NomSite va_NomSite);       




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_07BasculementSD()
*
* Arguments
* 
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_ARG_INV		Argument invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_07BasculementSD();



/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_09EtatMachine(pa_EtatMachine)
*
* Arguments
* EtatMachine :	XZSEC_ETAT_NACTIF ou XZSEC_ETAT_NPASSIF.
*  
* Codes retour
* XZSCC_OK	ok.
*
* Mecanismes
******************************************************/

extern int XZSE_09EtatMachine (int *pa_EtatMachine);



/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_10AutoriseMajModeEtat( )
*
* Arguments
* 
* Codes retour
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks : RTS3.
******************************************************/

extern int XZSE_10AutoriseMajModeEtat();




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_11EnvoieEtatSSysteme(va_NomMachine, 
*			    va_NomSSysteme, 
*			    va_EtatSSysteme)
*
* Arguments 
* NomMachine   : Nom de la machine du sous-systeme.
* NomSSysteme  : Nom du sous-systeme.
* EtatSSysteme : XZSEC_ETAT_ NOK , XZSEC_ETAT_OK ou
*		 XZSEC_SANS_ETAT.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOS_INV		Nom du sous-systeme invalide.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument EtatSSysteme invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_11EnvoieEtatSSysteme(XZSCT_NomMachine	va_NomMachine,
				     XZSCT_NomSSysteme	va_NomSSysteme,
				     int		va_EtatSSysteme);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_12EnvoieEtatTache(va_NomMachine, 
*			 va_NomTache, 
*			 va_EtatTache
			 va_Priorite)
*
* Arguments
* NomMachine : Nom de la machine de la tache.
* NomTache   : Nom de la tache.
* EtatTache  : XZSEC_ETAT_ NOK , XZSEC_ETAT_OK 
*		ou XZSEC_SANS_ETAT.
* Priorte msg RTworks
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOT_INV		Nom de tache invalide.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument EtatTache invalide.
*
* Mecanismes
* RTworks : RTS2.
* ******************************************************/

extern int XZSE_12EnvoieEtatTache(XZSCT_NomMachine	va_NomMachine,
				  XZSCT_NomTache	va_NomTache, 
				  int			va_EtatTache,
				  int			va_Priorite);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_13EnvoieEtatMachine(va_NomMachine, 
*			   va_EtatMachine)
*
* Arguments
* NomMachine  :	Nom du calculateur.
* EtatMachine :	XZSEC_SANS_ETAT ou
*		XZSEC_ETAT_VEILLE ou       
*		XZSEC_ETAT_NACTIF ou       
*		XZSEC_ETAT_NPASSIF ou      
*		XZSEC_ETAT_DEGRADE.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument EtatMachine invalide.
*
* Mecanismes
* RTworks : RTS2.
* ******************************************************/

extern int XZSE_13EnvoieEtatMachine(XZSCT_NomMachine 	va_NomMachine,
				    int 		va_EtatMachine);



/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_14EnvoieEtatPeriph(va_NomPeriph, va_EtatPeriph)
*
* Arguments
* NomPeriph  : Nom du peripherique.
* EtatPeriph : XZSEC_ETAT_ NOK ou XZSEC_ETAT_OK.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_ARG_INV		Argument EtatPeriph invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_14EnvoieEtatPeriph(XZSCT_NomPeriph 	va_NomPeriph,
				   int 			va_EtatPeriph);


/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_15EnvoieEtatInterface(va_NomInterface, 
*			     va_EtatInterface)
*
* Arguments
* NomInterface    : Nom de l'interface.
* EtatInterface   : XZSEC_ETAT_ NOK ou XZSEC_ETAT_OK.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_ARG_INV		Argument EtatInterface invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_15EnvoieEtatInterface(XZSCT_NomInterface va_NomInterface,
				      int 		va_EtatInterface);


/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_16EnvoieEtatConcentrateur(pa_NomZse, 
*				 va_EtatZse)
*
* Arguments
* NomZse  : Nom du concentrateur ZSE.
* EtatZse : XZSEC_ETAT_ NOK ou XZSEC_ETAT_OK.
*
* Codes retour
* XZSCC_OK               Ok.
* XZSEC_ARG_INV          Argument EtatZse invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_16EnvoieEtatConcentrateur(char	*pa_NomZse, 
					  int	va_EtatZse);


/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_17EnvoieModeStation(va_NomMachine, 
*			   va_ModeIns,
*			   va_ModeRec)
*
* Arguments
* NomMachine	: Nom de la machine.
* ModeIns	: XZSEC_MODE_INS ou XZSEC_MODE_NOR.
* ModeRec	: XZSEC_MODE_REC ou XZSEC_MODE_NOR.
*
* Codes retour
* XZSCC_OK 		Ok.
* XZSEC_NOM_INV		Nom de la machine invalide.
* XZSEC_ARG_INV		Argument ModeIns ou ModeRec 
*			invalide.
*
* Mecanismes
* RTworks : RTS2.
*******************************************************/

extern int XZSE_17EnvoieModeStation(XZSCT_NomMachine 	va_NomMachine, 
				    int 		va_ModeIns, 
				    int 		va_ModeRec);



/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_18NomSCActif(va_NomMachine)
*
* Arguments
* NomMachine : Nom du serveur de communication actif.
*
* Codes retour
* XZSCC_OK		ok.
*
* Mecanismes
*******************************************************/

extern int XZSE_18NomSCActif(XZSCT_NomMachine va_NomMachine);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_19NomSDActif(va_NomMachine)
*
* Arguments
* NomMachine :	Nom du serveur de donnees actif.
*
* Codes retour
* XZSCC_OK		ok.
*
* Mecanismes
*******************************************************/

extern int XZSE_19NomSDActif(XZSCT_NomMachine va_NomMachine);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_20InitSynoptiqueAG()
*
* Arguments
* 
* Codes retour
* XZSCC_OK                                Ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_20InitSynoptiqueAG();




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_21InitSynoptiqueSS(va_NomSite)
*
* Arguments
* NomSite :	Nom du site.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOA_INV		Argument NomSite invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_21InitSynoptiqueSS(XZSCT_NomSite va_NomSite);





/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_22InitSynoptiqueTache(va_NomMachine)
*
* Arguments
* NomMachine :		Nom de la machine.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_NOM_INV		Nom de la machine invalide.
*
* Mecanismes
* RTworks : RTS2.
*******************************************************/

extern int XZSE_22InitSynoptiqueTache(XZSCT_NomMachine va_NomMachine);





/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_23InitSynoptiqueZSE()
*
* Arguments
* 
* Codes retour
* XZSCC_OK		Ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_23InitSynoptiqueZSE();





/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_25ActualiserEtatMachine(va_EtatMachine)
*
* Arguments
* EtatMachine :		XZSEC_ETAT_NACTIF ou
*			XZSEC_ETAT_NPASSIF.
*
* Codes retour
* XZSCC_OK		ok.
*
* Mecanismes
******************************************************/

extern int XZSE_25ActualiserEtatMachine(int va_EtatMachine);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_26EnvoieEtatTrace(va_NomMachine, 
*			 va_NomTache,
*			 va_NivTache, 
*			 va_VisuTrace)
* Arguments
* NomMachine :	Nom de la machine de la tache.
* NomTache   :	Nom de la tache.
* NivTrace   :	Niveau de trace de la tache.
* VisuTrace  :	XZSTC_ECRAN ou XZSTC_NECRAN.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_ARG_INV		Argument VisuTrace invalide.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSE_26EnvoieEtatTrace(XZSCT_NomMachine 	va_NomMachine,
				  XZSCT_NomTache 	va_NomTache,
				  unsigned int		va_NivTache,
				  int 			va_VisuTrace);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_27ActualiserNomMachineActive()
*
* Arguments
*
* Codes retour
* XZSCC_OK		ok.
*
* Mecanismes
******************************************************/

extern int XZSE_27ActualiserNomMachineActive(char *NomMachine);




/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSE_28EnvoieEtatTacheTmp(va_NomMachine, 
*			    va_NomTache, 
*			    va_EtatTache)
*
* Arguments
* NomMachine :	Nom de la machine de la tache.
* NomTache   :	Nom de la tache.
* EtatTache  :	XZSEC_ETAT_ NOK , XZSEC_ETAT_OK ou 
*		XZSEC_SANS_ETAT.
*
* Codes retour
* XZSCC_OK		Ok.
* XZSEC_ARG_INV		Argument EtatTache invalide.
*
* Mecanismes
* RTworks : RTS2.
*******************************************************/

extern int XZSE_28EnvoieEtatTacheTmp(XZSCT_NomMachine 	va_NomMachine,
				     XZSCT_NomTache 	va_NomTache,
				     int 		va_EtatTache);





/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
* Arguments
* pa_NomMachine : Nom de la machine dont on veut connaitre
* les informations demandees.
*
* Fonction
* Cette fonction permet de recuperer via les messages 
* appropries l'etat d'une machine, de ses sous-systemes et
* de ses taches.
*
* Codes retour
* XDC_OK		Ok.
* XDC_NOK
*
* Mecanismes
********************************************************/

extern int XZSE_30DemanderEtatMachineAppli(XZSCT_NomMachine pa_NomMachine);


/* Fichier : $Id: xzse.h,v 1.5 2010/11/30 17:48:52 gesconf Exp $      Release : $Revision: 1.5 $        Date : $Date: 2010/11/30 17:48:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSE 
******************************************************
* ALGORITHME :
*
*
* ARGUMENTS EN ENTREE :
*
* XDY_Booleen	va_abonnement		: Abonnement : Debut ou fin d'abonnement
* XDY_Fonction	pa_fonction		: Nom de la fonction utilisateur
*
* ARGUMENTS EN SORTIE :
*
* int		pa_resultat		: Code retour resultat
*
* CODE RETOUR : 
*   XDC_OK		Succes
*
* CONDITION D'UTILISATION
*   Ce service est demande pour s'abonner aux etat de communication des Sites
*
* FONCTION 
*   Si demande d'abonnement
*     Alors : armement CallBack et joindre le Datagroup correspondant
*     Sinon : Desarmment CallBack (si demande fermeture) et disjoindre le Datagroup
********************************************************/

extern int XZSE_Abt_Etat_Comm_Site(XDY_Booleen va_abonnement, XDY_Fonction pa_fonction);

#endif

/* fin de fichier */



