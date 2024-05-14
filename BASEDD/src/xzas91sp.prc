/*E*/
/*  Fichier : $Id: xzas91sp.prc,v 1.4 2021/04/12 17:37:54 pc2dpdy Exp $      Release : $Revision: 1.4 $        Date : $Date: 2021/04/12 17:37:54 $
------------------------------------------------------
* STERIA *  PROJET MIGRAZUR 
------------------------------------------------------
* SOUS-SYSTEME  BASEDD
------------------------------------------------------
* FICHIER xzassp.prc
------------------------------------------------------
* DESCRIPTION DU MODULE :
*
* Lecture et ecriture des mesures de trafic
*
------------------------------------------------------
* HISTORIQUE :
*
* PNI   05/01/07	: Creation DEM605 v1.1
* PNI   05/01/07	: recherche la dernière données dans les 12dernières mn DEM605 v1.2
* ABE	12/04/21	: Ajout if defined DEV pour get dernière donnée en base pour dev DEM-SAE130 1.3
*
-----------------------------------------------------*/

/*X*/
/*-----------------------------------------------------
* Service rendu
* Fourni les dernières données RADT pour les 6 dernières minutes
* 
* Sequence d'appel
* SP	XZAS91_Dernieres_Donnees_RADT
* 
* Arguments en entree
* 
* Arguments en sortie
* XDY_Datetime	va_Horodate	: date des donnees
* XDY_Eqt va_Station		: no station
* XDY_Sens va_Sens		: sens du pt de mesure
* XDY_Mot  va_Q			: Q
* XDY_Octet va_V		: V
* XDY_Octet va_TO		: TO
* XDY_Octet va_PL		: PL
* XDY_Booleen va_validite	: Validite des donnees
*
* Code retour
* XDC_OK
* XDC_NOK		: probleme d'acces aux mesures
* XDC_ARG_INV		: parametres d'entree null, ou valeur non autorisee ou
*                 	  station inexistante
* XZASC_RADT_PAS_MES	: pas de mesures
* 
* Conditions d'utilisation
*
* La procedure stockee au CI recherche les informations 
* dans la base EXP pour l'horodate comprise entre l'horodate courante et horodate courante - 6 mn.
-------------------------------------------------------*/

use PRC
go
drop proc XZAS91
go

create procedure XZAS91
as
	declare @vl_Station smallint, @vl_Sens tinyint,
		@vl_DateSup datetime, @vl_DateInf datetime, @vl_Status int



	create table #MES (horodate datetime, station smallint, sens tinyint, Q smallint, validite_Q bit, V tinyint, validite_V bit, T_O tinyint, validite_TO bit, PL tinyint, validite_PL bit)
	/*A controle parametres en entree */


	/*A  recherche des bornes horodate*/
	select @vl_DateSup = getdate()
	select @vl_DateInf = dateadd( minute,-12,@vl_DateSup)
#if defined (DEV)
	select @vl_DateSup=max(horodate) from EXP..MES_TRA
	select @vl_DateInf = dateadd( minute,-12,@vl_DateSup)

#endif
	

	/*select  @vl_DateSup,@vl_DateInf*/

	/*A Recherche des infos */
/*	select horodate,P.station, P.sens,Q ,validite_Q, V,    validite_V, T_O,  validite_TO, PL,   validite_PL
	from CFG..RES_CHA P, EXP..MES_TRA M
	where 
		horodate > @vl_DateInf and horodate <= @vl_DateSup and P.station = M.station and P.sens = M.sens and voie=0 and
		horodate > @vl_DateInf and horodate <= @vl_DateSup*/

  	declare pt_mesures cursor
  	for select distinct station, sens
      	from CFG..RES_CHA

	open pt_mesures
	fetch pt_mesures into @vl_Station,@vl_Sens
	while (@@sqlstatus = 0 )
  	begin
		insert into #MES 
		select horodate,station, sens,isnull(Q,0) ,validite_Q, isnull(V,0),    validite_V, isnull(T_O,0),  validite_TO, isnull(PL,0),   validite_PL
		from  EXP..MES_TRA 
		where 
			station = @vl_Station and sens = @vl_Sens and voie=0 and
			horodate > @vl_DateInf and horodate <= @vl_DateSup
		having station = @vl_Station and sens = @vl_Sens and voie=0 and
			horodate > @vl_DateInf and horodate <= @vl_DateSup and horodate=max(horodate)

		fetch pt_mesures into @vl_Station,@vl_Sens
	end
	
	select @vl_DateSup = max(horodate) from #MES 

	select @vl_DateSup,station, sens,Q ,validite_Q, V,    validite_V, T_O,  validite_TO, PL,   validite_PL
	from #MES 
	order by station, sens
	
	return XDC_OK
go
