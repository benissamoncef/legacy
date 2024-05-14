/*E*/
/*  Fichier : $Id: xzae146.h,v 1.5 2011/04/04 15:32:39 pc2dpdy Exp $     Release : $Revision: 1.5 $        Date : $Date: 2011/04/04 15:32:39 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae146sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des fiches main courante
*
------------------------------------------------------
* HISTORIQUE :
*
* Penot	02/12/97	: Creation	(V 1.1)
* Guilhou 02/12/97	: nom proc 	1.2
* JMG	15/02/11	: ajout nom zautoroute en sortie 1.3
-----------------------------------------------------*/

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
        #include "xzag.h"
        #include "asql_com.h"

/* definitions de constantes exportees */

#define XZAE146C_NB_PARM_RETURN        9

/*X*/
/*-----------------------------------------------------
* Service rendu
* 
* 
* Sequence d'appel
* SP	XZAE146_Recherche_Loc_Rds
* 
* Arguments en entree
* XDY_Entier		va_pr_in
* XDY_Octet		va_autoroute_in
* XDY_Entier 		va_pr_inf_out
* XDY_Entier 		va_pr_sup_out
* char 			ta_nom_inf_out[50]
* char 			ta_nom_sup_out[50]
* XDY_Entier 		va_num_inf_out
* XDY_Entier 		va_num_sup_out
* Arguments en sortie
* 
* Code retour
* XDC_OK
* XDC_NOK	: 
*
* XDC_ARG_INV	: parametres d'entree null
* 
* Conditions d'utilisation
* 
* Fonction
* 
-------------------------------------------------------*/

 
#define XZAEC_XZAE146_RPC_NAME	"XZAE146" 


int XZAE146_Recherche_Loc_Rds ( 
				XDY_Entier      va_pr,
                                XDY_Octet      va_autoroute,
                                char            *va_num_table_out,
				XDY_Entier      *va_pr_inf_out,
                                XDY_Entier      *va_pr_sup_out,
				char            *ta_nom_inf_out,
                                char            *ta_nom_sup_out,
                                XDY_Entier      *va_num_inf_out,
                                XDY_Entier      *va_num_sup_out,
				char            *va_nom_autoroute_out
                                );

