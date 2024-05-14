/*E*/
/* Fichier : $Id: xzao564sp.prc,v 1.1 2012/07/04 11:32:36 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:32:36 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao564sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao564sp.h" */

/* static char *version = "@(#)xzao564.c	1.1 23/18/94 : xzao564" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO564' and type = 'P')
	drop procedure XZAO564
go

create procedure XZAO564
   @va_Basedd_in             char(3),
   @va_NumeroZone_in        smallint,
   @va_NumeroStation_in		smallint,
   @va_SensStation_in		smallint,
   @va_VidageTable_in		smallint,
   @va_Param_out			smallint output
   as 

/* 
*
* SP    XZAO552SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_NumeroZone_in
*				    va_NumeroStation_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    

* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Mise a jour des donnees dans la table REG_COM
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
    declare @vl_numero smallint


    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
     
    select @va_Param_out = 0
     
    if @va_Basedd_in             = null
		return XDC_ARG_INV
  

      
    if @va_Basedd_in = XDC_BASE_CFG
	begin
			
			if @va_VidageTable_in  = 1
		begin
			select station from CFG..REG_CON where station = @va_NumeroStation_in
			if @@rowcount = 0
			begin				
				delete from CFG..REG_COM where 
				station = @va_NumeroStation_in and 
				zone = @va_NumeroZone_in
				and sens = @va_SensStation_in
				return XDC_OK
			end
			else
			begin
				select @va_Param_out = 1
				return XDC_OK
			end
		end
		else
		begin
			select @vl_numero = station from CFG..REG_COM where
			station = @va_NumeroStation_in and
			zone = @va_NumeroZone_in
			and sens = @va_SensStation_in
			if @@rowcount = 0 
			begin
				insert into CFG..REG_COM ( station, zone,sens)
		  		values ( @va_NumeroStation_in, @va_NumeroZone_in,@va_SensStation_in)
								
		 		if @@rowcount <> 1
					return XDC_NOK
			end
		end	
	end


#ifdef CI      
	else
	begin	
		if @va_VidageTable_in  = 1
		begin
			select station from CFT..REG_CON where station = @va_NumeroStation_in
			if @@rowcount = 0
			begin				
				delete from CFT..REG_COM where 
				station = @va_NumeroStation_in and 
				zone = @va_NumeroZone_in
				and sens = @va_SensStation_in
				return XDC_OK
			end
			else
			begin
				select @va_Param_out = 1
				return XDC_OK
			end
		end
		else
		begin
			select @vl_numero = station from CFT..REG_COM where
			station = @va_NumeroStation_in and
			zone = @va_NumeroZone_in
			and sens = @va_SensStation_in
			if @@rowcount = 0 
			begin
				insert into CFT..REG_COM ( station, zone,sens)
		  		values ( @va_NumeroStation_in, @va_NumeroZone_in,@va_SensStation_in)
								
		 		if @@rowcount <> 1
					return XDC_NOK
			end
		end
	end
#endif
     
    return XDC_OK
         
go  
