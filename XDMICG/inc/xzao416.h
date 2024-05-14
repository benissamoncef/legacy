/*E*/
/*  Fichier : $Id: xzao416.h,v 1.2 2000/12/11 14:39:38 gesconf Exp $     Release : $Revision: 1.2 $        Date : $Date: 2000/12/11 14:39:38 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao416.h
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

#ifndef xzao416
#define xzao416

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO416_RPC_NAME "XZAO416"

#define XZAOC_XZAO416_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO416_Ajouter_Zon_PMV (   XDY_Basedd              va_Basedd_in,
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
