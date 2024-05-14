/*E*/
/*  Fichier : $Id: xzaf16sp.prc,v 1.3 1998/09/22 13:39:22 gaborit Exp $      Release : $Revision: 1.3 $        Date : $Date: 1998/09/22 13:39:22 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzaf16sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des synoptiques pour animation
* de l'ihm pour la formation
*
------------------------------------------------------
* HISTORIQUE :
*
* C.T.	16/01/95	: Creation 
* C.T.	03/05/96	: recherche dans la base EXP pour MES_NIV
*                         (n'est plus historisee) V1.2
* P.N 	02/07/98	: separation EXP/HIS 1.3
* ABK   26/09/23 	: Ajout district CA (DEM-483)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Sequence d'appel
* SP	XZAF16_Lire_Niveaux_Station
* 
* Arguments en entree
* XDC_Mot	va_Station_in	
* XDC_Horodate	va_Horodate_in	
* 
* Arguments en sortie
* XDC_Sens	va_Sens1_in	
* XDC_Octet	va_Niveau1_in	
* XDC_Sens	va_Sens2_in	
* XDC_Octet	va_Niveau2_in	
* 
* Service rendu
* Retourner les niveaux d'une station de comptage a
* la date precisee 
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
* 
* Conditions d'utilisation
* Utilisable au CI uniquement
*
* Fonction
* select from MES_NIV
*
---------------------------------------------------*/

use PRC
go


create procedure  XZAF;16
	@va_Station_in	smallint = null,
	@va_Horodate_in	datetime = null,
	@va_Sens1_out	T_SENS	= null output,
	@va_Niveau1_out	tinyint = null output,
	@va_Sens2_out	T_SENS 	= null output,
	@va_Niveau2_out	tinyint = null output
as
	declare @vl_Status int, @vl_site tinyint
	
	if @va_Station_in = null or @va_Horodate_in = null
		return XDC_ARG_INV

	/* initialisation variable locale */
	select @vl_site = numero from CFG..RES_DIS where type = XDC_TYPEM_SD

#ifdef HIST
			if @vl_site = XDC_CI
				exec @vl_Status = SQL_CI.PRC..XZAF;16
					 @va_Station_in,
					 @va_Horodate_in,
					 @va_Sens1_out output,
					 @va_Niveau1_out output,
					 @va_Sens2_out	 output,
					 @va_Niveau2_out	 output
			
			else if @vl_site = XDC_CA
				exec @vl_Status = SQL_CA.PRC..XZAF;16
					 @va_Station_in,
					 @va_Horodate_in,
					 @va_Sens1_out output,
					 @va_Niveau1_out output,
					 @va_Sens2_out	 output,
					 @va_Niveau2_out	 output	

			if @vl_Status = null
                        	return XDC_PRC_INC
                	else return @vl_Status
#else

	/*A initialisation des valeurs des sens */
	select	@va_Sens1_out = XDC_SENS_SUD,
		@va_Sens2_out = XDC_SENS_NORD

	/*A tester si la station est dans le sens SUD et rechercher le niveau */
	if not exists ( select * from CFG..RES_CHA
			where	station = @va_Station_in and sens = XDC_SENS_SUD)
		select	@va_Sens1_out = XDC_SENS_INCONNU, 
			@va_Niveau1_out = XDC_RADT_NIV_INCONNU
	else
	begin
		/*B niveau de la station lie au sens SUD */
		select
			@va_Niveau1_out = niveau
		from EXP..MES_NIV
		where	station = @va_Station_in and sens = XDC_SENS_SUD and
			horodate in (	select max(horodate) from EXP..MES_NIV
					where	station = @va_Station_in and
						sens = XDC_SENS_SUD and
						horodate <= @va_Horodate_in)
	
		if @@rowcount = 0
			select @va_Niveau1_out = XDC_RADT_NIV_INCONNU
	end

	/*A tester si la station est dans le sens NORD et rechercher le niveau */
	if not exists ( select * from CFG..RES_CHA
			where	station = @va_Station_in and sens = XDC_SENS_NORD)
		select	@va_Sens2_out = XDC_SENS_INCONNU,
			@va_Niveau2_out = XDC_RADT_NIV_INCONNU
	else
	begin
		/*B niveau de la station lie au sens NORD */
		select
			@va_Niveau2_out = niveau
		from EXP..MES_NIV
		where	station = @va_Station_in and sens = XDC_SENS_NORD and
			horodate in (	select max(horodate) from EXP..MES_NIV
					where	station = @va_Station_in and
						sens = XDC_SENS_NORD and
						horodate <= @va_Horodate_in)
	
	
		if @@rowcount = 0
			select @va_Niveau2_out = XDC_RADT_NIV_INCONNU
	end

	return XDC_OK
#endif
go
