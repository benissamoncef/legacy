/*E*/
/* Fichier : $Id: xzao436.h,v 1.2 2016/07/19 18:44:46 devgfi Exp $     Release : $Revision: 1.2 $        Date : $Date: 2016/07/19 18:44:46 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao436.h
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

#ifndef xzao436
#define xzao436

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO436_RPC_NAME "XZAO456"

#define XZAOC_XZAO436_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO436_Ajouter_Zon_BAD (   XDY_Basedd              va_Basedd_in,
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
