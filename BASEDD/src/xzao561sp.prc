/*E*/
/* Fichier : $Id: xzao561sp.prc,v 1.1 2012/07/04 11:32:08 pc2dpdy Exp $        Release : $Revision: 1.1 $        Date : $Date: 2012/07/04 11:32:08 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzao561sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des actions
*
------------------------------------------------------
* HISTORIQUE :
*
* VR		24/01/12 : Creation (DEM/1016)
------------------------------------------------------*/

/* fichiers inclus */

/* #include "xzao561sp.h" */

/* static char *version = "@(#)xzao561.c	1.1 23/18/94 : xzao561" ; */

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

if exists (select * from sysobjects where name = 'XZAO561' and type = 'P')
	drop procedure XZAO561
go

create procedure XZAO561
   @va_Basedd_in             char(3),
   @va_NumZone_in			 smallint,
   @va_NumScen_in			 smallint
   as 

/* 
*
* SP    XZAO555SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Basedd        va_Basedd_in
*  		va_NumZone_in
*
* PARAMETRES EN SORTIE :
*
* XDY_ResConf       va_Resultat_out
*
* CODE RETOUR :
*
* 0    XDC_OK
* 1    XDC_NOK 
*
*
* FONCTION :
*
* Affichage des donnees de la table REG_CON
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* VR    13 Jan 2012    : Creation (DEM/1016)
------------------------------------------------------*/
if @va_Basedd_in             = null or
   @va_NumZone_in            = null
		return XDC_ARG_INV


if @va_Basedd_in = XDC_BASE_CFG
    begin
	if @va_NumScen_in = -1 
		begin
		  	select numero, libelle, type_horaire,
				jour, horodate_debut, horodate_fin, heure_debut,
				minute_debut, heure_fin,
				minute_fin, cond_alternative,		
				seuil, donnee_trafic, operateur,
				scenario,
				conjonction1, conjonction2, conjonction3, conjonction4,	
				station, type_jour, sens,
				condition_simple, zone
		  	from   CFG..REG_CON
		  	where  zone = @va_NumZone_in
	        order by station , sens
		end
    	else
		begin
			select CND.numero, 
				libelle, type_horaire,
				jour, horodate_debut,
				horodate_fin, heure_debut,
				minute_debut, heure_fin,	
				minute_fin, cond_alternative,
				seuil, donnee_trafic,
				operateur,
				ASSO.scenario,
				conjonction1, conjonction2, conjonction3, conjonction4,	
				station, type_jour, sens,
				condition_simple, CND.zone
			from   CFG..REG_CON CND, CFG..REG_ASS ASSO
			where  CND.zone = @va_NumZone_in
			and  ASSO.scenario = @va_NumScen_in 
			and ASSO.numero = CND.numero
			and ASSO.zone = CND.zone
		        order by station , sens
       		end
       end

#ifdef CI      
    else
    begin
	if @va_NumScen_in = -1 
	begin
	  select numero, libelle, type_horaire,
		jour, horodate_debut, horodate_fin, heure_debut,
		minute_debut, heure_fin,
		minute_fin, cond_alternative,		
		seuil, donnee_trafic, operateur,
		scenario,
		conjonction1, conjonction2, conjonction3, conjonction4,	
		station, type_jour, sens,
		condition_simple,zone
	  from   CFT..REG_CON
	  where  zone = @va_NumZone_in
          order by station , sens
       end
       else
       begin
	select CND.numero, 
		libelle, type_horaire,
		jour, horodate_debut,
		horodate_fin, heure_debut,
		minute_debut, heure_fin,	
		minute_fin, cond_alternative,
		seuil, donnee_trafic,
		operateur,
		ASSO.scenario,
		conjonction1, conjonction2, conjonction3, conjonction4,	
		station, type_jour, sens,
		condition_simple,CND.zone
	from   CFT..REG_CON CND, CFT..REG_ASS ASSO
	where  CND.zone = @va_NumZone_in
	and  ASSO.scenario = @va_NumScen_in 
	and ASSO.numero = CND.numero
	and ASSO.zone = CND.zone
		        order by station , sens
     end
   end
#endif

     
    return XDC_OK
         
go  
