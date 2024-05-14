/*E*/
/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSTIMER * FICHIER xzsm.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSTIMER
*        - Declarations externes des fonctions
*        - Declarations externes des variables
*        - Declarations des constantes
*        - Declarations des types
*
*   Reference DCG-021 chapitre 3.4  
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville   24 Aou 1994      : Creation
*  T.Milleville   10 Mar 1995      : Ajout d'un argment precisant
*	le nom du Site dans la fonction XZSM_06.. V1.10
*  T.Milleville   5 Jul 1995      : Ajout d'un argment dans la structure
*	ENR_TIMER  precisant la priorite V1.11
* JPL		08/12/10 : Migration architecture HP ia64 (DEM 961) : declaration correcte de 'XZSM_09ModeHorodate'  1.12
* JPL		11/01/11 : Migration architecture HP ia64 (DEM 971) : declaration XZSM_00InitServices, XZSM_16  1.13
*******************************************************/

#ifndef xzsm
#define xzsm

/* fichiers inclus */

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "xdy.h"
#include "xzst.h"
#include "xzsc.h"
#include "xzse.h" 

/* definitions de constantes exportees */

#define XZSMC_OK			XDC_OK
#define XZSMC_NOK			XDC_NOK
#define XZSMC_HORODATE_NUL		(0.0)
#define XZSMC_CLEPROG_NUL		(NULL)
#define XZSMC_ARG_INV			(XDC_ARG_INV)
#define XZSMC_INI_INV			(XDC_ARG_INV+1)
#define XZSMC_RTW_INV			(XDC_ARG_INV+2)
#define XZSMC_TYPE_SYSTEME		(0)
#define XZSMC_TYPE_RTWORKS		(1)
#define XZSMC_RTDAQ			(1)
#define XZSMC_NSUPV			(2)
#define XZSMC_SYSTE			(3)
#define XZSMC_ECRITURE			0
#define XZSMC_LECTURE			1

#define XZSMC_SEC_VRAI 			0	
#define XZSMC_SEC_FAUX 			1	

/* definitions de types exportes */

	/* Horodate au format jj/mm/aaaa hh:mm:ss */
typedef char 		XZSMT_Horodate[20];    

	/* Heure  au format HH:MM:SS */
typedef char		XZSMT_Heure[9];

	/* Date au format JJ/MM/AAAA */
typedef char		XZSMT_Date[11];

	/* Delai au format jj:hh:mm:ss */
typedef char		XZSMT_Delai[12];

typedef char 		XZSMT_HorodateInv[15];
typedef char 		XZSMT_CleProg[50];

typedef struct {
	double          Horodate ;
	int             Delai,
			IdMsg ;
	XZSCT_Datagroup Dg ;
	XZSMT_CleProg   Cle ;
	int   		Priorite ;
	void *          Suivant ;

} XZSMT_ENRG_TIMER ;


/* declaration des donnees globales */

int     XZSMV_TimeRecu ;


/* declaration des fonctions externes */



/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSM
******************************************************
* DESCRIPTION DE LA FONCTION :
* Cette fonction permet au process utilisateur de
* gerer les redemandes de programmation de reveil
*
******************************************************
* SEQUENCE D'APPEL :
*/

extern void XZSM_00InitServices();





/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_01DemanderProgrammation(HorodateSec, Delai, 
* 			       Datagroup, IdMesg,
*			       CleProg,Secur)
*
* Arguments
* HorodateSec : Horodate de declenchement de la 
*		programmation.
* Delai       :	Delai de programmation cyclique en 
*		secondes.
* Datagroup   : Datagroup destinataire du message.
* IdMesg      : Identifiant du message a envoyer.
* CleProg     : Cle identifiant la programmation du 
*		timer et permettant d'annuler la 
*		programmation.
* Secur       :  Flag indiquant une securite de programmation
*
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSMC_INI_INV		initialisation non effectuee
*			(primitive XZSC_07).
* XZSCC_OK		ok.
* Mecanismes
* RTworks :       RTS2
******************************************************/

extern int XZSM_01DemanderProgrammation(double HorodateSec,
					 int Delai, 
					 XZSCT_Datagroup Datagroup, 
					 int IdMesg, 
					 XZSMT_CleProg CleProg,
					 int Secur);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_02AnnulerProgrammation(CleProg, Datagroup,
*			      IdMesg)
*
* Arguments
* CleProg   :	Cle annulant la programmation.
* Datagroup :	Annulation de toutes les programmations
*		concernant ce Datagroup.
* IdMesg    :	Annulation de toutes les programmations 
*		concernant cet identifiant du message.
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSMC_INI_INV		initialisation non effectuee
*			(primitive XZSC_07).
* XZSCC_OK		ok.
* Aucun code retour n'est prevu si la cle fournie 
* n'existe pas. En effet cela necessiterait d'attendre
* un message provenant du timer ou de faire une gestion
* des programmations en local de la tache demandeur.
*
* Mecanismes
* RTworks :       RTS2
******************************************************/

extern int XZSM_02AnnulerProgrammation(XZSMT_CleProg CleProg,
					XZSCT_Datagroup	Datagroup,
					int IdMesg);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_05DemanderDiffusionMode(NomTache, NomMachine)
*
* Arguments
* NomTache   :	Nom de la tache dont le mode est a
*		actualiser.
* NomMachine :	Nom de la machine de la tache.
*
* Codes retour
* XZSCC_OK	ok.
*
* Mecanismes
* RTworks :       RTS2.
******************************************************/

extern int XZSM_05DemanderDiffusionMode(XZSCT_NomTache NomTache,
					XZSCT_NomMachine NomMachine);



/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_06AutoriserMajHorodate(int va_ModeIns,
*				XZSCT_NomSite va_NomSite)
*
* Arguments
* ModeEmis : XSSMC_RTDAQ ou XZSMC_NSUPV
*
* Codes retour
* XZSMC_INI_INV		initialisation deja effectuee.
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks : RTS1.
******************************************************/

extern int XZSM_06AutoriserMajHorodate(
		int va_ModeIns,
		XZSCT_NomSite	va_NomSite);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_07LireHorodate(TypeHorloge, JourSemaine,
*		      HorodateSec)
*
* Arguments
* TypeHorloge :	XZSMC_TYPE_SYSTEME ou 
*		XZSMC_TYPE_RTWORKS
* JourSemaine : Jour de la semaine de l'horodate.
* HorodateSec : Horodate lue en secondes.
* 
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSMC_OK     		ok
*
* Mecanismes
* Disponibilite de l'horodate sur toutes les taches.
******************************************************/

extern int XZSM_07LireHorodate(int TypeHorloge, 
				int *JourSemaine, 
				double *HorodateSec);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_08EmettreHorodate(HorodateSec)
*
* Arguments
* HorodateSec : Horodate en secondes a envoyer.
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks :       RTS2
******************************************************/

extern int XZSM_08EmettreHorodate(double HorodateSec);



/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_09ModeHorodate(Mode_Operation, Mode_Emission)
*
* Arguments
* ModeEmis :	XSSMC_RTDAQ ou XZSMC_NSUPV ou XZSMC_SYSTE
*
* Codes retour
* XZSMC_ARG_INV		argument invalide.
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks : RTS1 , RTS2 et RTS3.
******************************************************/

extern int XZSM_09ModeHorodate(int Mode_Operation,
				   int * Mode_Emission);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_10ActualiserDeltaHorodate(HorodateSec)
*
* Arguments
* HorodateSec : Horodate en secondes.
*
* Codes retour
* XZSMC_INI_INV		Initialisation non effectuee.
* XZSCC_OK		ok.
*
* Mecanismes
* Diffusion de l'horodate a travers le reseau par le
* datagroup "_time", et disponibilite de l'horodate 
* sur toutes les taches.
* ******************************************************/

extern int XZSM_10ActualiserDeltaHorodate(double HorodateSec);
extern int XZSM_10ActualiserDeltaHorodateU(double HorodateSec);


/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_11ConversionHorodate(HorodateSec, Horodate)
*
* Arguments
* HorodateSec :	Horodate en secondes
* Horodate    : Horodate au format horodate.
*
* Codes retour
* XZSCC_OK	ok.
* 
* Mecanismes
*******************************************************/

extern int XZSM_11ConversionHorodate(double HorodateSec, 
				      XZSMT_Horodate *Horodate);





/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_12ConversionHorodateInv(HorodateSec, HorodateInv)
*
* Arguments
* HorodateSec : Horodate en secondes
* HorodateInv : Horodate au format inverse.
*
* Codes retour
* XZSCC_OK	ok.
* Mecanismes
*******************************************************/

extern int XZSM_12ConversionHorodateInv(double HorodateSec,
					 XZSMT_HorodateInv *HorodateInv);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
*
* 
* Arguments
* Horodate    : Horodate au format horodate.
* HorodateSec : Horodate en secondes
*
* Codes retour
* XZSCC_OK	 ok.
* 
* Mecanismes
*******************************************************/

extern int XZSM_13ConversionHorodateSec(XZSMT_Horodate	Horodate, 
					 double *HorodateSec);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_14ConversionHorodateSecDelai(HorodateSec, Delai)
*
* Arguments
* HorodateSec :	Horodate en secondes
* Delai    : Horodate au format Delai
*
* Codes retour
* XZSCC_OK	ok.
* 
* Mecanismes
*******************************************************/

extern int XZSM_14ConversionHorodateSecDelai(double HorodateSec, 
				      XZSMT_Delai *Delai);




/* Fichier : $Id: xzsm.h,v 1.14 2019/04/23 11:20:31 pc2dpdy Exp $	      Release : $Revision: 1.14 $        Date : $Date: 2019/04/23 11:20:31 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTIMER 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSM_16ConvHeureNbSec(va_Date, va_Heure, *pa_NbSec)
*
* Arguments
* va_Date	: Date au Format JJ/MM/AAAA
* va_Heure	: Heure au Format HH:MM:SS
* pa_NbSec 	Nombre de Secondes
*
* Codes retour
* XDC_OK	si l'heure a pu etre recuperee
* XDC_NOK	sinon
* 
* Mecanismes
*******************************************************/

int XZSM_16ConvHeureNbSec(XZSMT_Date  	va_Date,
			XZSMT_Heure	va_Heure,
			double 		* pa_NbSec);



#endif

/* fin de fichier */
