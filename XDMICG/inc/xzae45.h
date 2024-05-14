/*E*/
/*Fichier : $Id: xzae45.h,v 1.1 2008/11/17 10:51:42 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2008/11/17 10:51:42 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE45 * FICHIER XZAE45.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL	29 Aou 2008	: Creation (dem/835) 1.0
------------------------------------------------------ */

#ifndef XZAE45
#define XZAE45

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
#define XZAE45C_RPC_NAME "XZAE;45"

#define XZAE45C_NB_PARM_RETURN 38


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE45_Lire_Fiche_Vehicule : recupere les infos 
*  d'une fmc vehicule
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE45_Lire_Fiche_Vehicule ( XDY_Entier    va_numero_in ,
                                        XDY_Octet     va_cle_in,
                                        XDY_Octet    *va_Type_out,
                                        XDY_Octet    *va_passagers_out,
                                        XDY_Booleen  *va_bebe_out,
                                        XDY_Booleen  *va_persAgee_out,
                                        XDY_Booleen  *va_animaux_out,
                                        XDY_Booleen  *va_type_veh_escota_out,
                                        XDY_Octet    *va_chargement_out,
                                        char         *va_codemat_out,
                                        XDY_Entier   *va_depannage_out,
                                        XDY_Octet    *va_site_depannage_out,
                                        char         *va_texte_out,
                                        XDY_Mot      *va_numveh_out,
                                        char         *va_marque_out,
                                        char         *va_type_out,
                                        char         *va_couleur_out,
                                        char         *va_immatriculation_out,
                                        XDY_Mot      *va_code_pays_out,
                                        char         *va_pays_out,
                                        char         *va_nom_out,
                                        char         *va_ville_out,
                                        char         *va_adresse_out,
                                        char         *va_code_danger_out,
                                        XDY_Mot      *va_code_nature_panne_out,
                                        char         *va_nature_panne_out,
                                        char         *va_positionnement_out,
                                        XDY_Octet    *va_vehicule_roulant_out,
                                        char         *va_enlev_dem_par_out,
                                        XDY_Entier   *va_numero_fiche_appel_out,
                                        XDY_Octet    *va_pres_mat_dangereuse_out,
                                        XDY_Octet    *va_fax_envoye_out,
                                        XDY_Mot      *va_code_marque_out,
                                        XDY_Mot      *va_code_couleur_out,
                                        XDY_Mot      *va_code_enlev_dem_par_out,
                                        XDY_Octet    *va_site_creation_out,
                                        XDY_Mot      *va_nb_bebes_out,
                                        XDY_Mot      *va_nb_agees_out);


/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Eqt		va_numero_in
* XDY_Sens		va_cle_in
*				
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*

* FONCTION :
*
*
*
* MECANISMES :
*
------------------------------------------------------*/
#endif
