/*E*/
/*Fichier : $Id: xzao322sp.prc,v 1.1 1996/11/06 18:31:42 gaborit Exp $      Release : $Revision: 1.1 $        Date : $Date: 1996/11/06 18:31:42 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao322sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao322sp.h" */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des types de carburants
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

/* 
* Sequence d'appel
* SP	XZAO322_Lire_Carburants
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
*
* Retourne la liste des types de carburants
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
* Retourne les parametres numero de type, nom du carburant, supprime
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* Torregrossa	17 Oct 1996	: Creation
------------------------------------------------------*/
use PRC
go

create procedure XZAO;322
as 

	select 
		numero, 
		nom,
		supprime 
	from CFG..TYP_CRB
      
	return XDC_OK
         
go  
