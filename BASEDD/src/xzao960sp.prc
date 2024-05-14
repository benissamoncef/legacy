/*
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAO * FICHIER xzao960sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Insertion des données de temps de parcours FCD dans la table TDP_FCD
*
------------------------------------------------------
* SEQUENCE D'APPEL : 
*
* SP	XZAO960SP
*
* PARAMETRES EN ENTREE :
*
* XDY_Horodate		Horodate donne
* XDY_PR		    PR de debut
* XDY_PR		    PR de fin
* XDY_Sens		    Sens
* XDY_TDP		    Valeur TDP
* XDY_Octet		    Indicateur validite
* XDY_NomAuto		Nom autoroute
*
* PARAMETRES EN SORTIE :	
*
* VALEUR RENDUE :
* 
* CODE RETOUR :
*
* 0	XDC_OK
* 1	XDC_NOK
* 2	XDC_ARG_INV		
*
* CONDITIONS D'UTILISATION :
*
* FONCTION :
*
* Insert dans la table TDP_FCD
*
* MECANISMES :
*
------------------------------------------------------
* HISTORIQUE :
*
* LPE		23/07/20 : Creation  TDP_FCD  DEM-SAE152   1.0
* PNI   	02/10/20      : Optimisation mécanisme de purge (delete correspondant à chaque insert TDP_FCD) 1.2
* ABE   	05/10/20      : Changement purge pour donnés vielle d'une semaine max DEM-SAE152 1.3
* PNI		01/02/21      : Patche pour insert en base avec sens inversé pour A57 1.4
* JPL		16/10/23      : Suppression de la restriction aux autoroutes d'ESCOTA (SAE-504)  1.5
* JPL		30/01/24      : Arrondi de l'horodate a un multiple de 6 minutes (SAE-504)  1.6
* JPL		04/04/24      : Pas d'insertion de valeurs existantes en un lieu et date donnes  1.8
------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAO960' and type = 'P')
	drop procedure XZAO960
go


create procedure XZAO960
	@va_Horodate_in		datetime,
	@va_PR_debut_in		T_PR,
	@va_PR_fin_in		T_PR,
	@va_Sens_in		tinyint,
	@va_Tdp_in		int,
	@va_Validite_in		tinyint,
	@va_Autoroute_in	char(6)
as
   
   /* Declaration des variables locales */
   
	declare	@vl_Autoroute	tinyint,
		@vl_PR		T_PR ,
                @vl_Minutes int,
                @vl_Inversee smallint,
		@vl_Horodate smalldatetime

   
/* Test des parametres d'entree :
Si un des parametres d'entree est egal a null alors XDC_ARG_INV   */

if @va_Horodate_in = null
   	or @va_PR_debut_in = null		
   	or @va_PR_fin_in = null
   	or @va_Sens_in = null
   	or @va_Tdp_in = null
   	or @va_Validite_in = null
   	or @va_Autoroute_in = null
	      	return XDC_ARG_INV

else   
begin
      
    /* Recuperation du numero d'autoroute a partir du nom de l'autoroute */
	select @vl_Autoroute = A.numero, @vl_Inversee=inversee
	from CFG..RES_AUT A, CFG..RES_AUT_INV I
	where nom like @va_Autoroute_in and A.numero=I.numero

	/* si le nom d'autoroute n'est pas trouve alors on quitte */
	if @vl_Autoroute=null
		return XDC_OK

    /* Echange des PR pour que PR_debut<PR_fin si sens 2 et PR_debut>PR_fin si sens 1 sur l'A57*/
	if @vl_Inversee=-1
	begin
		select @vl_PR = @va_PR_debut_in
		select @va_PR_debut_in = @va_PR_fin_in
		select @va_PR_fin_in = @vl_PR
	end

    /* Arrondi de l'horodate a un multiple de 6 minutes */
    select @va_Horodate_in=dateadd(minute, -(datepart(minute,@va_Horodate_in) % 6), @va_Horodate_in)
    select @va_Horodate_in=convert(datetime,convert(char,@va_Horodate_in))

    /* Insertion des données dans la table TDP_FCD */
	if @vl_Autoroute=8 
	begin
		if @va_Sens_in=XDC_SENS_SUD
			select @va_Sens_in=XDC_SENS_NORD
		else
			select @va_Sens_in=XDC_SENS_SUD
	end

	
	if not exists (select tdp from EXP..TDP_FCD
	               where horodate = @va_Horodate_in
	                 and autoroute = @vl_Autoroute and sens = @va_Sens_in
	                 and PR_debut = @va_PR_debut_in and PR_fin = @va_PR_fin_in)
	begin
		insert into EXP..TDP_FCD
				(PR_debut,
				PR_fin,
				sens,		
				tdp,
				validite,
				horodate,
				autoroute)
		values (@va_PR_debut_in, @va_PR_fin_in, @va_Sens_in, @va_Tdp_in, @va_Validite_in, @va_Horodate_in, @vl_Autoroute)
	end

        select @vl_Horodate=getdate()
        select @vl_Minutes=datepart(minute,@vl_Horodate)%6
        if @vl_Minutes<>0
                select @vl_Horodate=dateadd(minute,-@vl_Minutes,@vl_Horodate)

	select @vl_Horodate=dateadd(day,-1,@vl_Horodate)
        delete EXP..TDP_FCD
        where PR_debut=@va_PR_debut_in and sens= @va_Sens_in and horodate =@vl_Horodate

      return XDC_OK
   
end
         
go  
