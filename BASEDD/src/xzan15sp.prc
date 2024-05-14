/*E*/
/*Fichier : $Id: xzan15sp.prc,v 1.7 2016/03/29 19:48:24 devgfi Exp $      Release : $Revision: 1.7 $        Date : $Date: 2016/03/29 19:48:24 $
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
* volcic	26 Oct 1994	: Creation
* gaborit	08 dec 1994	: ajout numero de fax 1.2
* C.T.		14 Fev 1995	: Modif ordre de retour 
*                                 des donnees 1.3
* cluchague     02 Fev 1999     : Gestion site_concerne 1.4
* cluchague     14 Aou 2008     : on renvoie le numero de fax concatene avec l'@ email (au lieu de fax tout seul) 
				  car 1 des 2 est forcemment null ceci afin d'eviter de retourner 
				  un champ vide REPERE1 DEM802 1.5
* cluchague     24 Oct 2008     : on ne prend en compte dans l'annuaire que les enregistrements dont inhibition =0 1.6 DEM834
* JPL		25/03/2016	: Generalisation : utilisation du debut de nom comme motif (pour DEM 1155)  1.7
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

create procedure XZAN;15
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
           select numero, nom, ltrim(fax+adresse_email) from CFG..ANN_FIC
           where nom like @va_Motif_Debut_Nom_in + '%'
	   and (inhibition is null or inhibition != 1)
	   order by nom
	 end
	 else
	 begin
	   /* REPERE1 */
           select numero, nom, ltrim(fax+adresse_email) from CFG..ANN_FIC
           where nom like @va_Motif_Debut_Nom_in + '%'
	   and ( site_concerne = @va_Site_concerne_in or site_concerne= null  or site_concerne = 0)
	   and (inhibition is null or inhibition != 1)
	   order by nom
         end
         
      end
   end
     
   return ( XDC_OK )
         
go  
