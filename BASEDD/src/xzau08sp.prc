/*E*/
/*Fichier : $Id: xzau08sp.prc,v 1.1 1994/11/15 12:16:36 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 12:16:36 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau08sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des profils (nom + numero)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;08
   as 

/* 
*
* SP	XZAU08SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZAUT_ListeListeProfils
*
* VALEUR RENDUE :
*
* Retourne la liste des profils (nom + numero)
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
* MECANISMES :
*
------------------------------------------------------*/
   
   /*A
   ** Recherche de la liste des profils
   */
         
   select 	numero,
   		nom
   
   from CFG..OPR_PRF   		
   
   return ( XDC_OK )
     
go  
