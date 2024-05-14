/*E*/
/*  Fichier : $Id: xzao428sp.prc,v 1.2 2017/08/23 08:36:35 gesconf Exp $        Release : $Revision: 1.2 $        Date : $Date: 2017/08/23 08:36:35 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao428sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Verifie le distric e recouvrement È 
*
------------------------------------------------------*/
/*------------------------------------------------------
* HISTORIQUE :
*
* P.NI     	08/03/05	: Creation 	V 1.1 (DEM439)
* P.NI     	22/08/17	: Prise en compte des FMC sur la portion A1001 pour recouvrement V1.2 DEM1238
* ---------------------------------------------------*/

/*X------------------------------------------------------
* SERVICE RENDU : 
*
*  Verifie le district de recouvrement
*
------------------------------------------------------
* SEQUENCE D'APPEL :

* SP	XZAE428_Verifie_DistrictRecouvrement
* 
* Arguments en entree
* XDY_Mot	va_District_in
* XDY_Autoroute	va_Autoroute_in
* XDY_Mot	va_Portion_in
* XDY_Sens	va_Sens_in
* XDY_Mot	va_Type_in
* XDY_Mot	va_DistrictRecouvrement_in
* XDY_Mot	va_DistrictRecouvrement_out
* 
* Arguments en sortie
* 
* Service rendu
* Met en base la fiche correspondante 
* (doit être appele apres Ecrire_Fiche_xxx)
* 
* Code retour
* XDC_OK
* 
* Conditions d'utilisation
* 
* Fonction
* Verifie si le district fournit est en amont de la portion dans le sens donne.
* Si c'est le cas, renvoie ce district sinon NULL
-------------------------------------------------------*/
use PRC
go
drop procedure XZAO428
go

create procedure XZAO428
   @va_District_in			tinyint,
   @va_Autoroute_in			tinyint,
   @va_Portion_in			tinyint,
   @va_Sens_in		tinyint,
   @va_DistrictRecouvrement_in			tinyint,
   @va_DistrictRecouvrement_out			tinyint = null output
   as 

declare
   
   /*A
   ** Definition des variables globales
   */

   @vl_AutorouteInversee	smallint,
   @vl_NumDistrict	tinyint,
   @vl_NumDistrictRec	tinyint,
   @vl_PR		int,
   @vl_Numero		smallint,
   @vl_nom_por		char(5)

	/*Recherche le sens de l'autoroute */
   select @vl_nom_por =nom from CFG..RES_POR where numero=@va_Portion_in

   if (@vl_nom_por ="A1001") and (@va_Sens_in = XDC_SENS_NORD)
   begin
	select @va_DistrictRecouvrement_out=@va_DistrictRecouvrement_in
   	select @va_DistrictRecouvrement_out
   	return XDC_OK
   end
   
   
   select @vl_AutorouteInversee=inversee from CFG..RES_AUT_INV where numero=@va_Autoroute_in
   
   if (@va_Sens_in = XDC_SENS_SUD and @vl_AutorouteInversee=1) or 
   	(@va_Sens_in = XDC_SENS_NORD and @vl_AutorouteInversee=-1)
   begin
   	select @vl_PR=PR_fin from CFG..RES_POR where numero=@va_Portion_in
   	select @va_DistrictRecouvrement_out=POR2.district
   	from CFG..RES_POR POR1, CFG..RES_POR POR2
   	where POR2.district=@va_DistrictRecouvrement_in and
   		POR1.district=@va_District_in and
   		POR2.PR_fin = POR1.PR_debut and
   		POR1.autoroute=POR2.autoroute and
   		POR1.autoroute=@va_Autoroute_in and
   		POR1.PR_debut < @vl_PR
   	if @va_DistrictRecouvrement_out = null
   	begin
   		select @va_DistrictRecouvrement_out=POR1.district
   		from CFG..RES_POR POR1, CFG..RES_POR POR2, CFG..RES_NEU NEU
   		where POR2.autoroute=@va_Autoroute_in and POR1.autoroute!=@va_Autoroute_in and
   			POR1.numero=NEU.portion1 and POR2.numero=NEU.portion2 and
   			POR1.district=@va_DistrictRecouvrement_in and POR2.district=@va_District_in and
   			POR2.district_recouvrement=@va_DistrictRecouvrement_in and 
   			NEU.extremite_portion_2=0 and
  			POR2.PR_debut < @vl_PR
   	end 
   end
   else if    (@va_Sens_in = XDC_SENS_NORD and @vl_AutorouteInversee=1) or 
   	(@va_Sens_in = XDC_SENS_SUD and @vl_AutorouteInversee=-1)
   begin
   	select @vl_PR=PR_debut from CFG..RES_POR where numero=@va_Portion_in
   	select @va_DistrictRecouvrement_out=POR2.district
   	from CFG..RES_POR POR1, CFG..RES_POR POR2
   	where POR2.district=@va_DistrictRecouvrement_in and
   		POR1.district=@va_District_in and
   		POR2.PR_debut = POR1.PR_fin and
   		POR1.autoroute=POR2.autoroute and
   		POR1.autoroute=@va_Autoroute_in and
   		POR1.PR_fin > @vl_PR
   		
  	if @va_DistrictRecouvrement_out = null
   	begin
   		select @va_DistrictRecouvrement_out=POR1.district
   		from CFG..RES_POR POR1, CFG..RES_POR POR2, CFG..RES_NEU NEU
   		where POR2.autoroute=@va_Autoroute_in and POR1.autoroute!=@va_Autoroute_in and
   			POR1.numero=NEU.portion1 and POR2.numero=NEU.portion2 and
   			POR1.district=@va_DistrictRecouvrement_in and POR2.district=@va_District_in and
   			POR2.district_recouvrement=@va_DistrictRecouvrement_in and 
   			NEU.extremite_portion_2=1 and
   			POR2.PR_fin > @vl_PR
   	end 
   end
      
   select @va_DistrictRecouvrement_out
   return XDC_OK
         
go
