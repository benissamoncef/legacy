/*E*/
/* Fichier : $Id: scon_ser.h,v 1.6 1994/12/14 09:04:05 milleville Exp $	Release : $Revision: 1.6 $        Date : $Date: 1994/12/14 09:04:05 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE scon_service * FICHIER scon_service.h
******************************************************
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
******************************************************
* HISTORIQUE :
*
* T.Milleville	23 Nov 94 	: Creation
******************************************************/

#ifndef scon_service
#define scon_service

/* fichiers inclus */

#include "xdc.h"
#include "xzsc.h"
#include "xzsa.h"
#include "xzsx.h"

#include <rtworks/common.h>
#include <rtworks/ipc.h>


#include <X11/Xatom.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#include <Mrm/MrmAppl.h>
#include <Mrm/MrmPublic.h>
#include <Xm/Xm.h>
#include <Xm/CascadeB.h>
#include <Xm/DialogS.h>
#include <Xm/Form.h>
#include <Xm/Label.h>
#include <Xm/PushB.h>
#include <Xm/RowColumn.h>
#include <Xm/Separator.h>
#include <Xm/TextF.h>
#include <Xm/Text.h>
#include <Xm/ToggleBG.h>
#include <stdio.h>


/* definitions de constantes exportees */

#define CG_CRESIT 0  /* creation de site */
#define CG_CRESSY 1  /* creation de sous-systeme */ 
#define CG_CRECAL 2  /* creation de calculateur */
#define CG_CREENT 3  /* creation d'entite */
#define CG_CRETAS 4  /* creation de tache */ 
#define CG_DESSIT 5  /* destruction de site */
#define CG_DESSSY 6  /* destruction de sous-systeme */
#define CG_DESCAL 7  /* destruction de calculateur */ 
#define CG_DESENT 8  /* destruction d'entite */
#define CG_DESTAS 9  /* destruction de tache */  

#define CG_CRE_TAC 10
#define CG_DES_TAC 11
#define CG_CRE_MAC 12
#define CG_DES_MAC 13
#define CG_CO_MAC 14

#define CG_VIDE    0 
#define CG_NONVIDE 1 
#define CG_SITE 2 

#define CG_CHAINE_DEFAUT "?" 
#define CG_CHAR_DEFAUT '?' 


/*  Declaration de la Liste des RTLicenses par DEFAUT *****/
#define CG_RTLICENSE_DEFAUT	"SDCI1"


/* definitions de types exportes */

typedef char TG_MessageType[50];

/* definition de macro exportees */

/* declarations de donnees globales */

MrmHierarchy vg_mrm_hierarchy;
MrmCode vg_class;

/* delaration de fonctions externes */

extern void EcritChampsCMA( char * );
extern void EcritChampsCOMA( char *, char * );
extern void EcritChampsMAC( XZSXT_Machine, int );
extern void EcritChampsCTM( char *, char *);
extern void EcritChampsTAC( XZSXT_Tache , int );
extern void EcritChampsMSG( XZSXT_ActionMessage );
extern void EcritChampsSIT( char * );
extern void EcritChampsSSY( char *);
extern void EcritChampsCAL( char * );
extern void EcritChampsCJA( char * );
extern void EcritChampsENT( char *, char *, char *, char * );
extern void EcritChampsTAS( char *, char * );



extern int LitChampsCMA( XZSCT_NomMachine );
extern int LitChampsMAC( XZSXT_Machine * ); 
extern int LitChampsCTM( XZSCT_NomMachine, XZSCT_NomTache );
extern int LitChampsTAC( XZSXT_Tache * );
extern int LitChampsCJA( long * );
extern int LitChampsMSG( XZSXT_ActionMessage * );
extern int LitChampsSIT( XZSCT_NomSite );
extern int LitChampsSSY( XZSCT_NomSSysteme, int * );
extern int LitChampsCAL( XZSCT_NomMachine );
extern int LitChampsENT( int * , XZSCT_NomSite, XZSCT_NomEntite );
extern int LitChampsTAS( XZSCT_NomSSysteme, int *, XZSCT_NomTache , int *);

extern void TACH_ARM_CB( Widget, XtPointer, XtPointer );
extern void PERI_ARM_CB( Widget, XtPointer, XtPointer );
extern void AUTR_ARM_CB( Widget, XtPointer, XtPointer );

extern void DemandeConf1 ( TG_MessageType );
extern void DemandeConf2 ( TG_MessageType );
extern void AfficheErreur ( TG_MessageType );
#endif
