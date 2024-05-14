/*E*/
/*Fichier : $Id: xzao21sp.prc,v 1.15 2021/05/20 08:52:01 gesconf Exp $      Release : $Revision: 1.15 $        Date : $Date: 2021/05/20 08:52:01 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao21sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* < description rapide des fonctions remplies par le module
*   et reference au dossier de conception generale
*   ou reference au fichier xzao21sp.h >
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO21SP_Rech_Echangeur
*
* PARAMETRES EN ENTREE :
*
* XDY_Autoroute		Autoroute_In
* XDY_PR		PR_In
* XDY_Sens		Sens_In
* XDY_Horodate		Horodate_in
*
* PARAMETRES EN SORTIE :	
*
* XDY_Eqt		NumEch
*
* VALEUR RENDUE :
*
* Recherche le premier echangeur en amont et verifie qu'il possede une
* signalisation non inhibee
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
* Select dans les tables EQT_GEN (EQUIPEMENT) et EQT_ECH (DEMI-ECHANGEUR)
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* volcic	25 Aug 1994	: Creation
* B.G.          17/11/94 : modif (1.6)
* B.G.          24/11/94 : ajout va_District_out(1.7)
* B.G.          24/11/94 : ajout output (1.8)
* B.G.          25/11/94 : correction pb incoherence ech bitume et eqt (1.9)
* C.T.  	10/01/95 : Ajout une date en entree (v1.10)
*                          pour la formation
* B.G.          17/01/95 : modif site(1.11)
* PN         	29/06/98 : separartion EXP/HIS 1.12
* LCL           11/07/18        : correction syntaxe from
* PNI		30/03/21  : XZAO;21  du SQL_CI s'execute explicitement sur SQL_HIS
* PNI 	11/09/23	: Prise en compte du flag RES_ECH.supprime=XDC_FAUX SAE-517
* GGY	26/09/23 :	Ajout district CA (DEM483)
------------------------------------------------------*/


use PRC
go

create procedure XZAO;21
	@va_Autoroute_in	tinyint = null,
   	@va_PR_in		int = null,
   	@va_Sens_in		tinyint = null,
   	@va_Ech_out		smallint =null output,
   	@va_District_out	tinyint = null output,
	@va_Horodate_in		datetime = null 
as 
    declare @vl_PR int, @vl_Horodate datetime, @vl_Status int, @vl_Site tinyint
      
   /* Test des parametres d'entree :
      Si un des parametres d'entree est egal a null alors XDC_NOK   */
      
   if @va_Autoroute_in = null or @va_PR_in = null or @va_Sens_in = null
      return XDC_ARG_INV
   else   
      
      /* Recherche de l'echangeur possedant une signalisation non inhibee
         en amont de PR                              */  
    begin
    
/*B ramene le sens sur section courante */

    	if @va_Sens_in=XDC_SENS_SORTIE_NORD or
    	   @va_Sens_in=XDC_SENS_ENTREE_NORD
    	select @va_Sens_in=XDC_SENS_NORD
    	
    	if @va_Sens_in=XDC_SENS_SORTIE_SUD or
    	   @va_Sens_in=XDC_SENS_ENTREE_SUD
    	select @va_Sens_in=XDC_SENS_SUD
    	
/*A Si le sens est inconnu : retourne null */
	
	if	@va_Sens_in<>XDC_SENS_NORD and @va_Sens_in<>XDC_SENS_SUD
	return XDC_OK
	
/*A Suivant le sens , recherche le PR */

	if	@va_Sens_in=XDC_SENS_SUD
	begin
			select 	@vl_PR=max(ECH.PR)
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and ECH.supprime = XDC_FAUX and
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens1<=@va_PR_in and	-- en amont (SUD)
				ECH.supprime = 0 and
				ECH.voies_sortie_sens1<>0
	end
	else
	begin
			select 	@vl_PR=min(ECH.PR)
			from	CFG..RES_ECH ECH,
				CFG..RES_POR POR
			where	ECH.portion=POR.numero and ECH.supprime = XDC_FAUX and 
				POR.autoroute=@va_Autoroute_in and
				ECH.PR_sortie_sens2>=@va_PR_in and	-- en amont (NORD)
				ECH.supprime = 0 and
				ECH.voies_sortie_sens2<>0
	end

	select 	@va_Ech_out=numero
	from 	CFG..EQT_GEN
	where 	type=XDC_EQT_ECH and
		autoroute=@va_Autoroute_in and
		sens=@va_Sens_in and
		PR=@vl_PR
			
#ifdef CI
		/* recherche la derniere date avant la date donnee */
		select
			@vl_Horodate = max(horodate) 
		from EXP..EQT_DSP DSP
		where	DSP.equipement = @va_Ech_out and
			DSP.type = XDC_EQT_ECH and
			DSP.horodate <= @va_Horodate_in

		/* tester si l'equipement est disponible   */
		/* a la derniere date avant la date donnee */
		select 
			@va_District_out = DSP.sit
		from EXP..EQT_DSP DSP
		where	DSP.horodate = @vl_Horodate and
			DSP.equipement = @va_Ech_out and
			DSP.type = XDC_EQT_ECH and
			DSP.inhibe=XDC_NON and 	-- non inhibe
			DSP.HS=XDC_NON 		-- non HS

#else
#ifdef HIST
		/* recherche la derniere date avant la date donnee */
		select
			@vl_Horodate = max(horodate) 
		from HIS..EQT_DSP DSP
		where	DSP.equipement = @va_Ech_out and
			DSP.type = XDC_EQT_ECH and
			DSP.horodate <= @va_Horodate_in

		/* tester si l'equipement est disponible   */
		/* a la derniere date avant la date donnee */
		select 
			@va_District_out = DSP.sit
		from HIS..EQT_DSP DSP
		where	DSP.horodate = @vl_Horodate and
			DSP.equipement = @va_Ech_out and
			DSP.type = XDC_EQT_ECH and
			DSP.inhibe=XDC_NON and 	-- non inhibe
			DSP.HS=XDC_NON 		-- non HS
#else

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	/* Si le site n'est pas CI ou CA ou l'horodate est nulle */
	if @va_Horodate_in is null or @@rowcount <> 1
		select @va_District_out=sit
		from EXP..EQT_DSP
		where type=XDC_EQT_ECH and
	      		equipement=@va_Ech_out and
	      		dernier=XDC_OUI and
	      		HS=XDC_NON and
	      		inhibe=XDC_NON
#endif
#endif
	      
	if @@rowcount<>1 select @va_Ech_out=null, @va_District_out=null
    end
    return XDC_OK     
go  
