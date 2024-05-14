/*E*/
/*Fichier : $Id: xzae66.h,v 1.1 2017/03/31 17:24:36 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/03/31 17:24:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAE66 * FICHIER XZAE66.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	21/06/16	creation saga 1.1 
------------------------------------------------------ */

#ifndef XZAE66
#define XZAE66

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
#define XZAE66C_RPC_NAME "XZAE66"

#define XZAE66C_NB_PARM_RETURN 21


/* definitions de types exportes */



/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

/*X------------------------------------------------------
* SERVICE RENDU : 
*  XZAE66_Infos_obligatoires : recupere les infos 
*  necessaires d un fichier evt strada
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

extern int XZAE66_Lire_Config_Voies (    XDY_Autoroute      va_NumAutoroute_in ,
			XDY_PR       va_PR_in,
                                        XDY_Octet       va_Sens_in,
                                        XDY_Octet       va_PtCaracteristique_in,
                                        XDY_Octet       va_TypePtCaracteristique_in,
                                        XDY_Octet       va_Position_in,
					XDY_Mot         va_Type_in,
                                        XDY_Autoroute       *va_NumAutoroute_out,
                                        XDY_PR          *va_PR_out,
                                        XDY_Octet       *va_Sens_out,
                                        XDY_Octet       *va_PtCaracteristique_out,
                                        XDY_Octet       *va_TypePtCaracteristique_out,
                                        XDY_Octet       *va_Position_out,
					XDY_Octet       *va_VR_out,
                                        XDY_Octet       *va_VM2_out,
                                        XDY_Octet       *va_VM1_out,
                                        XDY_Octet       *va_VL_out,
XDY_Octet       *va_BAU_out,
                                        XDY_Octet       *va_VR_I_out,
                                        XDY_Octet       *va_VM2_I_out,
                                        XDY_Octet       *va_VM1_I_out,
XDY_Octet       *va_VL_I_out,
                                        XDY_Octet       *va_BAU_I_out,
XDY_Octet       *va_BAU_Etroite_out,
                                        XDY_Octet       *va_BAU_I_Etroite_out,
XDY_Octet       *va_District_out,
                                        XDY_Octet       *va_DistrictRecouvr_out,
                                        int             *va_Resultat_out);
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
