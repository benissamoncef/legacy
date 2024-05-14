/*E Fichier : $Id: isbi_sra.h,v 1.3 2017/03/31 18:14:51 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2017/03/31 18:14:51 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME GESIHM
------------------------------------------------------
* MODULE MSRA * FICHIER isyn_sra.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
*  Creation et Mise a jour des Variables d'animations 
*   graphiques
*
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	12 Sep 1994	: Creation
* JMG   20/03/17        saga DEM1191
------------------------------------------------------ */

#ifndef MSRA
#define MSRA

/* fichiers inclus */
#include "math.h"        

#include "xzahc.h"
#include "xzst.h"
#include "xzsc.h"
#include "xzea.h"
#include "xzea180.h"
#include "xzea71.h"
#include "xdy.h"


/* definitions de constantes exportees */

#define CM_RAF_TRONCON  0
#define CM_RAF_DISTRICT 1
#define CM_RAF_GENERALE 2

#define CM_BOUCHON 0
#define CM_NEUTRA  1
#define CM_INVERSE 2
#define CM_BASCBOU 3
#define CM_BOUCHONVOIE 4

#define CM_INVERSER_MAP  2
#define CM_MAP_OK        3

#define CM_LONG_CLEF	120
#define CM_LONG_NOMMOB	80

#define CM_COEF_TRANS 1 /*10000*/


/* definitions de types exportes */
typedef struct
   {
   char clef[CM_LONG_CLEF+1];
   float localisation1;
   float localisation2;
   float niveau;
   char ident_mob[CM_LONG_NOMMOB+1];
   XDY_Booleen marquer;
   } tm_mob;

typedef struct
   {
   XDY_Eqt Numero;
   XDY_Sens Sens;
   XDY_Booleen SurDistrict;
   } tm_radtanim;



/* definition de macro exportees */

/* declarations de donnees globales */
extern XDY_Booleen	vm_traitement_bloc;
extern tm_radtanim	*pm_radtanim;

/* declarations de donnees externes */


extern XZSCT_NomMachine vm_NomMachine;
extern XZSCT_NomSite    vm_NomSite;
extern int	    	vm_TypeMachine;


/* delaration de fonctions externes */

#endif
