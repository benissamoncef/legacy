/*E*/
/*Fichier : $Id: xzan19sp.prc,v 1.1 2018/01/12 16:24:47 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2018/01/12 16:24:47 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan15sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Annuaire >
*
------------------------------------------------------
* HISTORIQUE :
*
------------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des fiches (numero, nom, adresse fax ou mail)
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAN19' and type = 'P')
	drop procedure XZAN19
go

create procedure XZAN19
   @va_Motif_Debut_Nom_in	varchar(25),
   @va_Site_concerne_in         tinyint = null    
   as 

/* 
*
* SP	XZAN15SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Nom		va_Motif_Debut_Nom_in
*
* PARAMETRES EN SORTIE :
*
* XZANT_ListeAnn
*
* VALEUR RENDUE :
*
* Retourne les fiches (numero, nom, adresse) dont le debut de nom correspond au motif specifie.
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
*
* CONDITIONS D'UTILISATION :
*
* Attention au volume des donnees ( rendre 1 lettre obligatoire )
*
* FONCTION :
*
* MECANISMES :
*
------------------------------------------------------*/
   
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if @va_Motif_Debut_Nom_in = null
      
      return ( XDC_ARG_INV )
   
   else   
   
   /*A
   ** Si les parametres sont non nuls
   */
   begin
      
      /*A
      ** Travail dans la base CFG
      */   
      
      /*A
      ** Les Premieres Lettres possedent moins d'une lettre
      ** ou la premiere lettre n'est pas une majuscule
      ** return (XDC_NOK)
      */
      
      if ( char_length ( @va_Motif_Debut_Nom_in ) < 1 ) or
        ( substring ( @va_Motif_Debut_Nom_in , 1, 1 ) != upper ( substring ( @va_Motif_Debut_Nom_in , 1, 1 ) ) )
      
         return ( XDC_NOK )
         
      else
      
      begin
      
      /*A
      ** Les Premieres Lettres possedent au moins une lettre
      ** et la premiere lettre est une majuscule
      ** Recherche des fiches
      */  
         
	 if ( @va_Site_concerne_in = null  or @va_Site_concerne_in = 0) /* on est donc au CI */
	 begin
	   /* REPERE1 */
           select numero, nom, ltrim(tel1) from CFG..ANN_FIC
           where nom like @va_Motif_Debut_Nom_in + '%'
	   and (inhibition is null or inhibition != 1)
	   order by nom
	 end
	 else
	 begin
	   /* REPERE1 */
           select numero, nom, ltrim(tel1) from CFG..ANN_FIC
           where nom like @va_Motif_Debut_Nom_in + '%'
	   and ( site_concerne = @va_Site_concerne_in or site_concerne= null  or site_concerne = 0)
	   and (inhibition is null or inhibition != 1)
	   order by nom
         end
         
      end
   end
     
   return ( XDC_OK )
         
go  
