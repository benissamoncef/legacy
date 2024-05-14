/*E*/
/* Fichier : $Id: stim_tra.h,v 1.3 1995/07/20 15:57:51 milleville Exp $        Release : $Revision: 1.3 $        Date : $Date: 1995/07/20 15:57:51 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE STIM_TRA * FICHIER stim_tra.h
******************************************************
* DESCRIPTION DU MODULE :
*
* Ce module contient les fonctions d'entree de la tache.
* Demande de programmation, demande d'annulation de
* programmation, reveil sur signal UNIX.
*
******************************************************
* HISTORIQUE :
*
* T.Milleville 27 Sep 1994	: Creation
*
******************************************************/

#ifndef sm_traiter
#define sm_traiter

/* fichiers inclus */

/* definitions de constantes exportees */

/* definitions de types exportes */

/* definition de macro exportees */

/* declarations de donnees globales */




/* delaration de fonctions externes */
extern	void	sm_Reveiller_Signal();
extern	void	sm_Reveiller_Cb();
extern	void	sm_Annuler_Cb();




#endif
