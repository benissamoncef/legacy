/*E*/
/* Fichier : xzss.h
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSS * FICHIER xzss.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSS
*        - Declarations externes des fonctions
*        - Declarations externes des variables
*        - Declarations des constantes
*        - Declarations des types
*
*   Reference DCG-021 chapitre 3.6 
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  01 Sep 1994      : Creation
*  T.Milleville   13 Fev 1995      : Ajout Argument aux modules
*                                       d'interface XZSS_03 et XZSS_04
*                               pour preciser le type d'impression V1.5
* JPL	11/01/11 : Migration architecture HP ia64 (DEM 971) : declaration XZSS_20, XZSS_21  1.6
* JPL	19/10/21 : Ensemble de fonctions de Minuterie -Timer- XZSS_14, XZSS_15, XZSS_18; Temporisation XZSS_22 (SAE_339)  1.7
*******************************************************/

#ifndef xzss
#define xzss

/* fichiers inclus */

#include "xdc.h"
#include "xdg.h"
#include "xzse.h"
#include "xzsc.h"

#include <errno.h>
#include <termio.h>  

/* definitions de constantes exportees */

#define XZSSC_ERR_SYS		1
#define XZSSC_FIC_INV		2
#define XZSSC_DEV_INV		3
#define XZSSC_DEV_CFN		4
#define XZSSC_TYPE_IMPRIMA	5
#define XZSSC_TYPE_TRACEUR	6
#define XZSSC_ARG_INV		7	
#define XZSSC_ERRBUFSIZE	300

/* Vitesse de Transfert  sur la liaison RS232 */
#define XZSSC_RS_300		B300
#define XZSCC_RS_600		B600
#define XZSCC_RS_1200		B1200
#define XZSSC_RS_2400		B2400
#define XZSSC_RS_4800		B4800
#define XZSSC_RS_9600		B9600

#define XZSSC_TIMEOUT		10000 	/* temps d'attente en lecture */
#define XZSSC_RS_8_BITS_INFO	13

/* Flags de permission d'acces aux fichiers */
#define XZSSC_FIC_WR		O_WRONLY
#define XZSSC_FIC_RD		O_RDONLY
#define XZSSC_FIC_AP		O_APPEND
#define XZSSC_FIC_CR		O_CREAT
#define XZSSC_FIC_RDWR		O_RDWR


#define XZSSC_FIC_WR_STREAM 	"w"
#define XZSSC_FIC_RD_STREAM 	"r"
#define XZSSC_FIC_AP_STREAM 	"a"
#define XZSSC_FIC_RPLUS_STREAM 	"r+"

/* definitions de types exportes */


/* declaration des donnees globales */
#define XZSSC_PATH_ABS_SIZE		200
#define XZSSC_LINE_CMD_SIZE		150
#define XZSSC_NUM_CMD			10

	/* Sans parite,8bits,1stop bit */
#define XZSSC_8BITS_PNON_1SB		0	
	/* Sans parite,8bits,2stop bit */
#define XZSSC_8BITS_PNON_2SB		1	
	/* parite paire,8bits,1stop bit */
#define XZSSC_8BITS_PPAIRE_1SB		2	
	/* parite paire,8bits,2stop bit */
#define XZSSC_8BITS_PPAIRE_2SB		3	
	/* Parite impaire,8bits,1stop bit */
#define XZSSC_8BITS_PIMPAIRE_1SB	4	
	/* parite impaire,8bits,1stop bit */
#define XZSSC_8BITS_PIMPAIRE_2SB	5	


/*typedef enum XZSST_param
{
  XZSSC_8BITS_PPAIRE_1SB,
  XZSSC_8BITS_PNON_1SB,
  XZSSC_8BITS_PIMPAIRE_1SB,
  XZSSC_8BITS_PNON_2SB,
  XZSSC_8BITS_PPAIRE_2SB,
  XZSSC_8BITS_PIMPAIRE_2SB
} XZSSV_param;*/


/* declaration de fonctions internes */


/* declaration des fonctions externes */



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_01SauveFichierSD(NomFichier, PathFicS)
*
* Arguments
* NomFichier : Nom du fichier a sauvegarder.
* PathFicS   : Chemin d'acces du fichier a sauvegarder.
*
* Codes retour
* XZSCC_OK		ok
* XDC_ARG_INV		Nom d'argument egal a NULL.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_DEV_INV		Device inconnu (compte-rendu 
*			de XZSC_08).
* XZSSC_ERR_SYS		Erreur execution shell. 
*
* Mecanismes
* SYS1 , SYS2 , SYS3.
******************************************************/

extern int XZSS_01SauveFichierSD (char*		NomFichier,
				  char* 	PathFicS);



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_02RestaureFichierSD(NomFichier, PathFicR)
*
* Arguments
* NomFichier : Nom du fichier a restaurer.
* PathFicR   : Chemin d'acces du fichier a restaurer.
*
* Codes retour
* XZSCC_OK		ok
* XDC_ARG_INV		Nom d'argument egal a NULL.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_DEV_INV		Device inconnu (compte-rendu 
*			de XZSC_08).
* XZSSC_ERR_SYS		Erreur execution shell. 
*
* Mecanismes
******************************************************/

extern int XZSS_02RestaureFichierSD (char*		NomFichier, 
				     char* 		PathFicR);



extern int XZSS_03ImprimerFichierSec(int	TypePeriph,
				   char  	*NomFic, 
				   char 	*PathFic,
				   int		vl_TypeImpression);


/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_04ImprimerFichier(TypePeriph, NomFic, PathFic,vl_TypeImpression)
*
* Arguments
* TypePeriph 	: XZSSC_TYPE_IMPRIMA ou 
*		  XZSSC_TYPE_TRACEUR
* NomFic	: Nom du fichier a imprimer.
* PathFic	: Chemin d'acces du fichier.
*
* Codes retour
* XZSCC_OK		ok
* XDC_ARG_INV		Argument TypePeriph incorrect.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_DEV_INV		Device inconnu (compte-rendu 
*			de XZSC_11 et XZSC_12).
* XZSSC_ERR_SYS		Erreur execution shell. 
*
* Mecanismes
* SYS1 , SYS3.
******************************************************/
extern int XZSS_04ImprimerFichier(int 		TypePeriph,
				   char  	*NomFic, 
				   char 	*PathFic,
				   int		vl_TypeImpression);


/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_05TraitSignaux()
*
* Arguments
* 
* Codes retour
* XZSCC_OK	ok
*
* Mecanismes
******************************************************/

extern int XZSS_05TraitSignaux();




/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_06DetruireFichier(NomFichier)
*
* Arguments
* NomFichier      : Nom du fichier a detruire.
*
* Codes retour
* XZSCC_OK		ok
* XDC_ARG_INV		Argument incorrect.
* XZSSC_FIC_INV		Fichier inconnu.
*
* Mecanismes
* SYS1.
******************************************************/

extern int XZSS_06DetruireFichier(char *NomFichier);



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_07TransfertFichier(NomMachineOrig, NomFicOrig, 
* PathOrig, NomMachineDest, NomFicDest, PathDest)
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
* Codes retour
* XZSCC_OK		ok
* XDC_ARG_INV		Argument incorrect.
* XZSSC_FIC_INV		Fichier inconnu.
* XZSSC_ERR_SYS		Erreur execution shell.
*
* Mecanismes
* SYS1 , SYS3.
*******************************************************/

extern int XZSS_07TransfertFichier(XZSCT_NomMachine NomMachineOrig,
				   char *NomFicOrig,
				   char *PathOrig,
				   XZSCT_NomMachine NomMachineDest,
				   char *NomFicDest,
				   char *PathDest);



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_08OuvertureRS232(Device, Vitesse, NbitInfo, 
* NbitStop, Parite,Fd)
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
* Codes retour
* XZSCC_OK 		ok
* XDC_ARG_INV		Argument incorrect.
* XZSSC_DEV_INV		Ouverture du device incorrecte.
* XZSSC_DEV_CFN		Configuration du device 
*			incorrecte.
*
* Mecanismes
* SYS7.
******************************************************/
extern int XZSS_08OuvertureRS232 (char*		Device,
				  unsigned int 	Vitesse,
				  int 		FlagParam,
				  int 		*Fd);





/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_09FermetureRS232(Fd)
*
* Arguments
* Fd			: Descripteur du fichier.
*
* Fonction
* Utilisation de la fonction "close".
*
* Codes retour
* XZSCC_OK		ok
* XZSSC_DEV_INV		Device invalide.
*
* Mecanismes
*******************************************************/

extern int XZSS_09FermetureRS232 (int 		Fd);



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_10LectureRS232(Fd, NbCar, Buffer)
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
* XZSCC_OK		ok
* XZSSC_DEV_INV		Probleme lecture.
*
* Mecanismes
*******************************************************/

extern int XZSS_10LectureRS232 (int 	Fd, 
				char 	*Buffer,
				int 	Size);





/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_11EcritureRS232(Fd, NbCar, Buffer)
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
* XZSCC_OK		ok
* XZSSC_DEV_INV		Probleme d'ecriture.
*
* Mecanismes
*******************************************************/

extern int XZSS_11EcritureRS232 (int 	Fd, 
				 char*	Buffer,
				 int 	Size);




/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_12OuvrirFichier(FlagO, NomFichier,Fd)
* 
* Arguments
* FlagO		: XZSSC_FIC_RD ou XZSSC_FIC_WR ou
*		  XZSSC_FIC_CR ou XZSSC_FIC_AP ou 
*		  XZSSC_FIC_RDWR.
* NomFichier	: Nom du fichier complet avec chemin
*		  d'acces a ouvrir.
* Fd		: Descripteur du fichier.
*
* Codes retour
* XZSCC_OK		ok
* XZSSC_FIC_INV		Probleme d'ouverture selon le 
*			compte_rendu de la fonction 
*			fopen.
*
* Mecanismes
*******************************************************/

extern int XZSS_12OuvrirFichier (int 	FlagO, 
				 char 	*NomFichier, 
				 int 	*Fd);



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_13FermerFichier(Fd)
* 
* Arguments
* Fd		: Descripteur du fichier.
* 
* Fonction
* Utilisation de la fonction "fclose".
* 
* Codes retour
* XZSCC_OK 		ok
* XZSSC_FIC_INV		Probleme de fermeture selon le 
*			compte_rendu de la fonction 
*			fclose.
* Mecanismes
*******************************************************/

extern int XZSS_13FermerFichier (int Fd);


/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSSYSTE 
******************************************************
* ALGORITHME :
*
* Arguments
* NbSeconde		nombre de secondes
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
extern int XZSS_14ArmerTimeout (int);


/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSSYSTE 
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
********************************************************/

extern int XZSS_15ConsulterTimeout ();


/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_16OuvrirFichierStream(FlagO, NomFichier,Fd)
* 
* Arguments
* FlagO		: XZSSC_FIC_RD ou XZSSC_FIC_WR ou
*		  XZSSC_FIC_CR ou XZSSC_FIC_AP ou 
*		  XZSSC_FIC_RDWR.
* NomFichier	: Nom du fichier complet avec chemin
*		  d'acces a ouvrir.
* Fd		: Descripteur du fichier.
*
* Codes retour
* XZSCC_OK		ok
* XZSSC_FIC_INV		Probleme d'ouverture selon le 
*			compte_rendu de la fonction 
*			fopen.
*
* Mecanismes
*******************************************************/

extern int XZSS_16OuvrirFichierStream (char * 	FlagO, 
				 char 	*NomFichier, 
				 FILE 	**Fd);



/*
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_17FermerFichierStream(Fd)
* 
* Arguments
* Fd		: Descripteur du fichier.
* 
* Fonction
* Utilisation de la fonction "fclose".
* 
* Codes retour
* XZSCC_OK 		ok
* XZSSC_FIC_INV		Probleme de fermeture selon le 
*			compte_rendu de la fonction 
*			fclose.
* Mecanismes
*******************************************************/

extern int XZSS_17FermerFichierStream (FILE * Fd);



/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_18AnnulerTimeout()
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

extern int XZSS_18AnnulerTimeout ();



/*
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSS
******************************************************
* DESCRIPTION DE LA FONCTION :
*
*	A partir du nom d'une tache, retrouve son PID
*
******************************************************
* SEQUENCE D'APPEL :
* XZSS_20PidTache (pa_nomTache)
*
* Arguments
* pa_nomTache	: Nom de la tache dont on cherche le PID.
* 
* Fonction
*
* 
* Valeur de retour
* PID de la tache 	si elle existe
* 0			sinon
*
* Mecanismes
*******************************************************/

extern int XZSS_20PidTache (XZSCT_NomTache pa_nomTache);



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
* XZSS_21LectureRepertoire (pa_nomRepert, pa_listeFic)
*
* Arguments
* pa_nomRepert	: Nom du repertoire a lister
* pa_listeFic	: pointeur vers la liste de ses fichiers
* 
* Fonction
*
* 
* Valeur de retour
* XDC_NOK si la liste des fichiers n'a pu etre obtenue
* XDC_OK sinon
*
* Mecanismes
*******************************************************/

extern int XZSS_21LectureRepertoire (char *pa_nomRepert,
				char	***pa_listeFic);



/*****************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSS
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZSS_22Attendre(int)
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

extern int XZSS_22Attendre (int);

#endif

/* fin de fichier */
