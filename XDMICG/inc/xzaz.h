/*E*/
/*  Fichier : 	$Id: xzaz.h,v 1.5 1995/03/07 17:29:36 gaborit Exp $      Release : $Revision: 1.5 $        Date : $Date: 1995/03/07 17:29:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE XZAZ * FICHIER XZAZ.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* reception de travaux de satir
*
------------------------------------------------------
* HISTORIQUE :
*
* gaborit	15 jan 1995	: Creation
* gaborit	17 jan 1995	: Modif structure (V1.2)
* gaborit	17 jan 1995	: Ajout prototype (V1.3)
* C.T.		18 jan 1995	: Ajout fichier de constantes (V1.4)
* C.T.		06 Mar 1995	: Modif interface de la fonction et
*                                 la structure XZAZT_Travaux (V 1.5)
------------------------------------------------------ */

#ifndef XZAZ
#define XZAZ

/* fichiers inclus */
#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */
#include "xzazc.h"

/* definitions de types exportes */

typedef struct {

	XDY_Entier	Numero;
	XDY_Octet	Cle;
	XDY_Horodate	DebutPrevu;
	XDY_Horodate	Debut;
	XDY_Horodate	FinPrevue;
	XDY_Autoroute	Autoroute;
	XDY_PR		PR;
	XDY_Sens	Sens;
	XDY_Voie	VR;
	XDY_Voie	VM2;
	XDY_Voie	VM1;
	XDY_Voie	VL;
	XDY_Voie	BAU;
	XDY_Voie	VR_I;
	XDY_Voie	VM2_I;
	XDY_Voie	VM1_I;
	XDY_Voie	VL_I;
	XDY_Voie	BAU_I;
	XDY_Commentaire	Texte;
	XDY_Booleen	Nature;
	XDY_Octet	Type;
	XDY_Booleen	Continu;
	XDY_Horodate	HoroDebC;
	XDY_Horodate	HoroFinC;
	XDY_Booleen	Mobile;
	XDY_PR		PRqueueM;
	XDY_PR		PRteteM;
	XDY_Sens	SensM;
	XDY_PR		PRtete;
	XDY_Booleen	VoiesRet;	
} XZAZT_Travaux;		

extern int XZAZ01_Ecrire_Travaux_Satir( XZAZT_Travaux, XDY_Evt *);

#endif
