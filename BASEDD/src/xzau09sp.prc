/*E*/
/*Fichier : $Id: xzau09sp.prc,v 1.1 1994/11/15 12:16:38 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1994/11/15 12:16:38 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau09sp.prc
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
*  Retourne la liste des fonctions du SAE ( numero + nom )
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;09
   as 

/* 
*
* SP	XZAU09SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZAUT_ListeListeFonctions
*
* VALEUR RENDUE :
*
* Retourne la liste des fonctions du SAE ( numero + nom )
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
   ** Recherche de la liste des fonctions du SAE
   */
         
   select 	numero,
   		nom
   
   from CFG..OPR_FCT_NOM   		
   
   return ( XDC_OK )
     
go  
