/*E*/
/*Fichier : @(#)xzao433sp.prc	1.3      Release : 1.3        Date : 02/14/97
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao433sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < Module Objets Statiques >
*
------------------------------------------------------*/
/* fichiers inclus */

/* declaration de fonctions internes */

/* definition de fonctions externes */


/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Liste des zones origines et destinations pour tdp a envoyer a RTFM
*
------------------------------------------------------
* SEQUENCE D'APPEL :
*
* SP	XZAO433_Liste_origines_destinations
*
* PARAMETRES EN ENTREE :
*
* @va_sens         int 
*
* PARAMETRES EN SORTIE :
*
* VALEUR RENDUE :
*
* Retourne la liste des Zone De Parcours destination possibles  a partir de la zone origine
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
* Retourne les  libelles des zones destinations tdp possibles
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* CHI	29/06/2005: Creation v1.1
* LCL	25/01/2012: Ajout Autoroute (DEM/1014)
*
------------------------------------------------------*/
    
use PRC
go

drop procedure XZAO433
go

create procedure XZAO433
   @va_autoroute	int,
   @va_sens		int,	
   @va_zone_origine	char(30)
   as 
   declare
		@vl_PR                 int
      
   /*premier appel pour proposer les destinations origines en fonction du sens */
   if @va_zone_origine = ""
   begin
   	if @va_sens = 1
		begin
		select distinct DTP.libelle_TFM 
		from CFG..RES_DTP DTP
		where DTP.inhibition   = 0
		and   DTP.sens         = 1
		and   DTP.autoroute    = @va_autoroute
  		order by DTP.PR asc
		end

   	if @va_sens = 2
		begin
		select distinct DTP.libelle_TFM
		from CFG..RES_DTP DTP
		where DTP.inhibition   = 0
		and   DTP.sens         = 2
		and   DTP.autoroute    = @va_autoroute
		order by DTP.PR desc
		end

   end
   else
   begin
	if @va_sens = 1
		begin
		select @vl_PR = DTP.PR
		from CFG..RES_DTP DTP
		where DTP.libelle_TFM = @va_zone_origine
		and   DTP.sens = 1	
		
		select distinct DTP.libelle_TFM
		from CFG..RES_DTP DTP
		where DTP.inhibition   = 0
		and   DTP.sens         = 1
		and   DTP.autoroute    = @va_autoroute
		and   DTP.PR > @vl_PR
		order by DTP.PR asc
		end

	if @va_sens = 2
		begin
		select @vl_PR = DTP.PR 
		from CFG..RES_DTP DTP
		where DTP.libelle_TFM = @va_zone_origine
		and   DTP.sens = 2

		select distinct DTP.libelle_TFM
		from CFG..RES_DTP DTP
		where DTP.inhibition   = 0
		and   DTP.sens         = 2
		and   DTP.autoroute    = @va_autoroute
		and   DTP.PR < @vl_PR
		order by DTP.PR desc
		end
   end
   return XDC_OK
go  
