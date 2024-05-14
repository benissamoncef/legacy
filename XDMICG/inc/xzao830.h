/*E*/
/* Fichier : $Id: xzao830.h,v 1.1 2017/02/10 09:04:04 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2017/02/10 09:04:04 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao830.h
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

#ifndef xzao830
#define xzao830

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO830_RPC_NAME "XZAO830"

#define XZAOC_XZAO830_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO830_Ajouter_Zon_PMVA_SAGA (   XDY_Basedd              va_Basedd_in,
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
