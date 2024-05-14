/*E*/
/*Fichier : $Id: xzan50sp.prc,v 1.2 2018/12/13 15:31:48 pc2dpdy Exp $      Release : $Revision: 1.2 $        Date : $Date: 2018/12/13 15:31:48 $
------------------------------------------------------
* GTIE *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* MODULE XZAN * FICHIER xzan50sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* gestion de l'annuaire. Recuperation d'une fiche annuaire depuis
* son numero radio. Utilisee pour RMVI
------------------------------------------------------
* HISTORIQUE :
*
* JMG	14/11/18	creation RMVI DEM1315 1.1
* JMG	13/12/18	mise a jour entete 1.2
* GGY	26/09/23	Ajout district CA (DEM483)
------------------------------------------------------*/

use PRC
go

if exists (select 1 from sysobjects where name = 'XZAN50' and type = 'P')
	drop procedure XZAN50
go


/*X------------------------------------------------------
* SERVICE RENDU : 
*
* recupere un interlocuteur en fonction de son numero radio
*  
------------------------------------------------------
* SEQUENCE D'APPEL :
*/

create procedure XZAN50

	@va_radio_in		varchar(15),
	@va_pr_in		int,
	@va_autoroute_in	tinyint,
	@va_sens_in		tinyint,
	@va_type_in		tinyint,
	@va_filtre_in		tinyint
	
	
/*
* ARGUMENTS EN ENTREE :
* ARGUMENTS EN SORTIE :
*
* CODE RETOUR : 
*
*  XDC_OK 
*  XDC_NOK
*
* CONDITION D'UTILISATION :
*
* FONCTION :
*
* Lit une proposition d'appel telephone ou bip d'un plan d'actions
*
* MECANISMES :
*
------------------------------------------------------*/


as 

#ifdef HIST
declare @vl_Status int, 
		@vl_Site tinyint

	/* Verification du site */
	select  @vl_Site = numero
		from CFG..RES_DIS
		where serveur = @@servername and type = XDC_TYPEM_SD

	if @vl_Site = XDC_CI
		exec @vl_Status = SQL_CI.PRC..XZAN50
			@va_radio_in		,
			@va_pr_in,
			@va_autoroute_in,
			@va_sens_in,
			@va_type_in,
			@va_filtre_in

	else if @vl_Site = XDC_CA
		exec @vl_Status = SQL_CA.PRC..XZAN50
			@va_radio_in		,
			@va_pr_in,
			@va_autoroute_in,
			@va_sens_in,
			@va_type_in,
			@va_filtre_in

        if @vl_Status = null
                return XDC_PRC_INC
        else return @vl_Status
#else
    declare @vl_Heure datetime, @vl_HeureCar char(9),
   	@vl_Type tinyint,
	@vl_SousType char(25),@vl_NumInt smallint,
	@vl_site        tinyint,
	@vl_Num smallint,@vl_NomInt char(25),
	@vl_Compt smallint,
	@vl_Sens tinyint,
	@vl_date datetime,
	@vl_nom  T_NOM,
	@vl_radio T_TEL_NUM,
	@vl_numero smallint,
	@vl_nb int, @vl_nb1 int


   /*A
   ** Si une des variables d'entree est nulle alors retourne XDC_ARG_INV
   */
   
   if @va_radio_in = null
  
      return XDC_ARG_INV
  
   else
   
   /*A
   ** Si les variables d'entree sont non nulles
   */
   
   begin
     
	select @vl_site = numero from CFG..RES_DIS
	where serveur = @@servername

	select @vl_nb1=0
	select @vl_nb=0

        select @vl_nom=nom, @vl_numero=numero, @vl_radio=radio
	from CFG..ANN_FIC
	where right(CFG..ANN_FIC.radio,5) = @va_radio_in
	and inhibition = 0

	/*recherche si intervenant pas deja en intervention*/
	if @va_filtre_in = 1
	begin
	select @vl_nb = count(*)
	from EXP..ACT_GEN g, EXP..ACT_SIS s
	where g.numero = s.actio
	and s.nom_patrouille = @vl_nom
	and g.heure_fin is null
	and g.heure_succes is not null	
	and g.sit = @vl_site

	select @vl_nb1 = count(*)
	from EXP..ACT_GEN g, EXP..ACT_INT s
	where g.numero = s.actio
	and s.nom = @vl_nom
	and g.heure_fin is null
	and g.heure_succes is not null
	and g.sit = @vl_site
	end
	else
	begin
		select @vl_nb=0
	end

	if @vl_nb + @vl_nb1 = 0
		select "",@vl_nom, @vl_numero, 0, @vl_radio
	else
		select "","",0,0,""
   end   
   return (XDC_OK)      
#endif
go
