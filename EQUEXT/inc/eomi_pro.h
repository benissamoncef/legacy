/*E Fichier : $Id: eomi_pro.h,v 1.1 1995/01/04 21:49:45 nouaille Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/01/04 21:49:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TEOMi * FICHIER eomi_pro.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonction protocole maitre tache TEOMi
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
------------------------------------------------------ */

#ifndef eomi_pro
#define eomi_pro

/* fichiers inclus */
#include "eomi_common.h"
#include "ex_mtcr.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* type local EPRO qui decrit les conditions d'envoi d'une trame OERATOR de structure LCR */
typedef struct EPROT_Conditions
{
    int		nb_essais;	/* condition = nbr. essais d'envoi trame equip. OPERATOR */
    int		nb_err_crc;	/* nbr. errs. CRC detectees */
    int		nb_non_rep;	/* nbre. non reponses aux trames envoyees */
    double	val_timer;	/* valeur timer pour attente reponse */
}EPROT_Conditions;

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

#endif
