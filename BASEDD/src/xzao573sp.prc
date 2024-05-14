/*E*/
/* Fichier : $Id: xzao573sp.prc,v 1.3 2018/07/11 11:21:52 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2018/07/11 11:21:52 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao573sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		13/02/12 : Creation (DEM/1016)
* LCL		05/07/17 : modif PRA
* LCL           11/07/18        : correction syntaxe from
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao573sp.h" */

/* static char *version = "@(#)xzao573.c	1.1 23/18/94 : xzao573" ; */

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

if exists (select * from sysobjects where name = 'XZAO573' and type = 'P')
	drop procedure XZAO573
go

	create procedure XZAO573
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
* Recherche les PMV suivants : 
*	Les PMV dans la zone de regulation.
*	Les PMV en amont de la zone, possedant une partie de la zone de regulation dans leur zone d influence PMV.
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

						if @va_Sens_in = XDC_SENS_SUD
							begin
								select PMV.numero, PMV.type, PMV.liaison_serie, PMV.rgs
								from   CFG..EQT_GEN GEN,
									   CFG..EQT_PMV PMV
								where  GEN.type = XDC_EQT_PMV
								and	   GEN.numero = PMV.numero
								and	   GEN.PR >= @va_Debut_in
								and    GEN.PR <= @va_Fin_in
								and    GEN.autoroute = @va_Autoroute_in
								UNION
								select PMV.numero, PMV.type, PMV.liaison_serie, PMV.rgs
								from   CFG..EQT_PMV PMV,
									   CFG..ZON_REG ZON,
									   CFG..EQT_GEN GEN
								where  GEN.type = XDC_EQT_PMV
								and    GEN.numero = ZON.numero
								and	   GEN.numero = PMV.numero
								and    GEN.autoroute = @va_Autoroute_in
								and    ZON.debut  <= @va_Debut_in
								and	   ZON.fin	  <  @va_Fin_in
								and    ZON.autoroute = @va_Autoroute_in
							end
							
							if @va_Sens_in = XDC_SENS_NORD
							begin
								select PMV.numero, PMV.type, PMV.liaison_serie, PMV.rgs
								from   CFG..EQT_GEN GEN,
									   CFG..EQT_PMV PMV
								where  GEN.type = XDC_EQT_PMV
								and	   GEN.numero = PMV.numero
								and	   GEN.PR >= @va_Debut_in
								and    GEN.PR <= @va_Fin_in
								and    GEN.autoroute = @va_Autoroute_in
								UNION
								select PMV.numero, PMV.type, PMV.liaison_serie, PMV.rgs
								from   CFG..EQT_PMV PMV,
									   CFG..ZON_REG ZON,
									   CFG..EQT_GEN GEN
								where  GEN.type = XDC_EQT_PMV
								and    GEN.numero = ZON.numero
								and	   GEN.numero = PMV.numero
								and    GEN.autoroute = @va_Autoroute_in
								and    ZON.debut  <= @va_Debut_in
								and	   ZON.fin	  <  @va_Fin_in
								and    ZON.autoroute = @va_Autoroute_in
							end

							/*A
							** Affectation du parametre de sortie Resultat a XDC_MODIF
							*/

							select @va_Resultat_out = XDC_MODIF

									return XDC_NOK	   
						   
					end   
#ifdef CI           
#endif
      
	    end
    end         
     
    return XDC_OK
         
go  
