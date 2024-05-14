/*E*/
/*X Fichier : $Id: xzae40.h,v 1.3 2019/05/13 10:25:38 devgfi Exp $        $Revision: 1.3 $        $Date: 2019/05/13 10:25:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE40 * FICHIER XZAE40.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
* JMG	11/10/08	CREATION 	DEM 835
* JMG	26/11/16	ajout majeur 1.2 lot 23
* JPL	04/04/19	Procédure dégroupée ; ajout de l'indicateur d'écoulement (DEM 1326)  1.3
------------------------------------------------------ */

#ifndef XZAE40
#define XZAE40

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
#define XZAE40C_RPC_NAME "XZAE40"

#define XZAE40C_NB_PARM_RETURN 62


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE40_Lire_Fiche_MC :  
* Retourne les champs generiques d'une fiche main
* courante a l'heure precisee
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE40_Lire_Fiche_MC (       XDY_Horodate    va_Horodate_in,
                                        XDY_Entier	va_numero_in ,
					XDY_Octet	va_cle_in,
        XDY_Entier   *va_Numero_out,
        XDY_Octet    *va_Cle_out,
        XDY_Octet    *va_Site_out     ,
        XDY_Octet    *va_Type_out,
        XDY_Horodate *va_DebutPrevu_out  ,
        XDY_Horodate *va_Debut_out      ,
        XDY_Horodate *va_FinPrevu_out  ,
        XDY_Horodate *va_Fin_out      ,
        XDY_Horodate *va_Validation_out,
        XDY_Entier   *va_NumCause_out   ,
        XDY_Octet    *va_CleCause_out  ,
        XDY_Mot      *va_TypeCause_out,
        XDY_Entier   *va_NumAlerte_out ,
        XDY_Horodate *va_HoroAlerte_out,
        XDY_Nom      va_TypeAlerte_out  ,
        XDY_Nom      va_Origine_out    ,
        XDY_Booleen  *va_Confirme_out,
        XDY_Octet    *va_Autoroute_out ,
        XDY_PR       *va_PR_out            ,
        XDY_Octet    *va_Sens_out    ,
        XDY_Octet    *va_PointCar_out,
        XDY_Nom      va_NomPtCar_out   ,
        XDY_Octet    *va_VR_out    ,
        XDY_Octet    *va_VM2_out  ,
        XDY_Octet    *va_VM1_out  ,
        XDY_Octet    *va_VL_out  ,
        XDY_Octet    *va_BAU_out,
        XDY_Octet    *va_VR_I_out   ,
        XDY_Octet    *va_VM2_I_out ,
        XDY_Octet    *va_VM1_I_out,
        XDY_Octet    *va_VL_I_out,
        XDY_Octet    *va_BAU_I_out ,
        XDY_Octet    *va_AutorouteCause_out ,
        XDY_PR       *va_PRCause_out        ,
        XDY_Octet    *va_SensCause_out,
        XDY_Horodate *va_HoroCause_out ,
        XDY_Octet    *va_Position_out  ,
        XDY_Octet    *va_Degats_out   ,
        XDY_Mot      *va_TypeAnterieur_out,
        XDY_Octet    *va_FausseAlerte_out,
        XDY_Octet    *va_BAU_Etroite_out   ,
        XDY_Octet    *va_BAU_I_Etroite_out ,
        XDY_Entier   *va_Longueur_out,
        XDY_Nom      va_AbrPtCar_out     ,
        XDY_Octet    *va_NumPtCar_out    ,
        XDY_Octet    *va_Degrade_out    ,                
        XDY_Entier        *va_NumEvtInit_out,
        XDY_Octet         *va_CleEvtInit_out,
        XDY_Commentaire   va_ComEvtInit_out,
        XDY_Octet         *va_TypeEvtInit_out,
        XDY_Octet         *va_AutorouteEvtInit_out,
        XDY_PR            *va_PREvtInit_out,
        XDY_Octet         *va_SensEvtInit_out,
        XDY_Horodate      *va_HoroEvtInit_out,
        XDY_Octet    *va_SiteAlerte_out,
        XDY_Octet    *va_Datex_out    ,
        XDY_Octet    *va_Trafic_out  ,
        XDY_Octet    *va_Gravite_out,
        XDY_Booleen  *va_Bloquant_out,
        XDY_Octet    *va_Duree_out,
      XDY_Octet  *va_Majeur_out,
      XDY_Octet    *va_Ecoulement_out);




/*
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier		va_numero_in
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
