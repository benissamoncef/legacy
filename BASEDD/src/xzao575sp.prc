/*E*/
/* Fichier : $Id: xzao575sp.prc,v 1.4 2018/11/19 15:44:06 devgfi Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/11/19 15:44:06 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao575sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Creation (DEM/1016)
* LCL		05/07/17 : Modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao575sp.h" */

/* static char *version = "@(#)xzao575.c	1.1 23/18/94 : xzao575" ; */

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

if exists (select * from sysobjects where name = 'XZAO575' and type = 'P')
	drop procedure XZAO575
go

	create procedure XZAO575
	   @va_Autoroute_in		smallint,
	   @va_Sens_in			smallint,
	   @va_Debut_in			int,
	   @va_Fin_in			int,
	   @va_Resultat_out		int output
	   as 

/* 
*
* SP    XZAO553SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  					va_Autoroute_in
* 					va_Sens_in
*  					va_Debut_in
*  					va_Fin_in
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
* Recherche des PMVA dans la zone de regulation
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Fev 2012    : Creation (DEM/1016)
* GGY	26/09/23	: Ajout district CA (DEM483)
------------------------------------------------------*/
declare	@vl_Site	tinyint

    select @va_Resultat_out = XDC_LU
   
    begin
      
	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

		/* Si le site est CI ou CA */
		if @@rowcount = 1
		begin

            /*A
            **  Travail dans la base CFG
            */   
      
					begin
                        /*A
                        ** Modification de l equipement
                        */

							if @va_Sens_in = XDC_SENS_SUD or 
							   @va_Sens_in = XDC_SENS_ENTREE_SUD or
							   @va_Sens_in = XDC_SENS_SORTIE_SUD
							begin
								select PMVA.numero, PMVA.type, PMVA.liaison_serie, PMVA.rgs
								from   CFG..EQT_GEN GEN,
									   CFG..EQT_PMVA PMVA
								where  GEN.type = XDC_EQT_PMVA
								and	   GEN.numero = PMVA.numero
								and	   GEN.PR >= @va_Debut_in
								and    GEN.PR <= @va_Fin_in
								and    GEN.autoroute = @va_Autoroute_in
								and	   GEN.sens = @va_Sens_in
							end
							
							if @va_Sens_in = XDC_SENS_NORD or 
							   @va_Sens_in = XDC_SENS_ENTREE_NORD or
							   @va_Sens_in = XDC_SENS_SORTIE_NORD
							begin
								select PMVA.numero, PMVA.type, PMVA.liaison_serie, PMVA.rgs
								from   CFG..EQT_GEN GEN,
									   CFG..EQT_PMV PMVA
								where  GEN.type = XDC_EQT_PMVA
								and	   GEN.numero = PMVA.numero
								and	   GEN.PR >= @va_Debut_in
								and    GEN.PR <= @va_Fin_in
								and    GEN.autoroute = @va_Autoroute_in
								and	   GEN.sens = @va_Sens_in
							end
					end   
#ifdef CI      
#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
