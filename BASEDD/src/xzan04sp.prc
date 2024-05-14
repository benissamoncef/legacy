/*E*/
/*Fichier : $Id: xzan04sp.prc,v 1.1 1994/11/15 12:17:41 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 12:17:41 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan04sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	26 Oct 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzan04sp.h" */

/* static char *version = "@(#)xzan04.c	1.1 23/18/94 : xzan04" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des categories d'astreinte (numero + nom)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAN;04
   as 

/* 
*
* SP	XZAN04SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Liste des categories d'astreinte (numero + nom)
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
* select * from ANN_AST_CAT
*
* MECANISMES :
*
------------------------------------------------------*/
   
   /*A
   ** Travail dans la base CFG
   ** Recherche du type d'astreinte
   */   
      
   select * from CFG..ANN_AST_CAT         
     
   return ( XDC_OK )
         
go  
