/*E*/
/* Fichier : $Id: xzao435.h,v 1.2 2016/07/19 18:44:44 devgfi Exp $     Release : $Revision: 1.2 $        Date : $Date: 2016/07/19 18:44:44 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao435.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	16/06/05	creation
------------------------------------------------------ */

#ifndef xzao435
#define xzao435

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO435_RPC_NAME "XZAO455"

#define XZAOC_XZAO435_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO435_Ajouter_Zon_BAF (   XDY_Basedd              va_Basedd_in,
                                char           		*va_autoroute_pmv_in,
                                XDY_Sens                va_sens_pmv_in,
                                XDY_PR                  va_pr_pmv_in,
                                char               	*va_site_pmv_in,
                                char           		*va_autoroute_zone_in,
                                XDY_Sens                va_sens_zone_in,
                                XDY_PR                  va_debut_zone_in,
                                XDY_PR                  va_fin_zone_in,
                                XDY_Booleen             va_inhibition_zone_in,
                                XDY_ResConf             *va_Resultat_out );



#endif
