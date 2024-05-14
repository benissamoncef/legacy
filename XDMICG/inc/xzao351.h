/*E*/
/*  Fichier : $Id: xzao351.h,v 1.1 2009/05/25 15:25:15 pc2dpdy Exp $     Release : $Revision: 1.1 $        Date : $Date: 2009/05/25 15:25:15 $
------------------------------------------------------
* STERIA PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao351.h
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

#ifndef xzao351
#define xzao351

/* fichiers inclus */

/* definitions de constantes exportees */

#define XZAOC_XZAO351_RPC_NAME "XZAO351"

#define XZAOC_XZAO351_NB_PARM_RETURN 1

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAO351_Ajouter_Zon_TUN (   XDY_Basedd              va_Basedd_in,
                                char           		*va_autoroute_tun_in,
                                XDY_Sens                va_sens_tun_in,
                                XDY_PR                  va_pr_tun_in,
                                char           		*va_autoroute_zone_in,
                                XDY_Sens                va_sens_zone_in,
                                XDY_PR                  va_debut_zone_in,
                                XDY_PR                  va_fin_zone_in,
				XDY_Octet		va_type_zone_in,
				char			*va_groupe_tunnel_in,
                                XDY_Booleen             va_inhibition_zone_in,
				XDY_PR			va_PRPSTT_in,
                                XDY_ResConf             *va_Resultat_out );



#endif
