/*E*/
/* Fichier : $Id: xzao429.h,v 1.1 2007/03/26 10:29:53 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2007/03/26 10:29:53 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao429.h
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

#ifndef xzao429
#define xzao429

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO429_RPC_NAME "XZAO4290"

#define XZAOC_XZAO429_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO429_Ajouter_Zon_PMVA (   XDY_Basedd              va_Basedd_in,
                                char           		*va_autoroute_pmv_in,
                                XDY_Sens                va_sens_pmv_in,
                                XDY_PR                  va_pr_pmv_in,
                                char               	*va_site_pmv_in,
                                char           		*va_autoroute_zone_in,
                                XDY_Sens                va_sens_zone_in,
                                XDY_PR                  va_debut_zone_in,
                                XDY_PR                  va_fin_zone_in,
                                char                    *va_direction_zone_in,
                                XDY_Sens		va_type_zone_in,
                                XDY_Booleen             va_inhibition_zone_in,
                                XDY_ResConf             *va_Resultat_out );



#endif
