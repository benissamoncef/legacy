/*E*/
/*Fichier : $Id: xzan03sp.prc,v 1.2 1996/02/06 17:44:43 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1996/02/06 17:44:43 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan03sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
* torregrossa	06 Fev 1996	: liste retournee par ordre alphabetique
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan03sp.h" */

/* static char *version = "@(#)xzan03.c	1.1 23/18/94 : xzan03" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des types d'astreinte (numero + nom)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;03
   as 

/* 
*
* SP	XZAN03SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Liste des types d'astreinte (numero + nom)
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
* select * from TYP_AST
*
* MECANISMES :
*
------------------------------------------------------*/
   
   /*A
   ** Travail dans la base CFG
   ** Recherche du type d'astreinte
   */   
      
   select * from CFG..TYP_AST          
   order by nom
     
   return ( XDC_OK )
         
go  
