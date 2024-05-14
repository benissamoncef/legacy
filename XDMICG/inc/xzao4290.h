/*E*/
/* Fichier : $Id: xzao4290.h,v 1.1 2011/02/15 11:00:22 gesconf Exp $     Release : $Revision: 1.1 $        Date : $Date: 2011/02/15 11:00:22 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao4290.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* JPL	14/02/11 : Creation d'apres xzao429.h (DEM 953)  1.1
------------------------------------------------------ */

#ifndef xzao4290
#define xzao4290

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO4290_RPC_NAME "XZAO4290"

#define XZAOC_XZAO4290_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO4290_Ajouter_Zon_PMVA (   XDY_Basedd              va_Basedd_in,
                                char           		*va_autoroute_pmv_in,
                                XDY_Sens                va_sens_pmv_in,
                                XDY_PR                  va_pr_pmv_in,
                                char               	*va_site_pmv_in,
                                XDY_Entier              va_distance_pmv_in,
                                char           		*va_autoroute_zone_in,
                                XDY_Sens                va_sens_zone_in,
                                XDY_PR                  va_debut_zone_in,
                                XDY_PR                  va_fin_zone_in,
                                char                    *va_direction_zone_in,
                                XDY_Sens		va_type_zone_in,
                                XDY_Booleen             va_inhibition_zone_in,
                                XDY_ResConf             *va_Resultat_out );



#endif
