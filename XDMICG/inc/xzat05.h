/*E*/
/*  Fichier : 	$Id: xzat05.h,v 1.6 2021/01/06 09:44:34 akkaakka Exp $  Release : $Revision: 1.6 $ Date : $Date: 2021/01/06 09:44:34 $
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
* volcic       03 Nov 1994     : Creation
* volcic       30 Nov 1994     : Modif Nb parametres sorties (v1.3)
* JMG		5 dec 2011 : linux
* JMG		14/09/12 : suppression ";"
------------------------------------------------------ */

#ifndef xzat05
#define xzat05

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

#define XZATC_XZAT05_RPC_NAME "XZAT05"

#define XZATC_XZAT05_NB_PARM_RETURN 25

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */


/* delaration de fonctions externes */

extern int XZAT05_Utilisation_PMV ( 	XDY_Horodate,
					XDY_Mot,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Ligne_PMV ,
					XDY_Ligne_PMV ,
					XDY_Ligne_PMV ,
					XDY_Ligne_PMV ,
					XDY_Ligne_PMV ,
					XDY_Ligne_PMV ,
					XDY_Booleen *,
					XDY_Booleen *,
					XDY_Booleen *,
					XDY_Booleen *,
					char *,
					char *,
					XDY_Octet *,
					XDY_Booleen *,
					XDY_Entier *,
					XDY_Octet *,
					XDY_Octet *,
					XDY_Entier * );



#endif
