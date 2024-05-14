/*E*/
/*Fichier : $Id: xzac15.h,v 1.1 2012/07/04 10:58:14 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 10:58:14 $
------------------------------------------------------
* STERIA * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE * FICHIER xzac15.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
*  ABE	01/01/22	: Creation SAE-DEM367
------------------------------------------------------ */

#ifndef xzac15
#define xzac15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

/* Include Sybase */
#include <ctpublic.h>
#include <cstypes.h>

/* Include applicatif Migrazur */
#include "xdy.h"
#include "xdc.h"
#include "xzae.h"
#include "xzag.h"
#include "asql_com.h"

/* definitions de constantes exportees */

#define XZAC15C_RPC_NAME "XZAC;15"

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAC15_Creer_Modifier_Action ( XDY_Entier		va_NumEvt_in,
                            XDY_Octet		va_CleEvt_in,
                            XDY_Entier		va_NumAction_in,
                            XDY_Octet		va_Site_in,
                            XDY_Nom		va_NomDepanneur_in,
                            XDY_Nom		va_Resultat_in,
                            XDY_Horodate		va_HoroAppel_in,
                            XDY_Horodate		va_HoroArrivee_in,
                            XDY_Operateur		va_Operateur_in,
                            XDY_Horodate		va_HoroDepart_in,
                            XDY_Horodate		va_HoroAnnul_in,
                            XDY_Octet             va_renfort_in,
                            XDY_Octet             va_patrouille_presente_in,
                            XDY_Octet             va_garagiste_contact_pc_in,
                            XDY_Octet             va_annul_intervention_in,
                            XDY_Entier            va_code_type_astreinte_in,
                            XDY_Entier            va_interlocuteur_in,
                            XDY_Octet		va_site_responsable_in);

#endif
