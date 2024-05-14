/*E*/
/*Fichier : $Id: xzau10sp.prc,v 1.2 1994/12/15 11:55:29 gaborit Exp $      Release : $Revision: 1.2 $        Date : $Date: 1994/12/15 11:55:29 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAU * FICHIER xzau10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Utilisateurs >
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	02 Nov 1994	: Creation
* gaborit	15 dec 1994	: modif (jointure externe+ordre) 1.2
------------------------------------------------------*/

/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  Retourne la liste des utilisateurs
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAU;10
   as 

/* 
*
* SP	XZAU10SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
*
* XZAUT_ListeUtil
*
* VALEUR RENDUE :
*
* Retourne la liste des utilisateurs
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
  (     
   select 	IDF.operateur,
   		OPE.nom,
   		IDF.login,
   		IDF.profil,
   		PRF.nom
   
   from CFG..OPR_NOM OPE, CFG..OPR_LOG_MDP IDF, CFG..OPR_PRF PRF
   
   where IDF.operateur 	= OPE.numero 
     and IDF.profil 	= PRF.numero
   
   union
   select 	OPE.numero,
   		OPE.nom,
   		null,
   		null,
   		null
   
   from CFG..OPR_NOM OPE
   
   where OPE.numero not in (select distinct operateur from CFG..OPR_LOG_MDP)
  )
   order by  OPE.nom 		
   
   return ( XDC_OK )
     
go  
