/*E*/
/*  Fichier : 	$Id: xzae22.h,v 1.4 1994/11/17 11:37:30 volcic Exp $      Release : $Revision: 1.4 $        Date : $Date: 1994/11/17 11:37:30 $
------------------------------------------------------
* GTIE  *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME XDMICG
------------------------------------------------------
* MODULE xzao * FICHIER xzae22.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module d'interface des evenements >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	08 Sep 1994	: Creation
* volcic	10 Oct 1994	: Modification (ajout include xdc.h, xdy.h) (v 1.2)
------------------------------------------------------*/

#ifndef xzae22
#define xzae22

/* fichiers inclus */

#include "ctpublic.h"
#include "cstypes.h"
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzst.h"
#include "xzsc.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAEC_XZAE22_RPC_NAME "XZAE;41"

#define XZAEC_XZAE22_NB_PARM_RETURN 13

/* definitions de types exportes */

typedef struct 
{
   XDY_Evt	Evt;
   XDY_Octet	NbPassagers;
   XDY_Booleen	Bebe;
   XDY_Booleen	PersAgees;
   XDY_Booleen	Animaux;
   XDY_Booleen	VehEscota;
   XDY_Octet	VolCharge;
   XDY_MatDang	MatDang;
   XDY_Entier	Depannage;
   XDY_Octet	SiteDepannage;
   char		Commentaire[101];
   XDY_Entier	NumVehicule;
   
} XZAET_AccVeh;

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAE22_Decrire_Accident_PA (	XDY_Horodate,
					XDY_Evt,
					XDY_FonctionInt,
					XDY_FonctionInt,
					XZAET_Accident *,
					XZAET_ComplAcc * );

											
						
extern tg_Connexion  *XZAGV_Connexion;

extern tg_Context    *XZAGV_Context;



#endif
