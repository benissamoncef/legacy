/*E*/
/*Fichier : $Id: xzau16sp.prc,v 1.1 1995/05/19 20:42:17 volcic Exp $      Release : $Revision: 1.1 $        Date : $Date: 1995/05/19 20:42:17 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	19 Mai 1995	: Creation
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des Mots de passe ( nom + mot_de_passe + num_profil)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;16
   as 

/* 
*
* SP	XZAU16SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZAUT_ListeListeFonctions
*
* VALEUR RENDUE :
*
*  Retourne la liste des Mots de passe ( nom + mot_de_passe + num_profil)
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
         
   select ( RTRIM ( LOG.login ) + ',' + LOG.mot_de_passe )
   
   from CFG..OPR_LOG_MDP LOG,  CFG..OPR_PRF PRF
   
   where PRF.nom like "%MAINTENANCE%" 
     and LOG.profil=PRF.numero 		
   
   return ( XDC_OK )
     
go  
