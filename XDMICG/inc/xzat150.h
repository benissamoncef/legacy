/*E*/
/*  Fichier : 	$Id: xzat150.h,v 1.1 2017/02/10 09:04:38 pc2dpdy Exp $  Release : $Revision: 1.1 $ Date : $Date: 2017/02/10 09:04:38 $
------------------------------------------------------
* GTIE * PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME BASEDD
------------------------------------------------------
* MODULE XZAT * FICHIER xzat05.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Equipements >
*
------------------------------------------------------
* HISTORIQUE :
*
* JMG	28/11/16	creation SAGA 1.1 
------------------------------------------------------ */

#ifndef xzat150
#define xzat150

/* fichiers inclus */
        /* Include systeme */
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
        #include "xzst.h"
        #include "xzsc.h"
        #include "xzag.h"
        #include "asql_com.h"
        #include "xzis.h"
        #include "xzahc.h"

/* definitions de constantes exportees */

#define XZATC_XZAT150_RPC_NAME "XZAT150"

#define XZATC_XZAT150_NB_PARM_RETURN 30

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

int XZAT150_Utilisation_Eqt_SAGA  (     XDY_Horodate    va_Horodate_in,
                                XDY_Mot         va_NumEqt_in,
                                XDY_Octet       *va_DispoPMV_out,
                                XDY_Ligne_PMV   va_Ligne1_out,
                                XDY_Ligne_PMV   va_Ligne2_out,
                                XDY_Ligne_PMV   va_Ligne3_out,
                                XDY_Ligne_PMV   va_Ligne4_out,
                                XDY_Ligne_PMV   va_Ligne5_out,
                                XDY_Ligne_PMV   va_Alternat1_out,
                                XDY_Ligne_PMV   va_Alternat2_out,
                                XDY_Ligne_PMV   va_Alternat3_out,
                                XDY_Ligne_PMV   va_Alternat4_out,
                                XDY_Ligne_PMV   va_Alternat5_out,
                                XDY_Booleen     *va_Clignotement_out,
                                char            *va_Picto_out,
                                XDY_Mot         *va_Luminosite_out,
                                char            *va_TextePicto_out,
                                XDY_Mot         *va_Barriere_out,
                                XDY_Mot         *va_Scenario_out,
                                XDY_Entier      *va_NumEvt_out,
                                XDY_Octet       *va_CleEvt_out,
                                XDY_Octet       *va_Priorite_out,
                                XDY_Entier      *va_DistanceEvt_out,
                                XDY_Octet       *va_TypeSAGA_out);


#endif
