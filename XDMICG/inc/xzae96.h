/*E*/
/*Fichier : $Id: xzae96.h,v 1.2 2017/03/31 17:23:45 pc2dpdy Exp $     Release : $Revision: 1.2 $        Date : $Date: 2017/03/31 17:23:45 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE96 * FICHIER XZAE96.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG   08/02/16 creation SAGA 1.0
------------------------------------------------------ */

#ifndef XZAE96
#define XZAE96

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
#define XZAE96C_RPC_NAME "XZAE96"

#define XZAE96C_NB_PARM_RETURN 1


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE96_Ecrire_Fiche_Vehicule (       XDY_Entier      va_numero_in ,
                                        XDY_Octet       va_cle_in,
                                        XDY_Octet       va_Type_in,
                                        XDY_Octet       va_Passagers_in,
										XDY_Booleen		va_Bebe_in,
										XDY_Booleen		va_PersAgee_in,
                                        XDY_Booleen     va_Animaux_in,
                                        XDY_Booleen     va_VehEscota_in,
                                        XDY_Octet       va_Chargement_in,
                                        char            *va_CodeMat_in,
										XDY_Entier		va_Depannage_in,
										XDY_District	va_SiteDepannage_in,
                                        char            *va_Texte_in,
                                        XDY_Mot         va_NumeroVehicule_in,
										XDY_Eqt			va_Poste_operateur_in,
										XDY_Texte       va_marque_in,
										XDY_Texte		va_type_in,
										XDY_Texte		va_couleur_in,
										XDY_Texte		va_immatriculation_in,
										XDY_Mot			va_code_pays_in,
										XDY_Texte		va_pays_in,
										XDY_Texte		va_nom_in,
										XDY_Texte		va_ville_in,
										XDY_Texte		va_adresse_in,
										XDY_Texte		va_code_danger_in,
										XDY_Mot			va_code_nature_panne_in,
										XDY_Texte		va_nature_panne_in,
										XDY_Texte		va_positionnement_in,
										XDY_Octet		va_vehicule_roulant_in,
										XDY_Texte		va_enlev_dem_par_in,
										XDY_Entier		va_numero_fiche_appel_in,
										XDY_Octet		va_pres_mat_dangereuse_in,
										XDY_Octet		va_fax_envoye_in,
										XDY_Mot			va_code_marque_in,
										XDY_Mot			va_code_couleur_in,
										XDY_Mot			va_code_enlev_demande_par_in,
										XDY_Entier		va_sit_in,
										XDY_Octet		va_site_creation_in,
                                        XDY_Mot         va_nb_bebes_in,
                                        XDY_Mot         va_nb_agees_in,										
										XDY_Mot			va_flag_association_in,
										XDY_Texte		va_id_ava_in,
										XDY_Entier		va_dangerosite_in);


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
