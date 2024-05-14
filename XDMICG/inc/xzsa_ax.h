/*E*/
/* Fichier : $Id: xzsa_ax.h,v 1.1 1996/02/19 18:31:20 noel Exp $	      Release : $Revision: 1.1 $        Date : $Date: 1996/02/19 18:31:20 $
******************************************************
* STERIA *  PROJET MIGRAZUR
******************************************************
* MODULE XZADMIN  * FICHIER xzsa.h
******************************************************
* DESCRIPTION DU MODULE :
*
*   - Diffusion modification dynamique administration
*     systeme
*   - Consultation de l'administration systeme
*
* Reference DCG-021 chapitre 3.1
*
******************************************************
* HISTORIQUE :
*
*  T.Milleville  13 Sep 1994      : Creation
*
*  T.Milleville  29 Dec 1994      : Ajout de la fonction
*					XZSA_29ListeEntiteSite V 1.5
*  
*  T.Milleville  6 Feb 1994      : Ajout de la constante
*					XZSAC_TYPE_TPRE	 	V 1.6
*  T.Milleville  9 Avr 1995      : Ajout de la constante
*					XZSAC_TYPE_TSHE	V
*  T.Milleville  2 Mai 1995      : Ajout d'un module d'interface 
*	XZSA_15TacheTempMachine V1.8
*  T.Milleville  6 Oct 1995      : Ajout constante pour preciser
*	le type de l'imprimante + fonction pour rechercher le type
*	de l'entite XZSA_26TypeEntite V1.10
*  B. Noel	19 Fev 1996	: Ajout de marqueurs pour filtre Applix		V1.11
*******************************************************/

/******************************************************************************
**                                                                           **
** Ce fichier definit des constantes communes a tous les sous-systemes,      **
** y compris le sous-systeme IHM developpe avec Applix.                      **
** Des commentaires de la forme "Non Applix - debut" et "Non Applix - fin"   **
** delimitent par la suite les zones dont la syntaxe n'est pas reconnue par  **
** l'interpreteur Applix de maniere a pouvoir les filtrer simplement.        **
**                                                                           **
******************************************************************************/


#define XZSAC_TYPE_IMP_NB	10    		/* Imprimante NOIR et BLANC */
#define XZSAC_TYPE_IMP_CL	11    		/* Imprimante COULEUR */


/* fin de fichier */
