/*E*/
/*Fichier :  $Id: xzao892sp.prc,v 1.4 2018/07/11 11:08:08 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2018/07/11 11:08:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao892sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
/* fichiers inclus */

/* #include "xzao892sp.h" */

/* static char *version = "@(#)xzao892.c    1.1 11/15/07 : xzao892" ; */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Ajout d'une sequence de retour a la normal du domaine signalisation dans la base
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAO892' and type = 'P')
	drop procedure XZAO892
go

create procedure XZAO892
    @va_Basedd_in           char(3),
    @va_NumeroEquipment_in   tinyint,
    @va_ScenarioTerrain_in  tinyint,
    @va_ScenarioRetourNormal_in tinyint,
    @va_Resultat_out        int output
    as 

/* 
*
* SP    XZAO892SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd           va_Basedd_in
* XDY_Entier           va_NumeroEquipment_in
* XDY_Entier           va_ScenarioTerrain_in
* XDY_Entier           va_ScenarioRetourNormal_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out    
*
* VALEUR RENDUE :
*
* Ajoute une sequence autorisee d'un tunnel en base
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
* Insertion ou modification dans la table EQT_SQR_GTC_SIG 
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* 
* ABE	11/08/2022	: Creation DEM-SAE406
* GGY	26/09/23	: Ajout district CA (DEM483)
* GGY	17/10/23	: Correction si CFG vide (DEM483)
------------------------------------------------------*/
declare
   
    /*A
    ** Definition des variables globales
    */

    @vl_NumScenarioTerrain      smallint,
    @vl_Site					char(10)

    select @va_Resultat_out = XDC_ECHEC

    /*A
    ** Test des parametres d'entree :
    **   Si un des parametres d'entree est egal a null alors XDC_NOK   
    */

    if @va_Basedd_in                = null  
    or @va_NumeroEquipment_in       = null 
    or @va_ScenarioTerrain_in       = null 
    or @va_ScenarioRetourNormal_in  = null

        return XDC_ARG_INV

    else   
    begin

		/* Verification que le site est bien celui du CI ou CA */
		select  @vl_Site = @@servername
	   
		/* Si le site est CI ou CA */
		if @vl_Site=XDC_SQLCI or @vl_Site=XDC_SQLCA
		begin

            /*A
            **  Travail dans la base CFG
            */   
            if @va_Basedd_in = XDC_BASE_CFG
                begin

                /*
                *   Verification que l'equipement tunnel existe dans CFG..EQT_GEN
                */
                if not exists (select * from CFG..EQT_GEN where numero = @va_NumeroEquipment_in and type = XDC_EQT_TUBE)
                    return XDC_ARG_INV

                /*A
                ** Verification de l'existence du scenario dans EQT_SQR_GTC_SIG
                */
                select @vl_NumScenarioTerrain = scenario_terrain
                from   CFG..EQT_SQR_GTC_SIG
                where  scenario_terrain = @va_ScenarioTerrain_in and numero_equipement = @va_NumeroEquipment_in

                /*A
                ** La sequence existe dans EQT_SQR_GTC_SIG
                */

                if @vl_NumScenarioTerrain <> null
                begin

                    update CFG..EQT_SQR_GTC_SIG set scenario_retour_normal = @va_ScenarioRetourNormal_in 
                        where scenario_terrain = @va_ScenarioTerrain_in and numero_equipement = @va_NumeroEquipment_in

                    select @va_Resultat_out = XDC_MODIF
                end
                else
                begin
                    insert into CFG..EQT_SQR_GTC_SIG (   numero_equipement,
                                                        scenario_terrain,
                                                        scenario_retour_normal)
                                                    values (@va_NumeroEquipment_in,
                                                        @va_ScenarioTerrain_in,
                                                        @va_ScenarioRetourNormal_in)
               
                    select @va_Resultat_out = XDC_AJOUT
                end   
            end

#ifdef CI

            else
         
            /*A
            **  Travail dans la base CFT
            */   

            begin
                /*A
              ** Verification de l'existence de la sequence dans EQT_SQR_GTC_SIG
                */
                select @vl_NumScenarioTerrain = scenario_terrain
                from   CFT..EQT_SQR_GTC_SIG
                where  scenario_terrain = @va_ScenarioTerrain_in and numero_equipement = @va_NumeroEquipment_in

                /*A
                ** La sequence existe dans EQT_SQR_GTC_SIG
                */

                if @vl_NumScenarioTerrain <> null
                begin

                    update CFT..EQT_SQR_GTC_SIG set scenario_retour_normal = @va_ScenarioRetourNormal_in 
                        where scenario_terrain = @va_ScenarioTerrain_in and numero_equipement = @va_NumeroEquipment_in

                    select @va_Resultat_out = XDC_MODIF
                end
                else
                begin
                    insert into CFT..EQT_SQR_GTC_SIG (   numero_equipement,
                                                        scenario_terrain,
                                                        scenario_retour_normal)
                                                    values (@va_NumeroEquipment_in,
                                                        @va_ScenarioTerrain_in,
                                                        @va_ScenarioRetourNormal_in)
               
                    select @va_Resultat_out = XDC_AJOUT
                end
            end 
#endif

        end
    end         

    return XDC_OK

go  
