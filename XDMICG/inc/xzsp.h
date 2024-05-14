/*E*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $      Release : $Revision: 1.5 $        Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSP * FICHIER xzsp.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSP
*        - Declarations externes des fonctions
*        - Declarations externes des variables
*        - Declarations des constantes
*        - Declarations des types
*
*   Reference DCG-021 chapitre 3.7  
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  5 Oct 1994	: Creation
*
*  T.Milleville  21 Dec 1994	: Modifiaction de la taille 
*					de TypeFichier     V1.3
*  T.Milleville  26 Jul 1995	: Ajout constante XZSP_ARRET_MARCHE V1.4
*
*  JPL		13/07/2016	: Declaration des fonctions XZSP_05MajFichierAdm et XZSP_09EcritVersionLog  1.5
*******************************************************/
#ifndef xzsp
#define xzsp

/* fichiers inclus */

#include <xdc.h>
#include <xdm.h>
#include <xdg.h>
#include <xzsc.h>
#include <xzst.h>
#include <xzsm.h>
#include <xzsx.h>
#include <xzss.h>

/* definitions de constantes exportees */

	/* Arret de MIGRAZUR sauf supervision */
#define XZSPC_ARRET	0	
	/* Demarrage de MIGRAZUR a partir supervision */
#define XZSPC_MARCHE	1	
	/* Arret total de MIGRAZUR, y compris supervision */
#define XZSPC_FIN_MIGRAZUR	2
	/* Arret tache + relance SUPERV */
#define XZSPC_ARRET_MARCHE	3	


#define XZSPC_NOM_INV	3	/* Nom machine invalide */
#define XZSPC_NOS_INV	4	/* Nom ss systeme invalide */
#define XZSPC_NOT_INV	5	/* Nom tache invalide */
#define XZSPC_ARG_INV	6	/* Arg invalide */
#define XZSPC_FIC_INV	7	/* Pb  lecture fichier */
#define XZSPC_ACTUEL	8	/* Indicateur consultation Admi actuelle */
#define XZSPC_HISTO	9	/* Indicateur consultation Admi Historique */
 
#define XZSPC_MAX_ARG	200


/* definitions de types exportes */

	/* Horodate au format JJ/MM/AAAA HH:MM:SS  */
typedef char	XZSPT_Horodate[20];       
	/* Nom d'un Fichier **/
typedef char    XZSPT_NomFichier[20];     
	/* Type de FIchier */
typedef char    XZSPT_TypeFichier[20];     


/* Structure de donnees pour le fichier des Versions Logicielles */
typedef struct { 
	XZSPT_TypeFichier	TypeFic;
	XZSCT_NomMachine 	NomMachine;
	XZSPT_NomFichier	NomFichier;
	XZSCT_NumVersion        NumVersion;
	XZSPT_Horodate		Horodate;
} XZSPT_ComposantVL;



/* declaration des donnees globales */

/* declaration des fonctions externes */

extern int 	XZSP_AppliqueFonction 	(void		(*pa_FonctionUtil)(), 
					 int		va_NumTexte, 
					 XZSPT_Horodate	va_Horodate,
					 int		va_NumEnr,
					 char*		va_Args);


/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_01CmdApplication(NomMachine, MarcheArret)
*
* Arguments
* NomMachine  :	Nom de la machine sur lequel on veut 
*		commander l'application.
* MarcheArret :	Indicateur de lancement ou d'arret
*		XZSPC_MARCHE ou XZSPC_ARRET
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine invalide.
* XZSPC_ARG_INV		Argument MarcheArret invalide.
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_01CmdApplication(XZSCT_NomMachine NomMachine,
				 int MarcheArret);


/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_02CmdSSyst(NomMachine, NomSSysteme, MarcheArret)
*
* Arguments
* NomMachine  :	Nom de la machine sur lequel on veut 
*		commander le sous-systeme.
* NomSSysteme :	Nom du sous-systeme a arreter ou 
*		demarrer.
* MarcheArret :	Indicateur de lancement ou d'arret 
*		XZSPC_MARCHE ou XZSPC_ARRET
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine invalide.
* XZSPC_NOS_INV		Nom du sous-systeme invalide.
* XZSPC_ARG_INV		Argument MarcheArret invalide.
* XZSCC_OK 		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_02CmdSSyst(XZSCT_NomMachine NomMachine, 
			   XZSCT_NomSSysteme NomSSysteme, 
			   int MarcheArret);




/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_03CmdTache(NomMachine, NomTache, MarcheArret)
*
* Arguments
* NomMachine  :	Nom de la machine sur lequel on veut 
*		commander la tache.
* NomTache    :	Nom de la tache a arreter ou demarrer.
* MarcheArret :	Indicateur de lancement ou d'arret 
*		XZSPC_MARCHE ou XZSPC_ARRET
*
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine
* XZSPC_NOT_INV		Nom de la tache invalide.
* XZSPC_ARG_INV		Argument MarcheArret invalide.
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_03CmdTache(XZSCT_NomMachine NomMachine,
			    XZSCT_NomTache NomTache, 
			    int MarcheArret);



/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_04ArretMachine(NomMachine, IndicRelance)
*
* Arguments
* NomMachine  :	Nom de la machine que l'on veut arreter.
* IndicRelance:	Indicateur de relance automatique 
* 		XZSPC_MARCHE ou XZSPC_ARRET.
*
* Codes retour
* XZSPC_NOM_INV		Nom de la machine invalide.
* XZSPC_ARG_INV		Argument IndicRelance invalide.
* XZSCC_OK		ok.
*
* Mecanismes
* RTworks : RTS2.
******************************************************/

extern int XZSP_04ArretMachine( XZSCT_NomMachine NomMachine, 
				int IndicRelance);



extern int XZSP_05MajFichierAdm( XZSCT_NomMachine );


/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_07ConsultJournalAdmin(FonctionUtil, NbEnrg)
*
* Arguments
* FonctionUtil : Pointeur sur fonction utilisateur.
* NbEnrg       : Nombre d'enregistrements du journal
*
* Codes retour
* XZSPC_FIC_INV		Probleme de lecture du journal 
*			de l'administration systeme.
* XZSCC_OK 		ok.
*
* Mecanismes
* RTworks : RTS2
******************************************************/

extern int XZSP_07ConsultJournalAdmin	(void (*FonctionUtil)(), 
					 int *NbEnrg);


/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_08ListeVersionLog(FonctionUtil, IndHisto, NbElem)
*
* Arguments
* FonctionUtil	: Pointeur sur fonction utilisateur.
* IndHisto	: XZSPC_ACTUEL ou XZSPC_HISTO
* NbElem	: Nombre d'elements de la liste.
*
* Codes retour
* XZSCC_OK 		ok.
* XZSPC_FIC_INV		Erreur lecture fichier des 
*			versions logicielles.
*
* Mecanismes
* Pointeur de fonction.
********************************************************/
extern int XZSP_08ListeVersionLog	(void (*FonctionUtil)(), 
					 int IndHisto, 
					 int *NbElem);



extern int XZSP_09EcritVersionLog( XZSPT_ComposantVL );



/*X*/
/* Fichier : $Id: xzsp.h,v 1.5 2016/07/13 22:40:05 devgfi Exp $	Release : $Revision: 1.5 $       Date : $Date: 2016/07/13 22:40:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSP 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSP_10ImprimeJournalAdmin()=
*
* Arguments
*
* Codes retour
*   XDC_OK :  ok
*
* Mecanismes
* Pointeur de fonction.
********************************************************/
extern int XZSP_10ImprimeJournalAdmin();

#endif

/* fin de fichier */

