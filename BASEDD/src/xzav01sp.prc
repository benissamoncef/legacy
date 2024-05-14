/*E*/
/*Fichier : $Id: xzav01sp.prc,v 1.1 2012/07/04 11:38:17 pc2dpdy Exp $      Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:38:17 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAV * FICHIER xzav01sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzav01sp.h" */

/* static char *version = "@(#)xzav01.c    1.1 04/09/07 : xzav01" ; */

/* declaration de fonctions internes */

/* definitions de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'un eqt du  domaine eclairage dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAV01' and type = 'P')
	drop procedure XZAV01
go

create procedure XZAV01
    @va_Basedd_in             char(3),
    @va_horodate_in           datetime,
    @va_type_in        		  tinyint,
    @va_libelle_in         	  char(200),
    @va_alerte_in			  int,
    @va_numero_fmc_in		  int,
    @va_cle_fmc_in			  tinyint,
    @va_acq_alerte_in		  tinyint,
    @va_num_station_in		  smallint,
    @va_sens_station_in		  tinyint,
    @va_scenario_in			  smallint,
    @va_condition_in		  int,
    @va_simulation_in         bit,
    @va_zone_in				  smallint
    as 

/* 
*
* SP    XZAV01SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*      			    va_horodate_in
*      			    va_type_in
*        		    va_libelle_in
*        		    va_alerte_in
*        		    va_numero_fmc_in
*        		    va_cle_fmc_in
*        		    va_acq_alerte_in
*					va_num_station_in
*					va_sens_station_in
*        		    va_scenario_in
*        		    va_condition_in
*        	        va_simulation_in
*        		    va_zone_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* 
* 
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
* CONDITIONS D'UTILISATION :
*
* Config OFF LINE
*
* FONCTION :
*
* Insertion dans la table REG_TRA
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    19 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
   
    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */
      
    if @va_Basedd_in             = null or
       @va_horodate_in             = null or 
       @va_type_in          = null or 
       @va_libelle_in         = null or 
       @va_simulation_in                 = null

        return XDC_ARG_INV
   
	      if @va_type_in=0
	      begin
		delete EXP..REG_TRA
		where simulation = 1
		return XDC_OK
	      end

              /*A
                   ** Insertion dans la table REG_TRA
                    */
            
                        insert into EXP..REG_TRA ( horodate,
                                                   type,
                                                   libelle,
                                                   alerte,
                                                   numero_fmc,
                                                   cle_fmc,
                                                   acq_alerte,
                                                   scenario,
                                                   condition,
                                                   simulation,
                                                   zone,
                                                   numero_station,
                                                   sens_station)
                                          values ( @va_horodate_in,
                                          		    @va_type_in,
                                          		    @va_libelle_in,
                                          		    @va_alerte_in,
                                          		    @va_numero_fmc_in,
                                          		    @va_cle_fmc_in,
												    @va_acq_alerte_in,
												    @va_scenario_in,
												    @va_condition_in,
												    @va_simulation_in,
												    @va_zone_in,
												    @va_num_station_in,
												    @va_sens_station_in	)
												                  
						
								/*A
								** Si erreur d'insertion
								*/

								if @@rowcount <> 1
								begin

									/*A
									** Affectation du parametre de sortie Resultat a XDC_MODIF
									*/

									return XDC_NOK
								end   
     
    return XDC_OK
         
go  
