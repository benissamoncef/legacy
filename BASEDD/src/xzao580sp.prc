/*E*/
/* Fichier : $Id: xzao580sp.prc,v 1.1 2012/07/04 11:35:10 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:35:10 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao580sp.prc
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

/* #include "xzao580sp.h" */

/* static char *version = "@(#)xzao580.c	1.1 23/18/94 : xzao580" ; */

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

if exists (select * from sysobjects where name = 'XZAO580' and type = 'P')
	drop procedure XZAO580
go

	create procedure XZAO580
	   @va_Basedd_in         	     char(3),
	   @va_NumeroCondition_in		 smallint,
	   @va_NumeroScen_in		     smallint,
	   @va_Zone_in				smallint,
	   @va_Ajout_in			smallint
	   as 

	   
/* 
*
* SP    XZAO580SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
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
* Mise � jour des donn�es dans la table ZON_REG
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
declare @vl_numero smallint
   
  if @va_Basedd_in             = null
		return XDC_ARG_INV
       
  if @va_Basedd_in = XDC_BASE_CFG
  begin
    if @va_Ajout_in=1
	    begin
			insert into CFG..REG_ASS (numero, scenario, zone)
			values (@va_NumeroCondition_in, @va_NumeroScen_in, @va_Zone_in)
    	end
    else
    	begin
			delete CFG..REG_ASS
			where scenario = @va_NumeroScen_in
			and zone = @va_Zone_in
    	end
  end
#ifdef  CI 
  else
    begin
    	if @va_Ajout_in=1
    		begin
				insert into CFT..REG_ASS (numero, scenario, zone)
				values (@va_NumeroCondition_in, @va_NumeroScen_in, @va_Zone_in)
    		end
    	else
    		begin
				delete CFT..REG_ASS
				where scenario = @va_NumeroScen_in
				and zone = @va_Zone_in
    		end
  	end
#endif
      
    return XDC_OK
         
go  
