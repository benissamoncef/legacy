/*E Fichier : $Id: asrv_arp.h,v 1.6 2020/11/27 09:07:24 pc2dpdy Exp $      Release : $Revision: 1.6 $        Date : $Date: 2020/11/27 09:07:24 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE ARP * FICHIER ARP.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale >
*
------------------------------------------------------
* HISTORIQUE :
*
* mercier	28 Oct 1994	: Creation
* Torregrossa	09 Jan 1998	: Modif nb param (1532) v1.2
* JMG		18/03/11 : ajout aarp11 1.3
* ABE		27/04/20	: ajout arpDatex2 SAE-130 V1.4
* ABE		24/09/20	: revert version précedente (V1.4) SAE-130 V1.5
* ABE		27/11/20	: remise de la version 1.4 pour publication sur ajout FMC au CIT DEM-SAE130 v1.6
* GGY		15/01/24	: Ajout appel aarp02 pour envoi msg XDM_AS_UPDATE_PA (DEM568)
------------------------------------------------------ */

#ifndef ARP
#define ARP

/* fichiers inclus */
        #include        <stdio.h>
        /* include Progiciels */
        #include        "ospublic.h"
        #include        "oserror.h"
        /* include MIGRAZUR */
        #include "xdc.h"
        #include "xdy.h"
	#include "xzit.h"
        #include "asrv_rpcom.h"

/* definitions de constantes exportees */

#define ca_aarp01_nb_param	13
#define ca_aarp02_nb_param	4
#define ca_aarp11_nb_param	15
#define ca_aarpDate2_nb_param	2

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */

/* delaration de fonctions externes */

extern CS_RETCODE aarp01(SRV_PROC        *);
extern CS_RETCODE aarp02(SRV_PROC        *);
extern CS_RETCODE aarp11(SRV_PROC        *);
extern CS_RETCODE aarpDatex2(SRV_PROC        *);

#endif


