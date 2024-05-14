/*E*/
/*Fichier : $Id: xzam11sp.prc,v 1.2 1995/01/04 10:26:06 volcic Exp $      Release : $Revision: 1.2 $        Date : $Date: 1995/01/04 10:26:06 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAM * FICHIER xzam11sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alarmes >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	04 Nov 1994	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*   Retourne la liste des DT connues du SAE
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAM;11
   as 

/* 
*
* SP	XZAM11SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* ListeAlarme et DT
*
* VALEUR RENDUE :
*
* Retourne la liste des DT connues du SAE
*
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*  NumFicheAlarme inconnu
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* select dans ALM_EQT
*
* MECANISMES :
*
------------------------------------------------------*/
   
   /*A
   ** Recherche de la liste des DT connues du SAE
   */
         
      select type,
             type_equipement,
             equipement,
             demande_de_travaux		
         
         from  EXP..ALM_EQT
         
         where demande_de_travaux != null
     
   return ( XDC_OK )
         
go  
