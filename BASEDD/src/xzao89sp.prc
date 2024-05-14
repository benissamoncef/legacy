/*E*/
/*Fichier : $Id: xzao89sp.prc,v 1.1 2017/11/20 10:54:45 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2017/11/20 10:54:45 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao89sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------
* HISTORIQUE :
*
* LCL           11 Oct 2017     : Creation TIGMOT_FLASH
------------------------------------------------------ */

/* fichiers inclus */

/* static char *version = "@(#)xzao89.c	1.1 11/10/17 : xzao89" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des equipements PO, SD, ST, Imp?
*
*  TypeObjet_in : 1 PO / 2 ST / 3 SD / 4 Imp / 0 tous
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select name from sysobjects where name = 'XZAO89' and type = 'P')
	drop procedure XZAO89
go

create procedure XZAO89
   @va_Basedd_in       	char(3),
   @va_TypeObjet_in	tinyint	
   as 

/* 
*
* SP	XZAO89SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Entier	va_Basedd_in			
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
*  Liste des equipements de type choisi 
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
* Retourne numero, nom
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
------------------------------------------------------*/
declare

   /*A
   ** Declaration des variables locales
   */
   @vl_bidon tinyint

   /*A
   ** Test des parametres d'entree :
   **   Si un des parametres d'entree est egal a null alors XDC_NOK   
   */
    
   /*! transfo en paramÉtres nulls bug applix */
   if rtrim ( @va_Basedd_in ) = null		select @va_Basedd_in = null
   
   if @va_Basedd_in = null
   begin
      return XDC_ARG_INV
   enD
   else   
   begin
      /*A
      **  Travail dans la base CFG
      */   
      if @va_Basedd_in = XDC_BASE_CFG
      begin   
      
         /*A
         ** Recherche des equipements du type spécifié
         */
         
         if @va_TypeObjet_in = 0
         begin
            
            select gen.numero, gen.nom 
		from CFG..EQT_GEN gen
	      where gen.type = XDC_EQT_MAC
	      or gen.type = XDC_EQT_SRV
	      order by gen.nom

	 end 
	 else 
	 begin
		if @va_TypeObjet_in = 1
		begin
			select gen.numero, gen.nom 
			from CFG..EQT_GEN gen
			where gen.type = XDC_EQT_MAC
			and (gen.nom like 'PO%' 
			or gen.nom like 'SP%'
			or gen.nom like 'SS%')
			order by gen.nom
		end
		else 
		begin
			if @va_TypeObjet_in = 2
			begin
				select gen.numero, gen.nom
				from CFG..EQT_GEN gen
				where gen.type = XDC_EQT_SRV
				order by gen.nom
			end
			else
			begin
				if @va_TypeObjet_in = 3
				begin
					select gen.numero, gen.nom
					from CFG..EQT_GEN gen
					where gen.type = XDC_EQT_MAC
					and gen.nom like 'SD%'
					order by gen.nom
				end
				else
				begin
					select gen.numero, gen.nom
					from CFG..EQT_GEN gen
					where gen.type = XDC_EQT_MAC
					and gen.nom like 'IMP%'
					order by gen.nom
				end
			end
		end
	 end                        
         
      end

#ifdef CI
      
      else
      
      /*A
      **  Travail dans la base CFT
      */   
      
      begin   
	/*A
	** Recherche des equipements du type spécifié
	*/
	if @va_TypeObjet_in = 0
	begin

		select gen.numero, gen.nom
		from CFT..EQT_GEN gen
		where gen.type = XDC_EQT_MAC
		or gen.type = XDC_EQT_SRV
		order by gen.nom

	end
	else 
	begin
		if @va_TypeObjet_in = 1
		begin
			select gen.numero, gen.nom
			from CFT..EQT_GEN gen
			where gen.type = XDC_EQT_MAC
			and (gen.nom like 'PO%'
			or gen.nom like 'SP%'
			or gen.nom like 'SS%')
			order by gen.nom
		end
		else
		begin
			if @va_TypeObjet_in = 2
			begin
				select gen.numero, gen.nom
				from CFT..EQT_GEN gen
				where gen.type = XDC_EQT_SRV
				order by gen.nom
			end
			else
			begin
				if @va_TypeObjet_in = 3
				begin
					select gen.numero, gen.nom
					from CFT..EQT_GEN gen
					where gen.type = XDC_EQT_MAC
					and gen.nom like 'SD%'
					order by gen.nom
				end
				else
				begin
					select gen.numero, gen.nom
					from CFT..EQT_GEN gen
					where gen.type = XDC_EQT_MAC
					and gen.nom like 'IMP%'
					order by gen.nom
				end
			end
		end
	end

  end
#endif
      
   end
      
   return XDC_OK
         
go  
