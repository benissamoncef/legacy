/*E*/
/*  Fichier : $Id: xzao816.h,v 1.1 2017/02/10 09:04:22 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2017/02/10 09:04:22 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao816.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* Guilhou	02/01/98	creation (dem/1534)
* Guilhou	20/09/00	modif interface
------------------------------------------------------ */

#ifndef xzao816
#define xzao816

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO816_RPC_NAME "XZAO816"

#define XZAOC_XZAO816_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO816_Ajouter_Zon_PMV_SAGA (   XDY_Basedd              va_Basedd_in,
                                char           		*va_autoroute_pmv_in,
                                XDY_Sens                va_sens_pmv_in,
                                XDY_PR                  va_pr_pmv_in,
                                char               	*va_site_pmv_in,
                                char           		*va_autoroute_zone_in,
                                XDY_Sens                va_sens_zone_in,
                                XDY_PR                  va_debut_zone_in,
                                XDY_PR                  va_fin_zone_in,
                                char                    *va_direction_zone_in,
                                XDY_Booleen             va_inhibition_zone_in,
				XDY_PR			va_pr_1_pmv_amont_in,
				XDY_Sens		va_sens_1_pmv_amont_in,
				char			*va_autoroute_1_pmv_amont_in,
				int			va_distance_1_pmv_amont_in,
				XDY_PR			va_pr_1_pmv_amont_bis_in,
				XDY_Sens		va_sens_1_pmv_amont_bis_in,
				char			*va_autoroute_1_pmv_amont_bis_in,
				int			va_distance_1_pmv_amont_bis_in,
				XDY_PR			va_pr_1_pmv_amont_ter_in,
				XDY_Sens		va_sens_1_pmv_amont_ter_in,
				char			*va_autoroute_1_pmv_amont_ter_in,
				int			va_distance_1_pmv_amont_ter_in,
                                XDY_ResConf             *va_Resultat_out );



#endif
