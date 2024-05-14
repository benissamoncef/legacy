/*E Fichier : $Id: etmi_pro.h,v 1.2 1995/05/04 20:14:29 cornu Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/05/04 20:14:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME EQUEXT
------------------------------------------------------
* MODULE TETMi * FICHIER etmi_pro.h
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Definition fonction protocole maitre tache TELMi
*
*
------------------------------------------------------
* HISTORIQUE :
*
* Nouaille M.	Version 1.1	02 Dec 1994	: Creation
* Fontaine C.   Version 1.2     03 Mai 1995     : Reprise entete
------------------------------------------------------ */

#ifndef etmi_pro
#define etmi_pro

/* fichiers inclus */
#include "etmi_common.h"
#include "ex_mtcr.h"

/* definitions de constantes exportees */

/* definitions de types exportes */

/* type local EPRO qui decrit les conditions d'envoi d'une trame TEL de structure LCR */
typedef struct EPROT_Conditions
{
    int		nb_essais;	/* condition = nbr. essais d'envoi trame equip. TEL */
    int		nb_err_crc;	/* nbr. errs. CRC detectees */
    int		nb_non_rep;	/* nbre. non reponses aux trames envoyees */
    double	val_timer;	/* valeur timer pour attente reponse */
}EPROT_Conditions;

/* definition de macro exportees */

/* declarations de donnees globales */

/* declaration de fonctions externes */

#endif
