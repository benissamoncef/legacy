/*E*/
/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSIDMIN        * FICHIER xzsi.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   - ???
*   - 
*
* Reference DCG-023 chapitre 3.1
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 16 Nov 1994 : Creation         
* T.Milleville 08 Mar 1995 	: Ajout d'un Argument dans la
*	fonction XZZI_06 pour soecifier le destinataire V1.2
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : declaration XZSI_08AffMsgAdm  1.3
*******************************************************/


/******************************************************
* TESTS UNITAIRES  : RELECTURE CROISEE
*
*******************************************************/

#ifndef xzsi
#define xzsi
 
/* fichiers inclus */

#include "xzsc.h"
#include "xzst.h"
#include "xzia.h"

   
/* definitions de constantes exportees */

#define XZSIC_NOM_INV 1
#define XZSIC_INI_INV 2

#define XZSIC_MODE_INS     XZSEC_MODE_INS  
#define XZSIC_MODE_NOR     XZSEC_MODE_NOR  
#define XZSIC_ETAT_NACTIF  XZSEC_ETAT_NACTIF 
#define XZSIC_ETAT_NPASSIF XZSEC_ETAT_NPASSIF
#define XZSIC_ETAT_VEILLE  XZSEC_ETAT_VEILLE 
#define XZSIC_ETAT_DEGRADE XZSEC_ETAT_DEGRADE
#define XZSIC_SANS_ETAT    XZSEC_SANS_ETAT   
#define XZSIC_OK           XZSEC_ETAT_OK          
#define XZSIC_NOK          XZSEC_ETAT_NOK          


/* definitions de types exportes */
 
/* declaration des donnees globales */
  
/* declaration des fonctions externes */
   



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_01DiffEtatSSysteme(va_NomMachine,
*                     va_NomSSysteme,
*                     va_EtatSSysteme);
*
* Arguments
* NomMachine:     nom du calculateur sur lequel se 
*                 trouve le sous-systeme.
* NomSSysteme:    nom du sous-systeme
* EtatSSysteme:   etat du sous-systeme
*                 (XZSIC_OK ou XZGAC_NOK)
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/

extern int XZSI_01DiffEtatSSysteme(XZSCT_NomMachine 	va_NomMachine,
                         	XZSCT_NomSSysteme 	va_NomSSysteme,
                        	int 			va_EtatSSysteme);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_02DiffEtatTache(va_NomMachine,
*                      va_NomTache,
*                      va_EtatTache)
*
* Arguments
* NomMachine:     nom du calculateur sur lequel se
*                 trouve la tache.
* NomTache:       nom de la tache
* EtatTache:      etat de la tache (XZSIC_OK ou
*                 XZSIC_NOK)
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_02DiffEtatTache(XZSCT_NomMachine	va_NomMachine,
                         	XZSCT_NomTache		va_NomTache, 
		 	 	int 			va_EtatTache);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_03DiffEtatServeur(va_NomMachine, va_EtatMachine)
*
* Arguments
* NomMachine:     nom du serveur de communication ou
*                 de donnees.
* EtatMachine:    etat du calculateur (XZSIC_VEILLE,
*                 XZSIC_NOMACT, XZGAC_NOMPAS ou
*                 XZSIC_DEGRAD)
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_03DiffEtatServeur(XZSCT_NomMachine	va_NomMachine,
                              int		va_EtatMachine);




/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* void XZSI_04DiffEtatPeriph(va_NomPeriph,
*                            va_EtatPeriph)
*
* Arguments
* NomPeriph:      nom du peripherique
* EtatPeriph:     etat du peripherique (XZSIC_OK ou
*                 XZSIC_NOK)
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_04DiffEtatPeriph(XZSCT_NomPeriph	va_NomPeriph,
                                 int			va_EtatPeriph);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_05DiffEtatInterface(va_NomInterface,
*                          va_EtatInterface)
*
* Arguments
* NomInterface:   nom de l'interface
* EtatInterface:  etat de l'interface (XZSIC_OK ou
*                 XZSIC_NOK)
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_05DiffEtatInterface(XZSCT_NomInterface	va_NomInterface,
                       		    int 		va_EtatInterface);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_06AffMsgSyst(XZIAT_destinataire va_destinataire,va_NumTexte, ...)
*
* Arguments
* va_destinataire Destinataire du Msg
* NumTexte:       indice du message d'administration
* ...             chaines a ajouter au message
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_06AffMsgSyst(XZIAT_destinataire va_destinataire,
			int	va_NumTexte, ...);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_07DiffNivVisuTrace(va_NivTrace,
*                         va_VisuTrace,
*                         va_NomTache,
*                         va_NomMachine)
*
* Arguments
* NivTrace:       niveau de trace
* VisuTrace:      visualisation trace (ecran ou non)
* NomTache:       nom de la tache
* NomMachine:     nom de la machine
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_07DiffNivVisuTrace(XZSTT_NiveauTrace 	va_NivTrace,
			           int 			va_VisuTrace,
			           XZSCT_NomTache 	va_NomTache,
			           XZSCT_NomMachine 	va_NomMachine);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_08AffMsgAdm(va_destinataire,
*                  va_NumTexte ,...)
*
* Arguments
* va_destinataire Destinataire du Msg
* NumTexte:       indice du message d'administration
* P0,.., P9       chaines a ajouter au message
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/

extern int XZSI_08AffMsgAdm(
			XZIAT_destinataire va_destinataire,
			int   va_NumTexte, ...);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_09DiffEtatConcentrateur(va_NomConcentrateur,
*                              va_EtatConcentrateur)
*
* Arguments
* NomConcentrateur:       nom du concentrateur ZSE
* EtatConcentrateur:      etat du concentrateur
*                         (XZSIC_OK ou XZGAC_NOK)
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_09DiffEtatConcentrateur(
				XZSCT_NomMachine va_NomConcentrateur,
	      			int 		 va_EtatConcentrateur);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* void XZSI_10DiffModeStation(va_NomMachine,
*                             va_ModeInstr)
*
* Arguments
* NomMachine:     nom du calculateur
* ModeInstr       mode instruction ou non (XZSIC_ON,
*                 XZSIC_OFF)
*
* Codes retour
*
* Mecanismes
*
******************************************************/


extern int XZSI_10DiffModeStation(XZSCT_NomMachine 	va_NomMachine,
			   	  int 			va_ModeInstr);



/* Fichier : $Id: xzsi.h,v 1.3 2011/01/11 16:26:27 gesconf Exp $        Release : $Revision: 1.3 $        Date : $Date: 2011/01/11 16:26:27 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSIDMIN
******************************************************
* SEQUENCE D'APPEL :
* XZSI_11DiffEtatStation(va_NomMachine,
*                        va_EtatStation)
*
* Arguments
* NomMachine:     nom du calculateur.
* EtatStation:    etat du calculateur (XZSIC_VEILLE,
*                 XZSIC_NOMACT, XZGAC_NOMPAS ou
*                 XZSIC_DEGRAD)
*
*
* Codes retour
* XDC_OK	: ok
*
* Mecanismes
*
******************************************************/


extern int XZSI_11DiffEtatStation(XZSCT_NomMachine 	va_NomMachine,
			  	 int 			va_EtatStation);


#endif
/* fin de fichier */
