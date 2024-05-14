/*E*/
/*Fichier : $Id: xzst.h,v 1.11 1994/11/22 14:49:52 milleville Exp $      Release : $Revision: 1.11 $        Date : $Date: 1994/11/22 14:49:52 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZSTRACE * FICHIER xzst.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   Description du module d'interface XZSTRACE
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
*  T.Milleville  30 Aou 1994      : Creation
*  I.Thomas	17 Nov 1994	: Ajout XZSTC_REGLE
*******************************************************/
#ifndef xzst
#define xzst

/* fichiers inclus */

#include <errno.h>
#include <stdlib.h>

#include <xdc.h>
#include <xdg.h>
#include <xdm.h>
#include <xzsc.h> 


/* definitions de constantes exportees */

/* Niveaux de trace */

#define XZSTC_DEBUG1	0x00000001
#define XZSTC_DEBUG2	0x00000002
#define XZSTC_DEBUG3	0x00000004
#define XZSTC_FONCTION	0x00000008
#define XZSTC_INFO	0x00000010
#define XZSTC_MESSAGE	0x00000020
#define XZSTC_SGBD	0x00000040
#define XZSTC_WARNING   0x00000080
#define XZSTC_FATAL	0x00000100
#define XZSTC_INTERFACE	0x00000200
#define XZSTC_REGLE	0x00000400
#define XZSTC_TOUT	0xffffffff
#define XZSTC_RIEN	0x00000000

/* Special VisuTrace */
#define XZSTC_NECRAN	10
#define XZSTC_ECRAN	11


/* Divers */
#define XZSTC_DIFF_NULL	12
#define XZSTC_DIFF_TOUT	13
#define XZSTC_MAXARGS	14

/* Retours d'erreur */
#define XZSTC_ECR_INV	15
#define XZSTC_FERM_INV 	16
#define XZSTC_IDF_INV	17
#define XZSTC_INI_INV	18
#define XZSTC_INS_INV	19
#define XZSTC_NOM_INV	20
#define XZSTC_NOT_INV	21
#define XZSTC_ARG_INV	22
#define XZSTC_FIC_EXIST 0

/* Nom de la Variable d'environnement pour definir 
*	le repertoire des fichiers de traces */
#define XZSTC_REP_TRACE		"MIGRAZUR_REP_TRACE"


/* definitions de types exportes */

typedef unsigned int XZSTT_NiveauTrace;

/* declaration des donnees globales */
extern  XZSTT_NiveauTrace        XZSTV_NiveauTrace;
extern  int                     XZSTV_VisuTrace;
extern  XZSTT_NiveauTrace        XZSTV_NiveauTrace;
extern  int                     XZSTV_VisuTrace;


/* declaration des fonctions internes */
int		actualiserNivTrace ();


/* declaration des fonctions externes */



/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_01OvertureTrace()
*
* Arguments
*
* Codes retour
* XZSTC_ARG_INV         Argument invalide.
* XZSTC_IDF_INV         Probleme ouverture fichier.
* XZSTC_INS_INV         Erreur initialisation noyau 
* 			supervision.
* XDC_OK              ok.
*
* Mecanismes
* Fonction systeme UNIX fopen.
******************************************************/

extern int XZST_01OuvertureTrace ();




/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_02FermetureTrace()
*
* Arguments
*
* Codes retour
* XZSTC_INI_INV		Ouverture non effectuee.
* XDC_OK		ok.
*
* Mecanismes
* Fonction systeme UNIX fclose.
******************************************************/

extern int XZST_02FermetureTrace ();




/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_03EcritureTrace(NivTrace, Texte, P0, P1, P2, P3,
*		       P4, P5, P6, P7, P8, P9)
*
* Arguments
* NivTrace	: Niveau de trace affecte a l'ecriture 
*		  de cette trace.
* Texte		: Libelle et format du texte a ecrire.
* P0 a  P9	: Arguments optionnels a imprimer dans
*		  la trace. Le nombre d'arguments doit
*		  correspondre au format.
*
*
* Codes retour
* XZSTC_INI_INV		Ouverture non effectuee.
* XZSTC_ECR_INV		Probleme ecriture fichier.
* XDC_OK		ok.
*
* Mecanismes
* Fonctions systeme UNIX fprintf, fseek, et strlen.
******************************************************/

extern int XZST_03EcritureTrace (XZSTT_NiveauTrace NivTrace, 
				 char *Texte, 
				 ... );


/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_03EcritureTrace(Texte, P0, P1, P2, P3,
*		       P4, P5, P6, P7, P8, P9)
*
* Arguments
* Texte		: Libelle et format du texte a ecrire.
* P0 a  P9	: Arguments optionnels a imprimer dans
*		  la trace. Le nombre d'arguments doit
*		  correspondre au format.
*
*
* Codes retour
* XZSTC_INI_INV		Ouverture non effectuee.
* XZSTC_ECR_INV		Probleme ecriture fichier.
* XDC_OK		ok.
*
* Mecanismes
* Fonctions systeme UNIX fprintf, fseek, et strlen.
******************************************************/

extern int XZST_04TraceMess ( char *Texte, 
				 ... );


/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_06CmdNivVisuTrace(NomMachine, NomTache, 
*			 NivTrace, VisuTrace)
*
* Arguments
* NomMachine	: Nom de la machine ou se trouve la 
* 		  tache precisee.
* NomTache	: Nom de la tache dont les traces sont
* 		  a commander.
* NivTrace      : Niveau de trace a activer.
* VisuTrace	: XZSTC_ECRAN ou XZSTC_NECRAN.
*
* Codes retour
* XZSTC_ARG_INV           Argument VisuTrace invalide.
* XZSTC_NOM_INV           Nom machine invalide.
* XZSTC_NOT_INV           Nom tache invalide.
* XDC_OK                ok.
*
* Mecanismes
* RTworks : RTS2
******************************************************/

extern int XZST_06CmdNivVisuTrace (XZSCT_NomMachine NomMachine, 
				   XZSCT_NomTache NomTache, 
				   XZSTT_NiveauTrace NivTrace,
				   int VisuTrace);



/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_07AutoriserNivVisuTrace()
*
* Arguments
*
* Codes retour
* XDC_OK           ok.
*
* Mecanismes
* RTworks : RTS3
******************************************************/

extern int XZST_07AutoriserCmdNivVisuTrace ();




/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_08ForceNivVisuTraceLocal(NivTrace, VisuTrace)
*
* Arguments
* NivTrace  :	Niveau de trace \140 activer.
* VisuTrace :	XZSTC_ECRAN ou XZSTC_NECRAN.
*
* Codes retour
* XDC_OK           ok.
*
* Mecanismes
* RTworks : RTS2
******************************************************/

extern int XZST_08ForceNivVisuTraceLocal(XZSTT_NiveauTrace NivTrace, 
					 int VisuTrace);



/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_09InitSynoptiqueTrace(NomMachine)
*
* Arguments
* NomMachine :	Nom de la machine caracterisant le 
*		synoptique de gestion des traces.
*
* Codes retour
* XZSTC_NOM_INV		Nom machine invalide
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS2
* ******************************************************/

extern int XZST_09InitSynoptiqueTrace (XZSCT_NomMachine NomMachine);




/* Fichier : @(#)xzst.h	2.1      Release : 2.1        Date : 31 May 1994
******************************************************
* STERIA *  PROJET MIGRAZUR 
******************************************************
* MODULE XZSTRACE 
******************************************************
* DESCRIPTION DE LA FONCTION :
*
* 
******************************************************
* SEQUENCE D'APPEL :
* XZST_10ArchiverMsgSyst(NumTexte, P0, P1, P2, P3, P4, 
*			 P5, P6, P7,P8, P9)
*
* Arguments
* NumTexte :	Numero du libelle et format du texte a
* 		archiver.
* P0 a P9  :	Arguments optionnels a imprimer dans la 
*		trace. Le nombre d'arguments doit 
*		correspondre au format.
*
* Codes retour
* XZSTC_INI_INV		Initialisation non effectuee
* XDC_OK		ok.
*
* Mecanismes
* RTworks : RTS2
* ******************************************************/

extern int XZST_10ArchiverMsgSyst(int NumTexte, ... );

#endif

/* fin de fichier */



