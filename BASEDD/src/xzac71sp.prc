/*E*/
/*  Fichier : $Id: xzac71sp.prc,v 1.3 2019/01/07 14:53:38 pc2dpdy Exp $      Release : $Revision: 1.3 $        Date : $Date: 2019/01/07 14:53:38 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzac71sp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
* strada : fournit les infos sur les messages PMV a exporter
------------------------------------------------------
* HISTORIQUE :
*
* Niepceron	27/11/1998 : creation
* Niepceron	13/07/2010 : si heure_fin est nulle,on renvoie heure_lancement_fin 1.2
* JMG		08/08/18 : ligne PMV passe a 18 CAR. suppression SAGA DEM1306 1.3 
* GGY	28/04/23 : Augmentation taille vl_Bandeau a char(XDC_LGR_TXT_PICTO) et vl_picto (XDC_LGR_NOM_PICTO) (DEM-473)
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
*
*
* Sequence d'appel
* SP   XZAC71_Recherche_Derniers_PMV
*
* Arguments en entree
*
* Arguments en sortie
*
* Code retour
* XDC_OK
* <0 erreur sybase (voir Transact SQL p 13-15)
*
* Conditions d'utilisation
*
* Fonction
* Liste les infos PMV a exporter
-------------------------------------------------------*/

use PRC
go

if exists (select * from sysobjects where name = 'XZAC71' and type = 'P')
	drop procedure XZAC71
go

create procedure XZAC71
	
as
declare	@vl_actio int, @vl_sit tinyint,
	@vl_PR T_PR,
	@vl_sens tinyint,
	@vl_autoroute tinyint,
	@vl_ligne1 char(18),
	@vl_ligne2 char(18),
	@vl_ligne3 char(18),
	@vl_ligne4 char(18),
	@vl_picto char(XDC_LGR_NOM_PICTO),
	@vl_bandeau char(XDC_LGR_TXT_PICTO)
 
	create table #PMV ( actio int,sit tinyint null, 
				succes datetime null, 
				fin datetime null, 
				autoroute tinyint,
				PR	int,
				sens	tinyint,
				picto	char(XDC_LGR_NOM_PICTO) null,
				bandeau	char(XDC_LGR_TXT_PICTO) null,
				ligne1	char(18) null,
				ligne2	char(18) null,
				ligne3	char(18) null,
				ligne4	char(18) null,
				fmc int, cle tinyint)
	

	/* creation des curseurs */
	declare Pointeur_action cursor
	for select actio,sit
	from EXP..PMV_ADA
	
	open Pointeur_action
	fetch Pointeur_action into  @vl_actio, @vl_sit
		
	while (@@sqlstatus = 0)
	begin
		select @vl_ligne1=direction_ligne,
			@vl_ligne2=premiere_ligne,
			@vl_ligne3=deuxieme_ligne,
			@vl_ligne4=troisieme_ligne,
			@vl_picto=pictogramme,
			@vl_bandeau=bandeau
		from EXP..ACT_PMV 
		where actio=@vl_actio and sit = @vl_sit 
		
		select @vl_autoroute=autoroute,
			@vl_PR=PR,
			@vl_sens=sens
		from EXP..ACT_PMV ACT, CFG..EQT_GEN EQT 
		where actio=@vl_actio and sit = @vl_sit and
			PMV=numero and EQT.type=XDC_EQT_PMV
		
		insert into #PMV 
		select numero,sit,heure_succes,isnull(heure_fin,heure_lancement_fin),
			@vl_autoroute,@vl_PR,@vl_sens,@vl_picto,@vl_bandeau,
			@vl_ligne1,@vl_ligne2,@vl_ligne3,@vl_ligne4,evenement,cle
		from EXP..ACT_GEN
		where numero=@vl_actio and sit = @vl_sit
		
		fetch Pointeur_action into  @vl_actio, @vl_sit
	end
	
	close Pointeur_action
	
	delete EXP..PMV_ADA
	
	select * from #PMV
	
	return XDC_OK
go

