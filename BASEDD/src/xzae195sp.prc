/*E*/
/* Fichier : $Id: xzae195sp.prc,v 1.3 2021/02/25 17:05:34 pc2dpdy Exp $        Release : $Revision: 1.3 $        Date : $Date: 2021/02/25 17:05:34 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzae195sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Retourne si la FMC a changer de type sur la dernière entrée dans la base FMC_HIS_DER
*
------------------------------------------------------
* HISTORIQUE :
*
* ABE     27/11/20  : Creation DEM-SAE130 1.1
* ABE	  07/12/20  : Ajout vérification type antérieur dans TYP_FMC DEM-SAE130 1.2
* ABE	  05/02/2021 : Ajout recuperation si route coupe sur version précedente de la fmc DEM-SAE130 1.3
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
*
* Sequence d"appel
* SP    XZAE195
*
* Arguments en entree
* XDY_Entier        	Numero de la FMC
* XDY_Octet         	Cle de la FMC	
*
* Arguments en sortie
* XDY_Booleen	Vrai si la FMC à été retypé précedement
* XDY_Mot	Type_antérieur
* XDY_Booleen va_coupure_route_version_precedente
* XDY_Booleen va_coupure_route_inverse_version_precedente
*
* aucun :
*
* Service rendu
*    Retourne si la FMC a changer de type sur la dernière entrée dans la base FMC_HIS_DER
*
* Code retour
* XDC_OK
* XDC_NOK       :
* XDC_ARG_INV   : parametres d"entree null ou site incorrect
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d"utilisation
*
* Fonction
*
-----------------------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAE195' and type = 'P')
	drop procedure XZAE195
go


create procedure XZAE195
	@va_numero_in 	int,
	@va_cle_in		tinyint,
	@va_retypage_out 	bit output,
	@va_type_anterieur_out	tinyint output,
	@va_coupure_route_version_precedente	bit output,
	@va_coupure_route_inverse_version_precedente bit output
as				

declare
	@va_type			smallint,
	@vl_horodate			datetime,
	@VL				tinyint,
	@VM1				tinyint,
	@VM2				tinyint,
	@VR				tinyint,
	@BAU				tinyint,
	@VL_I				tinyint,
	@VM1_I				tinyint,
	@VM2_I				tinyint,
	@VR_I				tinyint,
	@BAU_I				tinyint

/*A
** Test des parametres d'entree :
**   Si un des parametres d'entree est egal a null alors XDC_ARG_INV
*/
if @va_numero_in = 0 select @va_numero_in = null
if @va_cle_in = 0 select @va_cle_in = null

if @va_numero_in = null				
or @va_cle_in = null
	return ( XDC_ARG_INV )

select	@va_type = type_FMC_valide,
	@va_type_anterieur_out = type_anterieur
from	EXP..FMC_HIS_DER
where	EXP..FMC_HIS_DER.numero = @va_numero_in
  and	EXP..FMC_HIS_DER.cle = @va_cle_in				

if @va_type != @va_type_anterieur_out
begin
	if exists(select numero from CFG..TYP_FMC where numero = @va_type_anterieur_out)
		select @va_retypage_out = 1
end
else
	select @va_retypage_out = 0

/* Détection si la FMC était en coupure route à la version précedente */

select @vl_horodate= max(horodate_validation) from EXP..FMC_HIS													
	where numero = @va_numero_in
	and cle = @va_cle_in
	and horodate_validation < (select max(horodate_validation) from EXP..FMC_HIS where numero = @va_numero_in and cle = @va_cle_in)
if @vl_horodate != NULL
begin
	select @VL=VL,@VM1=VM1,@VM2=VM2,@VR=VR,@BAU=BAU,@VL_I=VL_I,@VM1_I=VM1_I,@VM2_I=VM2_I,@VR_I=VR_I,@BAU_I=BAU_I from EXP..FMC_HIS												
		where numero = @va_numero_in
		and cle = @va_cle_in
		and horodate_validation=@vl_horodate
	select @vl_horodate
	select "VR","VM2","VM1","VL","BAU"
	select @VR,@VM2,@VM1,@VL,@BAU

	/* Tests coupure version précedente sens normal */		
	if(	(@VL= XDC_VOIE_INEXISTANTE OR @VL = XDC_VOIE_INCONNUE)
		AND (@VM1=XDC_VOIE_INEXISTANTE OR @VM1 = XDC_VOIE_INCONNUE)
		AND (@VM2=XDC_VOIE_INEXISTANTE OR @VM2 = XDC_VOIE_INCONNUE)
		AND (@VR=XDC_VOIE_INEXISTANTE OR @VR = XDC_VOIE_INCONNUE)
		AND (@BAU=XDC_VOIE_INEXISTANTE OR @BAU = XDC_VOIE_INCONNUE) )
	begin
		/* route inexistante */
		select @va_coupure_route_version_precedente=0
	end
	else if( @VL!= XDC_VOIE_SENS_NORMAL
		AND @VM1!=XDC_VOIE_SENS_NORMAL
		AND @VM2!=XDC_VOIE_SENS_NORMAL
		AND @VR!=XDC_VOIE_SENS_NORMAL				
		AND @BAU!=XDC_VOIE_SENS_NORMAL )	AND 
(@va_type != XZAEC_FMC_QueueBouchon AND @va_type != XZAEC_FMC_Basculement AND @va_type != XZAEC_FMC_BasculTravaux	)						
	begin
		/* coupure sens normal */
		select @va_coupure_route_version_precedente=1
	end
	else
		/* pas coupure sens normal */
		select @va_coupure_route_version_precedente=0

	select "VR_I","VM2_I","VM1_I","VL_I","BAU_I"
	select @VR_I,@VM2_I,@VM1_I,@VL_I,@BAU_I

	/* Tests coupure version précedente sens inverse */		
	if(	(@VL_I= XDC_VOIE_INEXISTANTE OR @VL_I = XDC_VOIE_INCONNUE)
		AND (@VM1_I=XDC_VOIE_INEXISTANTE OR @VM1_I = XDC_VOIE_INCONNUE)
		AND (@VM2_I=XDC_VOIE_INEXISTANTE OR @VM2_I = XDC_VOIE_INCONNUE)
		AND (@VR_I=XDC_VOIE_INEXISTANTE OR @VR_I = XDC_VOIE_INCONNUE)
		AND (@BAU_I=XDC_VOIE_INEXISTANTE OR @BAU_I = XDC_VOIE_INCONNUE) )
	begin
		/* route inverse inexistante */
		select @va_coupure_route_inverse_version_precedente=0
	end
	else if( @VL_I!= XDC_VOIE_SENS_NORMAL
		AND @VM1_I!=XDC_VOIE_SENS_NORMAL
		AND @VM2_I!=XDC_VOIE_SENS_NORMAL
		AND @VR_I!=XDC_VOIE_SENS_NORMAL				
		AND @BAU_I!=XDC_VOIE_SENS_NORMAL )								
	begin
		/* pas de coupure sens invers */
		select @va_coupure_route_inverse_version_precedente=1
	end
	else
		/* pas coupure sens inverse */
		select @va_coupure_route_inverse_version_precedente=0
end

return (XDC_OK)

go
