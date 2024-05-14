/*E*/
/*Fichier : $Id: xzaa10sp.prc,v 1.2 1998/01/07 10:59:58 penot Exp $      Release : $Revision: 1.2 $        Date : $Date: 1998/01/07 10:59:58 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAA * FICHIER xzaa10sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Interface Alerte >
*
------------------------------------------------------
* HISTORIQUE :
*
* B.G.  25/01/95	: creation(1.1)
* C.P.	07/01/98	: Correction gestion multi-sites dans 
*				  les procs (1.2)
------------------------------------------------------*/

/*X*/
/*------------------------------------------------------
* SERVICE RENDU : 
*
*  retourne le nombre de fmc par type sur le site local
* pour les equations d'alerte (en cours)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAA;10
   as 

/* 
*
* SP	XZAA10SP
*
* PARAMETRES EN ENTREE :
*
* PARAMETRES EN SORTIE :
* select:
* XDY_octet  type_fmc
* XDY_entier nombre
* VALEUR RENDUE :
*
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
    
	@vl_site		tinyint
	
      /* recherche le site local */
        select @vl_site = numero 
        from CFG..RES_DIS
        where serveur = @@servername

        if @vl_site is null
                return XDC_NOK
      
      /*A retourne le nombre de chaque FMC en cours dans le district */
      
      select 	type,
      		count(type)
      from EXP..FMC_GEN
      where fin=null
        and sit=@vl_site
        and debut<>null
      group by type
          
     
   return ( XDC_OK )
         
go  

