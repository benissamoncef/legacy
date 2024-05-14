/*E*/
/*Fichier : $Id: xzao148sp.prc,v 1.3 2019/01/29 11:17:19 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2019/01/29 11:17:19 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao148sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao148sp.h" */

/* static char *version = "$Id: xzao148sp.prc,v 1.3 2019/01/29 11:17:19 pc2dpdy Exp $ : xzao148" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des portions d'un district
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

create procedure XZAO;148
   @va_Basedd_in       	char(3),
   @va_NomDistrict_in 	char(25)	
   as 

/* 
*
* SP	XZAO148SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
* XDY_Nom	va_NomDistrict_in
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des portions d'un district
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
* Retourne  XZAOT_Portion
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	07 Oct 1994	: Creation
* volcic	22 Nov 1994	: Modification du type du district (v 1.2)
------------------------------------------------------*/
 
   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
      
   if rtrim ( @va_Basedd_in ) = null 		select @va_Basedd_in = null
   if rtrim ( @va_NomDistrict_in ) = null 	select @va_NomDistrict_in = null
   
   
   if @va_Basedd_in = null
   or @va_NomDistrict_in = null
    
      return XDC_ARG_INV
   
   else   
   
   /*A
   ** Les parametres d'entree sont non nuls
   */
   
   begin
      
      /*A
      **  Travail dans la base CFG
      */   
      
      if @va_Basedd_in = XDC_BASE_CFG
      begin
         
         /*A
         ** Recherche des portions du district
         */
      	if @va_NomDistrict_in = "Durance Provence"
	begin

	 select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
	 from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
	 where POR.district = DIS.numero
	   and DIS.nom      = @va_NomDistrict_in
	   and POR.numero != 67
         group by DIS.nom, POR.numero
	 union
         select POR.numero, 3, 72790, 79590, POR.district_recouvrement
	 from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
	 where POR.district = DIS.numero
	   and DIS.nom      = @va_NomDistrict_in
	   and POR.numero = 67
         group by DIS.nom, POR.numero

	end
	else
	begin

         select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
         from CFG..RES_POR POR, CFG..RES_DIS DIS, CFG..RES_DIS DISR
           where POR.district = DIS.numero
             and DIS.nom      = @va_NomDistrict_in
           group by DIS.nom, POR.numero   

	end

      end
      
            
      else

#ifdef CI
      
      begin
      /*A
      **  Travail dans la base CFT
      */   
      
         if @va_Basedd_in = XDC_BASE_CFT
         
         /*A
         ** Recherche des portions du district
         */
         	if @va_NomDistrict_in = "Durance Provence"
		begin

	         select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
        	 from CFT..RES_POR POR, CFT..RES_DIS DIS, CFT..RES_DIS DISR
           	 where POR.district = DIS.numero
             	   and DIS.nom      = @va_NomDistrict_in
		   and POR.numero != 67
           	 group by DIS.nom, POR.numero   
		 union
	         select POR.numero, 67, 72790, 79590, POR.district_recouvrement
        	 from CFT..RES_POR POR, CFT..RES_DIS DIS, CFT..RES_DIS DISR
           	 where POR.district = DIS.numero
             	   and DIS.nom      = @va_NomDistrict_in
		   and POR.numero   = 67
           	 group by DIS.nom, POR.numero   

		end
		else
		begin
         	  select POR.numero, POR.autoroute, POR.PR_debut, POR.PR_fin, POR.district_recouvrement
	          from CFT..RES_POR POR, CFT..RES_DIS DIS, CFT..RES_DIS DISR
         	  where POR.district = DIS.numero
                    and DIS.nom      = @va_NomDistrict_in
                    group by DIS.nom, POR.numero   
		end

	end
      
      return XDC_OK
      
#endif      

       return (XDC_NOK)
   end
   
      
   return XDC_OK
         
go  
